// NitroSDK WM: setting up the library, the command buffers, and dispatching what the ARM7 sends back.

#include "wm_private.h"
#include <stdarg.h>

// The wm_callback_control word in the system work area: the ARM7 sets it when a callback is pending.
#define WM_CALLBACK_CONTROL (*(u16 *) 0x027fff96)
#define WM_EXCEPTION_CB_MASK 0x0001

static u16 fifoBuf[WM_FIFO_BUF_NUM][WM_FIFO_BUF_SIZE / sizeof(u16)] __attribute__((aligned(32)));
static WMPortRecvCallback portRecvCallback;
static OSMessage bufMsg[WM_FIFO_BUF_NUM];
static OSMessageQueue bufMsgQ;
static WMArm9Buf *wm9buf;
static u16 wmInitialized;

static WMErrCode WmInitCore(void *wmSysBuf, u16 dmaNo, u32 bufSize);
static u32 *WmGetCommandBuffer4Arm7(void);
static void WmReceiveFifo(int tag, u32 fifo_buf_adr, BOOL err);
static void WmClearFifoRecvFlag(void);

WMErrCode WM_Init(void *sysBuf, u16 dmaNo) {
    WMErrCode result = WmInitCore(sysBuf, dmaNo, WM_SYSTEM_BUF_SIZE);
    if (result == WM_ERRCODE_SUCCESS) {
        wm9buf->scanOnlyFlag = FALSE;
    }
    return result;
}

static WMErrCode WmInitCore(void *wmSysBuf, u16 dmaNo, u32 bufSize) {
    OSIntrMode e = OS_DisableInterrupts();
    s32 i;
    s32 j;

    if (wmInitialized) {
        OS_RestoreInterrupts(e);
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    if (wmSysBuf == NULL) {
        OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (dmaNo > 3) {
        OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((u32) wmSysBuf & 0x01f) {
        OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }
    PXI_Init();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WM, PXI_PROC_ARM7)) {
        OS_RestoreInterrupts(e);
        return WM_ERRCODE_WM_DISABLE;
    }

    DC_InvalidateRange(wmSysBuf, bufSize);
    MI_DmaFill32(dmaNo, wmSysBuf, 0, bufSize);
    wm9buf = (WMArm9Buf *) wmSysBuf;
    wm9buf->WM7 = (void *) ((u32) wm9buf + WM_ARM9WM_BUF_SIZE);
    wm9buf->status = (WMStatus *) ((u32) wm9buf->WM7 + WM_ARM7WM_BUF_SIZE);
    wm9buf->indbuf = (u32 *) ((u32) wm9buf->status + WM_STATUS_BUF_SIZE);
    wm9buf->fifo7to9 = (u32 *) ((u32) wm9buf->indbuf + WM_FIFO_BUF_SIZE);
    WmClearFifoRecvFlag();

    wm9buf->dmaNo = dmaNo;
    wm9buf->connectedAidBitmap = 0;
    wm9buf->myAid = 0;
    for (i = 0; i < WM_NUM_OF_PORT; i++) {
        wm9buf->portCallbackTable[i] = NULL;
        wm9buf->portCallbackArgument[i] = NULL;
    }

    OS_InitMessageQueue(&bufMsgQ, bufMsg, WM_FIFO_BUF_NUM);
    for (j = 0; j < WM_FIFO_BUF_NUM; j++) {
        fifoBuf[j][0] = WM_API_REQUEST_ACCEPTED;
        DC_StoreRange(fifoBuf[j], 2);
        OS_SendMessage(&bufMsgQ, fifoBuf[j], OS_MESSAGE_BLOCK);
    }

    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, WmReceiveFifo);
    wmInitialized = 1;
    OS_RestoreInterrupts(e);
    return WM_ERRCODE_SUCCESS;
}

WMErrCode WM_Finish(void) {
    OSIntrMode e = OS_DisableInterrupts();
    WMErrCode result;

    if (WMi_CheckInitialized() != WM_ERRCODE_SUCCESS) {
        OS_RestoreInterrupts(e);
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    result = WMi_CheckStateEx(1, WM_STATE_READY);
    WM_CHECK_RESULT(result);

    WmClearFifoRecvFlag();
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, NULL);
    wm9buf = 0;
    wmInitialized = 0;
    OS_RestoreInterrupts(e);
    return WM_ERRCODE_SUCCESS;
}

