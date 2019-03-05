#define WASM_EXPORT __attribute__((visibility("default"))) extern "C"

// Will be provided by Javascript.
extern "C" {
	int get_input();
	void print_string(const char* str);
}

void trap() {
	__builtin_unreachable();
}

void print(const char* str) {
	const char* end = str;
	while (*(end++));
	print_string(str);
}

int two() {
	print("two() called");
	return 2;
}

WASM_EXPORT int petter() {
	return two() * get_input();
}
