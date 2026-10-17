// NitroSDK RTC: requests to the real-time clock, which sits on the ARM7's side. The ARM7 leaves what it reads
// in the system work area as BCD; this side converts it.

#include "nitro/os.h"
#include "nitro/pxi.h"
#include "nitro/rtc.h"

#define PXI_FIFO_TAG_RTC 5

#define RTC_PXI_COMMAND_MASK 0x00007f00
#define RTC_PXI_COMMAND_SHIFT 8
#define RTC_PXI_RESULT_MASK 0x000000ff
#define RTC_PXI_COMMAND_INTERRUPT 0x30

enum {
    RTC_PXI_RESULT_SUCCESS = 0,
    RTC_PXI_RESULT_INVALID_COMMAND,
    RTC_PXI_RESULT_ILLEGAL_STATUS,
    RTC_PXI_RESULT_BUSY,
    RTC_PXI_RESULT_FATAL_ERROR
};

typedef enum {
    RTC_SEQ_GET_DATE = 0,
    RTC_SEQ_GET_TIME,
    RTC_SEQ_GET_DATETIME,
    RTC_SEQ_SET_DATE,
    RTC_SEQ_SET_TIME,
    RTC_SEQ_SET_DATETIME,
    RTC_SEQ_GET_ALARM1_STATUS,
    RTC_SEQ_GET_ALARM2_STATUS,
    RTC_SEQ_GET_ALARM_PARAM,
    RTC_SEQ_SET_ALARM1_STATUS,
    RTC_SEQ_SET_ALARM2_STATUS,
    RTC_SEQ_SET_ALARM1_PARAM,
    RTC_SEQ_SET_ALARM2_PARAM,
    RTC_SEQ_SET_HOUR_FORMAT,
    RTC_SEQ_SET_REG_STATUS2,
    RTC_SEQ_SET_REG_ADJUST,
    RTC_SEQ_MAX
} RTCSequence;

#define RTC_LOCK_OFF 0
#define RTC_LOCK_ON 1

#define RTC_INTERRUPT_MODE_NONE 0x0
#define RTC_INTERRUPT_MODE_ALARM 0x4

#define RTC_ALARM_ENABLE_OFF 0
#define RTC_ALARM_ENABLE_ON 1
#define RTC_ALARM_ENABLE_WEEK 0x1
#define RTC_ALARM_ENABLE_HOUR 0x2
#define RTC_ALARM_ENABLE_MINUTE 0x4

typedef u32 RTCAlarmEnable;

typedef struct {
    RTCWeek week;   // 0x0
    u32 hour;       // 0x4
    u32 minute;     // 0x8
    u32 enable;     // 0xc
} RTCAlarmParam;

// The clock's registers as the ARM7 leaves them in the system work area (BCD).
typedef struct {
    u32 year : 8;
    u32 month : 5;
    u32 dummy0 : 3;
    u32 day : 6;
    u32 dummy1 : 2;
    u32 week : 3;
    u32 dummy2 : 5;
} RTCRawDate;

typedef struct {
    u32 hour : 6;
    u32 afternoon : 1;
    u32 dummy0 : 1;
    u32 minute : 7;
    u32 dummy1 : 1;
    u32 second : 7;
    u32 dummy2 : 9;
} RTCRawTime;

typedef struct {
    u16 reset : 1;
    u16 format : 1;
    u16 dummy0 : 2;
    u16 intr1 : 1;
    u16 intr2 : 1;
    u16 bld : 1;
    u16 poc : 1;
    u16 dummy1 : 8;
} RTCRawStatus1;

typedef struct {
    u16 intr1_mode : 4;
    u16 dummy0 : 2;
    u16 intr2_mode : 1;
    u16 test : 1;
    u16 dummy1 : 8;
} RTCRawStatus2;

typedef struct {
    u32 week : 3;
    u32 dummy0 : 3;
    u32 afternoon : 1;
    u32 we : 1;
    u32 hour : 6;
    u32 dummy1 : 1;
    u32 he : 1;
    u32 minute : 7;
    u32 me : 1;
    u32 dummy2 : 8;
} RTCRawAlarm;

typedef union {
    struct {
        RTCRawDate date;
        RTCRawTime time;
    } t;
    struct {
        RTCRawStatus1 status1;
        RTCRawStatus2 status2;
        RTCRawAlarm alarm;
    } a;
} RTCRawData;

