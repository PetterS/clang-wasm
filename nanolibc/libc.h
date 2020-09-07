#ifndef NANOLIBC_LIBC_H
#define NANOLIBC_LIBC_H
#include <stdarg.h>
#include <stddef.h>

extern "C" {
	typedef int locale_t;
	struct mbstate_t;
	typedef int mode_t;
	typedef void *FILE;

	// Will be provided by Javascript.
	void print_string(const char* str);

	void* memcpy(void* dest, const void* src, size_t count);
	void* memset (void * dest, int value, size_t count);
	int puts ( const char * str );

	int printf(const char* format, ...);
	int sprintf(char* buffer, const char* format, ...);
	int snprintf(char* buffer, size_t count, const char* format, ...);
	int vsnprintf(char* buffer, size_t count, const char* format, va_list va);
	int fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);

	void* malloc(size_t amount);
	void* realloc(void *ptr, size_t size);
	void* calloc(size_t num, size_t size);
	void free(void* mem);
}

void dump_memory();

[[noreturn]] void trap();

#endif
