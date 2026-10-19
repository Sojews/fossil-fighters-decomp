// The BIOS calls (the NitroSDK's libsyscall), which the build puts between the secure area's data in the first 2KB
// of ARM9 code.

#include "types.h"

asm void CpuSet(const void *src, void *dest, u32 ctrl) {
    swi 0xB
    bx lr
}

asm void CpuFastSet(const void *src, void *dest, u32 ctrl) {
    swi 0xC
    bx lr
}

asm s32 Div(s32 number, s32 denom) {
    swi 0x9
    bx lr
}

asm u32 IsDebugger(void) {
    swi 0xF
    bx lr
}

asm void LZ77UnCompReadByCallbackWrite16bit(const void *src, void *dest, void *param, const void *callbacks) {
    swi 0x12
    bx lr
}

asm void RLUnCompReadByCallbackWrite16bit(const void *src, void *dest, void *param, const void *callbacks) {
    swi 0x15
    bx lr
}

asm void IntrWait(u32 clear, u32 flags) {
    swi 0x4
    bx lr
}

asm u16 Sqrt(u32 src) {
    swi 0xD
    bx lr
}

asm s32 Mod(s32 number, s32 denom) {
    swi 0x9
    mov r0, r1
    bx lr
}

asm void LZ77UnCompReadNormalWrite8bit(const void *src, void *dest) {
    swi 0x11
    bx lr
}

asm u16 GetCRC16(u16 start, const void *data, u32 size) {
    swi 0xE
    bx lr
}

asm void BitUnPack(const void *src, void *dest, const void *param) {
    swi 0x10
    bx lr
}

asm void RLUnCompReadNormalWrite8bit(const void *src, void *dest) {
    swi 0x14
    bx lr
}

asm void SoftReset(void) {
    swi 0x0
    bx lr
}

asm void Halt(void) {
    swi 0x6
    bx lr
}

asm void HuffUnCompReadByCallback(const void *src, void *dest, void *param, const void *callbacks) {
    swi 0x13
    bx lr
}

asm void WaitByLoop(s32 count) {
    swi 0x3
    bx lr
}

asm void VBlankIntrWait(void) {
    mov r2, #0
    swi 0x5
    bx lr
}