#define RTC_RAW_DATA (*(RTCRawData *) 0x027ffde8)

typedef struct {
    u32 lock;               // 0x00
    RTCCallback callback;   // 0x04
    void *buffer[2];        // 0x08
    void *callbackArg;      // 0x10
    u32 sequence;           // 0x14
    u32 index;              // 0x18
    RTCInterrupt interrupt; // 0x1c
    RTCResult commonResult; // 0x20
} RTCWork;

static RTCWork rtcWork;
static u16 rtcInitialized;

static void RtcCommonCallback(int tag, u32 data, BOOL err);
static u32 RtcBCD2HEX(u32 bcd);
static void RtcGetResultCallback(RTCResult result, void *arg);
static void RtcWaitBusy(void);

void RTC_Init(void) {
    if (rtcInitialized) {
        return;
    }
    rtcInitialized = 1;
    rtcWork.lock = RTC_LOCK_OFF;
    rtcWork.callback = NULL;
    rtcWork.interrupt = NULL;
    rtcWork.buffer[0] = NULL;
    rtcWork.buffer[1] = NULL;
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_RTC, PXI_PROC_ARM7)) {
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_RTC, RtcCommonCallback);
}

RTCResult RTC_GetTimeAsync(RTCTime *time, RTCCallback callback, void *arg) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF) {
        OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    OS_RestoreInterrupts(enabled);

    rtcWork.sequence = RTC_SEQ_GET_TIME;
    rtcWork.index = 0;
    rtcWork.buffer[0] = time;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_ReadRawTimeAsync()) {
        return RTC_RESULT_SUCCESS;
    }
    return RTC_RESULT_SEND_ERROR;
}

