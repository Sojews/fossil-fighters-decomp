#ifndef GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_MENU_H
#define GAME_DINO_UI_DEBUG_KASEKI_ROCK_GET_MENU_MENU_H

#include "m/indicator.h"
#include "m/view.h"
#include "m/archive.h"
#include "m/resource.h"
#include "m/font.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MSmartOam;
struct DinoKasekiDefs;

class DinoUiDebugKasekiRockGetMenu : public MMenu {
public:
    DinoUiDebugKasekiRockGetMenu() : MMenu(0) {}
    virtual ~DinoUiDebugKasekiRockGetMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u32 mKaseki;        // 0xA8: the fossil's id
    u32 mUnkAC;         // 0xAC
    u32 mUnkB0;         // 0xB0
    MSmartOam *mOam;    // 0xB4
    MFont *mFont;       // 0xB8
    MTaskSet *mTasks;   // 0xBC: the cursor's moves
    MView *mCursor;     // 0xC0
    MMenuItem *mStartItem;  // 0xC4
    MMenuItem *mBackItem;   // 0xC8
};

class DinoUiDebugKasekiRockGetMenuKasekiRockIdItem : public MMenuItem {
public:
    DinoUiDebugKasekiRockGetMenuKasekiRockIdItem(MMenu *menu, u32 *value, MSmartOam *oam, MFont *font, MPoint center);
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
    MResource<DinoKasekiDefs> mDefs;    // 0xA0
    u32 *mBuffer;           // 0xB0: the rock ids
    u32 mCount;             // 0xB4: in mBuffer
};

#endif
