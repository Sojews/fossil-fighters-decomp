#ifndef GAME_DINO_UI_SENRYU_H
#define GAME_DINO_UI_SENRYU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class SenryuMenuItem : public MMenuItem {
public:
    virtual ~SenryuMenuItem();
    virtual MRect GetFrame();
};

class DinoSenryuBackButton : public SenryuMenuItem, public DinoHelpItem {
public:
    virtual ~DinoSenryuBackButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

class DinoUiSenryuMenuItem : public SenryuMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiSenryuMenuItem();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

class DinoUiSenryuList : public MProcess {
public:
    virtual ~DinoUiSenryuList();
    virtual void Idle();
};

class DinoUiSenryuListHideMenuItem : public SenryuMenuItem {
public:
    virtual ~DinoUiSenryuListHideMenuItem();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
};

class DinoUiSenryuSubMenu : public MTask {
public:
    virtual ~DinoUiSenryuSubMenu();
    virtual void Execute();
};

class SenryuKeyCheckTask : public MTask {
public:
    virtual ~SenryuKeyCheckTask();
    virtual void Execute();
};

class CurChangeTask : public MTask {
public:
    virtual ~CurChangeTask();
    virtual void Execute();
};

#endif