RTCResult RTC_GetTime(RTCTime *time) {
    rtcWork.commonResult = RTC_GetTimeAsync(time, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS) {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

RTCResult RTC_GetDateTimeAsync(RTCDate *date, RTCTime *time, RTCCallback callback, void *arg) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (rtcWork.lock != RTC_LOCK_OFF) {
        OS_RestoreInterrupts(enabled);
        return RTC_RESULT_BUSY;
    }
    rtcWork.lock = RTC_LOCK_ON;
    OS_RestoreInterrupts(enabled);

    rtcWork.sequence = RTC_SEQ_GET_DATETIME;
    rtcWork.index = 0;
    rtcWork.buffer[0] = date;
    rtcWork.buffer[1] = time;
    rtcWork.callback = callback;
    rtcWork.callbackArg = arg;
    if (RTCi_ReadRawDateTimeAsync()) {
        return RTC_RESULT_SUCCESS;
    }
    return RTC_RESULT_SEND_ERROR;
}

RTCResult RTC_GetDateTime(RTCDate *date, RTCTime *time) {
    rtcWork.commonResult = RTC_GetDateTimeAsync(date, time, RtcGetResultCallback, NULL);
    if (rtcWork.commonResult == RTC_RESULT_SUCCESS) {
        RtcWaitBusy();
    }
    return rtcWork.commonResult;
}

static void RtcCommonCallback(int tag, u32 data, BOOL err) {
#pragma unused(tag)
    u8 command;
    u8 result;
    RTCResult callbackResult;
    RTCCallback cb;

    if (err) {
        if (rtcWork.index) {
            rtcWork.index = 0;
        }
        if (rtcWork.lock != RTC_LOCK_OFF) {
            rtcWork.lock = RTC_LOCK_OFF;
        }
        if (rtcWork.callback) {
            cb = rtcWork.callback;
            rtcWork.callback = NULL;
            cb(RTC_RESULT_FATAL_ERROR, rtcWork.callbackArg);
        }
        return;
    }

    command = (u8) ((data & RTC_PXI_COMMAND_MASK) >> RTC_PXI_COMMAND_SHIFT);
    result = (u8) (data & RTC_PXI_RESULT_MASK);
    if (command == RTC_PXI_COMMAND_INTERRUPT) {
        if (rtcWork.interrupt) {
            rtcWork.interrupt();
        }
        return;
    }

    if (result == RTC_PXI_RESULT_SUCCESS) {
        callbackResult = RTC_RESULT_SUCCESS;
        switch (rtcWork.sequence) {
        case RTC_SEQ_GET_DATE: {
            RTCDate *pDst = (RTCDate *) rtcWork.buffer[0];
            pDst->year = RtcBCD2HEX(RTC_RAW_DATA.t.date.year);
            pDst->month = RtcBCD2HEX(RTC_RAW_DATA.t.date.month);
            pDst->day = RtcBCD2HEX(RTC_RAW_DATA.t.date.day);
            pDst->week = RTC_GetDayOfWeek(pDst);
            break;
        }
        case RTC_SEQ_GET_TIME: {
            RTCTime *pDst = (RTCTime *) rtcWork.buffer[0];
            pDst->hour = RtcBCD2HEX(RTC_RAW_DATA.t.time.hour);
            pDst->minute = RtcBCD2HEX(RTC_RAW_DATA.t.time.minute);
            pDst->second = RtcBCD2HEX(RTC_RAW_DATA.t.time.second);
            break;
        }
        case RTC_SEQ_GET_DATETIME: {
            RTCDate *pDst = (RTCDate *) rtcWork.buffer[0];
            pDst->year = RtcBCD2HEX(RTC_RAW_DATA.t.date.year);
            pDst->month = RtcBCD2HEX(RTC_RAW_DATA.t.date.month);
            pDst->day = RtcBCD2HEX(RTC_RAW_DATA.t.date.day);
            pDst->week = RTC_GetDayOfWeek(pDst);
        }
            {
                RTCTime *pDst = (RTCTime *) rtcWork.buffer[1];
                pDst->hour = RtcBCD2HEX(RTC_RAW_DATA.t.time.hour);
                pDst->minute = RtcBCD2HEX(RTC_RAW_DATA.t.time.minute);
                pDst->second = RtcBCD2HEX(RTC_RAW_DATA.t.time.second);
            }
            break;
        case RTC_SEQ_SET_DATE:
        case RTC_SEQ_SET_TIME:
        case RTC_SEQ_SET_DATETIME:
            break;
        case RTC_SEQ_GET_ALARM1_STATUS: {
            RTCAlarmEnable *pDst = (RTCAlarmEnable *) rtcWork.buffer[0];
            if (RTC_RAW_DATA.a.status2.intr1_mode == RTC_INTERRUPT_MODE_ALARM) {
                *pDst = RTC_ALARM_ENABLE_ON;
            } else {
                *pDst = RTC_ALARM_ENABLE_OFF;
            }
            break;
        }
        case RTC_SEQ_GET_ALARM2_STATUS: {
            RTCAlarmEnable *pDst = (RTCAlarmEnable *) rtcWork.buffer[0];
            if (RTC_RAW_DATA.a.status2.intr2_mode) {
                *pDst = RTC_ALARM_ENABLE_ON;
            } else {
                *pDst = RTC_ALARM_ENABLE_OFF;
            }
            break;
        }
        case RTC_SEQ_GET_ALARM_PARAM: {
            RTCAlarmParam *pDst = (RTCAlarmParam *) rtcWork.buffer[0];
            pDst->week = (RTCWeek) RTC_RAW_DATA.a.alarm.week;
            pDst->hour = RtcBCD2HEX(RTC_RAW_DATA.a.alarm.hour);
            pDst->minute = RtcBCD2HEX(RTC_RAW_DATA.a.alarm.minute);
            pDst->enable = RTC_ALARM_ENABLE_OFF;
            if (RTC_RAW_DATA.a.alarm.we) {
                pDst->enable += RTC_ALARM_ENABLE_WEEK;
            }
            if (RTC_RAW_DATA.a.alarm.he) {
                pDst->enable += RTC_ALARM_ENABLE_HOUR;
            }
            if (RTC_RAW_DATA.a.alarm.me) {
                pDst->enable += RTC_ALARM_ENABLE_MINUTE;
            }
            break;
        }
        case RTC_SEQ_SET_ALARM1_STATUS:
            if (rtcWork.index == 0) {
                if (*(RTCAlarmEnable *) rtcWork.buffer[0] == RTC_ALARM_ENABLE_ON) {
                    if (RTC_RAW_DATA.a.status2.intr1_mode != RTC_INTERRUPT_MODE_ALARM) {
                        rtcWork.index++;
                        RTC_RAW_DATA.a.status2.intr1_mode = RTC_INTERRUPT_MODE_ALARM;
                        if (!RTCi_WriteRawStatus2Async()) {
                            rtcWork.index = 0;
                            callbackResult = RTC_RESULT_SEND_ERROR;
                        }
                    }
                } else {
                    if (RTC_RAW_DATA.a.status2.intr1_mode != RTC_INTERRUPT_MODE_NONE) {
                        rtcWork.index++;
                        RTC_RAW_DATA.a.status2.intr1_mode = RTC_INTERRUPT_MODE_NONE;
                        if (!RTCi_WriteRawStatus2Async()) {
                            rtcWork.index = 0;
                            callbackResult = RTC_RESULT_SEND_ERROR;
                        }
                    }
                }
            } else {
                rtcWork.index = 0;
            }
            break;
        case RTC_SEQ_SET_ALARM2_STATUS:
            if (rtcWork.index == 0) {
                if (*(RTCAlarmEnable *) rtcWork.buffer[0] == RTC_ALARM_ENABLE_ON) {
                    if (!RTC_RAW_DATA.a.status2.intr2_mode) {
                        rtcWork.index++;
                        RTC_RAW_DATA.a.status2.intr2_mode = 1;
                        if (!RTCi_WriteRawStatus2Async()) {
                            rtcWork.index = 0;
                            callbackResult = RTC_RESULT_SEND_ERROR;
                        }
                    }
                } else {
                    if (RTC_RAW_DATA.a.status2.intr2_mode) {
                        rtcWork.index++;
                        RTC_RAW_DATA.a.status2.intr2_mode = 0;
                        if (!RTCi_WriteRawStatus2Async()) {
                            rtcWork.index = 0;
                            callbackResult = RTC_RESULT_SEND_ERROR;
                        }
                    }
                }
            } else {
                rtcWork.index = 0;
            }
            break;
        case RTC_SEQ_SET_ALARM1_PARAM:
        case RTC_SEQ_SET_ALARM2_PARAM:
        case RTC_SEQ_SET_HOUR_FORMAT:
        case RTC_SEQ_SET_REG_STATUS2:
        case RTC_SEQ_SET_REG_ADJUST:
            break;
        default:
            callbackResult = RTC_RESULT_INVALID_COMMAND;
            rtcWork.index = 0;
        }
    } else {
        rtcWork.index = 0;
        switch (result) {
        case RTC_PXI_RESULT_INVALID_COMMAND:
            callbackResult = RTC_RESULT_INVALID_COMMAND;
            break;
        case RTC_PXI_RESULT_ILLEGAL_STATUS:
            callbackResult = RTC_RESULT_ILLEGAL_STATUS;
            break;
        case RTC_PXI_RESULT_BUSY:
            callbackResult = RTC_RESULT_BUSY;
            break;
        case RTC_PXI_RESULT_FATAL_ERROR:
        default:
            callbackResult = RTC_RESULT_FATAL_ERROR;
        }
    }

    if (rtcWork.index == 0) {
        if (rtcWork.lock != RTC_LOCK_OFF) {
            rtcWork.lock = RTC_LOCK_OFF;
        }
        if (rtcWork.callback) {
            cb = rtcWork.callback;
            rtcWork.callback = NULL;
            cb(callbackResult, rtcWork.callbackArg);
        }
    }
}

static u32 RtcBCD2HEX(u32 bcd) {
    u32 hex = 0;
    s32 i;
    s32 w;

    for (i = 0; i < 8; i++) {
        if (((bcd >> (i * 4)) & 0x0000000f) >= 0x0a) {
            return 0;
        }
    }
    for (i = 0, w = 1; i < 8; i++, w *= 10) {
        hex += ((bcd >> (i * 4)) & 0x0000000f) * w;
    }
    return hex;
}

static void RtcGetResultCallback(RTCResult result, void *arg) {
#pragma unused(arg)
    rtcWork.commonResult = result;
}

#pragma thumb off
static asm void RtcWaitBusy(void) {
    ldr r12, =rtcWork.lock
@loop:
    ldr r0, [r12]
    cmp r0, #RTC_LOCK_ON
    beq @loop
    bx lr
}
#pragma thumb reset