void WMi_SetCallbackTable(WMApiid id, WMCallbackFunc callback) {
    wm9buf->CallbackTable[id] = callback;
}

static u32 *WmGetCommandBuffer4Arm7(void) {
    u32 *tmpAddr;
    if (!OS_ReceiveMessage(&bufMsgQ, (OSMessage *) &tmpAddr, OS_MESSAGE_NOBLOCK)) {
        return NULL;
    }
    DC_InvalidateRange(tmpAddr, sizeof(u16));
    if ((*(u16 *) tmpAddr & WM_API_REQUEST_ACCEPTED) == 0) {
        OS_JamMessage(&bufMsgQ, tmpAddr, OS_MESSAGE_BLOCK);
        return NULL;
    }
    return tmpAddr;
}

WMErrCode WMi_SendCommand(WMApiid id, u16 paramNum, ...) {
    va_list vlist;
    s32 i;
    int result;
    u32 *tmpAddr = WmGetCommandBuffer4Arm7();

    if (tmpAddr == NULL) {
        return WM_ERRCODE_FIFO_ERROR;
    }
    *(u16 *) tmpAddr = (u16) id;
    va_start(vlist, paramNum);
    for (i = 0; i < paramNum; i++) {
        tmpAddr[i + 1] = va_arg(vlist, u32);
    }
    va_end(vlist);
    DC_StoreRange(tmpAddr, WM_FIFO_BUF_SIZE);
    result = PXI_SendWordByFifo(PXI_FIFO_TAG_WM, (u32) tmpAddr, FALSE);
    OS_SendMessage(&bufMsgQ, tmpAddr, OS_MESSAGE_BLOCK);
    if (result < 0) {
        return WM_ERRCODE_FIFO_ERROR;
    }
    return WM_ERRCODE_OPERATING;
}

WMErrCode WMi_SendCommandDirect(void *data, u32 length) {
    int result;
    u32 *tmpAddr = WmGetCommandBuffer4Arm7();

    if (tmpAddr == NULL) {
        return WM_ERRCODE_FIFO_ERROR;
    }
    MI_CpuCopy8(data, tmpAddr, length);
    DC_StoreRange(tmpAddr, length);
    result = PXI_SendWordByFifo(PXI_FIFO_TAG_WM, (u32) tmpAddr, FALSE);
    OS_SendMessage(&bufMsgQ, tmpAddr, OS_MESSAGE_BLOCK);
    if (result < 0) {
        return WM_ERRCODE_FIFO_ERROR;
    }
    return WM_ERRCODE_OPERATING;
}

WMArm9Buf *WMi_GetSystemWork(void) {
    return wm9buf;
}

WMErrCode WMi_CheckInitialized(void) {
    if (!wmInitialized) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    return WM_ERRCODE_SUCCESS;
}

WMErrCode WMi_CheckIdle(void) {
    WMErrCode result = WMi_CheckInitialized();
    WM_CHECK_RESULT(result);
    DC_InvalidateRange(&wm9buf->status->state, 2);
    if (wm9buf->status->state == WM_STATE_READY || wm9buf->status->state == WM_STATE_STOP) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    return WM_ERRCODE_SUCCESS;
}

WMErrCode WMi_CheckStateEx(s32 paramNum, ...) {
    WMErrCode result;
    u16 now;
    u32 temp;
    va_list vlist;

    result = WMi_CheckInitialized();
    WM_CHECK_RESULT(result);
    DC_InvalidateRange(&wm9buf->status->state, 2);
    now = wm9buf->status->state;
    result = WM_ERRCODE_ILLEGAL_STATE;
    va_start(vlist, paramNum);
    for (; paramNum; paramNum--) {
        temp = va_arg(vlist, u32);
        if (temp == now) {
            result = WM_ERRCODE_SUCCESS;
        }
    }
    va_end(vlist);
    return result;
}

