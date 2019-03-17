#include <stddef.h>
#include <stdlib.h>

namespace {
constexpr size_t PAGE_SIZE = 64 * 1014;

size_t current_memory() {
	return __builtin_wasm_memory_size(0);
}

size_t grow_memory(size_t delta) {
	return __builtin_wasm_memory_grow(0, delta);
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

extern "C" {
	void* malloc(size_t amount) {
		void* ptr = (void*) (current_memory() * PAGE_SIZE);
		grow_memory(amount / PAGE_SIZE + 1);
		return ptr;
	}

	void free(void* mem) {

	}
}
