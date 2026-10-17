// NitroSDK RTC: the raw requests to the ARM7.

#include "nitro/pxi.h"
#include "nitro/rtc.h"

#define PXI_FIFO_TAG_RTC 5

#define RTC_PXI_COMMAND_MASK 0x00007f00
#define RTC_PXI_COMMAND_SHIFT 8

#define RTC_PXI_COMMAND_READ_DATETIME 0x10
#define RTC_PXI_COMMAND_READ_TIME 0x12
#define RTC_PXI_COMMAND_WRITE_STATUS2 0x27

static BOOL RtcSendPxiCommand(u32 command);

BOOL RTCi_ReadRawDateTimeAsync(void) {
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_DATETIME);
}

BOOL RTCi_ReadRawTimeAsync(void) {
    return RtcSendPxiCommand(RTC_PXI_COMMAND_READ_TIME);
}

BOOL RTCi_WriteRawStatus2Async(void) {
    return RtcSendPxiCommand(RTC_PXI_COMMAND_WRITE_STATUS2);
}

static BOOL RtcSendPxiCommand(u32 command) {
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_RTC, (command << RTC_PXI_COMMAND_SHIFT) & RTC_PXI_COMMAND_MASK, 0)) {
        return FALSE;
    }
    return TRUE;
}
