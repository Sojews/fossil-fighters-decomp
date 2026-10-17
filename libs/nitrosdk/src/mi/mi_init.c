// NitroSDK MI: setting up the memory system.

#include "nitro/mi.h"

void MI_SetWramBank(int cnt);

void MI_Init(void) {
    MI_SetWramBank(3);
    MI_StopDma(0);
}
