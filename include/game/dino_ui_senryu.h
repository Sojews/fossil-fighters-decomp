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

class DinoUiSenryuMenu;

class SenryuMenuItem : public MMenuItem {
public:
    virtual ~SenryuMenuItem();
    virtual MRect GetFrame();

    u32 mIndex;     // 0x88
    u32 mUnk8C;     // 0x8C
    MRect mFrame;   // 0x90
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

    u32 unkAC;
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

    u32 unkAC[2];
    MMotion *mMotion;   // 0xB4
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
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
};

class DinoUiSenryuSubMenu : public MTask {
public:
    DinoUiSenryuSubMenu(void *owner, u32 *value, u32 arg) : mOwner(owner), mValue(value), mArg(arg) {}
    virtual ~DinoUiSenryuSubMenu();
    virtual void Execute();

    void *mOwner;   // 0x10
    u32 *mValue;    // 0x14
    u32 mArg;       // 0x18
};

class SenryuKeyCheckTask : public MTask {
public:
    virtual ~SenryuKeyCheckTask();
    virtual void Execute();

    void *mOwner;   // 0x10
};

class CurChangeTask : public MTask {
public:
    CurChangeTask(DinoUiSenryuMenu *menu, u32 cur) {
        mUnk10 = (u32)menu;
        mUnk14 = cur;
    }
    virtual ~CurChangeTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

#endif
