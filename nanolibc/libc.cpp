#include "nanolibc/libc.h"

[[noreturn]] void trap() {
	__builtin_unreachable();
}

extern "C" {

// Will be provided by Javascript.
void print_string(const char* str);

void* memcpy(void* dest, const void* src, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		((char*) dest)[i] =  ((const char*) src)[i];
	}
	return dest;
}

void* memset (void * dest, int value, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		((unsigned char*) dest)[i] =  (unsigned char) value;
	}
	return dest;
}

int puts ( const char * str ) {
	print_string(str);
	return 0;
}


FILE* stdout = 0;
FILE* stderr = 0;
FILE* stdin = 0;

#define TRAP_FUNCTION(name, return_type, ...) \
	return_type name(__VA_ARGS__) { print_string(#name " is not implemented"); trap(); }

TRAP_FUNCTION(__ctype_get_mb_cur_max, size_t);
TRAP_FUNCTION(__cxa_atexit, int, int, int, int);
TRAP_FUNCTION(__cxa_guard_acquire, int, int);
TRAP_FUNCTION(__cxa_guard_release, void, int);
TRAP_FUNCTION(__cxa_pure_virtual, void);
TRAP_FUNCTION(__errno_location, int);
TRAP_FUNCTION(__lttf2, int, long long, long long, long long, long long);
TRAP_FUNCTION(abort, void);
TRAP_FUNCTION(btowc, int, int);
TRAP_FUNCTION(clock_gettime, int, int, int);
TRAP_FUNCTION(close, int, int);
TRAP_FUNCTION(fflush, int, FILE*);
TRAP_FUNCTION(fprintf, int, FILE*, const char*, ...);
TRAP_FUNCTION(freelocale, void, int);
TRAP_FUNCTION(fwrite, size_t, const void*, size_t, size_t, FILE*);
TRAP_FUNCTION(getc, int, FILE*);
TRAP_FUNCTION(iswalpha_l, int, int, int);
TRAP_FUNCTION(iswblank_l, int, int, int);
TRAP_FUNCTION(iswcntrl_l, int, int, int);
TRAP_FUNCTION(iswdigit_l, int, int, int);
TRAP_FUNCTION(iswlower_l, int, int, int);
TRAP_FUNCTION(iswprint_l, int, int, int);
TRAP_FUNCTION(iswpunct_l, int, int, int);
TRAP_FUNCTION(iswspace_l, int, int, int);
TRAP_FUNCTION(iswupper_l, int, int, int);
TRAP_FUNCTION	(iswxdigit_l, int, int, int);
TRAP_FUNCTION(localeconv, int);
TRAP_FUNCTION(mbrlen, int, int, int, int);
TRAP_FUNCTION(mbrtowc, int, int, int, int, int);
TRAP_FUNCTION(mbsnrtowcs, int, int, int, int, int, int);
TRAP_FUNCTION(mbsrtowcs, int, int, int, int, int);
TRAP_FUNCTION(mbtowc, int, wchar_t*, const char*, size_t);
TRAP_FUNCTION(memchr, void*, const void*, int ch, size_t);
TRAP_FUNCTION(memcmp, int, const void*, const void*, size_t);
TRAP_FUNCTION(memmove, void*, void*, const void*, size_t);
TRAP_FUNCTION(newlocale, int, int, int, int);
// TRAP_FUNCTION(open, int, const char *pathname, int flags, mode_t mode);
TRAP_FUNCTION(posix_memalign, int, void **memptr, size_t alignment, size_t size);
TRAP_FUNCTION(read, size_t, int fd, void *buf, size_t count);
TRAP_FUNCTION(setlocale, int, int, int);	
TRAP_FUNCTION(sscanf, int, const char*, const char*, ...);
TRAP_FUNCTION(strcmp, int, const char*, const char*); 
// TRAP_FUNCTION(strcoll_l, int, const char*, const char*, locale_t);
TRAP_FUNCTION(strerror, int, int);
TRAP_FUNCTION(strftime_l, size_t, int, int, int, int, int);
TRAP_FUNCTION(strtod, double, const char*, char**);
// TRAP_FUNCTION(strtod_l, double, const char*, char**, locale_t);
TRAP_FUNCTION(strtof, float, const char*, char**);
// TRAP_FUNCTION(strtof_l, float, const char*, char**, locale_t);
// TRAP_FUNCTION(strtol, long int, const char*, char**, int);
TRAP_FUNCTION(strtold, long double, const char*, char**);
// TRAP_FUNCTION(strtold_l, long double, const char*, char**, locale_t);
TRAP_FUNCTION(strtoll, long long int, const char*, char**, int);
// TRAP_FUNCTION(strtoll_l, long long int, const char*, char**, int, locale_t);
TRAP_FUNCTION(strtoul, unsigned long int, const char*, char**, int);
TRAP_FUNCTION(strtoull, unsigned long long int, const char*, char**, int);
// TRAP_FUNCTION(strtoull_l,  unsigned long long, const char*, char**, int, locale_t	);
// TRAP_FUNCTION(strxfrm_l, size_t, char          *dest, const char          *src, size_t count, locale_t);
TRAP_FUNCTION(swprintf, int,  wchar_t *buffer, size_t bufsz, const wchar_t* format, ...);
TRAP_FUNCTION(tolower_l, int, int, int);
TRAP_FUNCTION(toupper_l, int, int, int);
TRAP_FUNCTION(towlower_l, int, int, int);
TRAP_FUNCTION(towupper_l, int, int, int);
TRAP_FUNCTION(ungetc, int, int, FILE*);
TRAP_FUNCTION(uselocale, int, int);
TRAP_FUNCTION(vasprintf, int, char **strp, const char *fmt, va_list ap);
TRAP_FUNCTION(vsscanf, int, const char * buffer, const char * format, va_list vlist);
TRAP_FUNCTION(wcrtomb, size_t, char *s, wchar_t wc, mbstate_t *ps);
// TRAP_FUNCTION(wcscoll_l, int, const wchar_t*, const wchar_t*, locale_t);
TRAP_FUNCTION(wcslen, size_t, const wchar_t*);
TRAP_FUNCTION(wcsnrtombs, size_t, char *dest, const wchar_t **src, size_t nwc, size_t len, mbstate_t *ps);
TRAP_FUNCTION(wcstod, double, const char*, char**);
TRAP_FUNCTION(wcstof, float, const char*, char**);
TRAP_FUNCTION(wcstol, long int, const char*, char**, int);
TRAP_FUNCTION(wcstold, long double, const char*, char**);
TRAP_FUNCTION(wcstoll, long long int, const char*, char**, int);
TRAP_FUNCTION(wcstoul, unsigned long int, const char*, char**, int);
TRAP_FUNCTION(wcstoull, unsigned long long int, const char*, char**, int);
// TRAP_FUNCTION(wcsxfrm_l, size_t, wchar_t*, const wchar_t*, size_t, locale_t);
TRAP_FUNCTION(wctob, int, int);
TRAP_FUNCTION(wmemchr, wchar_t*, wchar_t*, wchar_t, size_t);
TRAP_FUNCTION(wmemcmp, int, wchar_t*, wchar_t*, size_t);
TRAP_FUNCTION(wmemcpy, wchar_t*, wchar_t*, wchar_t*, size_t);
TRAP_FUNCTION(wmemmove, wchar_t*, wchar_t*, wchar_t*, size_t);
TRAP_FUNCTION(wmemset, wchar_t*, wchar_t*, wchar_t, size_t);
}
