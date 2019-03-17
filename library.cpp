#include <stdio.h>
#include <stdlib.h>

#define WASM_EXPORT __attribute__((visibility("default"))) extern "C"
 
// Will be provided by Javascript.
extern "C" {
	int get_input();
}

int two() {
	printf("two() = %d called.\n", 2);
	return 2;
}

WASM_EXPORT int petter() {
	int* p = (int*) malloc(sizeof(int));
	p[0] = get_input();

	int result = two() * p[0];
	printf("Result will be %d.\n", result);
	return result;
}


WASM_EXPORT char* get_memory_for_string(int size) {
	return new char[size];
}

WASM_EXPORT void free_memory_for_string(char* str) {
	delete[] str;
}


WASM_EXPORT int string_to_int(const char* str) {
	int i = 0U;
	while ('0' <= *str && *str <= '9') {
		i = i * 10 + (int)((*str++) - '0');
	}
	return i;
}
