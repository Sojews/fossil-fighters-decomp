// Wireless play: MMultiPlay drives the WM library through its states (initialize, scan, connect as parent or
// child, MP communication with data sharing) and turns what happens into messages for the game. Each WM call is
// wrapped in a task, so a process can run a sequence of them.
#ifndef M_MULTI_PLAY_H
#define M_MULTI_PLAY_H

#include "m/task.h"
#include "m/observer.h"
#include "m/multi_message.h"
#include "m/looper.h"
#include "m/view.h"
#include "nitro/wm.h"
#include <vector>

#define MULTI_PLAY_WM_PORT_COUNT 4

// Counts how many owners have the wireless hardware running.
class MWirelessActiveFlag {
public:
    MWirelessActiveFlag(bool set);
    ~MWirelessActiveFlag();
    void Set();
    void Clear();

    u8 mSet;
};

// What happened since the last frame; MMultiPlay::Execute turns these into messages.
struct MMultiPlayChildEvents {
    u8 connected;       // 0x00
    u8 disconnected;    // 0x01
    u8 ssid[24];        // 0x02
    u8 macAddress[6];   // 0x1A
};

struct MMultiPlayParentEvents {
    u8 foundGameInfo[0x70];     // 0x00
    u8 updatedGameInfo[0x70];   // 0x70
    u8 bssid[6];                // 0xE0
    u8 found;                   // 0xE6
    u8 updated;                 // 0xE7
    u8 lost;                    // 0xE8
};

class MMultiPlayEvents {
public:
    MMultiPlayEvents(u32 maxChildren, u32 maxParents) {
        mMaxChildren = maxChildren;
        mMaxParents = maxParents;
        mInitialized = FALSE;
        mFinished = FALSE;
        mReset = FALSE;
        mFatalError = FALSE;
        mParentConnected = FALSE;
        mParentDisconnected = FALSE;
        mMpStarted = FALSE;
        mSynchronized = FALSE;
        mAnotherGameParentFound = FALSE;
        mChildren = new MMultiPlayChildEvents[mMaxChildren];
        mParents = new MMultiPlayParentEvents[mMaxParents];
        for (u32 i = 0; i < mMaxChildren; i++) {
            mChildren[i].connected = FALSE;
            mChildren[i].disconnected = FALSE;
        }
        for (u32 i = 0; i < mMaxParents; i++) {
            mParents[i].found = FALSE;
            mParents[i].updated = FALSE;
            mParents[i].lost = FALSE;
        }
    }
    ~MMultiPlayEvents() {
        delete[] mChildren;
        delete[] mParents;
    }

    u8 mInitialized;                // 0x00
    u8 mFinished;                   // 0x01
    u8 mReset;                      // 0x02
    u8 mFatalError;                 // 0x03
    u8 mParentConnected;            // 0x04
    u8 mParentDisconnected;         // 0x05
    u8 mMpStarted;                  // 0x06
    u8 mSynchronized;               // 0x07
    u8 mAnotherGameParentFound;     // 0x08
    u32 mAnotherGameGgid;           // 0x0C
    u8 mAnotherGameInfo[0x70];      // 0x10
    u8 mAnotherGameBssid[6];        // 0x80
    MMultiPlayChildEvents *mChildren;   // 0x88
    MMultiPlayParentEvents *mParents;   // 0x8C
    u32 mMaxChildren;               // 0x90
    u32 mMaxParents;                // 0x94
};

// A WM data sharing buffer, aligned to 32 bytes inside the space.
struct MMultiPlayDataSharing {
    u8 space[sizeof(WMDataSharingInfo) + 32];
    WMDataSharingInfo *info;
};

class MMultiPlay : public MTask, public MMessageSubject {
public:
    enum State {
        STATE_NONE = 0,
        STATE_STOPPED = 1,
        STATE_IDLE = 2,
        STATE_RESETTING = 3,
        STATE_SCANNING = 4,
        STATE_PARENT = 5,
        STATE_CHILD = 6,
        STATE_MP_PARENT = 7,
    };

    MMultiPlay(u32 ggid, u32 maxPlayers, u32 maxParents, u32 tgid, u32 parentMaxSize, u32 childMaxSize);
    virtual ~MMultiPlay();
    virtual void Execute();

