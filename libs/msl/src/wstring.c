#include <wstring.h>

size_t wcslen(const wchar_t *str) {
    size_t len = -1;

    do {
        len++;
    } while (*str++ != 0);

    return len;
}

wchar_t *wcscpy(wchar_t *dest, const wchar_t *src) {
    const wchar_t *p = src;
    wchar_t *q       = dest;

    while ((*q++ = *p++) != L'\0')
        ;

    return dest;
}

const wchar_t *wcschr(const wchar_t *str, wchar_t chr) {
    const wchar_t *p = str;
    wchar_t c        = chr;
    wchar_t ch;

    while (ch = *p++) {
        if (ch == c) {
            return p - 1;
        }
    }

    return c ? NULL : p - 1;
}
