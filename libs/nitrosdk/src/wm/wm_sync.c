// NitroSDK WM: the calls that answer at once (callbacks, buffer sizes, link quality, timing helpers).

#include "wm_private.h"
#include "nitro/rtc.h"

#define WM_GET_ALLOWED_CHANNEL_BEFORE_INIT 0x8000
#define WM_DEFAULT_BEACON_PERIOD 200
#define WM_DEFAULT_SCAN_PERIOD 30

// In the firmware's user settings, copied to the system work area at boot.
#define OS_ALLOWED_CHANNEL (*(u16 *) 0x027ffcfa)
#define OS_WIRELESS_ENABLED (*(u16 *) 0x027ffcf4)

#define OS_VBLANK_COUNT (*(vu32 *) 0x027ffc3c)

typedef enum {
    WM_LINK_LEVEL_0 = 0,
    WM_LINK_LEVEL_1,
    WM_LINK_LEVEL_2,
    WM_LINK_LEVEL_3
} WMLinkLevel;

WMErrCode WM_SetIndCallback(WMCallbackFunc callback) {
    OSIntrMode e = OS_DisableInterrupts();
    WMErrCode result = WMi_CheckInitialized();
    if (result != WM_ERRCODE_SUCCESS) {
        OS_RestoreInterrupts(e);
        return result;
    }
    WMi_GetSystemWork()->indCallback = callback;
    OS_RestoreInterrupts(e);
    return WM_ERRCODE_SUCCESS;
}

WMErrCode WM_SetPortCallback(u16 port, WMCallbackFunc callback, void *arg) {
    WMArm9Buf *w9b;
    OSIntrMode e;
    WMErrCode result;
    WMPortRecvCallback cb_Port;

    if (callback != NULL) {
        MI_CpuFill8(&cb_Port, 0, sizeof(WMPortRecvCallback));
        cb_Port.apiid = WM_APIID_PORT_RECV;
        cb_Port.errcode = WM_ERRCODE_SUCCESS;
        cb_Port.state = WM_STATECODE_PORT_INIT;
        cb_Port.port = port;
        cb_Port.recvBuf = NULL;
        cb_Port.data = NULL;
        cb_Port.length = 0;
        cb_Port.seqNo = 0xffff;
        cb_Port.arg = arg;
        cb_Port.aid = 0;
        OS_GetMacAddress(cb_Port.macAddress);
    }

    e = OS_DisableInterrupts();
    result = WMi_CheckInitialized();
    if (result != WM_ERRCODE_SUCCESS) {
        OS_RestoreInterrupts(e);
        return result;
    }
    w9b = WMi_GetSystemWork();
    w9b->portCallbackTable[port] = callback;
    w9b->portCallbackArgument[port] = arg;
    if (callback != NULL) {
        cb_Port.connectedAidBitmap = WM_GetConnectedAIDs();
        cb_Port.myAid = WM_GetAID();
        callback(&cb_Port);
    }
    OS_RestoreInterrupts(e);
    return WM_ERRCODE_SUCCESS;
}

int WM_GetMPSendBufferSize(void) {
    WMArm9Buf *p = WMi_GetSystemWork();
    WMErrCode result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_CHILD);
    if (result != WM_ERRCODE_SUCCESS) {
        return 0;
    }
    DC_InvalidateRange(&p->status->mp_flag, 4);
    if (p->status->mp_flag == TRUE) {
        return 0;
    }
    DC_InvalidateRange(&p->status->mp_sendSize, 4);
    return (p->status->mp_sendSize + 31) & ~31;
}

int WM_GetMPReceiveBufferSize(void) {
    int maxReceiveSize;
    WMArm9Buf *p = WMi_GetSystemWork();
    WMErrCode result;
    BOOL isParent;

    result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_CHILD);
    if (result != WM_ERRCODE_SUCCESS) {
        return 0;
    }
    DC_InvalidateRange(&p->status->mp_flag, 4);
    if (p->status->mp_flag == TRUE) {
        return 0;
    }
    DC_InvalidateRange(&p->status->aid, 2);
    isParent = (p->status->aid == 0) ? TRUE : FALSE;
    DC_InvalidateRange(&p->status->mp_recvSize, 2);
    maxReceiveSize = p->status->mp_recvSize;
    if (isParent == TRUE) {
        int maxEntry;
        DC_InvalidateRange(&p->status->pparam_maxEntry, 2);
        maxEntry = p->status->pparam_maxEntry;
        // (a header per child, the children, rounded to 32 bytes) for each of two buffers
        return ((((maxReceiveSize + 12) * maxEntry) + 41) & ~31) * 2;
    } else {
        return ((maxReceiveSize + 81) & ~31) * 2;
    }
}

u16 WM_GetAllowedChannel(void) {
    WMErrCode result = WMi_CheckInitialized();
    if (result != WM_ERRCODE_SUCCESS) {
        return WM_GET_ALLOWED_CHANNEL_BEFORE_INIT;
    }
    return OS_ALLOWED_CHANNEL;
}

BOOL WMi_IsWirelessEnabled(void) {
    if (OS_WIRELESS_ENABLED) {
        return TRUE;
    }
    return FALSE;
}

WMLinkLevel WM_GetLinkLevel(void) {
    WMArm9Buf *p = WMi_GetSystemWork();
    WMErrCode result = WMi_CheckInitialized();
    if (result != WM_ERRCODE_SUCCESS) {
        return WM_LINK_LEVEL_0;
    }
    DC_InvalidateRange(&p->status->state, 2);
    switch (p->status->state) {
    case WM_STATE_MP_PARENT:
        DC_InvalidateRange(&p->status->child_bitmap, 2);
        if (p->status->child_bitmap == 0) {
            return WM_LINK_LEVEL_0;
        }
    case WM_STATE_MP_CHILD:
    case WM_STATE_DCF_CHILD:
        DC_InvalidateRange(&p->status->linkLevel, 2);
        return (WMLinkLevel) p->status->linkLevel;
    }
    return WM_LINK_LEVEL_0;
}

u16 WM_GetDispersionBeaconPeriod(void) {
    u8 mac[WM_SIZE_MACADDR];
    u16 ret;
    s32 i;

    OS_GetMacAddress(mac);
    for (i = 0, ret = 0; i < WM_SIZE_MACADDR; i++) {
        ret += mac[i];
    }
    ret += OS_VBLANK_COUNT;
    ret *= 7;
    return (u16) (WM_DEFAULT_BEACON_PERIOD + (ret % 20));
}

u16 WM_GetDispersionScanPeriod(void) {
    u8 mac[WM_SIZE_MACADDR];
    u16 ret;
    s32 i;

    OS_GetMacAddress(mac);
    for (i = 0, ret = 0; i < WM_SIZE_MACADDR; i++) {
        ret += mac[i];
    }
    ret += OS_VBLANK_COUNT;
    ret *= 13;
    return (u16) (WM_DEFAULT_SCAN_PERIOD + (ret % 10));
}

u16 WM_GetNextTgid(void) {
    static u32 tgid_bak = 0x10000;
    if (tgid_bak == 0x10000) {
        RTCTime rt;
        RTC_Init();
        if (RTC_GetTime(&rt) == RTC_RESULT_SUCCESS) {
            tgid_bak = (u16) (rt.second + (rt.minute << 8));
        }
    }
    tgid_bak = (u16) (tgid_bak + 1);
    return (u16) tgid_bak;
}
