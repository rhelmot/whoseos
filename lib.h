#include <stdint.h>
#include <stddef.h>

size_t strlen (const char * str);
void * memcpy (void * dest, const void * src, size_t count);
void * memset (void * dest, int c, size_t count);
void kprintf(const char * fmt, int x);
