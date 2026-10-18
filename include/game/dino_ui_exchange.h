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

struct DinoPlayerProfile;

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

    void (DinoUiExchange::*mState)();               // 0xA8
    void (DinoUiExchange::*mOnMessage)(MMessage *msg); // 0xB0
    MMultiPlay *mMultiPlay;         // 0xB8
    u8 unkBC[0xD4 - 0xBC];
    MMotion *mOnes;                 // 0xD4: the count's digits
    MMotion *mTens;                 // 0xD8
    u32 mCount;                     // 0xDC
    int mMode;                      // 0xE0: 1: only the scan was started
    u32 mUnkE4;                     // 0xE4: a random 60-119 (frames)
    std::set<DinoMacAddress> mPartners; // 0xE8: the ones exchanged with already
    u8 unkF4[8];
    u8 mBeacon[0x18];               // 0xFC: sent when this side is the parent
    u8 unk114[0x2C];
    MArchivePtr mArchive;           // 0x140
    MArchiveFile mFile;             // 0x144
    u8 unk150[0x1EC - 0x150];
};

#endif
