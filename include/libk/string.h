#include <stdint.h>
#include <stddef.h>

void* memcpy(void* dest, const void* src, size_t num);

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t num);

size_t strlen(const char* str);