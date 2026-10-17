#ifndef GAME_DISP_BLOWER_MENU_H
#define GAME_DISP_BLOWER_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_ui_option_menu_item.h"

class DinoUiOptionSpeedMenuItem : public DinoUiOptionMenuItem {
public:
    virtual ~DinoUiOptionSpeedMenuItem();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

class BGMVolumeButton : public DinoUiOptionMenuItem {
public:
    virtual ~BGMVolumeButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class SEVolumeButton : public DinoUiOptionMenuItem {
public:
    virtual ~SEVolumeButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class MicTestButton : public DinoUiOptionMenuItem {
public:
    virtual ~MicTestButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOptionBackButton : public DinoUiOptionMenuItem {
public:
    virtual ~DinoUiOptionBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOptionOkButton : public DinoUiOptionMenuItem {
public:
    virtual ~DinoUiOptionOkButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOptionPower : public MTask, public MSleepHook {
public:
    virtual ~DinoUiOptionPower();
    virtual void Execute();
    virtual void OnSleep();
    virtual void OnWake();
};

class DinoUiOptionMenu : public DinoHelpBase, public MLooperPreLoopHook, public MMenu {
public:
    virtual ~DinoUiOptionMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnIdle();
    virtual void OnPreLoop();
};

#endif
