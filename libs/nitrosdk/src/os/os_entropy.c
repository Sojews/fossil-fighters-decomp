// NitroSDK OS: a little entropy from timers, counters, the MAC address and the touch panel and RTC state the
// ARM7 leaves in the system work area.

#include "nitro/os.h"

#define REG_VCOUNT (*(volatile u16 *) 0x04000006)
#define REG_KEYINPUT (*(volatile u16 *) 0x04000130)
#define REG_G3X_GXSTAT (*(volatile u32 *) 0x04000600)
#define HW_BUTTON_XY_BUF (*(volatile u16 *) 0x027fffa8)

typedef struct {
    u8 unk000[0x3c];
    u32 vblankCount;            // 0x03c
    u8 unk040[0x40];
    u8 nvramUserInfo[0x100];    // 0x080
    u8 unk180[0x68];
    u32 unk1e8;
    u32 unk1ec;
    u8 unk1f0[0x1a0];
    u32 unk390;
    u16 unk394;
    u16 unk396;
    u16 unk398;
    u8 unk39a[0x10];
    u16 unk3aa;
    u16 unk3ac;
} OSSystemWork;

static inline OSSystemWork *OS_GetSystemWork(void) {
    return (OSSystemWork *) 0x027ffc00;
}

extern volatile u64 OSi_TickCounter;
u16 OS_GetTickLo(void);

void OS_GetLowEntropyData(u32 buffer[8]) {
    const OSSystemWork *work;
    const u8 *nvram = OS_GetSystemWork()->nvramUserInfo;
    const u8 *macAddress = nvram + 0x74;
    u16 vcount = REG_VCOUNT;
    buffer[0] = (u32) (OS_GetTickLo() | (vcount << 16));
    buffer[1] = (u32) (*(u16 *) (macAddress + 4) << 16) ^ (u32) OSi_TickCounter;
    work = OS_GetSystemWork();
    buffer[2] = (u32) (OSi_TickCounter >> 32) ^ *(u32 *) macAddress ^ work->vblankCount;
    buffer[2] ^= REG_G3X_GXSTAT;
    buffer[3] = work->unk1e8;
    buffer[4] = work->unk1ec;
    buffer[5] = ((u32) work->unk394 << 16) ^ work->unk390;
    buffer[6] = (u32) ((work->unk3aa << 16) | work->unk3ac);
    buffer[7] = ((u32) work->unk398 << 16) | (REG_KEYINPUT | HW_BUTTON_XY_BUF);
}
