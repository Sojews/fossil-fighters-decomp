#include <mem.h>

wchar_t *wmemcpy(wchar_t *pDest, const wchar_t *pSrc, size_t num) {
    return (wchar_t *) memcpy(pDest, pSrc, num * sizeof(wchar_t));
}

wchar_t *wmemmove(wchar_t *pDest, const wchar_t *pSrc, size_t num) {
    return (wchar_t *) memmove(pDest, pSrc, num * sizeof(wchar_t));
}

wchar_t *wmemchr(const wchar_t *pSrc, wchar_t val, size_t num) {
    while (num != 0) {
        if (*pSrc == val) {
            return (wchar_t *) pSrc;
        }

        pSrc++;
        num--;
    }

    return 0;
}

int wmemcmp(const wchar_t *pSrc1, const wchar_t *pSrc2, size_t num) {
    int diff = 0;

    while (num != 0) {
        diff = *pSrc1 - *pSrc2;
        if (diff != 0) {
            break;
        }
        pSrc1++;
        pSrc2++;
        num--;
    }

    return diff;
}
