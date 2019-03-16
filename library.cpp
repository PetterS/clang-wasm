#include <stdio.h>
#include <stdlib.h>

#define WASM_EXPORT __attribute__((visibility("default"))) extern "C"
 
// Will be provided by Javascript.
extern "C" {
	int get_input();
}

int two() {
	puts("two() called");
	return 2;
}

WASM_EXPORT int petter() {
	int* p = (int*) malloc(sizeof(int));
	p[0] = get_input();

	return two() * p[0];
}
