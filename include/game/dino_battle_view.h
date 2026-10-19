#ifndef GAME_DINO_BATTLE_VIEW_H
#define GAME_DINO_BATTLE_VIEW_H

#include <map>

class DinoBattleViewCreature;
class DinoBattleView;
class DinoBattleViewCreatureInfo;

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/motion.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "game/dino_battle_help.h"

class DinoBattleViewUiSelectAreaCommon;

class DinoBattleViewUiRootMenuItem : public MMenuItem {
public:
    DinoBattleViewUiRootMenuItem(DinoBattleViewUiSelectAreaCommon *owner, u32 index, MPoint pos);
    virtual ~DinoBattleViewUiRootMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual u32 GetUnk4();  // from the item's row of the root menu table
    virtual u32 GetUnk0();
    void Refresh();     // (empty in the game)
    MPoint GetCursorPos();

    u32 mIndex;     // 0x88: the row
    MPoint mPos;    // 0x8C: where the cursor goes when no creature is there
    void *mOwner;   // 0x94: the DinoBattleViewUiSelectAreaCommon
    u8 mHasCreature;    // 0x98
    u8 mDisabled;   // 0x99
};

class DinoBattleViewUiSelectAreaCommon : public MMenu {
public:
    DinoBattleViewUiSelectAreaCommon(DinoBattleView *view, int player, MMessageObserver *target, u32 arg3, u8 arg4,
                                     u8 arg5);
    virtual ~DinoBattleViewUiSelectAreaCommon();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnFocusCreature(DinoBattleViewCreature *creature, MMenuItem *item);  // (empty in the game)
    DinoBattleViewCreature *GetItemCreature(MMenuItem *item);
    BOOL HandlePageItem(MMenuItem *item);
    void MoveCursor(MPoint pos, int frames, int arg);
    void MoveCursorTo(DinoBattleViewUiRootMenuItem *item, int frames, int arg);

    u32 mUnkA8;
    u32 mUnkAC;
    u32 mUnkB0;     // 0xB0: player index
    u32 mUnkB4;
    u32 mFocusedRow;    // 0xB8
    int mUnkBC;
    int mSideC0;        // 0xC0: the player's side
    u8 unkC4[0x1C];
    MPoint mUnkE0;
    u8 mUnkE8;
    u32 mUnkEC;     // 0xEC: DinoBattleView
    MMessageObserver *mTarget;  // 0xF0: told the choice
    MTask *mCursorTask;         // 0xF4: comes with the cursor
    MTaskSet *mUnkF8;           // 0xF8: waited on before answering
    MMotion *mCursor;           // 0xFC
    u8 unk100[0x18];
    DinoBattleViewCreatureInfo *mInfo;  // 0x118
    int mInfoKind;      // 0x11C
    MMenuItem *mItem120;    // 0x120
    MMenuItem *mPageItems[2];   // 0x124: page the info left and right
    u32 mUnk12C;
    std::map<u32, void *> mUnk130;  // 0x130
    u32 mUnk13C;
    u8 unk140[0x30];
    MTaskSet *mCursorAnime;     // 0x170
    u32 unk174;
    u8 mUnk178;
    u8 mUnk179;
    u8 mUnk17A;
};

class DinoBattleViewUiCreatureInfoMenu : public DinoHelpBase, public DinoBattleViewUiSelectAreaCommon {
public:
    virtual ~DinoBattleViewUiCreatureInfoMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    virtual void OnIdle();

};

class DinoBattleViewInfoLRItem : public MMenuItem {
public:
    virtual ~DinoBattleViewInfoLRItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    MView *mView;       // 0x88
    MPoint mOffset;     // 0x8C: of the touch area from the view
};

class DinoBattleViewCancelMenuItem : public MMenuItem {
public:
    virtual ~DinoBattleViewCancelMenuItem();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

class DinoBattleHelpCreatureInfo : public DinoBattleHelpBase {
public:
    DinoBattleHelpCreatureInfo(MDisplay *display, MRect rect, u32 textId, DinoBattleViewUiSelectAreaCommon *menu)
        : DinoBattleHelpBase(display, rect, textId), mMenu(menu) {}
    virtual ~DinoBattleHelpCreatureInfo();
    virtual const char *GetText();

    DinoBattleViewUiSelectAreaCommon *mMenu;    // 0x94
};

class DinoFrameItem : public MMenuItem {
public:
    virtual ~DinoFrameItem();
    virtual MRect GetFrame();

    MRect mFrame;   // 0x88
};

#endif
