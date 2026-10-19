#ifndef GAME_CAMERA_WIPE_H
#define GAME_CAMERA_WIPE_H

#include <string>
#include <vector>
#include "m/font.h"
#include "m/screen.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MIndicator;
class DinoBattleViewAbilityMenuBG;
class DinoBattleViewAbilityTextWindow;

class CameraWipe : public MProcess {
public:
    virtual ~CameraWipe();
    virtual void Idle();

    struct Owner {
        u8 unk00[0x88];
        MScreen *mScreen;   // 0x88
    } *mOwner;              // 0x74
    u32 mUnk78;             // 0x78
    int mMode;              // 0x7C
};

class M3DFadeTask : public MTask {
public:
    virtual ~M3DFadeTask();
    virtual void Execute();

    int mUnk10;     // 0x10
    int mUnk14;     // 0x14
};

class DinoBattleViewActionInfoBar : public MProcess {
public:
    virtual ~DinoBattleViewActionInfoBar();
    virtual void Idle();

    BOOL IsIdle();

    std::string mUnk74;                 // 0x74
    MIndicator *mText;                  // 0x80: the line shown
    u32 unk84;
    int mTimer;                         // 0x88: frames left to show it
    int mFade;                          // 0x8C: 0 (hidden) to 6
    DinoBattleViewAbilityMenuBG *mBg;   // 0x90
    DinoBattleViewAbilityTextWindow *mWindow;   // 0x94
    std::vector<std::string> mLines;    // 0x98: waiting to be shown
    MFont *mFont;                       // 0xA4
};

#endif
