#ifndef GAME_DINO_BATTLE_VIEW_ABILITY_MENU_BG_H
#define GAME_DINO_BATTLE_VIEW_ABILITY_MENU_BG_H

#include <vector>
#include "m/picture.h"

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
class MPalBlock;
class MChr;

class DinoBattleViewAbilityMenuBG : public MProcess {
public:
    virtual ~DinoBattleViewAbilityMenuBG();
    virtual void Idle();

    void SetPriority(int priority);
    void StateShow();

    MBg *mBg;                   // 0x74
    u32 unk78;
    MText *mText;               // 0x7C
    MPalBlock *mPalBlock;       // 0x80
    u32 unk84;
    MChr *mChr;                 // 0x88
    MPictureFile mPicture;      // 0x8C
    void (DinoBattleViewAbilityMenuBG::*mState)(); // 0xC0
    u8 unkC8[0x1000];
    std::vector<void *> mUnk10C8; // 0x10C8
    u8 mUnk10D4;                // 0x10D4: owns the BG resources

    class SetPriorityTask : public MTask {
    public:
        virtual ~SetPriorityTask();
        virtual void Execute();

        DinoBattleViewAbilityMenuBG *mBG;   // 0x10
        int mPriority;                      // 0x14
    };
};

#endif
