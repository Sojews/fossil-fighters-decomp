// NitroSDK SPI: the touch panel. The ARM7 samples the panel on request (or automatically, several times a
// frame) and leaves the raw reading in the system work area; this side turns raw readings into screen
// coordinates with the calibration the user set in the system settings.

#include "nitro/os.h"
#include "nitro/pxi.h"
#include "nitro/fx.h"

#define PXI_FIFO_TAG_TOUCHPANEL 6

#define SPI_PXI_START_BIT 0x02000000
#define SPI_PXI_END_BIT 0x01000000
#define SPI_PXI_INDEX_SHIFT 16
#define SPI_PXI_RESULT_COMMAND_MASK 0x7f00
#define SPI_PXI_RESULT_COMMAND_SHIFT 8
#define SPI_PXI_RESULT_DATA_MASK 0x00ff

#define SPI_PXI_COMMAND_TP_SAMPLING 0x00
#define SPI_PXI_COMMAND_TP_AUTO_ON 0x01
#define SPI_PXI_COMMAND_TP_AUTO_OFF 0x02
#define SPI_PXI_COMMAND_TP_AUTO_SAMPLING 0x10

enum {
    SPI_PXI_RESULT_SUCCESS = 0,
    SPI_PXI_RESULT_INVALID_COMMAND,
    SPI_PXI_RESULT_INVALID_PARAMETER,
    SPI_PXI_RESULT_ILLEGAL_STATUS,
    SPI_PXI_RESULT_EXCLUSIVE
};

#define REG_CP_DIVCNT (*(volatile u16 *) 0x04000280)
#define REG_CP_DIV_NUMER_L (*(volatile u32 *) 0x04000290)
#define REG_CP_DIV_DENOM (*(volatile u64 *) 0x04000298)
#define REG_CP_DIV_RESULT_L (*(volatile u32 *) 0x040002a0)
#define CP_DIV_32_32BIT_MODE 0
#define CP_BUSY 0x8000

// The latest raw reading, which the ARM7 keeps in the system work area.
#define HW_TOUCHPANEL_BUF 0x027fffaa
// The user's settings, copied from the firmware.
#define OS_NVRAM_USER_INFO 0x027ffc80

typedef enum {
    TP_TOUCH_OFF = 0,
    TP_TOUCH_ON = 1
} TPTouch;

#define TP_VALIDITY_VALID 0
#define TP_VALIDITY_INVALID_X 1
#define TP_VALIDITY_INVALID_Y 2
#define TP_VALIDITY_INVALID_XY 3

typedef enum {
    TP_REQUEST_COMMAND_SAMPLING = 0,
    TP_REQUEST_COMMAND_AUTO_ON,
    TP_REQUEST_COMMAND_AUTO_OFF,
    TP_REQUEST_COMMAND_SET_STABILITY,
    TP_REQUEST_COMMAND_AUTO_SAMPLING = 0x10
} TPRequestCommand;

#define TP_REQUEST_COMMAND_FLAG_SAMPLING (1 << TP_REQUEST_COMMAND_SAMPLING)
#define TP_REQUEST_COMMAND_FLAG_AUTO_ON (1 << TP_REQUEST_COMMAND_AUTO_ON)
#define TP_REQUEST_COMMAND_FLAG_AUTO_OFF (1 << TP_REQUEST_COMMAND_AUTO_OFF)

typedef enum {
    TP_RESULT_SUCCESS = 0,
    TP_RESULT_INVALID_PARAMETER,
    TP_RESULT_ILLEGAL_STATUS,
    TP_RESULT_EXCLUSIVE,
    TP_RESULT_PXI_BUSY
} TPRequestResult;

typedef enum {
    TP_STATE_READY = 0,
    TP_STATE_SAMPLING,
    TP_STATE_AUTO_SAMPLING,
    TP_STATE_AUTO_WAIT_END
} TPState;

