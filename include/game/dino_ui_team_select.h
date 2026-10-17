#ifndef GAME_DINO_UI_TEAM_SELECT_H
#define GAME_DINO_UI_TEAM_SELECT_H

#include "m/view.h"
#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiTeamSelectMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiTeamSelectMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};

class DinoUiTeamSelect : public MProcess {
public:
    virtual ~DinoUiTeamSelect();
    virtual void Idle();
};

class DinoUiTeamSelectTeamButton : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiTeamSelectTeamButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[12];
    MView *mView; // 0xA0
    u32 mTextId;        // 0xA4
};

#endif
