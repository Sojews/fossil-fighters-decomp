// NitroSDK WM: data sharing. Every frame each station contributes a fixed-size block; the parent gathers them
// into a data set and sends it to everyone, so all stations see the same frame of everyone's data.

#include "wm_private.h"

#define WM_DS_DATASET_NUM 4
#define WM_DS_DATA_SIZE 508
#define WM_DS_HEADER_SIZE 4
#define WM_PRIORITY_HIGH 1

typedef enum {
    WM_DS_STATE_READY = 0,
    WM_DS_STATE_START,
    WM_DS_STATE_PAUSING,
    WM_DS_STATE_PAUSED,
    WM_DS_STATE_RETRY_SEND,
    WM_DS_STATE_ERROR
} WMDataSharingState;

typedef struct {
    u16 aidBitmap;          // 0x000: whose data is still missing
    u16 receivedBitmap;     // 0x002
    u16 data[WM_DS_DATA_SIZE / sizeof(u16)];    // 0x004
} WMDataSet;

typedef struct {
    WMDataSet ds[WM_DS_DATASET_NUM];    // 0x000
    u16 seqNum[WM_DS_DATASET_NUM];      // 0x800
    u16 writeIndex;                     // 0x808
    u16 sendIndex;                      // 0x80a
    u16 readIndex;                      // 0x80c
    u16 aidBitmap;                      // 0x80e
    u16 dataLength;                     // 0x810
    u16 stationNumber;                  // 0x812
    u16 dataSetLength;                  // 0x814
    u16 port;                           // 0x816
    u16 doubleMode;                     // 0x818
    u16 currentSeqNum;                  // 0x81a
    u16 state;                          // 0x81c
    u16 reserved[1];
} WMDataSharingInfo;

u32 MATH_CountPopulation(u32 x);

static void WmDataSharingSetDataCallback(void *callback);
static void WmDataSharingReceiveCallback_Parent(void *callback);
static void WmDataSharingReceiveCallback_Child(void *callback);
static void WmDataSharingReceiveData(WMDataSharingInfo *dsInfo, u16 aid, u16 *data);
static void WmDataSharingSendDataSet(WMDataSharingInfo *dsInfo, BOOL delayed);
static u16 *WmGetSharedDataAddress(WMDataSharingInfo *dsInfo, u32 aidBitmap, u16 *receiveBuf, u32 aid);

WMErrCode WM_StartDataSharing(WMDataSharingInfo *dsInfo, u16 port, u16 aidBitmap, u16 dataLength,
                              BOOL doubleMode) {
    WMErrCode result;
    u16 aid;
    u16 connectedAIDs = 1;

    result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);
    WM_CHECK_RESULT(result);
    if (dsInfo == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (port >= WM_NUM_OF_PORT) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (aidBitmap == 0) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    aid = WM_GetAID();
    if (aid == 0) {
        connectedAIDs = WMi_GetMPReadyAIDs();
    }

    MIi_CpuClearFast(0, dsInfo, sizeof(WMDataSharingInfo));
    dsInfo->writeIndex = 0;
    dsInfo->sendIndex = 0;
    dsInfo->readIndex = 0;
    dsInfo->dataLength = dataLength;
    dsInfo->port = port;
    dsInfo->aidBitmap = 0;
    dsInfo->doubleMode = doubleMode ? TRUE : FALSE;
    aidBitmap |= 0x0001 << aid;
    dsInfo->aidBitmap = aidBitmap;
    {
        u32 n = MATH_CountPopulation(aidBitmap);
        dsInfo->stationNumber = (u16) n;
        dsInfo->dataSetLength = (u16) (dataLength * n);
    }
    if (dsInfo->dataSetLength > WM_DS_DATA_SIZE) {
        dsInfo->aidBitmap = 0;
        return WM_ERRCODE_INVALID_PARAM;
    }
    dsInfo->dataSetLength += WM_DS_HEADER_SIZE;
    dsInfo->state = WM_DS_STATE_START;

    if (aid == 0) {
        s32 i;
        for (i = 0; i < WM_DS_DATASET_NUM; i++) {
            dsInfo->ds[i].aidBitmap = (u16) (dsInfo->aidBitmap & (connectedAIDs | 0x0001));
        }
        WM_SetPortCallback(port, WmDataSharingReceiveCallback_Parent, dsInfo);
        for (i = 0; i < ((dsInfo->doubleMode == TRUE) ? 2 : 1); i++) {
            WMErrCode res;
            dsInfo->writeIndex = (u16) ((dsInfo->writeIndex + 1) & (WM_DS_DATASET_NUM - 1));
            res = WM_SetMPDataToPortEx(WmDataSharingSetDataCallback, dsInfo, (u16 *) &dsInfo->ds[i],
                                       dsInfo->dataSetLength, (u16) (connectedAIDs & dsInfo->aidBitmap),
                                       dsInfo->port, WM_PRIORITY_HIGH);
            if (res == WM_ERRCODE_NO_CHILD) {
                dsInfo->seqNum[i] = 0xffff;
                dsInfo->sendIndex = (u16) ((dsInfo->sendIndex + 1) & (WM_DS_DATASET_NUM - 1));
            } else if (res != WM_ERRCODE_SUCCESS && res != WM_ERRCODE_OPERATING) {
                dsInfo->state = WM_DS_STATE_ERROR;
                return WM_ERRCODE_FAILED;
            }
        }
    } else {
        dsInfo->sendIndex = WM_DS_DATASET_NUM - 1;
        WM_SetPortCallback(port, WmDataSharingReceiveCallback_Child, dsInfo);
    }
    return WM_ERRCODE_SUCCESS;
}

