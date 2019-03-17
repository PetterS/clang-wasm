#include <memory.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

#define memory_assert(cond) if (!(cond)) { printf("Memory error at %s:%d: %s.\n", __FILE__, __LINE__, #cond); trap(); }

// Useful description of this really simple malloc:
// https://danluu.com/malloc-tutorial/
struct MemoryBlock {
	size_t size = 0;
	MemoryBlock* next = nullptr;
	bool free = false;
};

MemoryBlock* start_block = nullptr;

MemoryBlock* new_block(MemoryBlock* last_block, size_t size) {
	MemoryBlock* block = (MemoryBlock*) sbrk(sizeof(MemoryBlock) + size);
	last_block->next = block;
	block->size = size;
	return block;
}

MemoryBlock* find_available(MemoryBlock** last_block, size_t size) {
	MemoryBlock* current = start_block;
	while (current && !(current->free && current->size >= size)) {
		*last_block = current;
		current = current->next;
	}
	return current;
}

extern "C" {
	void* malloc(size_t amount) {
		if (amount == 0) {
			return nullptr;
		}
		if (!start_block) {
			start_block = (MemoryBlock*) sbrk(sizeof(MemoryBlock));
		}
		
		MemoryBlock* last = start_block;
		MemoryBlock* block = find_available(&last, amount);
		if (block) {
			memory_assert(block->free);
			block->free = false;
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
		memory_assert(!block->free);
		if (block->size >= size) {
			return ptr;
		}

		void* new_ptr = malloc(size);
		memcpy(new_ptr, ptr, block->size);
		free(ptr);
		return new_ptr;
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
		memory_assert(!block->free);
		block->free = true;
	}
}
