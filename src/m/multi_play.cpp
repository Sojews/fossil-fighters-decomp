// MMultiPlay: wireless play over the WM library, and the tasks that wrap its steps.

#pragma ipa file

#include "m/multi_play.h"
#include "m/irq.h"
#include "m/dma.h"
#include "m/file.h"
#include "m/lcd.h"
#include "m/motion.h"
#include "m/smart_oam.h"
#include "nitro/pxi.h"
#include <algorithm>
#include <string.h>

// (The game keeps these two together, addressed from one base.)
static struct {
    u32 activeCount;        // owners with the wireless hardware running
    MMultiPlay *instance;
} sMultiPlay;

struct MMultiPlayParams {
    u32 unk00;
    u32 maxSharedDataSize;
    const char *wirelessIconPath;
    const char *linkIconPath;
    u32 unk10;
    u32 unk14;
    const char *sendIconPath;
};
// The TU's pool (left in the game's data: defined here, the file's ipa would fold the parameters into the code): the
// parameters, {0, 0x200, "m2:/motion/mp_wireless_icon.bin", "m2:/motion/mp_link_white.bin", 0x200, 15,
// "m2:/motion/mp_send_icon.bin"}, then the pointer-to-member constants the driver tasks copy their next steps from
// (see menu.cpp).
extern "C" struct {
    MMultiPlayParams params;                                        // 0x00
    u8 unk1C[0x24 - 0x1C];
    void (MMultiPlayWirelessDriverStartTask::*WaitStartUp_24)();  // 0x24
    void (MMultiPlayWirelessDriverStartTask::*StartUp_2C)();      // 0x2C
    void (MMultiPlayWirelessDriverStopTask::*WaitTerminate_34)();  // 0x34
    u8 unk3C[0x54 - 0x3C];
    void (MMultiPlayWirelessDriverStopTask::*Terminate_54)();      // 0x54
} data_020c4b0c;
#define sParams (data_020c4b0c.params)

// The icons' graphics, an archive image in the game's data (not reproduced here).
extern "C" const u8 gMultiPlayIconArchive[];

extern "C" void WVR_StartUpAsync(int texBank, void (*callback)(void *arg, int result), void *arg);
extern "C" void WVR_TerminateAsync(void (*callback)(void *arg, int result), void *arg);

static void MultiPlayPrint(const char *fmt, ...) {
}

MWirelessActiveFlag::MWirelessActiveFlag(bool set) {
    mSet = FALSE;
    if (set) {
        Set();
    }
}

MWirelessActiveFlag::~MWirelessActiveFlag() {
    Clear();
}

void MWirelessActiveFlag::Set() {
    MIrqLock lock;
    if (mSet) {
        return;
    }
    mSet = TRUE;
    sMultiPlay.activeCount++;
}

void MWirelessActiveFlag::Clear() {
    MIrqLock lock;
    if (!mSet) {
        return;
    }
    mSet = FALSE;
    sMultiPlay.activeCount--;
}

MTask *MMultiPlay::CreateSynchronizeDelayTask() {
    return new MMultiPlaySynchronizeDelayTask();
}

MTask *MMultiPlay::CreateReleaseSynchronizeTask() {
    return new MMultiPlayReleaseSynchronizeTask();
}

MMultiPlay::MMultiPlay(u32 ggid, u32 maxPlayers, u32 maxParents, u32 tgid, u32 parentMaxSize, u32 childMaxSize)
    : mActive(false) {
    mGgid = ggid;
    mMaxPlayers = maxPlayers;
    mTgid = tgid;
    mParentMaxSize = parentMaxSize;
    mChildMaxSize = childMaxSize;
    mMpRecvBuf = NULL;
    mMpSendBuf = NULL;
    mChannel = 0;
    mContinuousMp = FALSE;
    mSynchronizing = FALSE;
    mSharedDataSize = 16;
    mState = STATE_STOPPED;
    mReady = TRUE;
    mFailed = FALSE;
    mFatal = FALSE;
    mTimeoutReload = 0;
    mTimeout = 0;
    mMaxParents = maxParents;
    mSynchronizeDelay = 2;
    sMultiPlay.instance = this;
    mDmaNo = MAllocDmaChannel();
    mUserGameInfo = (u16 *) (((u32) mUserGameInfoSpace + 31) & ~31);
    mScanBufSpace = new u8[0x420];
    mParentsSpace = new u8[maxParents * sizeof(WMBssDesc) + 32];
    mParents = (WMBssDesc *) (((u32) mParentsSpace + 31) & ~31);
    mParentValid = new u32[maxParents];
    mParentLifetime = new u32[maxParents];
    mParentLifetimeReload = 0;

    u32 size = (0x500 - mMaxPlayers * 60) / mMaxPlayers;
    size = std::min(size, sParams.maxSharedDataSize);
    mParentMaxSize = (u16) std::min(mParentMaxSize, size);
    mChildMaxSize = (u16) std::min(mChildMaxSize, size);
    mMpRecvBuf = new u8[((std::max((((size + 14) * (mMaxPlayers - 1) + 0x29) & ~31) * 2,
                                   ((size + 0x55) & ~31) * 2) + 63) & ~63) + 32];
    mMpSendBuf = new u8[((std::max((size + 0x23) & ~31, (size + 0x21) & ~31) + 31) & ~31) + 32];
    mEvents = new MMultiPlayEvents(maxPlayers, maxParents);
}

