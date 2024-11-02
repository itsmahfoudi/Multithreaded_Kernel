#include "memory.h"


void *memset(void *dest, int val, unsigned int len) {
    unsigned char *ptr = (unsigned char *)dest;
    while(len-- > 0) {
        *ptr++ = val;
    }
    return dest;
}

int memcmp(const void *aptr, const void *bptr, unsigned int len) {
    const unsigned char *a = (const unsigned char *)aptr;
    const unsigned char *b = (const unsigned char *)bptr;
    for (unsigned int i = 0; i < len; i++) {
        if (a[i] < b[i]) {
            return -1;
        } else if (a[i] > b[i]) {
            return 1;
        }
    }
    return 0;
}