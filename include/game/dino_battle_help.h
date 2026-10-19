#ifndef GAME_DINO_BATTLE_HELP_H
#define GAME_DINO_BATTLE_HELP_H

#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleHelpBase : public MProcess, public DinoHelpItem {
public:
    DinoBattleHelpBase(MDisplay *display, MRect rect, u32 textId);
    // The game's destructor is inline; its vtable's owner (dino_battle_help.cpp) emits it out of line.
#ifdef DINO_BATTLE_HELP_BASE_OWNER
    virtual ~DinoBattleHelpBase();
#else
    virtual ~DinoBattleHelpBase() {}
#endif
    virtual void Idle();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    MRect mRect;        // 0x80
    u32 mTextId;        // 0x90
};

class DinoBattleViewAbilityIcon;

class DinoBattleHelpSupport : public DinoBattleHelpBase {
public:
    virtual ~DinoBattleHelpSupport();
    virtual MRect GetRect();

    DinoBattleViewAbilityIcon *mIcon;   // 0x94
};

#endif
