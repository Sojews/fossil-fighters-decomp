#ifndef GAME_DISP_BLOWER_MENU_H
#define GAME_DISP_BLOWER_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
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

#endif