#define TP_RAW_MAX 0x1000
#define TP_CALIBRATE_DOT_INV_SCALE_SHIFT 28
#define TP_CALIBRATE_ORIGIN_SCALE_SHIFT 2
#define TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT 8

typedef struct {
    u16 x;          // 0x0
    u16 y;          // 0x2
    u16 touch;      // 0x4
    u16 validity;   // 0x6
} TPData;

typedef struct {
    s16 x0;         // 0x0
    s16 y0;         // 0x2
    fx16 xDotSize;  // 0x4
    fx16 yDotSize;  // 0x6
} TPCalibrateParam;

typedef void (*TPRecvCallback)(TPRequestCommand command, TPRequestResult result, u16 index);

typedef struct {
    s32 x0;             // 0x00
    s32 xDotSize;       // 0x04
    s32 xDotSizeInv;    // 0x08
    s32 y0;             // 0x0c
    s32 yDotSize;       // 0x10
    s32 yDotSizeInv;    // 0x14
} TPiCalibrateParam;

typedef struct {
    TPRecvCallback callback;        // 0x00
    TPData buf;                     // 0x04: the result of a single sampling
    u16 index;                      // 0x0c: the latest entry of samplingBufs
    u16 frequence;                  // 0x0e: samplings per frame
    TPData *samplingBufs;           // 0x10
    u16 bufSize;                    // 0x14
    u8 dummy[2];
    TPiCalibrateParam calibrate;    // 0x18
    u16 calibrate_flg;              // 0x30
    vu16 state;                     // 0x32
    vu16 err_flg;                   // 0x34
    vu16 command_flg;               // 0x36
} TPStatus;

// The raw reading as the ARM7 packs it.
typedef union {
    struct {
        u32 x : 12;
        u32 y : 12;
        u32 touch : 1;
        u32 validity : 2;
        u32 dummy : 5;
    } e;
    u32 raw;
    u16 halfs[2];
} SPITpData;

// The calibration points the user set, in the firmware's user settings.
typedef struct {
    u8 unk00[0x58];
    u16 raw_x1;     // 0x58
    u16 raw_y1;     // 0x5a
    u8 dx1;         // 0x5c
    u8 dy1;         // 0x5d
    u16 raw_x2;     // 0x5e
    u16 raw_y2;     // 0x60
    u8 dx2;         // 0x62
    u8 dy2;         // 0x63
} NVRAMConfig;

static TPStatus tpState;
static u16 tpInitialized;

void TP_GetCalibratedPoint(TPData *disp, const TPData *raw);
u32 TP_CalcCalibrateParam(TPCalibrateParam *calibrate, u16 raw_x1, u16 raw_y1, u16 dx1, u16 dy1, u16 raw_x2,
                          u16 raw_y2, u16 dx2, u16 dy2);
void TP_WaitBusy(u32 command_flgs);

static inline void SetTPData(TPData *tp) {
    SPITpData spi_tp;
    spi_tp.halfs[0] = *(u16 *) HW_TOUCHPANEL_BUF;
    spi_tp.halfs[1] = *(u16 *) (HW_TOUCHPANEL_BUF + 2);
    tp->x = (u16) spi_tp.e.x;
    tp->y = (u16) spi_tp.e.y;
    tp->touch = (u8) spi_tp.e.touch;
    tp->validity = (u8) spi_tp.e.validity;
}

