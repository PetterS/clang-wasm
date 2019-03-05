#define WASM_EXPORT __attribute__((visibility("default"))) extern "C"

// Will be provided by Javascript.
extern "C" {
	int get_input();
	void print_string(const char* str, int length);
}

void print(const char* str) {
	const char* end = str;
	while (*(end++));
	print_string(str, end - str);
}

int two() {
	print("two() called");
	return 2;
}

WASM_EXPORT int petter() {
	return two() * get_input();
}
