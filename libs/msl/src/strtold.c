// MSL's string to floating point conversions. Not decompiled yet: __strtold itself.

#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio_api.h>

long double __strtold(int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg, unsigned long *chars_scanned,
                      int *overflow);

double strtod(const char *str, char **end) {
    unsigned long count;
    int overflow;
    double value;
    __InStrCtrl isc;
    isc.NextChar = (char *) str;
    isc.NullCharDetected = 0;
    value = __strtold(INT_MAX, &__StringRead, (void *) &isc, &count, &overflow);
    if (end) {
        *end = (char *) str + count;
    }
    {
        double abs_value = fabs(value);
        if (overflow || (value != 0.0 && (abs_value < DBL_MIN || abs_value > DBL_MAX))) {
            errno = ERANGE;
        }
    }
    return value;
}

double atof(const char *str) {
    return strtod(str, NULL);
}