void TPi_TpCallback(int tag, u32 data, BOOL err) {
#pragma unused(tag)
    u16 result = (u16) data;
    u16 command = (u16) ((result & SPI_PXI_RESULT_COMMAND_MASK) >> SPI_PXI_RESULT_COMMAND_SHIFT);
    int res;

    if (err) {
        tpState.err_flg |= 1 << command;
        if (tpState.callback) {
            tpState.callback((TPRequestCommand) command, TP_RESULT_PXI_BUSY, 0);
        }
        return;
    }

    if (command == SPI_PXI_COMMAND_TP_AUTO_SAMPLING) {
        tpState.index++;
        if (tpState.index >= tpState.bufSize) {
            tpState.index = 0;
        }
        SetTPData(&tpState.samplingBufs[tpState.index]);
        if (tpState.callback) {
            tpState.callback((TPRequestCommand) command, TP_RESULT_SUCCESS, (u8) tpState.index);
        }
        return;
    }

    if (!(data & SPI_PXI_END_BIT)) {
        return;
    }

    switch ((u8) result) {
    case SPI_PXI_RESULT_SUCCESS:
        switch (command) {
        case SPI_PXI_COMMAND_TP_SAMPLING:
            SetTPData(&tpState.buf);
            tpState.state = TP_STATE_READY;
            break;
        case SPI_PXI_COMMAND_TP_AUTO_ON:
            tpState.state = TP_STATE_AUTO_SAMPLING;
            break;
        case SPI_PXI_COMMAND_TP_AUTO_OFF:
            tpState.state = TP_STATE_READY;
            break;
        }
        tpState.command_flg &= ~(1 << command);
        if (tpState.callback) {
            tpState.callback((TPRequestCommand) command, TP_RESULT_SUCCESS, 0);
        }
        return;
    case SPI_PXI_RESULT_EXCLUSIVE:
        res = TP_RESULT_EXCLUSIVE;
        goto error;
    case SPI_PXI_RESULT_INVALID_PARAMETER:
        res = TP_RESULT_INVALID_PARAMETER;
        goto error;
    case SPI_PXI_RESULT_ILLEGAL_STATUS:
        res = TP_RESULT_ILLEGAL_STATUS;
    error:
        tpState.err_flg |= 1 << command;
        tpState.command_flg &= ~(1 << command);
        if (tpState.callback) {
            tpState.callback((TPRequestCommand) command, (TPRequestResult) (u8) res, 0);
        }
        return;
    case SPI_PXI_RESULT_INVALID_COMMAND:
    default:
        OS_Terminate();
    }
}

void TP_Init(void) {
    if (tpInitialized) {
        return;
    }
    tpInitialized = 1;
    PXI_Init();
    tpState.index = 0;
    tpState.callback = NULL;
    tpState.samplingBufs = NULL;
    tpState.state = TP_STATE_READY;
    tpState.calibrate_flg = FALSE;
    tpState.command_flg = 0;
    tpState.err_flg = 0;
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_TOUCHPANEL, PXI_PROC_ARM7)) {
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_TOUCHPANEL, TPi_TpCallback);
}

BOOL TP_GetUserInfo(TPCalibrateParam *calibrate) {
    NVRAMConfig *info = (NVRAMConfig *) OS_NVRAM_USER_INFO;
    u16 x1 = info->raw_x1;
    u16 y1 = info->raw_y1;
    u16 dx1 = info->dx1;
    u16 dy1 = info->dy1;
    u16 x2 = info->raw_x2;
    u16 y2 = info->raw_y2;
    u16 dx2 = info->dx2;
    u16 dy2 = info->dy2;

    if ((x1 == 0 && x2 == 0 && y1 == 0 && y2 == 0) ||
        TP_CalcCalibrateParam(calibrate, x1, y1, dx1, dy1, x2, y2, dx2, dy2) != 0) {
        calibrate->x0 = 0;
        calibrate->y0 = 0;
        calibrate->xDotSize = 0;
        calibrate->yDotSize = 0;
        return TRUE;
    }
    return TRUE;
}

static inline void CP_SetDiv32_32(u32 numer, u32 denom) {
    REG_CP_DIVCNT = CP_DIV_32_32BIT_MODE;
    REG_CP_DIV_NUMER_L = numer;
    REG_CP_DIV_DENOM = denom;
}

static inline void CP_WaitDiv(void) {
    while (REG_CP_DIVCNT & CP_BUSY) {
    }
}