MMultiPlay::~MMultiPlay() {
    DC_FlushAll();
    delete mEvents;
    sMultiPlay.instance = NULL;
    MFreeDmaChannel(mDmaNo);
    delete[] mMpSendBuf;
    delete[] mMpRecvBuf;
    delete[] mScanBufSpace;
    delete[] mParentsSpace;
    delete[] mParentValid;
    delete[] mParentLifetime;
}

void MMultiPlay::Execute() {
    if (mTimeout != 0 && !mSynchronizing) {
        mTimeout--;
        if (mTimeout == 0) {
            Fail();
        }
    }
    if (mState == STATE_SCANNING) {
        for (u32 i = 0; i < mMaxParents; i++) {
            if (mParentValid[i] == TRUE && mParentLifetime[i] != 0) {
                MIrqLock lock;
                mParentLifetime[i]--;
                if (mParentLifetime[i] == 0) {
                    mParentValid[i] = FALSE;
                    mParentLifetime[i] = 0;
                    mEvents->mParents[i].lost = TRUE;
                }
            }
        }
    }

    MMessageSubject *subject = this;
    MMultiPlayEvents *events = mEvents;
    if (events->mInitialized) {
        events->mInitialized = FALSE;
        subject->Notify(new MMultiPlayInitializedMessage());
    }
    if (events->mFinished) {
        events->mFinished = FALSE;
        subject->Notify(new MMultiPlayFinishedMessage());
    }
    if (events->mReset) {
        events->mReset = FALSE;
        subject->Notify(new MMultiPlayResetMessage());
    }
    if (events->mParentConnected) {
        events->mParentConnected = FALSE;
        subject->Notify(new MMultiPlayParentConnectedMessage());
    }
    if (events->mParentDisconnected) {
        events->mParentDisconnected = FALSE;
        subject->Notify(new MMultiPlayParentDisconnectedMessage());
    }
    if (events->mMpStarted) {
        events->mMpStarted = FALSE;
        subject->Notify(new MMultiPlayMpStartedMessage());
    }
    if (events->mSynchronized) {
        events->mSynchronized = FALSE;
        subject->Notify(new MMultiPlaySynchronizedMessage());
    }
    if (events->mAnotherGameParentFound) {
        events->mAnotherGameParentFound = FALSE;
        subject->Notify(new MMultiPlayAnotherGameParentFoundMessage(events->mAnotherGameGgid,
                                                                    events->mAnotherGameInfo,
                                                                    events->mAnotherGameBssid));
    }
    for (u32 i = 0; i < events->mMaxChildren; i++) {
        MMultiPlayChildEvents *child = &events->mChildren[i];
        if (child->connected) {
            subject->Notify(new MMultiPlayChildConnectedMessage(i, child->ssid, child->macAddress));
            child->connected = FALSE;
        }
        if (child->disconnected) {
            subject->Notify(new MMultiPlayChildDisconnectedMessage(i));
            child->disconnected = FALSE;
        }
    }
    for (u32 i = 0; i < events->mMaxParents; i++) {
        MMultiPlayParentEvents *parent = &events->mParents[i];
        if (parent->found) {
            subject->Notify(new MMultiPlayParentFoundMessage(i, parent->foundGameInfo, parent->bssid));
            parent->found = FALSE;
        }
        if (parent->updated) {
            subject->Notify(new MMultiPlayParentUpdatedMessage(i, parent->updatedGameInfo));
            parent->updated = FALSE;
        }
        if (parent->lost) {
            subject->Notify(new MMultiPlayParentLostMessage(i));
            parent->lost = FALSE;
        }
    }
    if (events->mFatalError) {
        events->mFatalError = FALSE;
        subject->Notify(new MMultiPlayFatalErrorMessage());
    }
}

void MMultiPlay::SetParentLifetime(u32 frames) {
    mParentLifetimeReload = frames;
}

void MMultiPlay::Fail() {
    mTimeout = 0;
    mReady = FALSE;
    mFailed = TRUE;
    mState = STATE_NONE;
    mSynchronizing = FALSE;
    mEvents->mFatalError = TRUE;
}

u32 MMultiPlay::GetPlayerIndex() {
    return mPlayerIndex;
}

u8 MMultiPlay::IsFailed() {
    return mFailed;
}

u8 MMultiPlay::IsReady() {
    return mReady;
}

u8 MMultiPlay::IsFatal() {
    return mFatal;
}

u32 MMultiPlay::GetSharedDataSize() {
    return mSharedDataSize;
}

MTask *MMultiPlay::CreateWaitReadyTask() {
    return new MMultiPlayWaitReadyTask();
}

MTask *MMultiPlay::CreateFinishTask() {
    MTask *finish = new MMultiPlayFinishTask();
    MTask *reset = CreateResetTask();
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    set->AddTail(reset);
    set->AddTail(finish);
    return set;
}

void MMultiPlay::End() {
    mReady = FALSE;
    mEvents->mFinished = TRUE;
    if (WM_End(EndCallback) != WM_ERRCODE_OPERATING) {
        mState = STATE_STOPPED;
        mReady = TRUE;
    }
}

void MMultiPlay::EndCallback(void *arg) {
    sMultiPlay.instance->mState = STATE_STOPPED;
    sMultiPlay.instance->mReady = TRUE;
    sMultiPlay.instance->mActive.Clear();
}

MTask *MMultiPlay::CreateInitializeTask() {
    return new MMultiPlayInitializeTask();
}

void MMultiPlay::Initialize() {
    mReady = FALSE;
    if (WM_Initialize((void *) (((u32) mSystemBuf + 31) & ~31), InitializeCallback, mDmaNo) !=
        WM_ERRCODE_OPERATING) {
        Fail();
    }
}

