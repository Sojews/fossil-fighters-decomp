// NitroSDK WM: the wireless library's shared declarations. Requests go to the ARM7's wireless driver through the
// PXI FIFO in buffers taken from a small pool; replies and indications come back as callback structures.
#ifndef WM_PRIVATE_H
#define WM_PRIVATE_H

#include "nitro/os.h"
#include "nitro/mi.h"
#include "nitro/pxi.h"

#define PXI_FIFO_TAG_WM 10

#define WM_SIZE_MACADDR 6
#define WM_SIZE_CHILD_SSID 24
#define WM_NUM_OF_CALLBACK 44
#define WM_NUM_OF_PORT 16

#define WM_ARM9WM_BUF_SIZE 0x200
#define WM_ARM7WM_BUF_SIZE 0x300
#define WM_STATUS_BUF_SIZE 0x800
#define WM_FIFO_BUF_SIZE 0x100
#define WM_SYSTEM_BUF_SIZE (WM_ARM9WM_BUF_SIZE + WM_ARM7WM_BUF_SIZE + WM_STATUS_BUF_SIZE + WM_FIFO_BUF_SIZE * 2)
#define WM_FIFO_BUF_NUM 10
#define WM_API_REQUEST_ACCEPTED 0x8000

typedef enum {
    WM_ERRCODE_SUCCESS = 0,
    WM_ERRCODE_FAILED,
    WM_ERRCODE_OPERATING,
    WM_ERRCODE_ILLEGAL_STATE,
    WM_ERRCODE_WM_DISABLE,
    WM_ERRCODE_NO_KEYSET,
    WM_ERRCODE_INVALID_PARAM,
    WM_ERRCODE_NO_CHILD,
    WM_ERRCODE_FIFO_ERROR,
    WM_ERRCODE_TIMEOUT,
    WM_ERRCODE_SEND_QUEUE_FULL,
    WM_ERRCODE_NO_ENTRY,
    WM_ERRCODE_OVER_MAX_ENTRY,
    WM_ERRCODE_INVALID_POLLBITMAP,
    WM_ERRCODE_NO_DATASET,
    WM_ERRCODE_SEND_FAILED,
    WM_ERRCODE_NO_DATA,
    WM_ERRCODE_NOT_ENOUGH_MEM,
    WM_ERRCODE_DCF_TEST,
    WM_ERRCODE_FLASH_ERROR,
    WM_ERRCODE_MAX
} WMErrCode;

typedef enum {
    WM_APIID_INITIALIZE = 0,
    WM_APIID_RESET,
    WM_APIID_END,
    WM_APIID_ENABLE,
    WM_APIID_DISABLE,
    WM_APIID_POWER_ON,
    WM_APIID_POWER_OFF,
    WM_APIID_SET_P_PARAM,
    WM_APIID_START_PARENT,
    WM_APIID_END_PARENT,
    WM_APIID_START_SCAN,
    WM_APIID_END_SCAN,
    WM_APIID_START_CONNECT,
    WM_APIID_DISCONNECT,
    WM_APIID_START_MP,
    WM_APIID_SET_MP_DATA,
    WM_APIID_END_MP,
    WM_APIID_START_DCF,
    WM_APIID_SET_DCF_DATA,
    WM_APIID_END_DCF,
    WM_APIID_SET_WEPKEY,
    WM_APIID_START_KS,
    WM_APIID_END_KS,
    WM_APIID_GET_KEYSET,
    WM_APIID_SET_GAMEINFO,
    WM_APIID_SET_BEACON_IND,
    WM_APIID_START_TESTMODE,
    WM_APIID_STOP_TESTMODE,
    WM_APIID_VALARM_MP,
    WM_APIID_SET_LIFETIME,
    WM_APIID_MEASURE_CHANNEL,
    WM_APIID_INIT_W_COUNTER,
    WM_APIID_GET_W_COUNTER,
    WM_APIID_SET_ENTRY,
    WM_APIID_AUTO_DEAUTH,
    WM_APIID_SET_MP_PARAMETER,
    WM_APIID_SET_BEACON_PERIOD,
    WM_APIID_AUTO_DISCONNECT,
    WM_APIID_START_SCAN_EX,
    WM_APIID_SET_WEPKEY_EX,
    WM_APIID_SET_PS_MODE,
    WM_APIID_START_TESTRXMODE,
    WM_APIID_STOP_TESTRXMODE,
    WM_APIID_KICK_MP_PARENT,
    WM_APIID_ASYNC_KIND_MAX,
    WM_APIID_INDICATION = 0x80,
    WM_APIID_PORT_SEND,
    WM_APIID_PORT_RECV,
    WM_APIID_READ_STATUS,
    WM_APIID_UNKNOWN = 0xff
} WMApiid;

typedef enum {
    WM_STATE_READY = 0,
    WM_STATE_STOP,
    WM_STATE_IDLE,
    WM_STATE_CLASS1,
    WM_STATE_TESTMODE,
    WM_STATE_SCAN,
    WM_STATE_CONNECT,
    WM_STATE_PARENT,
    WM_STATE_CHILD,
    WM_STATE_MP_PARENT,
    WM_STATE_MP_CHILD,
    WM_STATE_DCF_CHILD,
    WM_STATE_TESTMODE_RX,
    WM_STATE_MAX
} WMState;

