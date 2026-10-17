#ifndef GAME_DINO_BATTLE_VIEW2_H
#define GAME_DINO_BATTLE_VIEW2_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_common_yes_no_menu_item_menu.h"
#include "game/dino_field_camera_hook.h"

class AnyInputTimerWait : public MProcess {
public:
    virtual ~AnyInputTimerWait();
    virtual void Idle();
};

class DinoBattleViewUiRootSelect : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoBattleViewUiRootSelect();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

class DinoBattleViewTurnEndDialog : public DinoCommonYesNoMenu {
public:
    virtual ~DinoBattleViewTurnEndDialog();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

class DinoBattleViewGivingUpDialog : public DinoCommonYesNoMenu {
public:
    virtual ~DinoBattleViewGivingUpDialog();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

class DinoBattleViewGivingUpMenu : public MProcess {
public:
    virtual ~DinoBattleViewGivingUpMenu();
    virtual void Idle();

    u8 unk74[0x4];
    void (DinoBattleViewGivingUpMenu::*mState)(); // 0x78
};

class MotionDisableHelp : public MMotion, public DinoHelpBase {
public:
    virtual ~MotionDisableHelp();
    // slot unk3 0x2060710 _ZN7MMotion11SetPositionE6MPoint
    // slot unk4 0x2060770 _ZN7MMotion11GetPositionEv
    // slot unk5 0x20619a4 _ZN7MMotion7SetSizeE6MPoint
    // slot unk6 0x20619ac _ZN7MMotion7GetSizeEv
    // slot unk7 0x2060c7c _ZN7MMotion8GetFrameEv
    // slot unk8 0x206077c _ZN7MMotion11SetPriorityEi
    // slot unk9 0x20607ac _ZN7MMotion11GetPriorityEv
    // slot unk10 0x2060804 _ZN7MMotion4ShowEv
    // slot unk11 0x206089c _ZN7MMotion4HideEv
    // slot unk12 0x20608dc _ZN7MMotion8IsHiddenEv
    // slot unk13 0x20619d0 _ZN7MMotion12GetPlaneMaskEv
    // slot unk14 0x2060ec8 _ZN7MMotion4PlayEii
    // slot unk15 0x20607b8 _ZN7MMotion9SetAnchorEi
    // slot unk16 0x20607e8 _ZN7MMotion9GetAnchorEv
    // slot unk17 0x206199c _ZN7MMotion5Unk44Ei
    // slot unk18 0x20619a0 _ZN7MMotion5Unk48Ei
};

class DinoBattleViewCreatureAttackIcon : public MMenuItem {
public:
    virtual ~DinoBattleViewCreatureAttackIcon();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoBattleViewUiRootSelectItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoBattleViewUiRootSelectItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x4];
    MMotion *mMotion;   // 0x98
    u8 unk9C[0x4];
    u32 mTextId;        // 0xA0
    int mFocusAnime;    // 0xA4
    int mBlurAnime;     // 0xA8
};

class MenuExitTask : public MTask {
public:
    virtual ~MenuExitTask();
    virtual void Execute();
};

class WaitEnableRootMenu : public MTask {
public:
    virtual ~WaitEnableRootMenu();
    virtual void Execute();

    void *mMenu;    // 0x10
};

class DinoBattleViewAbilityIcon : public MProcess {
public:
    virtual ~DinoBattleViewAbilityIcon();
    virtual void Idle();
};

#endif