void MMultiPlay::InitializeCallback(void *arg) {
    sMultiPlay.instance->mState = STATE_IDLE;
    sMultiPlay.instance->mReady = TRUE;
    sMultiPlay.instance->mEvents->mInitialized = TRUE;
    sMultiPlay.instance->mActive.Set();
    WM_SetIndCallback(IndicationCallback);
}

void MMultiPlay::IndicationCallback(void *arg) {
}

MTask *MMultiPlay::CreateResetTask() {
    return new MMultiPlayResetTask();
}

void MMultiPlay::Reset() {
    mState = STATE_RESETTING;
    mTimeout = 0;
    mReady = FALSE;
    mFailed = FALSE;
    mFatal = FALSE;
    mSynchronizing = FALSE;
    mEvents->mReset = TRUE;
    if (WM_Reset(ResetCallback) != WM_ERRCODE_OPERATING) {
        mState = STATE_IDLE;
        mReady = TRUE;
    }
}

void MMultiPlay::ResetCallback(void *arg) {
    sMultiPlay.instance->mState = STATE_IDLE;
    sMultiPlay.instance->mReady = TRUE;
}

MTask *MMultiPlay::CreateDisconnectTask(u16 aid) {
    return new MMultiPlayDisconnectTask(aid);
}

void MMultiPlay::Disconnect(u16 aid) {
    WM_Disconnect(DisconnectCallback, aid);
}

void MMultiPlay::DisconnectCallback(void *arg) {
}

MTask *MMultiPlay::CreateGetOptimumChannelTask(u16 *channel) {
    return new MMultiPlayGetOptimumChannelTask(channel);
}

void MMultiPlay::GetOptimumChannel(u16 *channel) {
    mReady = FALSE;
    mChannelOut = channel;
    mAllowedChannels = WM_GetAllowedChannel();
    if (!WMi_IsWirelessEnabled() || mAllowedChannels == 0x8000) {
        Fail();
        return;
    }
    mMeasureChannel = 1;
    mBestBusyRatio = 0xffff;
    MeasureNextChannel();
}

void MMultiPlay::MeasureNextChannel() {
    while (mMeasureChannel <= 14) {
        if (mAllowedChannels & (1 << (mMeasureChannel - 1))) {
            if (WM_MeasureChannel(MeasureChannelCallback, 0, 0, mMeasureChannel, 1) != WM_ERRCODE_OPERATING) {
                Fail();
            }
            return;
        }
        mMeasureChannel++;
    }
    if (mBestBusyRatio == 0xffff) {
        Fail();
        return;
    }
    mReady = TRUE;
}

void MMultiPlay::MeasureChannelCallback(void *arg) {
    sMultiPlay.instance->OnMeasureChannel((WMMeasureChannelCallback *) arg);
}

void MMultiPlay::OnMeasureChannel(WMMeasureChannelCallback *cb) {
    if (cb->errcode == WM_ERRCODE_FAILED) {
        Fail();
        return;
    }
    if (cb->ccaBusyRatio < mBestBusyRatio) {
        mBestBusyRatio = cb->ccaBusyRatio;
        *mChannelOut = mMeasureChannel;
    }
    mMeasureChannel++;
    MeasureNextChannel();
}

void MMultiPlay::StartMP() {
    mReady = FALSE;
    s32 recvSize = WM_GetMPReceiveBufferSize();
    s32 sendSize = WM_GetMPSendBufferSize();
    if (WM_StartMP(StartMPCallback, (u16 *) (((u32) mMpRecvBuf + 31) & ~31), recvSize,
                   (u16 *) (((u32) mMpSendBuf + 31) & ~31), sendSize, mContinuousMp == FALSE ? 1 : 0) !=
        WM_ERRCODE_OPERATING) {
        Fail();
    }
}

void MMultiPlay::StartMPCallback(void *arg) {
    sMultiPlay.instance->OnStartMP((WMStartMPCallback *) arg);
}

void MMultiPlay::OnStartMP(WMStartMPCallback *cb) {
    switch (cb->state) {
    case WM_STATECODE_MP_IND:
        if (cb->errcode == WM_ERRCODE_NO_DATA) {
            Fail();
        }
        break;
    case WM_STATECODE_MP_START:
        if (cb->errcode != WM_ERRCODE_SUCCESS) {
            Fail();
            return;
        }
        MultiPlayPrint("MP started\n");
        mState = STATE_MP_PARENT;
        mReady = TRUE;
        mEvents->mMpStarted = TRUE;
        mDataSharing[0].info = (WMDataSharingInfo *) (((u32) mDataSharing[0].space + 31) & ~31);
        if (WM_StartDataSharing(mDataSharing[0].info, 12, (1 << mMaxPlayers) - 1, 6, TRUE) !=
            WM_ERRCODE_SUCCESS) {
            Fail();
            return;
        }
        mDataSharing[1].info = (WMDataSharingInfo *) (((u32) mDataSharing[1].space + 31) & ~31);
        if (WM_StartDataSharing(mDataSharing[1].info, 13, (1 << mMaxPlayers) - 1, 2, TRUE) !=
            WM_ERRCODE_SUCCESS) {
            Fail();
            return;
        }
        mDataSharing[2].info = (WMDataSharingInfo *) (((u32) mDataSharing[2].space + 31) & ~31);
        if (WM_StartDataSharing(mDataSharing[2].info, 14, (1 << mMaxPlayers) - 1, 16, TRUE) !=
            WM_ERRCODE_SUCCESS) {
            Fail();
            return;
        }
        mDataSharing[3].info = (WMDataSharingInfo *) (((u32) mDataSharing[3].space + 31) & ~31);
        if (WM_StartDataSharing(mDataSharing[3].info, 15, (1 << mMaxPlayers) - 1,
                                sMultiPlay.instance->GetSharedDataSize(), TRUE) != WM_ERRCODE_SUCCESS) {
            Fail();
        }
        break;
    }
}