    MTask *CreateSynchronizeDelayTask();
    MTask *CreateReleaseSynchronizeTask();
    void SetParentLifetime(u32 frames);
    void Fail();
    u32 GetPlayerIndex();
    u8 IsFailed();
    u8 IsReady();
    u8 IsFatal();
    u32 GetSharedDataSize();
    MTask *CreateWaitReadyTask();
    MTask *CreateFinishTask();
    void End();
    static void EndCallback(void *arg);
    MTask *CreateInitializeTask();
    void Initialize();
    static void InitializeCallback(void *arg);
    static void IndicationCallback(void *arg);
    MTask *CreateResetTask();
    void Reset();
    static void ResetCallback(void *arg);
    MTask *CreateDisconnectTask(u16 aid);
    void Disconnect(u16 aid);
    static void DisconnectCallback(void *arg);
    MTask *CreateGetOptimumChannelTask(u16 *channel);
    void GetOptimumChannel(u16 *channel);
    void MeasureNextChannel();
    static void MeasureChannelCallback(void *arg);
    void OnMeasureChannel(WMMeasureChannelCallback *cb);
    void StartMP();
    static void StartMPCallback(void *arg);
    void OnStartMP(WMStartMPCallback *cb);
    MTask *CreateStartParentTask(const void *userGameInfo, u32 size, u16 channel);
    void StartParent(u16 channel, const void *userGameInfo, u32 size);
    static void SetParentParameterCallback(void *arg);
    void OnSetParentParameter(WMCallback *cb);
    static void StartParentCallback(void *arg);
    void OnStartParent(WMStartParentCallback *cb);
    MTask *CreateStartScanTask();
    void StartScan();
    void ScanNext();
    static void ScanCallback(void *arg);
    void OnBssFound(WMBssDesc *bss);
    void OnScan(WMStartScanExCallback *cb);
    MTask *CreateEndScanTask();
    void EndScan();
    static void EndScanCallback(void *arg);
    void OnEndScan(WMCallback *cb);
    MTask *CreateStartChildTask(u32 index, const u8 *ssid);
    void StartChild(u32 index, const u8 *ssid);
    static void StartConnectCallback(void *arg);
    void OnStartConnect(WMStartConnectCallback *cb);

    MWirelessActiveFlag mActive;    // 0x0038
    u32 mGgid;                      // 0x003C
    u32 mMaxPlayers;                // 0x0040
    u32 mTgid;                      // 0x0044: 0x10000 = pick the next one
    u32 mParentMaxSize;             // 0x0048
    u32 mChildMaxSize;              // 0x004C
    u32 mDmaNo;                     // 0x0050
    u8 mSystemBuf[WM_SYSTEM_BUF_SIZE + 32];  // 0x0054
    u8 *mMpRecvBuf;                 // 0x0F74
    u8 *mMpSendBuf;                 // 0x0F78
    u16 *mChannelOut;               // 0x0F7C
    u16 mMeasureChannel;            // 0x0F80
    u16 mAllowedChannels;           // 0x0F82
    u16 mBestBusyRatio;             // 0x0F84
    u16 mChannel;                   // 0x0F86
    u8 mContinuousMp;               // 0x0F88
    u16 mAid;                       // 0x0F8A
    u16 mConnectedAids;             // 0x0F8C
    u8 mSynchronizing;              // 0x0F8E: no children may join or leave
    u8 mIsParent;                   // 0x0F8F
    MMultiPlayDataSharing mDataSharing[MULTI_PLAY_WM_PORT_COUNT];  // 0x0F90
    u32 mSharedDataSize;            // 0x30A0
    u32 mPlayerIndex;               // 0x30A4
    u32 mPlayerCount;               // 0x30A8
    u32 mState;                     // 0x30AC
    u8 mReady;                      // 0x30B0
    u8 mFailed;                     // 0x30B1
    u8 mFatal;                      // 0x30B2
    u8 mUnk30B3;
    u16 mUnk30B4;
    u8 mParentParamSpace[sizeof(WMParentParam) + 34];  // 0x30B6
    WMParentParam *mParentParam;    // 0x3118
    u8 mUserGameInfoSpace[0x90];    // 0x311C
    u16 *mUserGameInfo;             // 0x31AC
    u32 mTimeoutReload;             // 0x31B0
    u32 mTimeout;                   // 0x31B4
    u32 mMaxParents;                // 0x31B8
    WMScanExParam mScanParam;       // 0x31BC
    u8 *mScanBufSpace;              // 0x3200
    u8 *mParentsSpace;              // 0x3204
    WMBssDesc *mParents;            // 0x3208
    u32 *mParentValid;              // 0x320C
    u32 *mParentLifetime;           // 0x3210
    u32 mParentLifetimeReload;      // 0x3214
    u8 mStopScan;                   // 0x3218
    MMultiPlayEvents *mEvents;      // 0x321C
    u32 mSynchronizeDelay;          // 0x3220
};

