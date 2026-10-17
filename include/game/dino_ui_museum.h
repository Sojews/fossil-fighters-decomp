#ifndef GAME_DINO_UI_MUSEUM_H
#define GAME_DINO_UI_MUSEUM_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiMuseum : public MProcess {
public:
    virtual ~DinoUiMuseum();
    virtual void Idle();
};

class DinoUiMuseumEffectLight : public MProcess {
public:
    virtual ~DinoUiMuseumEffectLight();
    virtual void Idle();
};

class DinoUiMuseumMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMuseumMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

class DinoUiMuseumBackButton : public DinoUiMuseumMenuItem {
public:
    virtual ~DinoUiMuseumBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMuseumPageLButton : public DinoUiMuseumMenuItem {
public:
    virtual ~DinoUiMuseumPageLButton();
    virtual void OnDecide();
};

class DinoUiMuseumPageRButton : public DinoUiMuseumMenuItem {
public:
    virtual ~DinoUiMuseumPageRButton();
    virtual void OnDecide();
};

class DinoUiMuseumSlotCreateTask : public MProcess {
public:
    virtual ~DinoUiMuseumSlotCreateTask();
    virtual void Idle();
};

class DinoUiMuseumInformationMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMuseumInformationMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

class DinoUiMuseumInformationBackButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMuseumInformationPageLButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationPageLButton();
    virtual void OnDecide();
};

class DinoUiMuseumInformationPageRButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationPageRButton();
    virtual void OnDecide();
};

class DinoUiMuseumInformationVoiceButton : public DinoUiMuseumInformationMenuItem {
public:
    virtual ~DinoUiMuseumInformationVoiceButton();
    virtual void OnDecide();
};

#endif
