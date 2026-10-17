// NitroSDK WM: MP communication (the parent polls the children once or more per frame).

#include "wm_private.h"

#define WM_MP_TMP_PARAM_MIN_FREQUENCY 0x0001
#define WM_MP_TMP_PARAM_FREQUENCY 0x0002

typedef struct {
    u32 mask;               // 0x00
    u16 minFrequency;       // 0x04
    u16 frequency;          // 0x06
    u8 rsv[0x14];
} WMMPTmpParam;

typedef struct {
    u16 apiid;              // 0x00
    u16 rsv1;
    u16 *recvBuf;           // 0x04
    u32 recvBufSize;        // 0x08: in halfwords
    u16 *sendBuf;           // 0x0c
    u32 sendBufSize;        // 0x10
    WMMPTmpParam tmpParam;  // 0x14
} WMStartMPReq;

static WMErrCode WMi_StartMP(WMCallbackFunc callback, u16 *recvBuf, u16 recvBufSize, u16 *sendBuf,
                             u16 sendBufSize, WMMPTmpParam *tmpParam);
int WM_GetMPReceiveBufferSize(void);
int WM_GetMPSendBufferSize(void);

static WMErrCode WMi_StartMP(WMCallbackFunc callback, u16 *recvBuf, u16 recvBufSize, u16 *sendBuf,
                             u16 sendBufSize, WMMPTmpParam *tmpParam) {
    WMErrCode result;
    WMStartMPReq Req;
    WMStatus *status = WMi_GetSystemWork()->status;

    result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_CHILD);
    WM_CHECK_RESULT(result);
    DC_InvalidateRange(&status->aid, 2);
    DC_InvalidateRange(&status->child_mpReady, 2);
    if (status->aid != 0 && status->child_mpReady != TRUE) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    DC_InvalidateRange(&status->mp_flag, 4);
    if (status->mp_flag == TRUE) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    if (recvBufSize & 0x3f) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (sendBufSize & 0x1f) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    DC_InvalidateRange(&status->mp_ignoreSizePrecheckMode, 2);
    if (status->mp_ignoreSizePrecheckMode == FALSE) {
        if (recvBufSize < WM_GetMPReceiveBufferSize()) {
            return WM_ERRCODE_INVALID_PARAM;
        }
        if (sendBufSize < WM_GetMPSendBufferSize()) {
            return WM_ERRCODE_INVALID_PARAM;
        }
    }
    WMi_SetCallbackTable(WM_APIID_START_MP, callback);
    MIi_CpuClear32(0, &Req, sizeof(Req));
    Req.apiid = WM_APIID_START_MP;
    Req.recvBuf = recvBuf;
    Req.recvBufSize = (u32) (recvBufSize / 2);
    Req.sendBuf = sendBuf;
    Req.sendBufSize = (u32) sendBufSize;
    MIi_CpuCopy32(tmpParam, &Req.tmpParam, sizeof(WMMPTmpParam));
    result = WMi_SendCommandDirect(&Req, sizeof(Req));
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}

WMErrCode WM_StartMP(WMCallbackFunc callback, u16 *recvBuf, u16 recvBufSize, u16 *sendBuf, u16 sendBufSize,
                     u16 mpFreq) {
    WMMPTmpParam tmpParam;
    MIi_CpuClear32(0, &tmpParam, sizeof(tmpParam));
    tmpParam.mask = WM_MP_TMP_PARAM_MIN_FREQUENCY | WM_MP_TMP_PARAM_FREQUENCY;
    tmpParam.minFrequency = mpFreq;
    tmpParam.frequency = mpFreq;
    return WMi_StartMP(callback, recvBuf, recvBufSize, sendBuf, sendBufSize, &tmpParam);
}

WMErrCode WM_SetMPDataToPortEx(WMCallbackFunc callback, void *arg, const u16 *sendData, u16 sendDataSize,
                               u16 destBitmap, u16 port, u16 prio) {
    WMErrCode result;
    u16 childBitmap = 1;
    WMStatus *status = WMi_GetSystemWork()->status;

    result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);
    WM_CHECK_RESULT(result);
    DC_InvalidateRange(&status->mp_sendSize, 2);
    DC_InvalidateRange(&status->aid, 2);
    if (status->aid == 0) {
        DC_InvalidateRange(&status->child_bitmap, 2);
        childBitmap = status->child_bitmap;
        DC_InvalidateRange(&status->mp_readyBitmap, 2);
    }
    if (sendData == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (childBitmap == 0) {
        return WM_ERRCODE_NO_CHILD;
    }
    DC_InvalidateRange(&status->mp_sendBuf, 2);
    if ((void *) sendData == status->mp_sendBuf) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (sendDataSize > WM_SIZE_MP_DATA_MAX) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (sendDataSize == 0) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    DC_StoreRange(sendData, sendDataSize);
    result = WMi_SendCommand(WM_APIID_SET_MP_DATA, 7, (u32) sendData, (u32) sendDataSize, (u32) destBitmap,
                             (u32) port, (u32) prio, (u32) callback, (u32) arg);
    WM_CHECK_RESULT(result);
    return WM_ERRCODE_OPERATING;
}