// Exchanges messages with the other consoles: sends game messages over MP and passes received ones to its
// observers.
// A serialized message waiting to go out, and the consoles it goes to.
struct MMultiPlayHubOutgoing {
    std::vector<char> *data;    // 0x0
    u16 mask;               // 0x4
};

// A message arriving from one console, assembled as its pieces come in.
struct MMultiPlayHubIncoming {
    std::vector<char> *data;    // 0x0
    u32 unk4;               // 0x4
    u32 unk8;               // 0x8
};

class MMultiPlayMessageHub : public MLooperVSyncTask, public MMessageSubject {
public:
    MMultiPlayMessageHub(int arg);
    virtual ~MMultiPlayMessageHub();
    virtual void Execute();
    virtual void OnVSync();

    // Sends a message to every console but this one.
    void Broadcast(MMessage *msg);
    // Sends a message to the consoles in an AID mask.
    void SendTo(u32 mask, MMessage *msg);
    // Sends a message to every console, this one included.
    void SendToAll(MMessage *msg);
    void SendPending();
    void ReceivePending();
    void SetUnk279(u8 value);

    std::vector<MMultiPlayHubOutgoing> mOutgoing;  // 0x44
    std::vector<char> *mSending;                    // 0x50: the message being sent
    std::vector<MMultiPlayHubIncoming> mIncoming;  // 0x54: one per console
    WMDataSet mReceived;    // 0x60
    u16 *mSendData;         // 0x260
    int mSelf;              // 0x264: this console's AID
    u32 unk268[2];
    u32 mConsoleCount;      // 0x270
    u32 unk274;
    u8 mRunning;            // 0x278: data sharing stepped last frame
    u8 mUnk279;             // 0x279
};

class MMultiPlaySynchronizeDelayTask : public MProcess {
public:
    MMultiPlaySynchronizeDelayTask() : MProcess(NULL) {}
    virtual ~MMultiPlaySynchronizeDelayTask() {}
    virtual void Idle();
};

// Agrees with the other stations on who is taking part: each sends the AIDs it sees connected until everyone
// reports the same set.
class MMultiPlaySynchronizeTask : public MLooperVSyncTask {
public:
    MMultiPlaySynchronizeTask() : MLooperVSyncTask(FALSE, FALSE) {
        mFrames = 0;
        mFirst = TRUE;
        Enable();
    }
    virtual ~MMultiPlaySynchronizeTask();
    virtual void OnVSync();

    u16 mSendAids;      // 0x1A
    WMDataSet mRecv;    // 0x1C
    u32 mFrames;        // 0x21C
    u8 mFirst;          // 0x220
};

class MMultiPlayReleaseSynchronizeTask : public MTransfer {
public:
    virtual ~MMultiPlayReleaseSynchronizeTask() {}
    virtual void Transfer();
};

class MMultiPlayWaitReadyTask : public MTask {
public:
    virtual ~MMultiPlayWaitReadyTask() {}
    virtual void Execute();
};

class MMultiPlayFinishTask : public MProcess {
public:
    MMultiPlayFinishTask() : MProcess(NULL) {}
    virtual ~MMultiPlayFinishTask() {}
    virtual void Idle();
};