WMErrCode WM_StepDataSharing(WMDataSharingInfo *dsInfo, const u16 *sendData, WMDataSet *receiveData) {
    WMErrCode result;
    u16 aid;
    u16 connectedAIDs;

    result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);
    WM_CHECK_RESULT(result);
    if (dsInfo == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (sendData == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (receiveData == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    aid = WM_GetAID();
    if (aid == 0) {
        connectedAIDs = WMi_GetMPReadyAIDs();
    }
    if (dsInfo->state == WM_DS_STATE_ERROR) {
        return WM_ERRCODE_FAILED;
    }
    if (dsInfo->state != WM_DS_STATE_START && dsInfo->state != WM_DS_STATE_RETRY_SEND) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    result = WM_ERRCODE_NO_DATASET;
    if (aid == 0) {
        BOOL sendFlag = FALSE;
        BOOL delayed = FALSE;

        if (dsInfo->state == WM_DS_STATE_RETRY_SEND) {
            WMErrCode res;
            u16 oldWI;
            dsInfo->state = WM_DS_STATE_START;
            oldWI = (u16) ((dsInfo->writeIndex + WM_DS_DATASET_NUM - 1) & (WM_DS_DATASET_NUM - 1));
            res = WM_SetMPDataToPortEx(WmDataSharingSetDataCallback, dsInfo, (u16 *) &dsInfo->ds[oldWI],
                                       dsInfo->dataSetLength, (u16) (connectedAIDs & dsInfo->aidBitmap),
                                       dsInfo->port, WM_PRIORITY_HIGH);
            if (res == WM_ERRCODE_NO_CHILD) {
                dsInfo->seqNum[oldWI] = 0xffff;
                dsInfo->sendIndex = (u16) ((dsInfo->sendIndex + 1) & (WM_DS_DATASET_NUM - 1));
            } else if (res != WM_ERRCODE_SUCCESS && res != WM_ERRCODE_OPERATING) {
                dsInfo->state = WM_DS_STATE_ERROR;
                return WM_ERRCODE_FAILED;
            }
        }
        if (dsInfo->readIndex != dsInfo->sendIndex) {
            dsInfo->ds[dsInfo->readIndex].aidBitmap |= 0x0001;
            MIi_CpuCopy16(&dsInfo->ds[dsInfo->readIndex], receiveData, sizeof(WMDataSet));
            result = WM_ERRCODE_SUCCESS;
            dsInfo->currentSeqNum = dsInfo->seqNum[dsInfo->readIndex];
            dsInfo->readIndex = (u16) ((dsInfo->readIndex + 1) & (WM_DS_DATASET_NUM - 1));
            sendFlag = TRUE;
            if (dsInfo->doubleMode == FALSE && connectedAIDs != 0 &&
                dsInfo->ds[dsInfo->writeIndex].aidBitmap == 0x0001) {
                delayed = TRUE;
            } else {
                delayed = FALSE;
            }
        }
        WmDataSharingSendDataSet(dsInfo, FALSE);
        if (sendFlag) {
            WmDataSharingReceiveData(dsInfo, 0, (u16 *) sendData);
            if (dsInfo->doubleMode == FALSE) {
                WmDataSharingSendDataSet(dsInfo, delayed);
            }
        }
    } else {
        BOOL sendFlag = FALSE;

        if (dsInfo->state == WM_DS_STATE_RETRY_SEND) {
            dsInfo->state = WM_DS_STATE_START;
            sendFlag = TRUE;
        } else if (dsInfo->readIndex != dsInfo->writeIndex) {
            if (!(dsInfo->ds[dsInfo->readIndex].aidBitmap & 0x0001)) {
                dsInfo->ds[dsInfo->readIndex].aidBitmap |= 0x0001;
            } else {
                MIi_CpuCopy16(&dsInfo->ds[dsInfo->readIndex], receiveData, sizeof(WMDataSet));
                sendFlag = TRUE;
                result = WM_ERRCODE_SUCCESS;
                dsInfo->currentSeqNum = dsInfo->seqNum[dsInfo->readIndex];
                dsInfo->readIndex = (u16) ((dsInfo->readIndex + 1) & (WM_DS_DATASET_NUM - 1));
            }
        }
        if (sendFlag) {
            WMErrCode res;
            // (The child's own block goes out from a fixed spot in the slot.)
            u16 *dataBuf = (u16 *) ((u8 *) &dsInfo->ds[dsInfo->sendIndex] + 32);
            MIi_CpuCopy16(sendData, dataBuf, dsInfo->dataLength);
            res = WM_SetMPDataToPortEx(WmDataSharingSetDataCallback, dsInfo, dataBuf, dsInfo->dataLength,
                                       dsInfo->aidBitmap, dsInfo->port, WM_PRIORITY_HIGH);
            dsInfo->sendIndex = (u16) ((dsInfo->sendIndex + 1) & (WM_DS_DATASET_NUM - 1));
            if (res != WM_ERRCODE_OPERATING && res != WM_ERRCODE_SUCCESS) {
                dsInfo->state = WM_DS_STATE_ERROR;
                result = WM_ERRCODE_FAILED;
            }
        }
    }
    return result;
}

static void WmDataSharingSetDataCallback(void *callback) {
    WMPortSendCallback *callback_p = (WMPortSendCallback *) callback;
    WMArm9Buf *w9b = WMi_GetSystemWork();
    WMDataSharingInfo *dsInfo = (WMDataSharingInfo *) w9b->portCallbackArgument[callback_p->port];

    if ((w9b->portCallbackTable[callback_p->port] == WmDataSharingReceiveCallback_Parent ||
         w9b->portCallbackTable[callback_p->port] == WmDataSharingReceiveCallback_Child) &&
        dsInfo != NULL && dsInfo == callback_p->arg) {
        u16 aid = WM_GetAID();
        if (callback_p->errcode == WM_ERRCODE_SUCCESS) {
            if (aid == 0) {
                dsInfo->seqNum[dsInfo->sendIndex] = (u16) (callback_p->seqNo >> 1);
                dsInfo->sendIndex = (u16) ((dsInfo->sendIndex + 1) & (WM_DS_DATASET_NUM - 1));
            }
        } else if (callback_p->errcode == WM_ERRCODE_SEND_QUEUE_FULL) {
            if (aid != 0) {
                dsInfo->sendIndex = (u16) ((dsInfo->sendIndex + WM_DS_DATASET_NUM - 1) & (WM_DS_DATASET_NUM - 1));
            }
            dsInfo->state = WM_DS_STATE_RETRY_SEND;
        } else {
            dsInfo->state = WM_DS_STATE_ERROR;
        }
    }
}

static void WmDataSharingReceiveCallback_Parent(void *callback) {
    WMPortRecvCallback *callback_p = (WMPortRecvCallback *) callback;
    WMDataSharingInfo *dsInfo = (WMDataSharingInfo *) callback_p->arg;

    if (dsInfo == NULL) {
        return;
    }
    if (callback_p->errcode == WM_ERRCODE_SUCCESS) {
        switch (callback_p->state) {
        case WM_STATECODE_PORT_RECV:
            WmDataSharingReceiveData(dsInfo, callback_p->aid, callback_p->data);
            WmDataSharingSendDataSet(dsInfo, FALSE);
            break;
        case WM_STATECODE_CONNECTED:
            WmDataSharingSendDataSet(dsInfo, FALSE);
            break;
        case WM_STATECODE_PORT_INIT:
            break;
        case WM_STATECODE_DISCONNECTED:
        case WM_STATECODE_DISCONNECTED_FROM_MYSELF: {
            u16 aid = callback_p->aid;
            u32 aidBit = 0x0001U << aid;
            OSIntrMode enabled = OS_DisableInterrupts();
            u16 wi = dsInfo->writeIndex;
            dsInfo->ds[wi].aidBitmap &= ~aidBit;
            if (dsInfo->doubleMode == TRUE) {
                dsInfo->ds[(u16) ((wi + 1) & (WM_DS_DATASET_NUM - 1))].aidBitmap &= ~aidBit;
            }
            OS_RestoreInterrupts(enabled);
            WmDataSharingSendDataSet(dsInfo, FALSE);
            if (dsInfo->doubleMode == TRUE) {
                WmDataSharingSendDataSet(dsInfo, FALSE);
            }
            break;
        }
        }
    } else {
        dsInfo->state = WM_DS_STATE_ERROR;
    }
}

static void WmDataSharingReceiveCallback_Child(void *callback) {
    WMPortRecvCallback *callback_p = (WMPortRecvCallback *) callback;
    WMDataSharingInfo *dsInfo = (WMDataSharingInfo *) callback_p->arg;

    if (dsInfo == NULL) {
        return;
    }
    if (callback_p->errcode == WM_ERRCODE_SUCCESS) {
        switch (callback_p->state) {
        case WM_STATECODE_PORT_RECV: {
            WMDataSet *dataSet = (WMDataSet *) callback_p->data;
            u16 length = callback_p->length;
            u16 aidBitmap = dataSet->aidBitmap;
            u16 aid = WM_GetAID();
            if (length != dsInfo->dataSetLength) {
                if (length > sizeof(WMDataSet)) {
                    length = sizeof(WMDataSet);
                }
            }
            if (length < WM_DS_HEADER_SIZE) {
                break;
            }
            if (aidBitmap & (0x0001 << aid)) {
                MIi_CpuCopy16(dataSet, &dsInfo->ds[dsInfo->writeIndex], length);
                dsInfo->seqNum[dsInfo->writeIndex] = (u16) (callback_p->seqNo >> 1);
                dsInfo->writeIndex = (u16) ((dsInfo->writeIndex + 1) & (WM_DS_DATASET_NUM - 1));
            }
            break;
        }
        case WM_STATECODE_CONNECTED:
        case WM_STATECODE_PORT_INIT:
        case WM_STATECODE_DISCONNECTED:
        case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
            break;
        }
    } else {
        dsInfo->state = WM_DS_STATE_ERROR;
    }
}

static void WmDataSharingReceiveData(WMDataSharingInfo *dsInfo, u16 aid, u16 *data) {
    u16 aidBit = (u16) (0x0001 << aid);

    if (dsInfo->aidBitmap & aidBit) {
        u16 *buf;
        u16 index = dsInfo->writeIndex;
        if (!(dsInfo->ds[index].aidBitmap & aidBit)) {
            if (dsInfo->doubleMode != TRUE) {
                return;
            }
            index = (u16) ((index + 1) & (WM_DS_DATASET_NUM - 1));
            if (!(dsInfo->ds[index].aidBitmap & aidBit)) {
                return;
            }
        }
        buf = WmGetSharedDataAddress(dsInfo, dsInfo->aidBitmap, dsInfo->ds[index].data, aid);
        if (data != NULL) {
            MIi_CpuCopy16(data, buf, dsInfo->dataLength);
        } else {
            MIi_CpuClear16(0, buf, dsInfo->dataLength);
        }
        {
            OSIntrMode enabled = OS_DisableInterrupts();
            dsInfo->ds[index].aidBitmap &= ~aidBit;
            dsInfo->ds[index].receivedBitmap |= aidBit;
            OS_RestoreInterrupts(enabled);
        }
    }
}

static void WmDataSharingSendDataSet(WMDataSharingInfo *dsInfo, BOOL delayed) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (dsInfo->ds[dsInfo->writeIndex].aidBitmap == 0) {
        WMErrCode res;
        u16 newWI, oldWI, resetWI;
        u16 connectedAIDs = WMi_GetMPReadyAIDs();

        oldWI = dsInfo->writeIndex;
        newWI = (u16) ((oldWI + 1) & (WM_DS_DATASET_NUM - 1));
        if (dsInfo->doubleMode == TRUE) {
            resetWI = (u16) ((newWI + 1) & (WM_DS_DATASET_NUM - 1));
        } else {
            resetWI = newWI;
        }
        MIi_CpuClear16(0, &dsInfo->ds[resetWI], sizeof(WMDataSet));
        dsInfo->ds[resetWI].aidBitmap = (u16) (dsInfo->aidBitmap & (connectedAIDs | 0x0001));
        dsInfo->writeIndex = newWI;
        dsInfo->ds[oldWI].aidBitmap = dsInfo->aidBitmap;
        if (delayed == TRUE) {
            dsInfo->ds[oldWI].aidBitmap &= ~0x0001;
        }
        OS_RestoreInterrupts(enabled);
        res = WM_SetMPDataToPortEx(WmDataSharingSetDataCallback, dsInfo, (u16 *) &dsInfo->ds[oldWI],
                                   dsInfo->dataSetLength, (u16) (connectedAIDs & dsInfo->aidBitmap), dsInfo->port,
                                   WM_PRIORITY_HIGH);
        if (res == WM_ERRCODE_NO_CHILD) {
            dsInfo->seqNum[oldWI] = 0xffff;
            dsInfo->sendIndex = (u16) ((dsInfo->sendIndex + 1) & (WM_DS_DATASET_NUM - 1));
        } else if (res != WM_ERRCODE_SUCCESS && res != WM_ERRCODE_OPERATING) {
            dsInfo->state = WM_DS_STATE_ERROR;
        }
    } else {
        OS_RestoreInterrupts(enabled);
    }
}

u16 *WM_GetSharedDataAddress(WMDataSharingInfo *dsInfo, WMDataSet *receiveData, u16 aid) {
    u16 aidBitmap = receiveData->aidBitmap;
    u16 receivedBitmap = receiveData->receivedBitmap;
    u32 aidBit = 0x0001U << aid;

    if (dsInfo == NULL) {
        return NULL;
    }
    if (receiveData == NULL) {
        return NULL;
    }
    if (!(aidBitmap & aidBit)) {
        return NULL;
    }
    if (receivedBitmap & aidBit) {
        return WmGetSharedDataAddress(dsInfo, aidBitmap, receiveData->data, aid);
    }
    return NULL;
}

static u16 *WmGetSharedDataAddress(WMDataSharingInfo *dsInfo, u32 aidBitmap, u16 *receiveBuf, u32 aid) {
    u32 mask = (0x0001U << aid) - 1;
    u32 count = MATH_CountPopulation(aidBitmap & mask);
    return (u16 *) ((u8 *) receiveBuf + dsInfo->dataLength * count);
}
