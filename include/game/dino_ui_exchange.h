#ifndef GAME_DINO_UI_EXCHANGE_H
#define GAME_DINO_UI_EXCHANGE_H

#include <set>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "m/motion.h"
#include "m/multi_play.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"

struct DinoPlayerProfile;
class DinoUiWirelessCancel;
class MSmartOam;

// A wireless partner's MAC address.
struct DinoMacAddress {
    u8 bytes[6];
};

inline bool operator<(const DinoMacAddress &a, const DinoMacAddress &b) {
    for (u32 i = 0; i < 6; i++) {
        if (a.bytes[i] < b.bytes[i]) {
            return true;
        }
        if (a.bytes[i] > b.bytes[i]) {
            return false;
        }
    }
    return false;
}

class MMultiPlayMessageHub;
class DinoUiExchange : public MProcess, public MMessageObserver {
public:
    DinoUiExchange();
    virtual ~DinoUiExchange();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void InitProfile(DinoPlayerProfile *profile);
    void ShowCount();
    void Start();
    void Reset();
    void StateEnd();
    void StateStart();
    void StateCheck();
    void StateAsk();
    void StateWait();
    void StateRestart();
    void StateSearch();
    void StateCancelled();
    void StateQuit();
    void StateLeave();
    void StateFinish();
    void OnScanMessage(MMessage *msg);
    void OnExchangeMessage(MMessage *msg);
    void StateNone();
    void StateExchange();
    // Joined: sends this side's rock, profile and done signals, then exchanges.
    void StateJoined();
    void Unk02155f24();
    // A random fossil an exchange can give.
    void MakeExchangeEntry(DinoBackupEntry *entry);
    // Sets when the next exchange may happen.
    void SetNextTime() {
        MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
        u32 wait = defs.Get()->exchangeWait;
        u64 frames = gDinoBackup->GetPlayFrames();
        mNextTime = frames + wait;
    }

    void (DinoUiExchange::*mState)();               // 0xA8
    void (DinoUiExchange::*mOnMessage)(MMessage *msg); // 0xB0
    MMultiPlay *mMultiPlay;         // 0xB8
    MMultiPlayMessageHub *mHub;     // 0xBC: the exchange's messages
    BOOL mAnswer;                   // 0xC0: whether to exchange
    MTaskSet *mTasks;               // 0xC4
    u32 unkC8;
    MSmartOam *mOam;                // 0xCC: the count's
    u32 unkD0;
    MMotion *mOnes;                 // 0xD4: the count's digits
    MMotion *mTens;                 // 0xD8
    u32 mCount;                     // 0xDC
    int mMode;                      // 0xE0: 1: only the scan was started
    u32 mUnkE4;                     // 0xE4: a random 60-119 (frames)
    std::set<DinoMacAddress> mPartners; // 0xE8: the ones exchanged with already
    u8 mPartnerMac[6];              // 0xF4
    u8 unkFA[2];
    u8 mBeacon[0x18];               // 0xFC: sent when this side is the parent
    u8 mPartnerInfo[4];             // 0x114: the partner's beacon
    u16 mPartnerName[9];            // 0x118
    u8 unk12A[2];
    u8 mDone[2];                    // 0x12C: each side has sent its DinoExchangeDoneSignal
    u8 unk12E[2];
    u64 mNextTime;                  // 0x130: in play frames
    DinoUiWirelessCancel *mCancel;  // 0x138
    u8 mCancelled;                  // 0x13C
    u8 unk13D[3];
    MResource<void> mResource;      // 0x140
    DinoBackupEntry mRock;          // 0x150: the partner's
    DinoPlayerProfile mProfile;     // 0x164: the partner's
    u8 mIsParent;                   // 0x1E8: this side found the partner
    u8 unk1E9[0x1EC - 0x1E9];
};

#endif