inline MMultiPlayStartParentTask::MMultiPlayStartParentTask(const void *userGameInfo, u32 size, u16 channel)
    : MProcess(NULL) {
    if (userGameInfo != NULL) {
        MI_CpuCopy8(userGameInfo, mUserGameInfo, size);
    }
    mUserGameInfoSize = size;
    if (channel != 0) {
        mChannel = channel;
    } else {
        mSerial.AddTail(sMultiPlay.instance->CreateGetOptimumChannelTask(&mChannel));
    }
}

MTask *MMultiPlay::CreateStartParentTask(const void *userGameInfo, u32 size, u16 channel) {
    MMultiPlayStartParentTask *task = new MMultiPlayStartParentTask(userGameInfo, size, channel);
    return task;
}

void MMultiPlay::StartParent(u16 channel, const void *userGameInfo, u32 size) {
    mAid = 0;
    mConnectedAids = 1;
    mIsParent = TRUE;
    mParentParam = (WMParentParam *) (((u32) mParentParamSpace + 31) & ~31);
    MI_CpuFill8(mParentParam, 0, sizeof(WMParentParam));
    MI_CpuCopy8(userGameInfo, mUserGameInfo, size);
    mParentParam->userGameInfo = mUserGameInfo;
    mParentParam->userGameInfoLength = size;
    mParentParam->ggid = mGgid;
    mParentParam->tgid = mTgid == 0x10000 ? WM_GetNextTgid() : mTgid;
    mParentParam->entryFlag = TRUE;
    mParentParam->maxEntry = mMaxPlayers - 1;
    mParentParam->beaconPeriod = WM_GetDispersionBeaconPeriod();
    mParentParam->channel = channel;
    mParentParam->parentMaxSize = mParentMaxSize;
    mParentParam->childMaxSize = mChildMaxSize;
    mReady = FALSE;
    if (WM_SetParentParameter(SetParentParameterCallback, mParentParam) != WM_ERRCODE_OPERATING) {
        Fail();
    }
}

void MMultiPlay::SetParentParameterCallback(void *arg) {
    sMultiPlay.instance->OnSetParentParameter((WMCallback *) arg);
}

void MMultiPlay::OnSetParentParameter(WMCallback *cb) {
    if (WM_StartParent(StartParentCallback) != WM_ERRCODE_OPERATING) {
        Fail();
    }
}

void MMultiPlay::StartParentCallback(void *arg) {
    sMultiPlay.instance->OnStartParent((WMStartParentCallback *) arg);
}

void MMultiPlay::OnStartParent(WMStartParentCallback *cb) {
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        Fail();
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_PARENT_START:
        MultiPlayPrint("parent started\n");
        mState = STATE_PARENT;
        mTimeout = mTimeoutReload;
        mReady = TRUE;
        StartMP();
        break;
    case WM_STATECODE_CONNECTED: {
        if (mSynchronizing) {
            Fail();
            return;
        }
        mConnectedAids |= 1 << cb->aid;
        u16 aid = cb->aid;
        MMultiPlayEvents *events = mEvents;
        if (!events->mChildren[aid].connected) {
            MI_CpuCopy8(cb->ssid, events->mChildren[aid].ssid, 24);
            MI_CpuCopy8(cb->macAddress, events->mChildren[aid].macAddress, 6);
            events->mChildren[aid].connected = TRUE;
        }
        break;
    }
    case WM_STATECODE_DISCONNECTED:
        mEvents->mChildren[cb->aid].disconnected = TRUE;
        mConnectedAids &= ~(1 << cb->aid);
        if (mSynchronizing) {
            mFatal = TRUE;
            Fail();
        }
        break;
    case WM_STATECODE_BEACON_SENT:
        break;
    }
}

MTask *MMultiPlay::CreateStartScanTask() {
    return new MMultiPlayStartScanTask();
}

void MMultiPlay::StartScan() {
    mState = STATE_SCANNING;
    mStopScan = FALSE;
    MI_CpuFill8(&mScanParam, 0, sizeof(mScanParam));
    mScanParam.scanBuf = (WMBssDesc *) (((u32) mScanBufSpace + 31) & ~31);
    mScanParam.scanBufSize = 0x400;
    mScanParam.channelList = WM_GetAllowedChannel();
    mScanParam.maxChannelTime = WM_GetDispersionScanPeriod();
    MI_CpuFill8(mScanParam.bssid, 0xff, sizeof(mScanParam.bssid));
    mScanParam.scanType = WM_SCANTYPE_ACTIVE;
    mScanParam.ssidLength = 0;
    for (u32 i = 0; i < mMaxParents; i++) {
        mParentValid[i] = FALSE;
        mParentLifetime[i] = mParentLifetimeReload;
    }
    ScanNext();
}

void MMultiPlay::ScanNext() {
    if (mStopScan) {
        mReady = TRUE;
        mState = STATE_IDLE;
        return;
    }
    mReady = FALSE;
    if (WM_StartScanEx(ScanCallback, &mScanParam) != WM_ERRCODE_OPERATING) {
        Fail();
    }
}

void MMultiPlay::ScanCallback(void *arg) {
    sMultiPlay.instance->OnScan((WMStartScanExCallback *) arg);
}

