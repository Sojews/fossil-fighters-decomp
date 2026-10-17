#ifndef GAME_DINO_UI_SENRYU_H
#define GAME_DINO_UI_SENRYU_H

#include "game/dino_resource.h"
#include "m/motion.h"
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

    u8 unk94[0x1C];
    MMotion *mMotion;   // 0xB0
    u32 mTextId;        // 0xB4
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

    u8 unk94[0x24];
    u32 mTextId;        // 0xB8
};

class DinoUiSenryuList : public MProcess {
public:
    DinoUiSenryuList();
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

    void *mOwner;   // 0x10
};

class CurChangeTask : public MTask {
public:
    virtual ~CurChangeTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

#endif
