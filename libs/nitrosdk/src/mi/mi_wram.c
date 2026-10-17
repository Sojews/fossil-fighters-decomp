// NitroSDK MI: giving the shared WRAM to the ARM9 or ARM7.

#include "nitro/mi.h"

void MI_SetWramBank(int cnt) {
    *(volatile u8 *) 0x04000247 = (u8) cnt;
}
