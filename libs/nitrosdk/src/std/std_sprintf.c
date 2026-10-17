// NitroSDK STD: a small vsnprintf (no floating point) that also passes Shift-JIS double-byte characters through.

#include "types.h"
#include <stdarg.h>

// The output: room left, write position, start.
typedef struct {
    u32 len;
    char *cur;
    char *base;
} dst_string;

enum {
    flag_blank = 0x0001,
    flag_plus = 0x0002,
    flag_sharp = 0x0004,
    flag_minus = 0x0008,
    flag_zero = 0x0010,
    flag_l1 = 0x0020,
    flag_h1 = 0x0040,
    flag_l2 = 0x0080,
    flag_h2 = 0x0100,
    flag_unsigned = 0x1000
};

int STD_TVSNPrintf(char *dst, u32 len, const char *fmt, va_list vlist);

static void string_put_char(dst_string *p, char c) {
    if (p->len > 0) {
        *p->cur = c;
        --p->len;
    }
    ++p->cur;
}

static void string_fill_char(dst_string *p, char c, int n) {
    if (n > 0) {
        u32 i;
        u32 k = p->len;
        if (k > (u32) n) {
            k = (u32) n;
        }
        for (i = 0; i < k; ++i) {
            p->cur[i] = c;
        }
        p->len -= k;
        p->cur += n;
    }
}

static void string_put_string(dst_string *p, const char *s, int n) {
    if (n > 0) {
        u32 i;
        u32 k = p->len;
        if (k > (u32) n) {
            k = (u32) n;
        }
        for (i = 0; i < k; ++i) {
            p->cur[i] = s[i];
        }
        p->len -= k;
        p->cur += n;
    }
}

int OS_VSPrintf(char *dst, const char *fmt, va_list vlist);

int OS_SPrintf(char *dst, const char *fmt, ...) {
    int ret;
    va_list vlist;
    va_start(vlist, fmt);
    ret = OS_VSPrintf(dst, fmt, vlist);
    va_end(vlist);
    return ret;
}

int OS_VSPrintf(char *dst, const char *fmt, va_list vlist) {
    return STD_TVSNPrintf(dst, 0x7fffffff, fmt, vlist);
}

