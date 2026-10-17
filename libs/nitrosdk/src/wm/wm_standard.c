// NitroSDK WM: the basic asynchronous requests (initialize, reset, parent/scan/connect, disconnect).

#include "wm_private.h"

#define WM_SCANTYPE_ACTIVE 0
#define WM_SCANTYPE_PASSIVE 1
#define WM_SCANTYPE_ACTIVE_CUSTOM 2
#define WM_SCANTYPE_PASSIVE_CUSTOM 3

typedef struct {
    void *scanBuf;          // 0x00
    u16 scanBufSize;        // 0x04
    u16 channelList;        // 0x06
    u16 maxChannelTime;     // 0x08
    u8 bssid[WM_SIZE_MACADDR];  // 0x0a
    u16 scanType;           // 0x10
    u16 ssidLength;         // 0x12
    u8 ssid[WM_SIZE_SSID];  // 0x14
    u16 ssidMatchLength;    // 0x34
    u16 rsv[2];
} WMScanExParam;

typedef struct {
    u16 apiid;              // 0x00
    u16 channelList;        // 0x02
    void *scanBuf;          // 0x04
    u16 scanBufSize;        // 0x08
    u16 maxChannelTime;     // 0x0a
    u8 bssid[WM_SIZE_MACADDR];  // 0x0c
    u16 scanType;           // 0x12
    u16 ssidLength;         // 0x14
    u8 ssid[WM_SIZE_SSID];  // 0x16
    u16 ssidMatchLength;    // 0x36
    u16 rsv[2];
} WMStartScanExReq;

typedef struct {
    u16 length;             // 0x00: in halfwords
} WMBssDesc;

typedef struct {
    u16 apiid;              // 0x00
    u16 reserved;
    WMBssDesc *pInfo;       // 0x04
    u8 ssid[WM_SIZE_CHILD_SSID];    // 0x08
    BOOL powerSave;         // 0x20
    u16 reserved2;
    u16 authMode;           // 0x26
} WMStartConnectReq;

static BOOL WmCheckParentParameter(const WMParentParam *param);

