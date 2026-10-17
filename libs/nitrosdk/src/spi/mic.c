// NitroSDK SPI: the microphone. Requests go to the ARM7 over the PXI FIFO; it samples into a buffer in main
// memory and reports back on the same tag.

#include "nitro/os.h"
#include "nitro/pxi.h"

#define PXI_FIFO_TAG_MIC 9

#define SPI_PXI_START_BIT 0x02000000
#define SPI_PXI_END_BIT 0x01000000
#define SPI_PXI_INDEX_SHIFT 16
#define SPI_PXI_RESULT_COMMAND_MASK 0x7f00
#define SPI_PXI_RESULT_COMMAND_SHIFT 8
#define SPI_PXI_RESULT_DATA_MASK 0x00ff

#define SPI_PXI_COMMAND_MIC_SAMPLING 0x40
#define SPI_PXI_COMMAND_MIC_AUTO_ON 0x41
#define SPI_PXI_COMMAND_MIC_AUTO_OFF 0x42
#define SPI_PXI_COMMAND_MIC_BUFFER_FULL 0x51

#define MIC_SAMPLING_RATE_LIMIT 1024

// Where the ARM7 leaves the latest sample and its address.
#define MIC_LAST_SAMPLING_ADDRESS (*(void **) 0x027fff90)
#define MIC_LAST_SAMPLING_DATA (*(u16 *) 0x027fff94)

typedef enum {
    MIC_RESULT_SUCCESS = 0,
    MIC_RESULT_BUSY,
    MIC_RESULT_ILLEGAL_PARAMETER,
    MIC_RESULT_SEND_ERROR,
    MIC_RESULT_INVALID_COMMAND,
    MIC_RESULT_ILLEGAL_STATUS,
    MIC_RESULT_FATAL_ERROR
} MICResult;

typedef enum {
    MIC_SAMPLING_TYPE_8BIT = 0,
    MIC_SAMPLING_TYPE_12BIT,
    MIC_SAMPLING_TYPE_SIGNED_8BIT,
    MIC_SAMPLING_TYPE_SIGNED_12BIT,
    MIC_SAMPLING_TYPE_12BIT_FILTER_OFF,
    MIC_SAMPLING_TYPE_SIGNED_12BIT_FILTER_OFF
} MICSamplingType;

enum {
    SPI_MIC_SAMPLING_TYPE_8BIT = 0x00,
    SPI_MIC_SAMPLING_TYPE_12BIT = 0x01,
    SPI_MIC_SAMPLING_TYPE_SIGNED_8BIT = 0x02,
    SPI_MIC_SAMPLING_TYPE_SIGNED_12BIT = 0x03,
    SPI_MIC_SAMPLING_TYPE_FILTER_OFF = 0x04,
    SPI_MIC_SAMPLING_TYPE_LOOP_AUTO = 0x10
};

// The ARM7's status codes.
enum {
    SPI_PXI_RESULT_SUCCESS = 0,
    SPI_PXI_RESULT_INVALID_COMMAND,
    SPI_PXI_RESULT_INVALID_PARAMETER,
    SPI_PXI_RESULT_ILLEGAL_STATUS,
    SPI_PXI_RESULT_EXCLUSIVE
};

typedef void (*MICCallback)(MICResult result, void *arg);

typedef struct {
    MICSamplingType type;       // 0x00
    void *buffer;               // 0x04: 32-byte aligned
    u32 size;                   // 0x08: a multiple of 32
    u32 rate;                   // 0x0c: in ARM7 cycles
    BOOL loop_enable;           // 0x10
    MICCallback full_callback;  // 0x14
    void *full_arg;             // 0x18
} MICAutoParam;

typedef struct {
    BOOL lock;                  // 0x00
    MICCallback callback;       // 0x04
    void *callbackArg;          // 0x08
    MICResult commonResult;     // 0x0c
    MICCallback full;           // 0x10
    void *fullArg;              // 0x14
    u16 *dst_buf;               // 0x18
} MICWork;

static MICWork micWork;
static u16 micInitialized;

static void MicCommonCallback(int tag, u32 data, BOOL err);
static BOOL MicStartAutoSampling(void *buf, u32 size, u32 span, u8 flags);
static BOOL MicStopAutoSampling(void);
static void MicGetResultCallback(MICResult result, void *arg);
static void MicWaitBusy(void);

