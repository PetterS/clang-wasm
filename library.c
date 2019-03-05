#define WASM_EXPORT __attribute__((visibility("default")))

// Will be provided by Javascript.
int get_input();

int two() {
	return 2;
}

WASM_EXPORT int petter() {
	return two() * get_input();
}
