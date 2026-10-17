// NitroSDK RTC: date arithmetic.

#include "nitro/rtc.h"

RTCWeek RTC_GetDayOfWeek(RTCDate *date) {
    s32 cent;
    s32 year = (s32) (2000 + date->year);
    s32 month = (s32) date->month;
    s32 day = (s32) date->day;

    // Zeller's congruence, with the year starting in March.
    month -= 2;
    if (month < 1) {
        month += 12;
        year--;
    }
    cent = year / 100;
    year = year % 100;
    return (RTCWeek) (((26 * month - 2) / 10 + day + year + year / 4 + cent / 4 + 5 * cent) % 7);
}
