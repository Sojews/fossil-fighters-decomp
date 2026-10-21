#ifndef GAME_DINO_BATTLE_VIEW_ABILITY_MENU_BG_H
#define GAME_DINO_BATTLE_VIEW_ABILITY_MENU_BG_H

#include <vector>
#include "m/picture.h"
#include "m/text.h"
#include "m/screen.h"

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MBg;
class MText;
class MPalUnit;
class MChr;
struct MChrBlock;

class DinoBattleViewAbilityMenuBG : public MProcess {
public:
    DinoBattleViewAbilityMenuBG();
    virtual ~DinoBattleViewAbilityMenuBG();
    virtual void Idle();

    void SetPriority(int priority);
    // The BG's screen offset (its text's, when it owns the BG).
    void SetOffset(MPoint offset) {
        if (mUnk10D4) {
            mText->SetOffset(offset);
        } else {
            mOffset = offset;
        }
    }
    // Slides the BG (a one-frame wait when it doesn't own it).
    MTask *CreateOffsetAnime(MPoint from, MPoint to, int duration, int ease) {
        if (mUnk10D4) {
            return mText->CreateOffsetAnime(from, to, duration, ease);
        }
        return new MTimer(1);
    }
    void StateShow();
    void StateUpdate();

    MBg *mBg;                   // 0x74
    u32 unk78;
    MText *mText;               // 0x7C
    MPalUnit *mPalBlock;       // 0x80
    MChrBlock *mChrBlock;       // 0x84
    MChr *mChr;                 // 0x88
    MPictureFile mPicture;      // 0x8C
    void (DinoBattleViewAbilityMenuBG::*mState)(); // 0xC0
    u16 mScreen[64][32];        // 0xC8: built here, then transferred
    std::vector<void *> mUnk10C8; // 0x10C8
    u8 mUnk10D4;                // 0x10D4: owns the BG resources
    MPoint mOffset;             // 0x10D8: its screen offset, when it doesn't own the BG

    class SetPriorityTask : public MTask {
    public:
        virtual ~SetPriorityTask();
        virtual void Execute();

        DinoBattleViewAbilityMenuBG *mBG;   // 0x10
        int mPriority;                      // 0x14
    };
};

#endif
