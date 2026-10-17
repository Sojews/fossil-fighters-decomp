#ifndef GAME_DINO_BATTLE_VIEW_H
#define GAME_DINO_BATTLE_VIEW_H

#include <map>

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "game/dino_battle_help.h"

class DinoBattleViewUiRootMenuItem : public MMenuItem {
public:
    virtual ~DinoBattleViewUiRootMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();

    u32 unk88[3];
    void *mOwner;   // 0x94
};

class DinoBattleViewUiSelectAreaCommon : public MMenu {
public:
    virtual ~DinoBattleViewUiSelectAreaCommon();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u32 unkA8[2];
    u32 mUnkB0;     // 0xB0: player index
    u8 unkB4[0x38];
    u32 mUnkEC;     // 0xEC: DinoBattleView
    u8 unkF0[0x40];
    std::map<u32, void *> mUnk130;  // 0x130
    u8 unk13C[0x40];
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
};

class DinoBattleViewCancelMenuItem : public MMenuItem {
public:
    virtual ~DinoBattleViewCancelMenuItem();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

class DinoBattleHelpCreatureInfo : public DinoBattleHelpBase {
public:
    virtual ~DinoBattleHelpCreatureInfo();
    virtual const char *GetText();
};

class DinoFrameItem : public MMenuItem {
public:
    virtual ~DinoFrameItem();
    virtual MRect GetFrame();

    MRect mFrame;   // 0x88
};

#endif
