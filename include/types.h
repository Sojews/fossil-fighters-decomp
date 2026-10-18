#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef unsigned long long u64;
typedef signed long long s64;

typedef volatile u32 vu32;
typedef volatile u16 vu16;
typedef volatile u64 vu64;
typedef volatile u8 vu8;

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

typedef s32 fx32;   // 20.12 fixed point
#define FX32_SHIFT 12

#endif
