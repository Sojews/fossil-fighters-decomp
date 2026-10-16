#include <math.h>

int __msl_count_bits32(unsigned long x) {
    x -= (x >> 1) & 0x55555555;
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x += x >> 8;
    x += x >> 16;
    return (unsigned char) x;
}

int __msl_count_leading_zero32(unsigned long x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return 32 - __msl_count_bits32(x);
}

int __msl_count_bits64(unsigned long long x) {
    x -= (x >> 1) & 0x5555555555555555ULL;
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    x += x >> 8;
    x += x >> 16;
    x += x >> 32;
    return (unsigned char) x;
}

int __signbitd(double x) {
    return __HI(x) & 0x80000000;
}

int __fpclassifyd(double x) {
    switch (__HI(x) & 0x7FF00000) {
        case 0x7FF00000:
            if ((__HI(x) & 0x000FFFFF) || (__LO(x) & 0xFFFFFFFF)) {
                return 1;
            } else {
                return 2;
            }
            break;
        case 0:
            if ((__HI(x) & 0x000FFFFF) || (__LO(x) & 0xFFFFFFFF)) {
                return 5;
            } else {
                return 3;
            }
            break;
    }

    return 4;
}