static inline s32 CP_GetDivResult32(void) {
    CP_WaitDiv();
    return (s32) REG_CP_DIV_RESULT_L;
}

void TP_SetCalibrateParam(const TPCalibrateParam *param) {
    OSIntrMode enabled;

    if (param == NULL) {
        tpState.calibrate_flg = FALSE;
        return;
    }
    enabled = OS_DisableInterrupts();
    if (param->xDotSize != 0) {
        CP_SetDiv32_32(0x10000000, (u32) param->xDotSize);
        tpState.calibrate.x0 = param->x0;
        tpState.calibrate.xDotSize = param->xDotSize;
        tpState.calibrate.xDotSizeInv = CP_GetDivResult32();
    } else {
        tpState.calibrate.x0 = 0;
        tpState.calibrate.xDotSize = 0;
        tpState.calibrate.xDotSizeInv = 0;
    }
    if (param->yDotSize != 0) {
        CP_SetDiv32_32(0x10000000, (u32) param->yDotSize);
        tpState.calibrate.y0 = param->y0;
        tpState.calibrate.yDotSize = param->yDotSize;
        tpState.calibrate.yDotSizeInv = CP_GetDivResult32();
    } else {
        tpState.calibrate.y0 = 0;
        tpState.calibrate.yDotSize = 0;
        tpState.calibrate.yDotSizeInv = 0;
    }
    OS_RestoreInterrupts(enabled);
    tpState.calibrate_flg = TRUE;
}

static inline BOOL TPi_SamplingNow(void) {
    if (PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                           SPI_PXI_START_BIT | SPI_PXI_END_BIT | (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_TP_SAMPLING << 8),
                           0) < 0) {
        return FALSE;
    }
    return TRUE;
}

static inline BOOL TPi_AutoSamplingOn(u16 vcount, u8 frequence) {
    if (PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                           SPI_PXI_START_BIT | (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_TP_AUTO_ON << 8) |
                               frequence,
                           0) < 0) {
        return FALSE;
    }
    if (PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL, SPI_PXI_END_BIT | (1 << SPI_PXI_INDEX_SHIFT) | vcount, 0) <
        0) {
        return FALSE;
    }
    return TRUE;
}

static inline BOOL TPi_AutoSamplingOff(void) {
    if (PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,
                           SPI_PXI_START_BIT | SPI_PXI_END_BIT | (0 << SPI_PXI_INDEX_SHIFT) |
                               (SPI_PXI_COMMAND_TP_AUTO_OFF << 8),
                           0) < 0) {
        return FALSE;
    }
    return TRUE;
}

void TP_RequestSamplingAsync(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (!TPi_SamplingNow()) {
        OS_RestoreInterrupts(enabled);
        tpState.err_flg |= TP_REQUEST_COMMAND_FLAG_SAMPLING;
        if (tpState.callback) {
            tpState.callback(TP_REQUEST_COMMAND_SAMPLING, TP_RESULT_PXI_BUSY, 0);
        }
        return;
    }
    tpState.command_flg |= TP_REQUEST_COMMAND_FLAG_SAMPLING;
    tpState.err_flg &= ~TP_REQUEST_COMMAND_FLAG_SAMPLING;
    OS_RestoreInterrupts(enabled);
}

u32 TP_GetCalibratedResult(TPData *result) {
    if (tpState.err_flg & TP_REQUEST_COMMAND_FLAG_SAMPLING) {
        return 1;
    }
    *result = tpState.buf;
    TP_GetCalibratedPoint(result, result);
    return 0;
}

u32 TP_WaitCalibratedResult(TPData *result) {
    TP_WaitBusy(TP_REQUEST_COMMAND_FLAG_SAMPLING);
    return TP_GetCalibratedResult(result);
}

