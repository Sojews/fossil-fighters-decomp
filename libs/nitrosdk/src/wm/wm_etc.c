// NitroSDK WM: miscellaneous requests (channel measurement, accepting new children).

#include "wm_private.h"

typedef struct {
    u16 apiid;          // 0x0
    u16 ccaMode;        // 0x2
    u16 edThreshold;    // 0x4
    u16 channel;        // 0x6
    u16 measureTime;    // 0x8
} WMMeasureChannelReq;

WMErrCode WM_MeasureChannel(WMCallbackFunc callback, u16 ccaMode, u16 edThreshold, u16 channel,
                            u16 measureTime) {
    WMErrCode result;
    WMArm9Buf *p = WMi_GetSystemWork();
    WMMeasureChannelReq Req;

#pragma unused(p)
    result = WMi_CheckStateEx(1, WM_STATE_IDLE);
    WM_CHECK_RESULT(result);
    WMi_SetCallbackTable(WM_APIID_MEASURE_CHANNEL, callback);
    Req.apiid = WM_APIID_MEASURE_CHANNEL;
    Req.ccaMode = ccaMode;
    Req.edThreshold = edThreshold;
    Req.channel = channel;
    Req.measureTime = measureTime;
    result = WMi_SendCommandDirect(&Req, sizeof(Req));
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_SetEntry(WMCallbackFunc callback, BOOL enabled) {
    WMErrCode result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_MP_PARENT);
    WM_CHECK_RESULT(result);
    WMi_SetCallbackTable(WM_APIID_SET_ENTRY, callback);
    result = WMi_SendCommand(WM_APIID_SET_ENTRY, 1, (u32) enabled);
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}