static void WmReceiveFifo(int tag, u32 fifo_buf_adr, BOOL err) {
#pragma unused(tag)
    WMCallback *callback_p = (WMCallback *) fifo_buf_adr;
    WMArm9Buf *w9b = wm9buf;

    if (err) {
        return;
    }
    DC_InvalidateRange(w9b->fifo7to9, WM_FIFO_BUF_SIZE);
    if (!w9b->scanOnlyFlag) {
        DC_InvalidateRange(w9b->status, WM_STATUS_BUF_SIZE);
    }
    if ((u32) callback_p != (u32) w9b->fifo7to9) {
        DC_InvalidateRange(callback_p, WM_FIFO_BUF_SIZE);
    }

    if (callback_p->apiid >= WM_NUM_OF_CALLBACK) {
        if (callback_p->apiid == WM_APIID_INDICATION) {
            if (callback_p->errcode == WM_ERRCODE_FLASH_ERROR) {
                OS_Terminate();
            }
            if (w9b->indCallback != NULL) {
                w9b->indCallback(callback_p);
            }
        } else if (callback_p->apiid == WM_APIID_PORT_RECV) {
            WMPortRecvCallback *cb_Port = (WMPortRecvCallback *) callback_p;
            if (w9b->portCallbackTable[cb_Port->port] != NULL) {
                cb_Port->arg = w9b->portCallbackArgument[cb_Port->port];
                cb_Port->connectedAidBitmap = (u16) w9b->connectedAidBitmap;
                DC_InvalidateRange(cb_Port->recvBuf, w9b->status->mp_recvBufSize);
                w9b->portCallbackTable[cb_Port->port](cb_Port);
            }
        } else if (callback_p->apiid == WM_APIID_PORT_SEND) {
            WMPortSendCallback *cb_Port = (WMPortSendCallback *) callback_p;
            callback_p->apiid = WM_APIID_SET_MP_DATA;
            if (cb_Port->callback != NULL) {
                cb_Port->callback(cb_Port);
            }
        }
    } else {
        if (callback_p->apiid == WM_APIID_START_MP) {
            WMStartMPCallback *cb_MP = (WMStartMPCallback *) callback_p;
            if ((cb_MP->state == WM_STATECODE_MP_IND || cb_MP->state == WM_STATECODE_MPACK_IND) &&
                cb_MP->errcode == WM_ERRCODE_SUCCESS) {
                DC_InvalidateRange(cb_MP->recvBuf, w9b->status->mp_recvBufSize);
            }
        }
        if (callback_p->apiid == WM_APIID_END && callback_p->errcode == WM_ERRCODE_SUCCESS) {
            WMCallbackFunc cb = w9b->CallbackTable[callback_p->apiid];
            WM_Finish();
            if (cb != NULL) {
                cb(callback_p);
            }
            return;
        }
        if (w9b->CallbackTable[callback_p->apiid] != NULL) {
            w9b->CallbackTable[callback_p->apiid](callback_p);
            if (!wmInitialized) {
                return;
            }
        }

        if (callback_p->apiid == WM_APIID_START_PARENT || callback_p->apiid == WM_APIID_START_CONNECT) {
            u16 state, aid, myAid, reason;
            u8 *macAddress;
            u8 *ssid;
            u16 parentSize, childSize;

            if (callback_p->apiid == WM_APIID_START_PARENT) {
                WMStartParentCallback *cb = (WMStartParentCallback *) callback_p;
                state = cb->state;
                aid = cb->aid;
                myAid = 0;
                macAddress = cb->macAddress;
                ssid = cb->ssid;
                reason = cb->reason;
                parentSize = cb->parentSize;
                childSize = cb->childSize;
            } else if (callback_p->apiid == WM_APIID_START_CONNECT) {
                WMStartConnectCallback *cb = (WMStartConnectCallback *) callback_p;
                state = cb->state;
                aid = 0;
                macAddress = cb->macAddress;
                ssid = NULL;
                myAid = cb->aid;
                reason = cb->reason;
                parentSize = cb->parentSize;
                childSize = cb->childSize;
            }
            if (state == WM_STATECODE_CONNECTED || state == WM_STATECODE_DISCONNECTED ||
                state == WM_STATECODE_DISCONNECTED_FROM_MYSELF) {
                u16 iPort;

                if (state == WM_STATECODE_CONNECTED) {
                    w9b->connectedAidBitmap |= 1 << aid;
                } else {
                    w9b->connectedAidBitmap &= ~(1 << aid);
                }
                w9b->myAid = myAid;

                MI_CpuFill8(&portRecvCallback, 0, sizeof(WMPortRecvCallback));
                portRecvCallback.apiid = WM_APIID_PORT_RECV;
                portRecvCallback.errcode = WM_ERRCODE_SUCCESS;
                portRecvCallback.state = state;
                portRecvCallback.recvBuf = NULL;
                portRecvCallback.data = NULL;
                portRecvCallback.length = 0;
                portRecvCallback.aid = aid;
                portRecvCallback.myAid = myAid;
                portRecvCallback.connectedAidBitmap = (u16) w9b->connectedAidBitmap;
                portRecvCallback.seqNo = 0xffff;
                portRecvCallback.reason = reason;
                MI_CpuCopy8(macAddress, portRecvCallback.macAddress, WM_SIZE_MACADDR);
                if (ssid != NULL) {
                    MIi_CpuCopy16(ssid, portRecvCallback.ssid, WM_SIZE_CHILD_SSID);
                } else {
                    MIi_CpuClear16(0, portRecvCallback.ssid, WM_SIZE_CHILD_SSID);
                }
                portRecvCallback.maxSendDataSize = (u16) (myAid == 0 ? parentSize : childSize);
                portRecvCallback.maxRecvDataSize = (u16) (myAid == 0 ? childSize : parentSize);
                for (iPort = 0; iPort < WM_NUM_OF_PORT; iPort++) {
                    portRecvCallback.port = iPort;
                    if (w9b->portCallbackTable[iPort] != NULL) {
                        portRecvCallback.arg = w9b->portCallbackArgument[iPort];
                        w9b->portCallbackTable[iPort](&portRecvCallback);
                    }
                }
            }
        }
    }

    DC_InvalidateRange(w9b->fifo7to9, WM_FIFO_BUF_SIZE);
    WmClearFifoRecvFlag();
    if ((u32) callback_p != (u32) w9b->fifo7to9) {
        callback_p->apiid |= WM_API_REQUEST_ACCEPTED;
        DC_StoreRange(callback_p, WM_FIFO_BUF_SIZE);
    }
}

