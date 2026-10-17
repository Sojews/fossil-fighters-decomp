// NitroSDK OS: OS_Init, which brings up the libraries in order.

#include "nitro/os.h"
#include "nitro/pxi.h"

void OS_InitArena(void);
void OS_InitArenaEx(void);
void OS_InitLock(void);
void OS_SetIrqStackChecker(void);
void OS_InitException(void);
void MI_Init(void);
void OS_InitVAlarm(void);
void OSi_InitVramExclusive(void);
void OS_InitThread(void);
void OS_InitReset(void);
void CTRDG_Init(void);
void CARD_Init(void);
void PM_Init(void);
void OSi_WaitVCount0(void);

void OS_Init(void) {
    OS_InitArena();
    PXI_Init();
    OS_InitLock();
    OS_InitArenaEx();
    OS_InitIrqTable();
    OS_SetIrqStackChecker();
    OS_InitException();
    MI_Init();
    OS_InitVAlarm();
    OSi_InitVramExclusive();
    OS_InitThread();
    OS_InitReset();
    CTRDG_Init();
    CARD_Init();
    PM_Init();
    OSi_WaitVCount0();
}