void MIC_Init(void) {
    if (micInitialized) {
        return;
    }
    micInitialized = 1;
    micWork.lock = FALSE;
    micWork.callback = NULL;
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_MIC, PXI_PROC_ARM7)) {
    }
    MIC_LAST_SAMPLING_ADDRESS = NULL;
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_MIC, MicCommonCallback);
}

MICResult MIC_StartAutoSamplingAsync(const MICAutoParam *param, MICCallback callback, void *arg) {
    OSIntrMode enabled;
    int t;
    u8 type;

    if ((u32) param->buffer & 0x1f) {
        return MIC_RESULT_ILLEGAL_PARAMETER;
    }
    if (param->size & 0x1f) {
        return MIC_RESULT_ILLEGAL_PARAMETER;
    }
    if (param->size <= 0) {
        return MIC_RESULT_ILLEGAL_PARAMETER;
    }
    if (param->rate < MIC_SAMPLING_RATE_LIMIT) {
        return MIC_RESULT_ILLEGAL_PARAMETER;
    }
    switch (param->type) {
    case MIC_SAMPLING_TYPE_8BIT:
        t = SPI_MIC_SAMPLING_TYPE_8BIT;
        break;
    case MIC_SAMPLING_TYPE_12BIT:
        t = SPI_MIC_SAMPLING_TYPE_12BIT;
        break;
    case MIC_SAMPLING_TYPE_SIGNED_8BIT:
        t = SPI_MIC_SAMPLING_TYPE_SIGNED_8BIT;
        break;
    case MIC_SAMPLING_TYPE_SIGNED_12BIT:
        t = SPI_MIC_SAMPLING_TYPE_SIGNED_12BIT;
        break;
    case MIC_SAMPLING_TYPE_12BIT_FILTER_OFF:
        t = SPI_MIC_SAMPLING_TYPE_12BIT | SPI_MIC_SAMPLING_TYPE_FILTER_OFF;
        break;
    case MIC_SAMPLING_TYPE_SIGNED_12BIT_FILTER_OFF:
        t = SPI_MIC_SAMPLING_TYPE_SIGNED_12BIT | SPI_MIC_SAMPLING_TYPE_FILTER_OFF;
        break;
    default:
        return MIC_RESULT_ILLEGAL_PARAMETER;
    }
    type = (u8) (param->loop_enable ? (u8) (t | SPI_MIC_SAMPLING_TYPE_LOOP_AUTO) : (u8) t);
    // (A second, empty flag is or-ed in: the narrowing it costs is in the game's code.)
    type = (u8) (type | 0);

    enabled = OS_DisableInterrupts();
    if (micWork.lock) {
        OS_RestoreInterrupts(enabled);
        return MIC_RESULT_BUSY;
    }
    micWork.lock = TRUE;
    OS_RestoreInterrupts(enabled);

    micWork.callback = callback;
    micWork.callbackArg = arg;
    micWork.full = param->full_callback;
    micWork.fullArg = param->full_arg;
    if (MicStartAutoSampling(param->buffer, param->size, param->rate, type)) {
        return MIC_RESULT_SUCCESS;
    }
    return MIC_RESULT_SEND_ERROR;
}

MICResult MIC_StartAutoSampling(const MICAutoParam *param) {
    micWork.commonResult = MIC_StartAutoSamplingAsync(param, MicGetResultCallback, NULL);
    if (micWork.commonResult == MIC_RESULT_SUCCESS) {
        MicWaitBusy();
    }
    return micWork.commonResult;
}

MICResult MIC_StopAutoSamplingAsync(MICCallback callback, void *arg) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (micWork.lock) {
        OS_RestoreInterrupts(enabled);
        return MIC_RESULT_BUSY;
    }
    micWork.lock = TRUE;
    OS_RestoreInterrupts(enabled);

    micWork.callback = callback;
    micWork.callbackArg = arg;
    if (MicStopAutoSampling()) {
        return MIC_RESULT_SUCCESS;
    }
    return MIC_RESULT_SEND_ERROR;
}

