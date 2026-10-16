#include <locale.h>
#include <mbstring.h>
#include <stddef.h>

char *strncpy(char *dest, const char *src, size_t n);
size_t strlen(const char *str);

int mbtowc(wchar_t *pwc, const char *s, int n) {
    return _current_locale.ctype_cmpt_ptr->decode_mb(pwc, s, n);
}

int __mbtowc_noconv(wchar_t *pwc, const char *str, int n) {
    const char *s = str;

    if (s == NULL) {
        return 0;
    }

    if (n == 0) {
        return -1;
    }

    if (pwc != NULL) {
        *pwc = (unsigned char) *s;
    }

    if (*s == 0) {
        return 0;
    }

    return 1;
}

int __wctomb_noconv(char *s, wchar_t wchar) {
    if (s == NULL) {
        return 0;
    }
    *s = wchar;
    return 1;
}

int wctomb(char *s, wchar_t wchar) {
    return _current_locale.ctype_cmpt_ptr->encode_wc(s, wchar);
}

size_t mbstowcs(wchar_t *pDest, const char *pSrc, size_t num) {
    int res;
    const char *src;
    int count;
    size_t src_len;

    src_len = strlen(pSrc);

    if (pDest != 0) {
        src = pSrc;

        for (count = 0; count < num; count++) {
            if (*src) {
                res = mbtowc(pDest++, src, src_len);

                if (res > 0) {
                    src += res;
                    src_len -= res;
                } else {
                    return -1;
                }
            } else {
                *pDest = 0;
                break;
            }
        }
    } else {
        count = 0;
    }

    return count;
}

size_t wcstombs(char *pDest, const wchar_t *pSrc, size_t num) {
    int count = 0;
    int res;
    char temp[4];

    if (pDest == NULL || pSrc == NULL) {
        return 0;
    }

    while (count <= num) {
        if (*pSrc == 0) {
            pDest[count] = 0;
            break;
        }
        res = wctomb(temp, *pSrc++);
        if (count + res > num) {
            break;
        }
        strncpy(pDest + count, temp, res);
        count += res;
    }

    return count;
}