static inline BOOL IsSameBssid(const u8 *a, const u8 *b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3] && a[4] == b[4] && a[5] == b[5];
}

void MMultiPlay::OnBssFound(WMBssDesc *bss) {
    u32 ggid = bss->gameInfo.ggid;
    if (ggid != mGgid) {
        if (bss->gameInfo.gameNameCount_attribute & 1) {
            MMultiPlayEvents *events = mEvents;
            events->mAnotherGameParentFound = TRUE;
            events->mAnotherGameGgid = ggid;
            MI_CpuCopy8(bss->gameInfo.userGameInfo, events->mAnotherGameInfo, 0x70);
            MI_CpuCopy8(bss->bssid, events->mAnotherGameBssid, 6);
        }
        return;
    }
    if (bss->gameInfo.gameNameCount_attribute & 1) {
        u32 i;
        for (i = 0; i < mMaxParents; i++) {
            if (mParentValid[i] == TRUE && IsSameBssid(mParents[i].bssid, bss->bssid)) {
                mParentLifetime[i] = mParentLifetimeReload;
                if (bss->gameInfo.tgid != mParents[i].gameInfo.tgid) {
                    MIi_CpuCopy32(bss, &mParents[i], sizeof(WMBssDesc));
                    u16 *info = mParents[i].gameInfo.userGameInfo;
                    MMultiPlayEvents *events = mEvents;
                    if (!events->mParents[i].updated) {
                        MI_CpuCopy8(info, events->mParents[i].updatedGameInfo, 0x70);
                        events->mParents[i].updated = TRUE;
                    }
                }
                return;
            }
        }
        for (i = 0; i < mMaxParents; i++) {
            if (mParentValid[i] == FALSE) {
                mParentValid[i] = TRUE;
                mParentLifetime[i] = mParentLifetimeReload;
                MIi_CpuCopy32(bss, &mParents[i], sizeof(WMBssDesc));
                u16 *info = mParents[i].gameInfo.userGameInfo;
                MMultiPlayEvents *events = mEvents;
                if (!events->mParents[i].found) {
                    MI_CpuCopy8(info, events->mParents[i].foundGameInfo, 0x70);
                    MI_CpuCopy8(bss->bssid, events->mParents[i].bssid, 6);
                    events->mParents[i].found = TRUE;
                }
                return;
            }
        }
    } else {
        for (u32 i = 0; i < mMaxParents; i++) {
            if (mParentValid[i] == TRUE && IsSameBssid(mParents[i].bssid, bss->bssid)) {
                mParentValid[i] = FALSE;
                mParentLifetime[i] = 0;
                mEvents->mParents[i].lost = TRUE;
                return;
            }
        }
    }
}

void MMultiPlay::OnScan(WMStartScanExCallback *cb) {
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        Fail();
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_PARENT_NOT_FOUND:
        break;
    case WM_STATECODE_PARENT_FOUND:
        for (int i = 0; i < cb->bssDescCount; i++) {
            OnBssFound(cb->bssDesc[i]);
        }
        break;
    }
    ScanNext();
}

MTask *MMultiPlay::CreateEndScanTask() {
    return new MMultiPlayEndScanTask();
}

void MMultiPlay::EndScan() {
    mReady = FALSE;
    if (WM_EndScan(EndScanCallback) != WM_ERRCODE_OPERATING) {
        Fail();
    }
}

void MMultiPlay::EndScanCallback(void *arg) {
    sMultiPlay.instance->OnEndScan((WMCallback *) arg);
}

void MMultiPlay::OnEndScan(WMCallback *cb) {
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        Fail();
        return;
    }
    mState = STATE_IDLE;
    mReady = TRUE;
}

MTask *MMultiPlay::CreateStartChildTask(u32 index, const u8 *ssid) {
    return new MMultiPlayStartChildTask(index, ssid);
}

void MMultiPlay::StartChild(u32 index, const u8 *ssid) {
    mReady = FALSE;
    mIsParent = FALSE;
    if (!mParentValid[index]) {
        Fail();
        return;
    }
    if (WM_StartConnectEx(StartConnectCallback, &mParents[index], ssid, TRUE, 0) != WM_ERRCODE_OPERATING) {
        Fail();
    }
}

void MMultiPlay::StartConnectCallback(void *arg) {
    sMultiPlay.instance->OnStartConnect((WMStartConnectCallback *) arg);
}

void MMultiPlay::OnStartConnect(WMStartConnectCallback *cb) {
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        Fail();
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_CONNECTED:
        mEvents->mParentConnected = TRUE;
        mAid = cb->aid;
        mState = STATE_CHILD;
        mReady = TRUE;
        StartMP();
        break;
    case WM_STATECODE_DISCONNECTED:
        mEvents->mParentDisconnected = TRUE;
        if (mSynchronizing) {
            mFatal = TRUE;
            Fail();
        }
        break;
    }
}

// Each step task runs one MMultiPlay operation, then waits for it to finish.
void MMultiPlayStartChildTask::Idle() {
    if (sMultiPlay.instance->IsFailed()) {
        Exit();
        return;
    }
    sMultiPlay.instance->StartChild(mIndex, mSsid);
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *wait = sMultiPlay.instance->CreateWaitReadyTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
}