#define WM_STATECODE_CONNECTED 7
#define WM_STATECODE_DISCONNECTED 9
#define WM_STATECODE_MP_IND 11
#define WM_STATECODE_MPACK_IND 12
#define WM_STATECODE_DISCONNECTED_FROM_MYSELF 26

typedef void (*WMCallbackFunc)(void *arg);

typedef struct {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
} WMCallback;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u16 state;              // 0x04
    u16 port;               // 0x06
    void *recvBuf;          // 0x08
    u16 *data;              // 0x0c
    u16 length;             // 0x10
    u16 aid;                // 0x12
    u8 macAddress[WM_SIZE_MACADDR];  // 0x14
    u16 seqNo;              // 0x1a
    void *arg;              // 0x1c
    u16 myAid;              // 0x20
    u16 connectedAidBitmap; // 0x22
    u8 ssid[WM_SIZE_CHILD_SSID];     // 0x24
    u16 reason;             // 0x3c
    u16 rsv;                // 0x3e
    u16 maxSendDataSize;    // 0x40
    u16 maxRecvDataSize;    // 0x42
} WMPortRecvCallback;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u8 unk04[0x18];
    WMCallbackFunc callback;    // 0x1c
} WMPortSendCallback;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u16 wlCmdID;            // 0x04
    u16 wlResult;           // 0x06
    u16 state;              // 0x08
    u8 macAddress[WM_SIZE_MACADDR];  // 0x0a
    u16 aid;                // 0x10
    u16 reason;             // 0x12
    u8 ssid[WM_SIZE_CHILD_SSID];     // 0x14
    u16 parentSize;         // 0x2c
    u16 childSize;          // 0x2e
} WMStartParentCallback;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u16 wlCmdID;            // 0x04
    u16 wlResult;           // 0x06
    u16 state;              // 0x08
    u16 aid;                // 0x0a
    u16 reason;             // 0x0c
    u16 wlStatus;           // 0x0e
    u8 macAddress[WM_SIZE_MACADDR];  // 0x10
    u16 parentSize;         // 0x16
    u16 childSize;          // 0x18
} WMStartConnectCallback;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u16 state;              // 0x04
    u16 reserved;           // 0x06
    void *recvBuf;          // 0x08
} WMStartMPCallback;

// The state the ARM7 keeps in main memory (only the fields the ARM9 side reads are named).
typedef struct {
    u16 state;              // 0x000
    u8 unk002[0xa];
    u32 mp_flag;            // 0x00c
    u8 unk010[0x2c];
    u16 mp_maxSendSize;     // 0x03c
    u16 mp_maxRecvSize;     // 0x03e
    u8 unk040[0x32];
    u16 mp_recvBufSize;     // 0x072
    u8 unk074[0x12];
    u16 mp_readyBitmap;     // 0x086
    u8 unk088[0x34];
    u16 linkLevel;          // 0x0bc
    u8 unk0be[0x3a];
    u16 pparam_maxEntry;    // 0x0f8
    u8 unk0fa[0x88];
    u16 child_bitmap;       // 0x182
    u8 unk184[4];
    u16 mp_parentInterval;  // 0x188 (?)
} WMStatus;

typedef struct {
    void *WM7;                          // 0x000: the ARM7's work area
    WMStatus *status;                   // 0x004
    u32 *unk008;
    u32 *indbuf;                        // 0x00c
    u32 *fifo7to9;                      // 0x010: the ARM7's replies
    u16 dmaNo;                          // 0x014
    u16 scanOnlyFlag;                   // 0x016
    WMCallbackFunc CallbackTable[WM_NUM_OF_CALLBACK];   // 0x018
    WMCallbackFunc indCallback;         // 0x0c8
    WMCallbackFunc portCallbackTable[WM_NUM_OF_PORT];   // 0x0cc
    void *portCallbackArgument[WM_NUM_OF_PORT];         // 0x10c
    u32 connectedAidBitmap;             // 0x14c
    u16 myAid;                          // 0x150
} WMArm9Buf;

WMErrCode WM_Finish(void);
WMErrCode WMi_SendCommand(WMApiid id, u16 paramNum, ...);
WMErrCode WMi_SendCommandDirect(void *data, u32 length);
WMArm9Buf *WMi_GetSystemWork(void);
WMErrCode WMi_CheckInitialized(void);
WMErrCode WMi_CheckIdle(void);
WMErrCode WMi_CheckStateEx(s32 paramNum, ...);
void WMi_SetCallbackTable(WMApiid id, WMCallbackFunc callback);
u16 WM_GetAID(void);
u16 WM_GetConnectedAIDs(void);

void OS_GetMacAddress(u8 *macAddress);
void OS_InitMessageQueue(OSMessageQueue *mq, OSMessage *msgArray, s32 msgCount);
BOOL OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
BOOL OS_JamMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
void DC_InvalidateRange(void *startAddr, u32 nBytes);

#define WM_CHECK_RESULT(res)                    \
    if ((res) != WM_ERRCODE_SUCCESS) {          \
        return (res);                           \
    }

#endif