WMErrCode WM_Initialize(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo) {
    WMErrCode result = WM_Init(wmSysBuf, dmaNo);
    WM_CHECK_RESULT(result);
    WMi_SetCallbackTable(WM_APIID_INITIALIZE, callback);
    {
        WMArm9Buf *p = WMi_GetSystemWork();
        result = WMi_SendCommand(WM_APIID_INITIALIZE, 3, (u32) p->WM7, (u32) p->status, (u32) p->fifo7to9);
    }
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_Reset(WMCallbackFunc callback) {
    WMErrCode result = WMi_CheckIdle();
    WM_CHECK_RESULT(result);
    WMi_SetCallbackTable(WM_APIID_RESET, callback);
    result = WMi_SendCommand(WM_APIID_RESET, 0);
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_End(WMCallbackFunc callback) {
    WMErrCode result = WMi_CheckStateEx(1, WM_STATE_IDLE);
    WM_CHECK_RESULT(result);
    WMi_SetCallbackTable(WM_APIID_END, callback);
    result = WMi_SendCommand(WM_APIID_END, 0);
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_SetParentParameter(WMCallbackFunc callback, const WMParentParam *pparaBuf) {
    WMErrCode result = WMi_CheckStateEx(1, WM_STATE_IDLE);
    WM_CHECK_RESULT(result);
    if (pparaBuf == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (pparaBuf->userGameInfoLength > 0 && pparaBuf->userGameInfo == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (pparaBuf->parentMaxSize + (pparaBuf->KS_Flag ? WM_KS_PARENT_DATA_SIZE : 0) > WM_SIZE_MP_DATA_MAX ||
        pparaBuf->childMaxSize + (pparaBuf->KS_Flag ? WM_KS_CHILD_DATA_SIZE : 0) > WM_SIZE_MP_DATA_MAX) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    (void) WmCheckParentParameter(pparaBuf);
    WMi_SetCallbackTable(WM_APIID_SET_P_PARAM, callback);
    DC_StoreRange(pparaBuf, WM_PARENT_PARAM_SIZE);
    if (pparaBuf->userGameInfoLength > 0) {
        DC_StoreRange(pparaBuf->userGameInfo, pparaBuf->userGameInfoLength);
    }
    result = WMi_SendCommand(WM_APIID_SET_P_PARAM, 1, pparaBuf);
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

static BOOL WmCheckParentParameter(const WMParentParam *param) {
    if (param->userGameInfoLength > WM_SIZE_USER_GAMEINFO) {
        return FALSE;
    }
    if (param->beaconPeriod < 10 || param->beaconPeriod > 1000) {
        return FALSE;
    }
    if (param->channel < 1 || param->channel > 14) {
        return FALSE;
    }
    return TRUE;
}

WMErrCode WMi_StartParentEx(WMCallbackFunc callback, BOOL powerSave) {
    WMErrCode result = WMi_CheckStateEx(1, WM_STATE_IDLE);
    WM_CHECK_RESULT(result);
    {
        WMArm9Buf *w9b = WMi_GetSystemWork();
        w9b->myAid = 0;
        w9b->connectedAidBitmap = 0;
    }
    WMi_SetCallbackTable(WM_APIID_START_PARENT, callback);
    result = WMi_SendCommand(WM_APIID_START_PARENT, 1, (u32) powerSave);
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_StartParent(WMCallbackFunc callback) {
    return WMi_StartParentEx(callback, TRUE);
}

WMErrCode WM_StartScanEx(WMCallbackFunc callback, const WMScanExParam *param) {
    WMErrCode result;
    WMStartScanExReq Req;

    result = WMi_CheckStateEx(3, WM_STATE_IDLE, WM_STATE_CLASS1, WM_STATE_SCAN);
    WM_CHECK_RESULT(result);
    if (param == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->scanBuf == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->scanBufSize > WM_SIZE_SCAN_EX_BUF) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->ssidLength > WM_SIZE_SSID) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->scanType != WM_SCANTYPE_ACTIVE && param->scanType != WM_SCANTYPE_PASSIVE &&
        param->scanType != WM_SCANTYPE_ACTIVE_CUSTOM && param->scanType != WM_SCANTYPE_PASSIVE_CUSTOM) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((param->scanType == WM_SCANTYPE_ACTIVE_CUSTOM || param->scanType == WM_SCANTYPE_PASSIVE_CUSTOM) &&
        param->ssidMatchLength > WM_SIZE_SSID) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    WMi_SetCallbackTable(WM_APIID_START_SCAN_EX, callback);
    Req.apiid = WM_APIID_START_SCAN_EX;
    Req.channelList = param->channelList;
    Req.scanBuf = param->scanBuf;
    Req.scanBufSize = param->scanBufSize;
    Req.maxChannelTime = param->maxChannelTime;
    MI_CpuCopy8(param->bssid, Req.bssid, WM_SIZE_MACADDR);
    Req.scanType = param->scanType;
    Req.ssidMatchLength = param->ssidMatchLength;
    Req.ssidLength = param->ssidLength;
    MI_CpuCopy8(param->ssid, Req.ssid, WM_SIZE_SSID);
    result = WMi_SendCommandDirect(&Req, sizeof(Req));
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_EndScan(WMCallbackFunc callback) {
    WMErrCode result = WMi_CheckStateEx(1, WM_STATE_SCAN);
    WM_CHECK_RESULT(result);
    WMi_SetCallbackTable(WM_APIID_END_SCAN, callback);
    result = WMi_SendCommand(WM_APIID_END_SCAN, 0);
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_StartConnectEx(WMCallbackFunc callback, const WMBssDesc *pInfo, const u8 *ssid, BOOL powerSave,
                            u16 authMode) {
    WMErrCode result;
    WMStartConnectReq Req;

    result = WMi_CheckStateEx(1, WM_STATE_IDLE);
    WM_CHECK_RESULT(result);
    if (pInfo == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    DC_StoreRange(pInfo, (u32) (pInfo->length * 2));
    {
        WMArm9Buf *w9b = WMi_GetSystemWork();
        w9b->myAid = 0;
        w9b->connectedAidBitmap = 0;
    }
    WMi_SetCallbackTable(WM_APIID_START_CONNECT, callback);
    Req.apiid = WM_APIID_START_CONNECT;
    Req.pInfo = (WMBssDesc *) pInfo;
    if (ssid != NULL) {
        MI_CpuCopy8(ssid, Req.ssid, WM_SIZE_CHILD_SSID);
    } else {
        MI_CpuFill8(Req.ssid, 0, WM_SIZE_CHILD_SSID);
    }
    Req.powerSave = powerSave;
    Req.authMode = authMode;
    result = WMi_SendCommandDirect(&Req, sizeof(Req));
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_Disconnect(WMCallbackFunc callback, u16 aid) {
    WMErrCode result;
    WMArm9Buf *p = WMi_GetSystemWork();

    result = WMi_CheckStateEx(5, WM_STATE_PARENT, WM_STATE_MP_PARENT, WM_STATE_CHILD, WM_STATE_MP_CHILD,
                              WM_STATE_DCF_CHILD);
    WM_CHECK_RESULT(result);
    if (p->status->state == WM_STATE_PARENT || p->status->state == WM_STATE_MP_PARENT) {
        if (aid < 1 || aid > WM_NUM_MAX_CHILD) {
            return WM_ERRCODE_INVALID_PARAM;
        }
        DC_InvalidateRange(&p->status->child_bitmap, 2);
        if (!(p->status->child_bitmap & (0x0001 << aid))) {
            return WM_ERRCODE_NO_CHILD;
        }
    } else if (aid != 0) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    WMi_SetCallbackTable(WM_APIID_DISCONNECT, callback);
    result = WMi_SendCommand(WM_APIID_DISCONNECT, 1, (u32) (0x0001 << aid));
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}