static void WmClearFifoRecvFlag(void) {
    if (WM_CALLBACK_CONTROL & WM_EXCEPTION_CB_MASK) {
        WM_CALLBACK_CONTROL &= ~WM_EXCEPTION_CB_MASK;
    }
}

u16 WM_GetAID(void) {
    u16 myAid;
    OSIntrMode e = OS_DisableInterrupts();
    if (wm9buf != NULL) {
        myAid = wm9buf->myAid;
    } else {
        myAid = 0;
    }
    OS_RestoreInterrupts(e);
    return myAid;
}

u16 WM_GetConnectedAIDs(void) {
    u32 connectedAidBitmap;
    OSIntrMode e = OS_DisableInterrupts();
    if (wm9buf != NULL) {
        connectedAidBitmap = wm9buf->connectedAidBitmap;
    } else {
        connectedAidBitmap = 0;
    }
    OS_RestoreInterrupts(e);
    return (u16) connectedAidBitmap;
}

u16 WMi_GetMPReadyAIDs(void) {
    u16 mpReadyAidBitmap;
    OSIntrMode e = OS_DisableInterrupts();
    if (wm9buf != NULL) {
        WMStatus *status = wm9buf->status;
        DC_InvalidateRange(&status->mp_readyBitmap, 2);
        mpReadyAidBitmap = status->mp_readyBitmap;
    } else {
        mpReadyAidBitmap = 0;
    }
    OS_RestoreInterrupts(e);
    return mpReadyAidBitmap;
}
