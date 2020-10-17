#ifndef _NANOLIB_C_H
#define _NANOLIB_C_H
#include <stdarg.h>
#include <stddef.h>

extern "C" {
    void* memcpy(void* dest, const void* src, size_t count);
    void* memset (void * dest, int value, size_t count);

    int puts ( const char * str );
    int printf(const char* format, ...);
    int sprintf(char* buffer, const char* format, ...);
    int snprintf(char* buffer, size_t count, const char* format, ...);
    int vsnprintf(char* buffer, size_t count, const char* format, va_list va);

    void* malloc(size_t amount);
    void* realloc(void *ptr, size_t size);
    void* calloc(size_t num, size_t size);
    void free(void* mem);
}

#endif
