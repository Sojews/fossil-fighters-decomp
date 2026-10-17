#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_MENU_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_MENU_H

#include "m/indicator.h"
#include "m/view.h"
#include "m/archive.h"
#include "m/font.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiDebugKasekiRockGetMenu : public MMenu {
public:
    DinoUiDebugKasekiRockGetMenu() : MMenu(0) {}
    virtual ~DinoUiDebugKasekiRockGetMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u32 mUnkA8[4];      // 0xA8
    MFont *mFont;       // 0xB8
    MTaskSet *mTasks;   // 0xBC: the cursor's moves
    MView *mCursor;     // 0xC0
    u32 mUnkC4[2];      // 0xC4
};

class DinoUiDebugKasekiRockGetMenuKasekiRockIdItem : public MMenuItem {
public:
    virtual ~DinoUiDebugKasekiRockGetMenuKasekiRockIdItem();
    virtual void Idle();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u32 *mValue;            // 0x88: set to the chosen rock id
    u32 mIndex;             // 0x8C: into mBuffer
    MIndicator *mView;      // 0x90
    u32 mUnk94;
    MMenuItem *mKeyItemA; // 0x98
    MMenuItem *mKeyItemB; // 0x9C
    MArchivePtr mArchive;   // 0xA0
    MArchiveFile mFile;     // 0xA4
    u32 *mBuffer;           // 0xB0: the rock ids
};

#endif
