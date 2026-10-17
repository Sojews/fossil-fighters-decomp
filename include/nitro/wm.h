// NitroSDK WM: the wireless manager, as the game sees it.
#ifndef NITRO_WM_H
#define NITRO_WM_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WM_ERRCODE_SUCCESS = 0,
    WM_ERRCODE_FAILED = 1,
    WM_ERRCODE_OPERATING = 2,
    WM_ERRCODE_ILLEGAL_STATE = 3,
    WM_ERRCODE_WM_DISABLE = 4,
    WM_ERRCODE_NO_DATASET = 5,
    WM_ERRCODE_INVALID_PARAM = 6,
    WM_ERRCODE_NO_CHILD = 7,
    WM_ERRCODE_FIFO_ERROR = 8,
    WM_ERRCODE_TIMEOUT = 9,
    WM_ERRCODE_SEND_QUEUE_FULL = 10,
    WM_ERRCODE_NO_ENTRY = 11,
    WM_ERRCODE_OVER_MAX_ENTRY = 12,
    WM_ERRCODE_INVALID_POLLBITMAP = 13,
    WM_ERRCODE_NO_DATA = 14
} WMErrCode;

typedef enum {
    WM_STATECODE_PARENT_START = 0,
    WM_STATECODE_BEACON_SENT = 2,
    WM_STATECODE_SCAN_START = 3,
    WM_STATECODE_PARENT_NOT_FOUND = 4,
    WM_STATECODE_PARENT_FOUND = 5,
    WM_STATECODE_CONNECT_START = 6,
    WM_STATECODE_CONNECTED = 7,
    WM_STATECODE_BEACON_LOST = 8,
    WM_STATECODE_DISCONNECTED = 9,
    WM_STATECODE_MP_START = 10,
    WM_STATECODE_MPEND_IND = 11,
    WM_STATECODE_MP_IND = 12
} WMStateCode;

#define WM_SYSTEM_BUF_SIZE 0xF00
#define WM_BSS_DESC_SIZE 0xC0
#define WM_SCANTYPE_ACTIVE 1

typedef void (*WMCallbackFunc)(void *arg);

typedef struct {
    u16 apiid;      // 0x00
    u16 errcode;    // 0x02
    u16 wlCmdID;    // 0x04
    u16 wlResult;   // 0x06
} WMCallback;

typedef struct {
    u16 magicNumber;            // 0x00
    u8 ver;                     // 0x02
    u8 platform;                // 0x03
    u32 ggid;                   // 0x04
    u16 tgid;                   // 0x08
    u8 userGameInfoLength;      // 0x0A
    u8 gameNameCount_attribute; // 0x0B: bit 0 = accepting children
    u16 parentMaxSize;          // 0x0C
    u16 childMaxSize;           // 0x0E
    u16 userGameInfo[0x70 / 2]; // 0x10
} WMGameInfo;

typedef struct {
    u16 length;             // 0x00
    u16 rssi;               // 0x02
    u8 bssid[6];            // 0x04
    u16 ssidLength;         // 0x0A
    u8 ssid[32];            // 0x0C
    u16 capaInfo;           // 0x2C
    u16 rateSet[2];         // 0x2E
    u16 beaconPeriod;       // 0x32
    u16 dtimPeriod;         // 0x34
    u16 channel;            // 0x36
    u16 cfpPeriod;          // 0x38
    u16 cfpMaxDuration;     // 0x3A
    u16 gameInfoLength;     // 0x3C
    u16 otherElementCount;  // 0x3E
    WMGameInfo gameInfo;    // 0x40
} WMBssDesc;

typedef struct {
    u16 *userGameInfo;          // 0x00
    u16 userGameInfoLength;     // 0x04
    u16 padding;                // 0x06
    u32 ggid;                   // 0x08
    u16 tgid;                   // 0x0C
    u16 entryFlag;              // 0x0E
    u16 maxEntry;               // 0x10
    u16 multiBootFlag;          // 0x12
    u16 KS_Flag;                // 0x14
    u16 CS_Flag;                // 0x16
    u16 beaconPeriod;           // 0x18
    u16 rsv1[4];                // 0x1A
    u16 rsv2[8];                // 0x22
    u16 channel;                // 0x32
    u16 parentMaxSize;          // 0x34
    u16 childMaxSize;           // 0x36
    u16 rsv[4];                 // 0x38
} WMParentParam;