void MMultiPlayEndScanTask::Idle() {
    if (sMultiPlay.instance->IsFailed()) {
        Exit();
        return;
    }
    switch (mStep++) {
    case 0:
        sMultiPlay.instance->mStopScan = TRUE;
        mSerial.AddTail(sMultiPlay.instance->CreateWaitReadyTask());
        break;
    case 1:
        sMultiPlay.instance->EndScan();
        mSerial.AddTail(sMultiPlay.instance->CreateWaitReadyTask());
        break;
    case 2:
        Exit();
        break;
    }
}

void MMultiPlayStartScanTask::Idle() {
    if (sMultiPlay.instance->IsFailed()) {
        Exit();
        return;
    }
    sMultiPlay.instance->StartScan();
    Exit();
}

void MMultiPlayStartParentTask::Idle() {
    if (sMultiPlay.instance->IsFailed()) {
        Exit();
        return;
    }
    sMultiPlay.instance->mChannel = mChannel;
    sMultiPlay.instance->StartParent(mChannel, mUserGameInfo, mUserGameInfoSize);
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *wait = sMultiPlay.instance->CreateWaitReadyTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
}

void MMultiPlayGetOptimumChannelTask::Idle() {
    if (sMultiPlay.instance->IsFailed()) {
        Exit();
        return;
    }
    sMultiPlay.instance->GetOptimumChannel(mChannel);
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *wait = sMultiPlay.instance->CreateWaitReadyTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
}

void MMultiPlayDisconnectTask::Execute() {
    sMultiPlay.instance->Disconnect(mAid);
    Exit();
}

void MMultiPlayResetTask::Idle() {
    if (sMultiPlay.instance->mState == MMultiPlay::STATE_STOPPED) {
        Exit();
        return;
    }
    sMultiPlay.instance->Reset();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *wait = sMultiPlay.instance->CreateWaitReadyTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
}

void MMultiPlayInitializeTask::Idle() {
    if (sMultiPlay.instance->IsFailed()) {
        Exit();
        return;
    }
    sMultiPlay.instance->Initialize();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *wait = sMultiPlay.instance->CreateWaitReadyTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
}

void MMultiPlayFinishTask::Idle() {
    if (sMultiPlay.instance->mState == MMultiPlay::STATE_STOPPED) {
        Exit();
        return;
    }
    sMultiPlay.instance->End();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *wait = sMultiPlay.instance->CreateWaitReadyTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
}

void MMultiPlayWaitReadyTask::Execute() {
    if (sMultiPlay.instance->IsReady() || sMultiPlay.instance->IsFailed()) {
        Exit();
    }
}

void MMultiPlayReleaseSynchronizeTask::Transfer() {
    if (sMultiPlay.instance->IsFailed()) {
        return;
    }
    sMultiPlay.instance->mSynchronizing = FALSE;
    if (sMultiPlay.instance->mIsParent) {
        WM_SetEntry(NULL, TRUE);
    }
}

void MMultiPlaySynchronizeDelayTask::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *sync = new MMultiPlaySynchronizeTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(sync);
    serial->AddTail(exit);
}

MMultiPlaySynchronizeTask::~MMultiPlaySynchronizeTask() {
    Disable();
}

void MMultiPlaySynchronizeTask::OnVSync() {
    if (mFirst) {
        mFirst = FALSE;
        sMultiPlay.instance->mSynchronizing = TRUE;
        if (sMultiPlay.instance->mIsParent) {
            WM_SetEntry(NULL, FALSE);
            if (sMultiPlay.instance->mConnectedAids == 1) {
                sMultiPlay.instance->Fail();
            }
        }
    }
    if (sMultiPlay.instance->IsFailed()) {
        Exit();
        return;
    }
    mSendAids = sMultiPlay.instance->mConnectedAids;
    if (WM_StepDataSharing(sMultiPlay.instance->mDataSharing[1].info, &mSendAids, &mRecv) != WM_ERRCODE_SUCCESS) {
        return;
    }
    if (mFrames++ < sMultiPlay.instance->mSynchronizeDelay) {
        return;
    }
    if (!(mRecv.receivedBitmap & 1)) {
        return;
    }
    u16 aids = *WM_GetSharedDataAddress(sMultiPlay.instance->mDataSharing[1].info, &mRecv, 0);
    if (aids != (mRecv.receivedBitmap & aids)) {
        return;
    }
    sMultiPlay.instance->mConnectedAids = aids;
    u32 index = 0;
    for (u32 aid = 0; aid < sMultiPlay.instance->mMaxPlayers; aid++) {
        if ((u16) (1 << aid) & aids) {
            if (aid == sMultiPlay.instance->mAid) {
                sMultiPlay.instance->mPlayerIndex = index;
            }
            index++;
        }
    }
    sMultiPlay.instance->mPlayerCount = index;
    sMultiPlay.instance->mEvents->mSynchronized = TRUE;
    Exit();
}

MMultiPlayIcon::MMultiPlayIcon(MDisplay *display, const char *path, BOOL fromEnd, u8 alwaysShown)
    : MFixedSizeView(MPoint(16, 16)) {
    mHidden = TRUE;
    mAlwaysShown = alwaysShown;
    mLinger = 0;
    MMountArchive("m2", gMultiPlayIconArchive);
    if (display == NULL) {
        display = gCurrentDisplay;
    }
    mOam = new MSmartOam(display, 1, 1, fromEnd);
    mMotion = new MMotion(path, mOam, 0, 1);
}

MMultiPlayIcon::~MMultiPlayIcon() {
    delete mMotion;
    delete mOam;
    MUnmountArchive("m2");
}

