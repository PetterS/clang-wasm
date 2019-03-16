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
