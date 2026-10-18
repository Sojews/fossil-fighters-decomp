// NitroSDK WVR: starting and stopping the ARM7's wireless library, which runs in VRAM banks C and/or D lent to the
// ARM7 for the purpose.

#include "nitro/os.h"
#include "nitro/pxi.h"

#define PXI_FIFO_TAG_WVR 15
#define PXI_PROC_ARM9 0
#define OS_LOCK_ID_ERROR (-3)
#define OS_VRAM_BANK_ID_C 0x4
#define OS_VRAM_BANK_ID_D 0x8

#define reg_GX_VRAMCNT_C (*(vu8 *) 0x04000242)
#define reg_GX_VRAMCNT_D (*(vu8 *) 0x04000243)

#define WVR_PXI_COMMAND_STARTUP 0x00010000
#define WVR_PXI_COMMAND_TERMINATE 0x00020000

typedef enum {
    GX_VRAM_ARM7_128_C = 0x4,
    GX_VRAM_ARM7_128_D = 0x8,
    GX_VRAM_ARM7_256_CD = 0xC
} GXVRamARM7;

typedef enum {
    WVR_RESULT_SUCCESS = 0,
    WVR_RESULT_OPERATING,
    WVR_RESULT_DISABLE,
    WVR_RESULT_INVALID_PARAM,
    WVR_RESULT_FIFO_ERROR,
    WVR_RESULT_ILLEGAL_STATUS,
    WVR_RESULT_VRAM_LOCKED,
    WVR_RESULT_FATAL_ERROR
} WVRResult;

typedef void (*WVRCallbackFunc)(void *arg, WVRResult result);

s32 OS_GetLockID(void);
BOOL OSi_TryLockVram(u16 bank, u16 lockId);
void OSi_UnlockVram(u16 bank, u16 lockId);

#pragma thumb off

static void WvrReceiveCallback(int tag, u32 data, BOOL err);
static void WvrDummyAsyncCallback(void *arg, WVRResult result);

static WVRCallbackFunc wvrCallback = NULL;
static void *wvrArg = NULL;
static vu16 wvrVRam = 0;
static vu16 wvrLockId = 0;

// Maps the banks to the ARM7 (bank C at 0x06000000, D after it) and asks it to start.
WVRResult WVR_StartUpAsync(GXVRamARM7 vram, WVRCallbackFunc callback, void *arg) {
    OSIntrMode e;

    PXI_Init();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM7)) {
        return WVR_RESULT_DISABLE;
    }
    while (wvrLockId == 0) {
        s32 result = OS_GetLockID();
        if (result == OS_LOCK_ID_ERROR) {
            return WVR_RESULT_FATAL_ERROR;
        }
        wvrLockId = (u16) result;
    }
    e = OS_DisableInterrupts();
    if (wvrCallback != NULL) {
        OS_RestoreInterrupts(e);
        return WVR_RESULT_ILLEGAL_STATUS;
    }
    if (wvrVRam) {
        OS_RestoreInterrupts(e);
        return WVR_RESULT_ILLEGAL_STATUS;
    }
    switch (vram) {
    case GX_VRAM_ARM7_128_C:
        if (!OSi_TryLockVram(OS_VRAM_BANK_ID_C, wvrLockId)) {
            OS_RestoreInterrupts(e);
            return WVR_RESULT_VRAM_LOCKED;
        }
        wvrVRam = OS_VRAM_BANK_ID_C;
        reg_GX_VRAMCNT_C = (2 << 0) | (0 << 3) | (1 << 7);
        break;
    case GX_VRAM_ARM7_128_D:
        if (!OSi_TryLockVram(OS_VRAM_BANK_ID_D, wvrLockId)) {
            OS_RestoreInterrupts(e);
            return WVR_RESULT_VRAM_LOCKED;
        }
        wvrVRam = OS_VRAM_BANK_ID_D;
        reg_GX_VRAMCNT_D = (2 << 0) | (0 << 3) | (1 << 7);
        break;
    case GX_VRAM_ARM7_256_CD:
        if (!OSi_TryLockVram(OS_VRAM_BANK_ID_C | OS_VRAM_BANK_ID_D, wvrLockId)) {
            OS_RestoreInterrupts(e);
            return WVR_RESULT_VRAM_LOCKED;
        }
        wvrVRam = OS_VRAM_BANK_ID_C | OS_VRAM_BANK_ID_D;
        reg_GX_VRAMCNT_C = (2 << 0) | (0 << 3) | (1 << 7);
        reg_GX_VRAMCNT_D = (2 << 0) | (1 << 3) | (1 << 7);
        break;
    default:
        OS_RestoreInterrupts(e);
        return WVR_RESULT_INVALID_PARAM;
    }
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM9)) {
        PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WVR, WvrReceiveCallback);
    }
    if (callback == NULL) {
        wvrCallback = WvrDummyAsyncCallback;
    } else {
        wvrCallback = callback;
    }
    wvrArg = arg;
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_WVR, WVR_PXI_COMMAND_STARTUP, FALSE)) {
        OSi_UnlockVram(wvrVRam, wvrLockId);
        wvrVRam = 0;
        wvrCallback = NULL;
        OS_RestoreInterrupts(e);
        return WVR_RESULT_FIFO_ERROR;
    }
    OS_RestoreInterrupts(e);
    return WVR_RESULT_OPERATING;
}

WVRResult WVR_TerminateAsync(WVRCallbackFunc callback, void *arg) {
    OSIntrMode e;

    PXI_Init();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM7)) {
        return WVR_RESULT_DISABLE;
    }
    e = OS_DisableInterrupts();
    if (wvrCallback != NULL) {
        OS_RestoreInterrupts(e);
        return WVR_RESULT_ILLEGAL_STATUS;
    }
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM9)) {
        PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WVR, WvrReceiveCallback);
    }
    if (callback == NULL) {
        wvrCallback = WvrDummyAsyncCallback;
    } else {
        wvrCallback = callback;
    }
    wvrArg = arg;
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_WVR, WVR_PXI_COMMAND_TERMINATE, FALSE)) {
        wvrCallback = NULL;
        OS_RestoreInterrupts(e);
        return WVR_RESULT_FIFO_ERROR;
    }
    OS_RestoreInterrupts(e);
    return WVR_RESULT_OPERATING;
}

// The ARM7's answer: the banks are given back once it has stopped (or failed to start).
static void WvrReceiveCallback(int tag, u32 data, BOOL err) {
    WVRCallbackFunc cb = wvrCallback;
    void *cbArg = wvrArg;
    WVRResult result = (WVRResult) (data & 0xFF);

    switch (data & 0xFFFF0000) {
    case WVR_PXI_COMMAND_STARTUP:
        switch (result) {
        case WVR_RESULT_FATAL_ERROR:
            if (wvrVRam != 0 && wvrLockId != 0) {
                OSi_UnlockVram(wvrVRam, wvrLockId);
                wvrVRam = 0;
            }
            break;
        }
        break;
    case WVR_PXI_COMMAND_TERMINATE:
        switch (result) {
        case WVR_RESULT_SUCCESS:
            if (wvrVRam != 0 && wvrLockId != 0) {
                OSi_UnlockVram(wvrVRam, wvrLockId);
                wvrVRam = 0;
            }
            break;
        }
        PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WVR, NULL);
        break;
    }
    if (cb != NULL) {
        wvrCallback = NULL;
        wvrArg = NULL;
        cb(cbArg, result);
    }
}

static void WvrDummyAsyncCallback(void *arg, WVRResult result) {
}

#pragma thumb reset