MICResult MIC_StopAutoSampling(void) {
    micWork.commonResult = MIC_StopAutoSamplingAsync(MicGetResultCallback, NULL);
    if (micWork.commonResult == MIC_RESULT_SUCCESS) {
        MicWaitBusy();
    }
    return micWork.commonResult;
}

static void MicCommonCallback(int tag, u32 data, BOOL err) {
#pragma unused(tag)
    u16 command;
    u16 pxiresult;
    MICResult result;
    MICCallback callback;

    if (err) {
        if (micWork.lock) {
            micWork.lock = FALSE;
        }
        if (micWork.callback) {
            callback = micWork.callback;
            micWork.callback = NULL;
            callback(MIC_RESULT_FATAL_ERROR, micWork.callbackArg);
        }
    }

    command = (u16) ((data & SPI_PXI_RESULT_COMMAND_MASK) >> SPI_PXI_RESULT_COMMAND_SHIFT);
    pxiresult = (u16) (data & SPI_PXI_RESULT_DATA_MASK);
    switch (pxiresult) {
    case SPI_PXI_RESULT_SUCCESS:
        result = MIC_RESULT_SUCCESS;
        break;
    case SPI_PXI_RESULT_INVALID_COMMAND:
        result = MIC_RESULT_INVALID_COMMAND;
        break;
    case SPI_PXI_RESULT_INVALID_PARAMETER:
        result = MIC_RESULT_ILLEGAL_PARAMETER;
        break;
    case SPI_PXI_RESULT_ILLEGAL_STATUS:
        result = MIC_RESULT_ILLEGAL_STATUS;
        break;
    case SPI_PXI_RESULT_EXCLUSIVE:
        result = MIC_RESULT_BUSY;
        break;
    default:
        result = MIC_RESULT_FATAL_ERROR;
    }

    if (command == SPI_PXI_COMMAND_MIC_BUFFER_FULL) {
        if (micWork.full) {
            micWork.full(result, micWork.fullArg);
        }
    } else {
        if (command == SPI_PXI_COMMAND_MIC_SAMPLING) {
            if (micWork.dst_buf) {
                *micWork.dst_buf = MIC_LAST_SAMPLING_DATA;
            }
        }
        if (micWork.lock) {
            micWork.lock = FALSE;
        }
        if (micWork.callback) {
            callback = micWork.callback;
            micWork.callback = NULL;
            callback(result, micWork.callbackArg);
        }
    }
}

static BOOL MicStartAutoSampling(void *buf, u32 size, u32 span, u8 flags) {
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_START_BIT | (0 << SPI_PXI_INDEX_SHIFT) |
                                   (SPI_PXI_COMMAND_MIC_AUTO_ON << 8) | (u32) flags,
                               0)) {
        return FALSE;
    }
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (1 << SPI_PXI_INDEX_SHIFT) | ((u32) buf >> 16), 0)) {
        return FALSE;
    }
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (2 << SPI_PXI_INDEX_SHIFT) | ((u32) buf & 0xffff), 0)) {
        return FALSE;
    }
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (3 << SPI_PXI_INDEX_SHIFT) | (size >> 16), 0)) {
        return FALSE;
    }
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (4 << SPI_PXI_INDEX_SHIFT) | (size & 0xffff), 0)) {
        return FALSE;
    }
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC, (5 << SPI_PXI_INDEX_SHIFT) | (span >> 16), 0)) {
        return FALSE;
    }
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_END_BIT | (6 << SPI_PXI_INDEX_SHIFT) | (span & 0xffff), 0)) {
        return FALSE;
    }
    return TRUE;
}

static BOOL MicStopAutoSampling(void) {
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_MIC,
                               SPI_PXI_START_BIT | SPI_PXI_END_BIT | (0 << SPI_PXI_INDEX_SHIFT) |
                                   (SPI_PXI_COMMAND_MIC_AUTO_OFF << 8),
                               0)) {
        return FALSE;
    }
    return TRUE;
}

static void MicGetResultCallback(MICResult result, void *arg) {
#pragma unused(arg)
    micWork.commonResult = result;
}

#pragma thumb off
static asm void MicWaitBusy(void) {
    ldr r12, =micWork.lock
@loop:
    ldr r0, [r12]
    cmp r0, #TRUE
    beq @loop
    bx lr
}
#pragma thumb reset
