#include "nanolibc/libc.h"

constexpr int line_buffer_size = 100;
int line_buffer_pos = 0;
char* line_buffer = nullptr;


extern "C" {

// Will be provided by Javascript.
void print_string(const char* str);

void _putchar(char ch) {
	if (!line_buffer) {
		line_buffer = (char*) malloc(line_buffer_size);
	}

	if (ch == '\n') {
		line_buffer[line_buffer_pos] = 0;
		print_string(line_buffer);
		line_buffer_pos = 0;
	} else if (line_buffer_pos < line_buffer_size - 1) {
		line_buffer[line_buffer_pos++] = ch;
	}

	if (line_buffer_pos == line_buffer_size - 1) {
		_putchar('\n');
	}
}

int putchar(int ch) {
	_putchar(ch);
	return ch;
}
}