void TP_RequestAutoSamplingStartAsync(u16 vcount, u16 frequence, TPData samplingBufs[], u16 bufSize) {
    u32 i;
    OSIntrMode enabled;

    tpState.samplingBufs = samplingBufs;
    tpState.index = 0;
    tpState.frequence = frequence;
    tpState.bufSize = bufSize;
    for (i = 0; i < bufSize; i++) {
        tpState.samplingBufs[i].touch = TP_TOUCH_OFF;
    }

    enabled = OS_DisableInterrupts();
    if (!(u8) TPi_AutoSamplingOn(vcount, (u8) frequence)) {
        OS_RestoreInterrupts(enabled);
        tpState.err_flg |= TP_REQUEST_COMMAND_FLAG_AUTO_ON;
        if (tpState.callback) {
            tpState.callback(TP_REQUEST_COMMAND_AUTO_ON, TP_RESULT_PXI_BUSY, 0);
        }
        return;
    }
    tpState.command_flg |= TP_REQUEST_COMMAND_FLAG_AUTO_ON;
    tpState.err_flg &= ~TP_REQUEST_COMMAND_FLAG_AUTO_ON;
    OS_RestoreInterrupts(enabled);
}

void TP_RequestAutoSamplingStopAsync(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (!TPi_AutoSamplingOff()) {
        OS_RestoreInterrupts(enabled);
        tpState.err_flg |= TP_REQUEST_COMMAND_FLAG_AUTO_OFF;
        if (tpState.callback) {
            tpState.callback(TP_REQUEST_COMMAND_AUTO_OFF, TP_RESULT_PXI_BUSY, 0);
        }
        return;
    }
    tpState.command_flg |= TP_REQUEST_COMMAND_FLAG_AUTO_OFF;
    tpState.err_flg &= ~TP_REQUEST_COMMAND_FLAG_AUTO_OFF;
    OS_RestoreInterrupts(enabled);
}

void TP_GetLatestRawPointInAuto(TPData *result) {
    s32 i;
    u32 curr_index;
    TPData *next_ptr;

    result->validity = TP_VALIDITY_INVALID_XY;
    curr_index = tpState.index;
    if (tpState.frequence == 1 || tpState.bufSize == 1) {
        *result = tpState.samplingBufs[curr_index];
        return;
    }
    for (i = 0; i < tpState.frequence && i < tpState.bufSize - 1; i++) {
        u32 index = curr_index - i;
        if ((s32) index < 0) {
            index += tpState.bufSize;
        }
        next_ptr = &tpState.samplingBufs[index];
        if (!next_ptr->touch) {
            *result = *next_ptr;
            return;
        }
        if ((result->validity & TP_VALIDITY_INVALID_X) && !(next_ptr->validity & TP_VALIDITY_INVALID_X)) {
            result->x = next_ptr->x;
            if (i != 0) {
                result->validity &= ~TP_VALIDITY_INVALID_X;
            }
        }
        if ((result->validity & TP_VALIDITY_INVALID_Y) && !(next_ptr->validity & TP_VALIDITY_INVALID_Y)) {
            result->y = next_ptr->y;
            if (i != 0) {
                result->validity &= ~TP_VALIDITY_INVALID_Y;
            }
        }
        if (result->validity == TP_VALIDITY_VALID) {
            result->touch = TP_TOUCH_ON;
            return;
        }
    }
    result->touch = TP_TOUCH_ON;
}

void TP_GetLatestCalibratedPointInAuto(TPData *result) {
    TP_GetLatestRawPointInAuto(result);
    TP_GetCalibratedPoint(result, result);
}