void MMultiPlayIcon::Execute() {
    if (mAlwaysShown) {
        if (mHidden) {
            mMotion->Hide();
        } else {
            mMotion->Show();
        }
    } else {
        if (sMultiPlay.activeCount != 0) {
            mLinger = 10;
        }
        if (mLinger != 0) {
            mLinger--;
        }
        if (mHidden || mLinger == 0) {
            mMotion->Hide();
        } else {
            mMotion->Show();
        }
    }
    if (mMotion->mState == STATE_RUN) {
        mMotion->Execute();
    }
    if (mOam->mState == STATE_RUN) {
        mOam->Execute();
    }
}

MMotion *MMultiPlayIcon::GetMotion() {
    return mMotion;
}

void MMultiPlayIcon::SetPosition(MPoint pos) {
    mMotion->SetPosition(pos);
}

MPoint MMultiPlayIcon::GetPosition() {
    return mMotion->GetPosition();
}

void MMultiPlayIcon::SetPriority(int priority) {
    mMotion->SetPriority(priority);
}

int MMultiPlayIcon::GetPriority() {
    return mMotion->GetPriority();
}

void MMultiPlayIcon::Show() {
    mHidden = FALSE;
}

void MMultiPlayIcon::Hide() {
    mHidden = TRUE;
}

BOOL MMultiPlayIcon::IsHidden() {
    return mHidden;
}

u32 MMultiPlayIcon::GetPlaneMask() {
    return 0x10;
}

MMultiPlayLinkIcon::MMultiPlayLinkIcon(MDisplay *display, BOOL fromEnd)
    : MMultiPlayIcon(display, sParams.linkIconPath, fromEnd, FALSE) {
}

void MMultiPlayLinkIcon::Execute() {
    int level = WM_GetLinkLevel();
    GetMotion()->Play(level + 2, 0);
    MMultiPlayIcon::Execute();
}

void MMultiPlayWirelessDriverStartTask::WaitStartUp() {
    if (mDone) {
        Exit();
    }
}

void MMultiPlayWirelessDriverStartTask::StartUp() {
    mDone = FALSE;
    WVR_StartUpAsync(gLcd->GetTextureSlotFlags(), StartUpCallback, this);
    mStep = data_020c4b0c.WaitStartUp_24;
}

void MMultiPlayWirelessDriverStartTask::StartUpCallback(void *arg, int result) {
    ((MMultiPlayWirelessDriverStartTask *) arg)->mDone = TRUE;
}

void MMultiPlayWirelessDriverStartTask::Begin() {
    PXI_Init();
    if (!PXI_IsCallbackReady(15, 1)) {
        Exit();
        return;
    }
    gLcd->UseTextureBanks(1);
    mStep = data_020c4b0c.StartUp_2C;
}

void MMultiPlayWirelessDriverStopTask::WaitTerminate() {
    if (mDone) {
        gLcd->FreeTextureBanks();
        Exit();
    }
}

void MMultiPlayWirelessDriverStopTask::Terminate() {
    mDone = FALSE;
    WVR_TerminateAsync(TerminateCallback, this);
    mStep = data_020c4b0c.WaitTerminate_34;
}

void MMultiPlayWirelessDriverStopTask::TerminateCallback(void *arg, int result) {
    ((MMultiPlayWirelessDriverStopTask *) arg)->mDone = TRUE;
}

void MMultiPlayWirelessDriverStopTask::Begin() {
    PXI_Init();
    if (!PXI_IsCallbackReady(15, 1)) {
        Exit();
        return;
    }
    mStep = data_020c4b0c.Terminate_54;
}

MMultiPlayMessageHub::MMultiPlayMessageHub(int first)
    : MLooperVSyncTask(first, FALSE), mSending(NULL), mRunning(FALSE), mUnk279(FALSE) {
    mSelf = sMultiPlay.instance->mPlayerIndex;
    mConnectedAids = sMultiPlay.instance->mConnectedAids;
    mUnk26C = 0;
    mConsoleCount = sMultiPlay.instance->mPlayerCount;
    mMaxPlayers = sMultiPlay.instance->mMaxPlayers;
    mSendData = (u16 *) new u8[sMultiPlay.instance->GetSharedDataSize()];
    MI_CpuFill8(mSendData, 0, sMultiPlay.instance->GetSharedDataSize());
    for (u32 i = 0; i < mConsoleCount; i++) {
        MMultiPlayHubIncoming in;
        in.data = new std::vector<char>;
        in.unk4 = 0;
        in.unk8 = 0;
        mIncoming.push_back(in);
    }
    Enable();
}

MMultiPlayMessageHub::~MMultiPlayMessageHub() {
    Disable();
    delete[] mSendData;
    for (u32 i = 0; i < mConsoleCount; i++) {
        delete mIncoming[i].data;
    }
    for (u32 i = 0; i < mOutgoing.size(); i++) {
        delete mOutgoing[i].data;
    }
    delete mSending;
}

void MMultiPlayMessageHub::SetUnk279(u8 value) {
    mUnk279 = value;
}

void MMultiPlayMessageHub::Broadcast(MMessage *msg) {
    SendTo(0xFFFF ^ (1 << mSelf), msg);
}

void MMultiPlayMessageHub::SendToAll(MMessage *msg) {
    SendTo(0xFFFF, msg);
}

// Serializes msg and queues it for the consoles in mask. NONMATCHING: the game spills mask and msg (and keeps the
// buffer in r7); so does the vector's range init, which goes through a char temporary on the stack.
void MMultiPlayMessageHub::SendTo(u32 mask, MMessage *msg) {
    msg->AddRef();
    u32 size = msg->GetSize();
    u8 *buf = new u8[size];
    msg->Write(buf);
    MMultiPlayHubOutgoing out;
    out.data = new std::vector<char>(std::__msl_move_iter<u8 *>(buf), std::__msl_move_iter<u8 *>(buf + size));
    out.mask = mask;
    mOutgoing.push_back(out);
    delete[] buf;
    msg->Release();
}

