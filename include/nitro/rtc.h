// The parts of the NitroSDK RTC (real-time clock) library the game uses (declarations only).
#ifndef NITRO_RTC_H
#define NITRO_RTC_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RTC_RESULT_SUCCESS = 0,
    RTC_RESULT_BUSY,
    RTC_RESULT_ILLEGAL_PARAMETER,
    RTC_RESULT_SEND_ERROR,
    RTC_RESULT_INVALID_COMMAND,
    RTC_RESULT_ILLEGAL_STATUS,
    RTC_RESULT_FATAL_ERROR,
    RTC_RESULT_MAX
} RTCResult;

typedef enum {
    RTC_WEEK_SUNDAY = 0,
    RTC_WEEK_MONDAY,
    RTC_WEEK_TUESDAY,
    RTC_WEEK_WEDNESDAY,
    RTC_WEEK_THURSDAY,
    RTC_WEEK_FRIDAY,
    RTC_WEEK_SATURDAY,
    RTC_WEEK_MAX
} RTCWeek;

typedef struct {
    u32 year;       // 0x0: 0-99, from 2000
    u32 month;      // 0x4
    u32 day;        // 0x8
    RTCWeek week;   // 0xc
} RTCDate;

typedef struct {
    u32 hour;       // 0x0
    u32 minute;     // 0x4
    u32 second;     // 0x8
} RTCTime;

typedef void (*RTCCallback)(RTCResult result, void *arg);
typedef void (*RTCInterrupt)(void);

void RTC_Init(void);
RTCResult RTC_GetTimeAsync(RTCTime *time, RTCCallback callback, void *arg);
RTCResult RTC_GetTime(RTCTime *time);
RTCResult RTC_GetDateTimeAsync(RTCDate *date, RTCTime *time, RTCCallback callback, void *arg);
RTCResult RTC_GetDateTime(RTCDate *date, RTCTime *time);
RTCWeek RTC_GetDayOfWeek(RTCDate *date);

BOOL RTCi_ReadRawDateTimeAsync(void);
BOOL RTCi_ReadRawTimeAsync(void);
BOOL RTCi_WriteRawStatus2Async(void);

#ifdef __cplusplus
}
#endif

#endif