u32 TP_CalcCalibrateParam(TPCalibrateParam *calibrate, u16 raw_x1, u16 raw_y1, u16 dx1, u16 dy1, u16 raw_x2,
                          u16 raw_y2, u16 dx2, u16 dy2) {
    s32 rx_width, dx_width, ry_width, dy_width;
    s32 tmp32;
    OSIntrMode enabled;

    if (raw_x1 >= TP_RAW_MAX || raw_y1 >= TP_RAW_MAX || raw_x2 >= TP_RAW_MAX || raw_y2 >= TP_RAW_MAX) {
        return 1;
    }
    if (dx1 >= 256 || dx2 >= 256 || dy1 >= 192 || dy2 >= 192) {
        return 1;
    }
    if (dx1 == dx2 || dy1 == dy2 || raw_x1 == raw_x2 || raw_y1 == raw_y2) {
        return 1;
    }

    enabled = OS_DisableInterrupts();
    rx_width = raw_x1 - raw_x2;
    dx_width = dx1 - dx2;
    CP_SetDiv32_32((u32) (rx_width << TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT), (u32) dx_width);
    ry_width = raw_y1 - raw_y2;
    dy_width = dy1 - dy2;
    tmp32 = CP_GetDivResult32();
    CP_SetDiv32_32((u32) (ry_width << TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT), (u32) dy_width);
    if (tmp32 >= 0x8000 || tmp32 < -0x8000) {
        OS_RestoreInterrupts(enabled);
        return 1;
    }
    calibrate->xDotSize = (fx16) tmp32;
    tmp32 = (s16) ((((raw_x1 + raw_x2) << TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT) - (dx1 + dx2) * calibrate->xDotSize) >>
                   (TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT - TP_CALIBRATE_ORIGIN_SCALE_SHIFT + 1));
    if (tmp32 >= 0x8000 || tmp32 < -0x8000) {
        OS_RestoreInterrupts(enabled);
        return 1;
    }
    calibrate->x0 = (s16) tmp32;

    tmp32 = CP_GetDivResult32();
    OS_RestoreInterrupts(enabled);
    if (tmp32 >= 0x8000 || tmp32 < -0x8000) {
        return 1;
    }
    calibrate->yDotSize = (fx16) tmp32;
    tmp32 = (s16) ((((raw_y1 + raw_y2) << TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT) - (dy1 + dy2) * calibrate->yDotSize) >>
                   (TP_CALIBRATE_DOT2ORIGIN_SCALE_SHIFT - TP_CALIBRATE_ORIGIN_SCALE_SHIFT + 1));
    if (tmp32 >= 0x8000 || tmp32 < -0x8000) {
        return 1;
    }
    calibrate->y0 = (s16) tmp32;
    return 0;
}

void TP_GetCalibratedPoint(TPData *disp, const TPData *raw) {
    TPiCalibrateParam *cal = &tpState.calibrate;

    if (!tpState.calibrate_flg) {
        *disp = *raw;
        return;
    }
    disp->touch = raw->touch;
    disp->validity = raw->validity;
    if (raw->touch == 0) {
        disp->x = 0;
        disp->y = 0;
        return;
    }
    disp->x = (u16) ((((s64) (raw->x << TP_CALIBRATE_ORIGIN_SCALE_SHIFT) - cal->x0) *
                      cal->xDotSizeInv) >>
                     (TP_CALIBRATE_DOT_INV_SCALE_SHIFT - TP_CALIBRATE_ORIGIN_SCALE_SHIFT - 4));
    if ((s16) disp->x < 0) {
        disp->x = 0;
    } else if ((s16) disp->x > 255) {
        disp->x = 255;
    }
    disp->y = (u16) ((((s64) (raw->y << TP_CALIBRATE_ORIGIN_SCALE_SHIFT) - cal->y0) *
                      cal->yDotSizeInv) >>
                     (TP_CALIBRATE_DOT_INV_SCALE_SHIFT - TP_CALIBRATE_ORIGIN_SCALE_SHIFT - 4));
    if ((s16) disp->y < 0) {
        disp->y = 0;
    } else if ((s16) disp->y > 191) {
        disp->y = 191;
    }
}

void TP_WaitBusy(u32 command_flgs) {
    while (tpState.command_flg & command_flgs) {
    }
}

u32 TP_CheckError(u32 command_flgs) {
    return (u32) (tpState.err_flg & command_flgs);
}
