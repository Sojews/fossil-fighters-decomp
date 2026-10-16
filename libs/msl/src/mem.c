#include <mem.h>
#include <mem_funcs.h>
#include <stddef.h>

void *memcpy(void *dst, const void *src, int n) {
    const char *p = (const char *) src;
    char *q       = (char *) dst;

    for (n++; --n;) {
        *q++ = *p++;
    }

    return dst;
}

void *memmove(void *dst, const void *src, size_t len) {
    const char *csrc;
    char *cdst;

    int reverse = (unsigned int) src < (unsigned int) dst;

    if (!reverse) {
        for (csrc = (const char *) src, cdst = (char *) dst, len++; --len;) {
            *cdst++ = *csrc++;
        }
    } else {
        for (csrc = (const char *) src + len, cdst = (char *) dst + len, len++; --len;) {
            *--cdst = *--csrc;
        }
    }

    return dst;
}

void *memset(void *dest, int val, int n) {
    __fill_mem(dest, val, n);
    return dest;
}

void *memchr(const void *src, int val, int n) {
    const unsigned char *p;

    unsigned int v = (val & 0xFF);

    for (p = (unsigned char *) src, n++; --n;) {
        if (*p++ == v) {
            return ((void *) (p - 1));
        }
    }

    return NULL;
}

void *__memrchr(const void *src, int val, int n) {
    const unsigned char *p;

    unsigned int v = (val & 0xFF);

    for (p = (unsigned char *) src + n, n++; --n;) {
        if (*--p == v) {
            return ((void *) p);
        }
    }

    return NULL;
}

int memcmp(const void *src1, const void *src2, int n) {
    const unsigned char *p1;
    const unsigned char *p2;

    for (p1 = (const unsigned char *) src1, p2 = (const unsigned char *) src2, n++; --n;) {
        if (*p1++ != *p2++) {
            return ((*--p1 < *--p2) ? -1 : +1);
        }
    }

    return 0;
}
