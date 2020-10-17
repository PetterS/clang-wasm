#include "libc.h"

[[noreturn]] void trap();

namespace {
constexpr size_t PAGE_SIZE = 64 * 1014;
ptrdiff_t current_usage = 0;

ptrdiff_t current_memory() {
	return __builtin_wasm_memory_size(0);
}

ptrdiff_t grow_memory(size_t delta) {
	return __builtin_wasm_memory_grow(0, delta);
}

void* sbrk(size_t increment) {
	if (current_usage == 0) {
		// First call to sbrk().
		current_usage = current_memory() * PAGE_SIZE;
	}
	void* return_value = (void*) current_usage;
	ptrdiff_t current_maximum = current_memory() * PAGE_SIZE;
	ptrdiff_t available = current_maximum - current_usage;
	if (available < increment) {
		grow_memory(increment / PAGE_SIZE + 1);
	}
	current_usage += increment;
	return return_value;
}
}

void* operator new(size_t sz) {
    return malloc(sz);
}

void* operator new[](size_t sz) {
    return malloc(sz);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

void dump_memory();
#define memory_assert(cond) \
	if (!(cond)) { \
		dump_memory(); \
		printf("Memory error at %s:%d: %s.\n", __FILE__, __LINE__, #cond); \
		trap(); \
	}


namespace {
// Useful description of this really simple malloc:
// https://danluu.com/malloc-tutorial/
struct MemoryBlock {
	size_t size;
	MemoryBlock* next;
	enum class State : unsigned int {FREE=0xFEFEFEFE, ALLOCATED=0xC1C1C1C1};
	State state;
};

MemoryBlock* start_block = nullptr;

MemoryBlock* new_block(MemoryBlock* last_block, size_t size) {
	MemoryBlock* block = (MemoryBlock*) sbrk(sizeof(MemoryBlock) + size);
	if (last_block) {
		last_block->next = block;
	}
	block->size = size;
	block->next = nullptr;
	block->state = MemoryBlock::State::ALLOCATED;
	return block;
}

MemoryBlock* find_available(MemoryBlock** last_block, size_t size) {
	MemoryBlock* current = start_block;
	MemoryBlock* first_free = nullptr;
	size_t total_free_size = 0;
	while (current && !(current->state == MemoryBlock::State::FREE && current->size >= size)) {
		*last_block = current;

		if (total_free_size == 0 && current->state == MemoryBlock::State::FREE) {
			total_free_size = current->size;
			first_free = current;
		} else if (current->state == MemoryBlock::State::FREE) {
			total_free_size += sizeof(MemoryBlock) + current->size;
		} else {
			total_free_size = 0;
		}

		// Can we merge consecutive blocks to fill this allocation?
		if (total_free_size >= size) {
			first_free->next = current->next;
			first_free->size = total_free_size;
			current = first_free;
			break;
		}

		current = current->next;
	}

	if (current && current->next && current->size > size + sizeof(MemoryBlock) + 10) {
		// This block is significantly larger than what is needed.
		// We can split it and only use the first part of it.
		int nblocks = 1 + 1 + size / sizeof(MemoryBlock);
		MemoryBlock* next = (MemoryBlock*) current + nblocks;	

		auto old_size = current->size;
		next->size = ((current->next - next) - 1) * sizeof(MemoryBlock);
		next->next = current->next;
		next->state = MemoryBlock::State::FREE;
		current->next = next;
		current->size = (nblocks - 1) * sizeof(MemoryBlock);

		memory_assert(old_size == current->size + sizeof(MemoryBlock) + next->size);			
	}


	return current;
}
}

void dump_memory() {
	printf("Memory dump\n");
	size_t allocated_usage = 0;
	size_t free_usage = 0;
	for (MemoryBlock* block = start_block; block; block = block->next) {
		memory_assert(block->state == MemoryBlock::State::FREE ||
		              block->state == MemoryBlock::State::ALLOCATED);
		if (block->state == MemoryBlock::State::ALLOCATED) {
			allocated_usage += block->size;
		} else {
			free_usage += block->size;
		}
		printf("  0x%X: size %d. %s.\n", 
		       (int) block,
		       (int) block->size,
		       block->state == MemoryBlock::State::FREE ? "FREE" : "ALLOCATED"); 
	}
	printf("  Cur. memory usage: %9d bytes.\n", (int) allocated_usage);
	printf("  Tot. memory usage: %9d bytes.\n", (int) (allocated_usage + free_usage));
	printf("  Peak memory usage: %9d bytes.\n", (int) current_usage);
}

extern "C" {
	void* malloc(size_t amount) {
		if (amount == 0) {
			return nullptr;
		}
		if (!start_block) {
			start_block = new_block(nullptr, amount);
			return start_block + 1;
		}
		
		MemoryBlock* last = start_block;
		MemoryBlock* block = find_available(&last, amount);
		if (block) {
			memory_assert(block->state == MemoryBlock::State::FREE);
			block->state = MemoryBlock::State::ALLOCATED;
		} else {
			block = new_block(last, amount);
		}
		return block + 1;
	}

	void* realloc(void *ptr, size_t size) {
		if (!ptr) {
			return malloc(size);
		}

		MemoryBlock* block = ((MemoryBlock*)ptr) - 1;
		memory_assert(block->state == MemoryBlock::State::ALLOCATED);
		if (block->size >= size) {
			return ptr;
		}

		if (block->next == nullptr) {
			// This is the last block, so we can just increase it.
			sbrk(size - block->size);
			block->size = size;
			return ptr;
		} else {
			void* new_ptr = malloc(size);
			memcpy(new_ptr, ptr, block->size);
			free(ptr);
			return new_ptr;
		}
	}

	void* calloc(size_t num, size_t size) {
		auto total = num * size;
		auto mem = malloc(total);
		memset(mem, 0, total);
		return mem;
	}

	void free(void* mem) {
		if (!mem) {
			return;
		}
		MemoryBlock* block = ((MemoryBlock*)mem) - 1;
		memory_assert(block->state == MemoryBlock::State::ALLOCATED);
		block->state = MemoryBlock::State::FREE;
	}
}