typedef struct {
    WMBssDesc *scanBuf;     // 0x00
    u16 scanBufSize;        // 0x04
    u16 channelList;        // 0x06
    u16 maxChannelTime;     // 0x08
    u8 bssid[6];            // 0x0A
    u16 scanType;           // 0x10
    u16 ssidLength;         // 0x12
    u8 ssid[32];            // 0x14
    u16 ssidMatchLength;    // 0x34
    u16 rsv[7];             // 0x36
} WMScanExParam;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u16 wlCmdID;            // 0x04
    u16 wlResult;           // 0x06
    u16 state;              // 0x08
    u8 macAddress[6];       // 0x0A
    u16 aid;                // 0x10
    u16 reason;             // 0x12
    u8 ssid[24];            // 0x14
    u16 parentSize;         // 0x2C
    u16 childSize;          // 0x2E
} WMStartParentCallback;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u16 wlCmdID;            // 0x04
    u16 wlResult;           // 0x06
    u16 state;              // 0x08
    u16 aid;                // 0x0A
    u16 reason;             // 0x0C
    u16 wlStatus;           // 0x0E
    u8 macAddress[6];       // 0x10
    u16 parentSize;         // 0x16
    u16 childSize;          // 0x18
} WMStartConnectCallback;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u16 state;              // 0x04
    u16 reserved[10];
} WMStartMPCallback;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u16 wlCmdID;            // 0x04
    u16 wlResult;           // 0x06
    u16 state;              // 0x08
    u16 channelList;        // 0x0A
    u16 reserved;           // 0x0C
    u16 bssDescCount;       // 0x0E
    WMBssDesc *bssDesc[16]; // 0x10
    u16 linkLevel[16];      // 0x50
} WMStartScanExCallback;

typedef struct {
    u16 apiid;              // 0x00
    u16 errcode;            // 0x02
    u16 wlCmdID;            // 0x04
    u16 wlResult;           // 0x06
    u16 channel;            // 0x08
    u16 ccaBusyRatio;       // 0x0A
} WMMeasureChannelCallback;

typedef struct {
    u16 aidBitmap;          // 0x00
    u16 receivedBitmap;     // 0x02
    u16 data[508 / 2];      // 0x04
} WMDataSet;

typedef struct {
    WMDataSet ds[4];
    u16 seqNum[4];
    u16 writeIndex;
    u16 sendIndex;
    u16 readIndex;
    u16 aidBitmap;
    u16 dataLength;
    u16 stationNumber;
    u16 dataSetLength;
    u16 port;
    u16 doubleMode;
    u16 currentSeqNum;
    u16 state;
    u16 reserved[1];
} WMDataSharingInfo;    // 0x820

WMErrCode WM_Initialize(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo);
WMErrCode WM_SetIndCallback(WMCallbackFunc callback);
WMErrCode WM_Reset(WMCallbackFunc callback);
WMErrCode WM_End(WMCallbackFunc callback);
WMErrCode WM_SetParentParameter(WMCallbackFunc callback, const WMParentParam *pparaBuf);
WMErrCode WM_StartParent(WMCallbackFunc callback);
WMErrCode WM_StartScanEx(WMCallbackFunc callback, const WMScanExParam *param);
WMErrCode WM_EndScan(WMCallbackFunc callback);
WMErrCode WM_StartConnectEx(WMCallbackFunc callback, const WMBssDesc *pInfo, const u8 *sslKey, BOOL powerSave,
                            u16 authMode);
WMErrCode WM_Disconnect(WMCallbackFunc callback, u16 aid);
WMErrCode WM_StartMP(WMCallbackFunc callback, u16 *recvBuf, u16 recvBufSize, u16 *sendBuf, u16 sendBufSize,
                     u16 mpFreq);
WMErrCode WM_MeasureChannel(WMCallbackFunc callback, u16 ccaMode, u16 edThreshold, u16 channel, u16 measureTime);
WMErrCode WM_SetEntry(WMCallbackFunc callback, BOOL enabled);
WMErrCode WM_StartDataSharing(WMDataSharingInfo *dsInfo, u16 port, u16 aidBitmap, u16 dataLength,
                              BOOL doubleMode);
WMErrCode WM_StepDataSharing(WMDataSharingInfo *dsInfo, const u16 *sendData, WMDataSet *receiveData);
u16 *WM_GetSharedDataAddress(WMDataSharingInfo *dsInfo, WMDataSet *receiveData, u16 aid);
u16 WM_GetAllowedChannel(void);
s32 WM_GetMPSendBufferSize(void);
s32 WM_GetMPReceiveBufferSize(void);
u32 WM_GetLinkLevel(void);
u16 WM_GetDispersionBeaconPeriod(void);
u16 WM_GetDispersionScanPeriod(void);
u16 WM_GetNextTgid(void);
BOOL WMi_IsWirelessEnabled(void);

#ifdef __cplusplus
}
#endif

#endif