class MMultiPlayInitializeTask : public MProcess {
public:
    MMultiPlayInitializeTask() : MProcess(NULL) {}
    virtual ~MMultiPlayInitializeTask() {}
    virtual void Idle();
};

class MMultiPlayResetTask : public MProcess {
public:
    MMultiPlayResetTask() : MProcess(NULL) {}
    virtual ~MMultiPlayResetTask() {}
    virtual void Idle();
};

class MMultiPlayDisconnectTask : public MTask {
public:
    MMultiPlayDisconnectTask(u16 aid) { mAid = aid; }
    virtual ~MMultiPlayDisconnectTask() {}
    virtual void Execute();

    u16 mAid;   // 0x10
};

class MMultiPlayGetOptimumChannelTask : public MProcess {
public:
    MMultiPlayGetOptimumChannelTask(u16 *channel) : MProcess(NULL) { mChannel = channel; }
    virtual ~MMultiPlayGetOptimumChannelTask() {}
    virtual void Idle();

    u16 *mChannel;  // 0x74
};

class MMultiPlayStartParentTask : public MProcess {
public:
    MMultiPlayStartParentTask(const void *userGameInfo, u32 size, u16 channel);
    virtual ~MMultiPlayStartParentTask() {}
    virtual void Idle();

    u8 mUserGameInfo[0x70]; // 0x74
    u32 mUserGameInfoSize;  // 0xE4
    u16 mChannel;           // 0xE8
};

class MMultiPlayStartScanTask : public MProcess {
public:
    MMultiPlayStartScanTask() : MProcess(NULL) {}
    virtual ~MMultiPlayStartScanTask() {}
    virtual void Idle();
};

class MMultiPlayEndScanTask : public MProcess {
public:
    MMultiPlayEndScanTask() : MProcess(NULL) { mStep = 0; }
    virtual ~MMultiPlayEndScanTask() {}
    virtual void Idle();

    u32 mStep;  // 0x74
};

class MMultiPlayStartChildTask : public MProcess {
public:
    MMultiPlayStartChildTask(u32 index, const u8 *ssid) : MProcess(NULL) {
        mIndex = index;
        if (ssid != NULL) {
            MI_CpuCopy8(ssid, mSsid, sizeof(mSsid));
        } else {
            MI_CpuFill8(mSsid, 0, sizeof(mSsid));
        }
    }
    virtual ~MMultiPlayStartChildTask() {}
    virtual void Idle();

    u8 mSsid[24];   // 0x74
    u32 mIndex;     // 0x8C
};

class MMotion;
class MSmartOam;

// An icon shown while wireless is in use.
class MMultiPlayIcon : public MFixedSizeView {
public:
    MMultiPlayIcon(MDisplay *display, const char *path, BOOL fromEnd, u8 alwaysShown);
    virtual ~MMultiPlayIcon();
    virtual void Execute();
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();

    MMotion *GetMotion();

    MMotion *mMotion;   // 0x18
    MSmartOam *mOam;    // 0x1C
    u8 mHidden;         // 0x20
    u8 mAlwaysShown;    // 0x21: otherwise only while the wireless hardware is running
    u32 mLinger;        // 0x24: frames to stay up after it stops
};

// Shows the link level.
class MMultiPlayLinkIcon : public MMultiPlayIcon {
public:
    MMultiPlayLinkIcon(MDisplay *display, BOOL fromEnd);
    virtual ~MMultiPlayLinkIcon() {}
    virtual void Execute();
};

// Starts and stops the wireless driver for Wii communication (WVR), one step per frame.
class MMultiPlayWirelessDriverStartTask : public MProcess {
public:
    typedef void (MMultiPlayWirelessDriverStartTask::*Step)();

    void WaitStartUp();
    void StartUp();
    static void StartUpCallback(void *arg, int result);
    void Begin();

    Step mStep;     // 0x74
    u8 mDone;       // 0x7C
};

class MMultiPlayWirelessDriverStopTask : public MProcess {
public:
    typedef void (MMultiPlayWirelessDriverStopTask::*Step)();

    void WaitTerminate();
    void Terminate();
    static void TerminateCallback(void *arg, int result);
    void Begin();

    Step mStep;     // 0x74
    u8 mDone;       // 0x7C
};

#endif
