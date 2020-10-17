#ifndef _NANOLIB_C_EXTRA_H
#define _NANOLIB_C_EXTRA_H

void dump_memory();

extern "C" {
    // Will be provided by Javascript.
    void print_string(const char* str);
}

#endif