int STD_TVSNPrintf(char *dst, u32 len, const char *fmt, va_list vlist) {
    char buf[24];
    int n_buf;
    char prefix[2];
    int n_prefix;
    const char *s = fmt;
    dst_string str;

    str.len = len;
    str.cur = str.base = dst;

    while (*s) {
        if ((u32) (((u8) *s ^ 0x20) - 0xa1) < 0x3c) {
            // The first byte of a Shift-JIS character: copy both bytes.
            string_put_char(&str, *s++);
            if (*s) {
                string_put_char(&str, *s++);
            }
        } else if (*s != '%') {
            string_put_char(&str, *s++);
        } else {
            int flag = 0;
            int width = 0;
            int precision = -1;
            int radix = 10;
            char hex_char = 'a' - 10;
            const char *p_start = s;

            for (;;) {
                switch (*++s) {
                case '+':
                    if (s[-1] != ' ') {
                        break;
                    }
                    flag |= flag_plus;
                    continue;
                case ' ':
                    flag |= flag_blank;
                    continue;
                case '-':
                    flag |= flag_minus;
                    continue;
                case '0':
                    flag |= flag_zero;
                    continue;
                }
                break;
            }

            if (*s == '*') {
                ++s;
                width = va_arg(vlist, int);
                if (width < 0) {
                    width = -width;
                    flag |= flag_minus;
                }
            } else {
                while (*s >= '0' && *s <= '9') {
                    width = (width * 10) + *s++ - '0';
                }
            }

            if (*s == '.') {
                ++s;
                precision = 0;
                if (*s == '*') {
                    ++s;
                    precision = va_arg(vlist, int);
                    if (precision < 0) {
                        precision = -1;
                    }
                } else {
                    while (*s >= '0' && *s <= '9') {
                        precision = (precision * 10) + *s++ - '0';
                    }
                }
            }

            switch (*s) {
            case 'h':
                if (*++s != 'h') {
                    flag |= flag_h1;
                } else {
                    ++s;
                    flag |= flag_h2;
                }
                break;
            case 'l':
                if (*++s != 'l') {
                    flag |= flag_l1;
                } else {
                    ++s;
                    flag |= flag_l2;
                }
                break;
            }

            switch (*s) {
            case 'd':
            case 'i':
                goto put_integer;
            case 'o':
                radix = 8;
                flag |= flag_unsigned;
                goto put_integer;
            case 'u':
                flag |= flag_unsigned;
                goto put_integer;
            case 'X':
                hex_char = 'A' - 10;
                goto put_hexadecimal;
            case 'x':
                goto put_hexadecimal;
            case 'p':
                flag |= flag_sharp;
                precision = 8;
                goto put_hexadecimal;
            case 'c':
                if (precision >= 0) {
                    goto put_invalid;
                }
                {
                    int c = va_arg(vlist, int);
                    if (flag & flag_minus) {
                        string_put_char(&str, (char) c);
                        string_fill_char(&str, ' ', width - 1);
                    } else {
                        char pad = (char) ((flag & flag_zero) ? '0' : ' ');
                        string_fill_char(&str, pad, width - 1);
                        string_put_char(&str, (char) c);
                    }
                    ++s;
                }
                break;
            case 's': {
                int n_bufs = 0;
                const char *p_bufs = va_arg(vlist, const char *);
                if (precision < 0) {
                    while (p_bufs[n_bufs]) {
                        ++n_bufs;
                    }
                } else {
                    while (n_bufs < precision && p_bufs[n_bufs]) {
                        ++n_bufs;
                    }
                }
                width -= n_bufs;
                if (flag & flag_minus) {
                    string_put_string(&str, p_bufs, n_bufs);
                    string_fill_char(&str, ' ', width);
                } else {
                    char pad = (char) ((flag & flag_zero) ? '0' : ' ');
                    string_fill_char(&str, pad, width);
                    string_put_string(&str, p_bufs, n_bufs);
                }
                ++s;
                break;
            }
            case 'n': {
                int count = str.cur - str.base;
                if (flag & flag_h2) {
                } else if (flag & flag_h1) {
                    *va_arg(vlist, s16 *) = (s16) count;
                } else if (flag & flag_l2) {
                    *va_arg(vlist, s64 *) = count;
                } else {
                    *va_arg(vlist, int *) = count;
                }
                ++s;
                break;
            }
            case '%':
                if (p_start + 1 != s) {
                    goto put_invalid;
                }
                string_put_char(&str, *s++);
                break;
            default:
            put_invalid:
                string_put_string(&str, p_start, s - p_start);
                break;

            put_hexadecimal:
                radix = 16;
                flag |= flag_unsigned;
            put_integer: {
                u64 val = 0;
                n_prefix = 0;
                if (flag & flag_minus) {
                    flag &= ~flag_zero;
                }
                if (precision < 0) {
                    precision = 1;
                } else {
                    flag &= ~flag_zero;
                }

                if (flag & flag_unsigned) {
                    if (flag & flag_h2) {
                        val = va_arg(vlist, u8);
                    } else if (flag & flag_h1) {
                        val = va_arg(vlist, u16);
                    } else if (flag & flag_l2) {
                        val = va_arg(vlist, u64);
                    } else {
                        val = va_arg(vlist, u32);
                    }
                    flag &= ~(flag_plus | flag_blank);
                    if (flag & flag_sharp) {
                        if (radix == 16) {
                            if (val != 0) {
                                prefix[0] = (char) (hex_char + (10 + 'x' - 'a'));
                                prefix[1] = '0';
                                n_prefix = 2;
                            }
                        } else if (radix == 8) {
                            prefix[0] = '0';
                            n_prefix = 1;
                        }
                    }
                } else {
                    if (flag & flag_h2) {
                        val = va_arg(vlist, s8);
                    } else if (flag & flag_h1) {
                        val = va_arg(vlist, s16);
                    } else if (flag & flag_l2) {
                        val = va_arg(vlist, u64);
                    } else {
                        val = va_arg(vlist, s32);
                    }
                    if ((val >> 32) & 0x80000000) {
                        val = ~val + 1;
                        prefix[0] = '-';
                        n_prefix = 1;
                    } else if (val || precision) {
                        if (flag & flag_plus) {
                            prefix[0] = '+';
                            n_prefix = 1;
                        } else if (flag & flag_blank) {
                            prefix[0] = ' ';
                            n_prefix = 1;
                        }
                    }
                }

                n_buf = 0;
                switch (radix) {
                case 8:
                    while (val != 0) {
                        int d = (int) (val & 0x07);
                        val >>= 3;
                        buf[n_buf++] = (char) (d + '0');
                    }
                    break;
                case 10:
                    if ((val >> 32) == 0) {
                        u32 v = (u32) val;
                        while (v != 0) {
                            u32 r = v / 10;
                            int d = (int) (v - (r * 10));
                            v = r;
                            buf[n_buf++] = (char) (d + '0');
                        }
                    } else {
                        while (val != 0) {
                            u64 r = val / 10;
                            int d = (int) (val - (r * 10));
                            val = r;
                            buf[n_buf++] = (char) (d + '0');
                        }
                    }
                    break;
                case 16:
                    while (val != 0) {
                        int d = (int) (val & 0x0f);
                        val >>= 4;
                        buf[n_buf++] = (char) ((d < 10) ? (d + '0') : (d + hex_char));
                    }
                    break;
                }
                if (n_prefix > 0 && prefix[0] == '0') {
                    n_prefix = 0;
                    buf[n_buf++] = '0';
                }
            }

                {
                    int n_pad = precision - n_buf;
                    if (flag & flag_zero) {
                        if (n_pad < width - n_buf - n_prefix) {
                            n_pad = width - n_buf - n_prefix;
                        }
                    }
                    if (n_pad > 0) {
                        width -= n_pad;
                    }
                    width -= n_prefix + n_buf;
                    if (!(flag & flag_minus)) {
                        string_fill_char(&str, ' ', width);
                    }
                    while (n_prefix > 0) {
                        string_put_char(&str, prefix[--n_prefix]);
                    }
                    string_fill_char(&str, '0', n_pad);
                    while (n_buf > 0) {
                        string_put_char(&str, buf[--n_buf]);
                    }
                    if (flag & flag_minus) {
                        string_fill_char(&str, ' ', width);
                    }
                    ++s;
                }
                break;
            }
        }
    }

    if (str.len > 0) {
        *str.cur = '\0';
    } else if (len > 0) {
        str.base[len - 1] = '\0';
    }
    return str.cur - str.base;
}
