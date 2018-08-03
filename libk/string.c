#include <include/libk/string.h>

void* memcpy(void* dest, const void* src, size_t num)
{
    if(!src || !dest)
        return NULL;

    const char* s_ptr = src;
    char* d_ptr = dest;

    for(size_t s = 0; s < num; s++)
        d_ptr[s] = s_ptr[s];

    return dest;
}

char* strcpy(char* dest, const char* src)
{
    if(!src || !dest)
        return NULL;

    while(*src != '\0')
    {
        *dest = *src;

        src++;
        dest++;
    }

    *dest = '\0';
    return dest;
}

char* strncpy(char* dest, const char* src, size_t num)
{
    if(!src || !dest)
        return NULL;

    if(num == 0)
        return dest;

    while(*src != '\0')
    {
        *dest = *src;

        src++;
        dest++;
        num--;

        if(num == 0)
            break;
    }

    while(num > 0)
    {
        *dest = '\0';
        dest++;

        num--;
    }

    return dest;
}

size_t strlen(const char* src)
{
    if(!src)
        return 0;

    size_t len = 0;

    while(*src != '\0')
    {
        len++;
        src++;
    }

    return len;
}
