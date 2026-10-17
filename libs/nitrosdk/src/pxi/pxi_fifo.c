// NitroSDK PXI: the FIFO between the ARM9 and the ARM7. Each word carries a tag picking the receiver's
// callback, an error flag and 26 bits of data.

#include "nitro/os.h"
#include "nitro/pxi.h"

#define REG_PXI_SUBPINTF (*(volatile u16 *) 0x04000180)
#define REG_PXI_FIFO_CNT (*(volatile u16 *) 0x04000184)
#define REG_PXI_SEND_FIFO (*(volatile u32 *) 0x04000188)
#define REG_PXI_RECV_FIFO (*(volatile u32 *) 0x04100000)

#define FIFO_CNT_SEND_FULL 0x0002
#define FIFO_CNT_RECV_EMPTY 0x0100
#define FIFO_CNT_ERR 0x4000
#define FIFO_CNT_ERR_CLEAR 0xc000

#define OS_IE_FIFO_RECV (1 << 18)
// The system work area at the end of main memory.
typedef struct {
    u8 unk000[0x388];
    u32 pxiHandleChecker[2];    // Which FIFO tags have a callback, per processor
} OSSystemWork;
static inline OSSystemWork *OS_GetSystemWork(void) {
    return (OSSystemWork *) 0x027ffc00;
}
#define PXI_HANDLE_CHECKER (OS_GetSystemWork()->pxiHandleChecker)

enum {
    PXI_FIFO_SUCCESS = 0,
    PXI_FIFO_FAIL_SEND_ERR = -1,
    PXI_FIFO_FAIL_SEND_FULL = -2,
    PXI_FIFO_FAIL_RECV_ERR = -3,
    PXI_FIFO_FAIL_RECV_EMPTY = -4
};

typedef union {
    u32 raw;
    struct {
        u32 tag : 5;
        u32 err : 1;
        u32 data : 26;
    } e;
} PXIFifoMessage;

static PXIFifoCallback FifoRecvCallbackTable[32];
static u16 FifoCtrlInit;

void PXIi_HandlerRecvFifoNotEmpty(void);

void PXI_Init(void) {
    PXI_InitFifo();
}

void PXI_InitFifo(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    int i;
    s32 timeout;
    int sync;
    if (!FifoCtrlInit) {
        FifoCtrlInit = TRUE;
        PXI_HANDLE_CHECKER[0] = 0;
        for (i = 0; i < 32; i++) {
            FifoRecvCallbackTable[i] = NULL;
        }
        REG_PXI_FIFO_CNT = 0xc408;
        OS_ResetRequestIrqMask(OS_IE_FIFO_RECV);
        OS_SetIrqFunction(OS_IE_FIFO_RECV, PXIi_HandlerRecvFifoNotEmpty);
        OS_EnableIrqMask(OS_IE_FIFO_RECV);
        for (i = 0;; i++) {
            sync = REG_PXI_SUBPINTF & 0xf;
            REG_PXI_SUBPINTF = sync << 8;
            if (sync == 0 && i > 4) {
                break;
            }
            timeout = 1000;
            while ((REG_PXI_SUBPINTF & 0xf) == sync) {
                if (timeout <= 0) {
                    i = 0;
                    break;
                }
                timeout--;
            }
        }
    }
    OS_RestoreInterrupts(enabled);
}

void PXI_SetFifoRecvCallback(int fifotag, PXIFifoCallback callback) {
    OSIntrMode enabled = OS_DisableInterrupts();
    FifoRecvCallbackTable[fifotag] = callback;
    if (callback != NULL) {
        PXI_HANDLE_CHECKER[0] |= 1 << fifotag;
    } else {
        PXI_HANDLE_CHECKER[0] &= ~(1 << fifotag);
    }
    OS_RestoreInterrupts(enabled);
}

BOOL PXI_IsCallbackReady(int fifotag, int proc) {
    OSSystemWork *work = OS_GetSystemWork();
    return (work->pxiHandleChecker[proc] & (1 << fifotag)) ? TRUE : FALSE;
}

int PXI_SendWordByFifo(int fifotag, u32 data, BOOL err) {
    PXIFifoMessage fifomsg;
    OSIntrMode enabled;
    fifomsg.e.tag = fifotag;
    fifomsg.e.err = err;
    fifomsg.e.data = data;
    if (REG_PXI_FIFO_CNT & FIFO_CNT_ERR) {
        REG_PXI_FIFO_CNT |= FIFO_CNT_ERR_CLEAR;
        return PXI_FIFO_FAIL_SEND_ERR;
    }
    enabled = OS_DisableInterrupts();
    if (REG_PXI_FIFO_CNT & FIFO_CNT_SEND_FULL) {
        OS_RestoreInterrupts(enabled);
        return PXI_FIFO_FAIL_SEND_FULL;
    }
    REG_PXI_SEND_FIFO = fifomsg.raw;
    OS_RestoreInterrupts(enabled);
    return PXI_FIFO_SUCCESS;
}

static inline int PXIi_GetFromFifo(u32 *data) {
    OSIntrMode enabled;
    if (REG_PXI_FIFO_CNT & FIFO_CNT_ERR) {
        REG_PXI_FIFO_CNT |= FIFO_CNT_ERR_CLEAR;
        return PXI_FIFO_FAIL_RECV_ERR;
    }
    enabled = OS_DisableInterrupts();
    if (REG_PXI_FIFO_CNT & FIFO_CNT_RECV_EMPTY) {
        OS_RestoreInterrupts(enabled);
        return PXI_FIFO_FAIL_RECV_EMPTY;
    }
    *data = REG_PXI_RECV_FIFO;
    OS_RestoreInterrupts(enabled);
    return PXI_FIFO_SUCCESS;
}

static inline int PXIi_SetToFifo(u32 data) {
    OSIntrMode enabled;
    if (REG_PXI_FIFO_CNT & FIFO_CNT_ERR) {
        REG_PXI_FIFO_CNT |= FIFO_CNT_ERR_CLEAR;
        return PXI_FIFO_FAIL_SEND_ERR;
    }
    enabled = OS_DisableInterrupts();
    if (REG_PXI_FIFO_CNT & FIFO_CNT_SEND_FULL) {
        OS_RestoreInterrupts(enabled);
        return PXI_FIFO_FAIL_SEND_FULL;
    }
    REG_PXI_SEND_FIFO = data;
    OS_RestoreInterrupts(enabled);
    return PXI_FIFO_SUCCESS;
}

void PXIi_HandlerRecvFifoNotEmpty(void) {
    PXIFifoMessage fifomsg;
    int ret;
    int tag;
    for (;;) {
        ret = PXIi_GetFromFifo(&fifomsg.raw);
        if (ret == PXI_FIFO_FAIL_RECV_EMPTY) {
            break;
        }
        if (ret == PXI_FIFO_FAIL_RECV_ERR) {
            continue;
        }
        tag = fifomsg.e.tag;
        if (tag == 0) {
            continue;
        }
        if (FifoRecvCallbackTable[tag] != NULL) {
            FifoRecvCallbackTable[tag](tag, fifomsg.e.data, fifomsg.e.err);
        } else if (!fifomsg.e.err) {
            fifomsg.e.err = TRUE;
            PXIi_SetToFifo(fifomsg.raw);
        }
    }
}