void MMultiPlayMessageHub::OnVSync() {
    if (!sMultiPlay.instance->IsFailed()) {
        mRunning = WM_StepDataSharing(sMultiPlay.instance->mDataSharing[3].info, mSendData, &mReceived) == WM_ERRCODE_SUCCESS;
    }
}

// The game's std::copy of chars (a memmove).
static inline char *MMultiPlayCopyChars(std::__vec_iterator<char> first, std::__vec_iterator<char> last, char *out) {
    memmove(out, &*first, &*last - &*first);
    return out + (&*last - &*first);
}

// Fills this frame's share of the send buffer: a message's header (its size and the consoles it's for) and then as
// much of it as fits, going on to the next message while there's room.
void MMultiPlayMessageHub::SendPending() {
    if (mUnk26C != mConnectedAids) {
        return;
    }
    char *out = (char *)mSendData;
    u32 left = sMultiPlay.instance->GetSharedDataSize();
    while (left >= 4) {
        if (mSending == NULL) {
            if (mOutgoing.size() == 0) {
                break;
            }
            std::vector<MMultiPlayHubOutgoing>::iterator next = mOutgoing.begin();
            mSending = next->data;
            u16 mask = next->mask;
            mOutgoing.erase(next);
            struct {
                u16 size;
                u16 mask;
            } header;
            header.size = mSending->size();
            header.mask = mask;
            MI_CpuCopy8(&header, out, 4);
            out += 4;
            left -= 4;
        } else {
            u32 n = std::min(left, (u32)mSending->size());
            MMultiPlayCopyChars(mSending->begin(), mSending->begin() + n, out);
            out += n;
            left -= n;
            if (n == mSending->size()) {
                delete mSending;
                mSending = NULL;
                if (!mUnk279) {
                    break;
                }
            } else {
                mSending->erase(mSending->begin(), mSending->begin() + n);
            }
        }
    }
    if (left != 0) {
        *(u16 *)out = 0;
    }
}

// Reads what each console shared this frame: message headers and pieces, put back together per console; a finished
// message for this console is sent to the observers. Until everyone is in step, the shared data is only compared
// with what this console sends. NONMATCHING: the game's stack slots for the insert's iterators and tag bytes differ
// (and so does one branch that then reaches); the vector insert it calls keeps a char temporary on the stack.
void MMultiPlayMessageHub::ReceivePending() {
    u32 index = 0;
    for (u32 aid = 0; aid < mMaxPlayers; aid++) {
        u32 bit = 1 << aid;
        if (!(mConnectedAids & bit)) {
            continue;
        }
        u32 i = index++;
        u8 *data = (u8 *)WM_GetSharedDataAddress(sMultiPlay.instance->mDataSharing[3].info, &mReceived, aid);
        if (data == NULL) {
            continue;
        }
        u32 left = sMultiPlay.instance->GetSharedDataSize();
        if (mConnectedAids != mUnk26C) {
            if (memcmp(data, mSendData, left) == 0) {
                mUnk26C |= bit;
            }
            continue;
        }
        if (left == 0) {
            continue;
        }
        do {
            if (mIncoming[i].unk8 == 0) {
                if (!mUnk279 && left != sMultiPlay.instance->GetSharedDataSize()) {
                    break;
                }
                struct {
                    u16 size;
                    u16 mask;
                } header;
                MI_CpuCopy8(data, &header, 4);
                if (header.size == 0) {
                    break;
                }
                mIncoming[i].unk8 = header.size;
                mIncoming[i].unk4 = header.mask;
                mIncoming[i].data->clear();
                mIncoming[i].data->reserve(header.size);
                data += 4;
                left -= 4;
            } else {
                u32 n = std::min(mIncoming[i].unk8, left);
                std::vector<char> *v = mIncoming[i].data;
                v->insert(v->end(), std::__msl_move_iter<u8 *>(data), std::__msl_move_iter<u8 *>(data + n));
                mIncoming[i].unk8 -= n;
                data += n;
                left -= n;
                if (mIncoming[i].unk8 == 0 && (mIncoming[i].unk4 & (1 << mSelf))) {
                    char *buf = new char[mIncoming[i].data->size()];
                    MMultiPlayCopyChars(mIncoming[i].data->begin(), mIncoming[i].data->end(), buf);
                    Notify(CreateMessage(buf));
                    delete[] buf;
                }
            }
        } while (left != 0);
    }
}

void MMultiPlayMessageHub::Execute() {
    if (!sMultiPlay.instance->IsFailed() && mRunning) {
        SendPending();
        ReceivePending();
    }
}

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the hub's send code is decompiled).
template class std::__vec_deleter<char, std::allocator<char> >;
template void std::__vec_deleter<MMultiPlayHubIncoming, std::allocator<MMultiPlayHubIncoming> >::sub_push_back(
    const MMultiPlayHubIncoming &x, Metrowerks::int2type<true>);
template std::__vec_deleter<MMultiPlayHubIncoming, std::allocator<MMultiPlayHubIncoming> >::~__vec_deleter();
template class std::__vec_deleter<MMultiPlayHubOutgoing, std::allocator<MMultiPlayHubOutgoing> >;
template std::__vec_deleter<MMultiPlayHubIncoming, std::allocator<MMultiPlayHubIncoming> &>::~__vec_deleter();
