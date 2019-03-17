#include <stddef.h>
#include <stdlib.h>

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

extern "C" {
	void* malloc(size_t amount) {
		return sbrk(amount);
	}

	void free(void* mem) {

	}
}
