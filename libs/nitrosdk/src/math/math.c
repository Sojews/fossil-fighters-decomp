// NitroSDK MATH: counting set bits.

#include "types.h"

u8 MATH_CountPopulation(u32 x) {
    x -= (x >> 1) & 0x55555555;
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x += x >> 4;
    x &= 0x0F0F0F0F;
    x += x >> 8;
    x += x >> 16;
    return (u8) x;
}
