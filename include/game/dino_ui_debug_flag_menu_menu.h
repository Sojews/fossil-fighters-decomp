#ifndef GAME_DINO_UI_DEBUG_FLAG_MENU_MENU_H
#define GAME_DINO_UI_DEBUG_FLAG_MENU_MENU_H

#include "m/font.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiDebugFlagMenu : public MMenu {
public:
    DinoUiDebugFlagMenu(int mode);
    virtual ~DinoUiDebugFlagMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    void ShowPage();

    int mMode;          // 0xA8
    u32 mUnkAC;
    MFont *mFont;       // 0xB0
    u32 mUnkB4[1];
    MTaskSet *mTasks;   // 0xB8: the cursor's moves
    MView *mCursor;     // 0xBC
    int mPage;          // 0xC0
    int mPageCount;     // 0xC4
    u8 unkC8[0xCC - 0xC8];
};

class DinoUiDebugFlagMenuItem : public MMenuItem {
public:
    virtual ~DinoUiDebugFlagMenuItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    u32 mUnk88;         // 0x88
    MFont *mFont;       // 0x8C
    MPoint mPos;        // 0x90
};

#endif
