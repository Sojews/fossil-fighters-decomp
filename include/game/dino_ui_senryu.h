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
    SenryuMenuItem(u32 index, DinoUiSenryuMenu *menu) {
        mIndex = index;
        mUnk8C = (u32)menu;
    }
    DinoUiSenryuMenu *GetMenu() { return (DinoUiSenryuMenu *)mUnk8C; }
    virtual ~SenryuMenuItem();
    virtual MRect GetFrame();

    u32 mIndex;     // 0x88
    u32 mUnk8C;     // 0x8C
    MRect mFrame;   // 0x90
};

class DinoSenryuBackButton : public SenryuMenuItem, public DinoHelpItem {
public:
    DinoSenryuBackButton(DinoUiSenryuMenu *menu, MSmartOam *oam);
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

// The page arrows (index 4 the previous page, 5 the next).
class DinoUiSenryuMenuItem : public SenryuMenuItem, public DinoHelpItem {
public:
    DinoUiSenryuMenuItem(u32 index, DinoUiSenryuMenu *menu, MPoint pos);
    virtual ~DinoUiSenryuMenuItem();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    MPoint mPos;        // 0xAC
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
    DinoUiSenryuListHideMenuItem(u32 index, DinoUiSenryuMenu *menu, MRect frame) : SenryuMenuItem(index, menu) {
        mFrame = frame;
    }
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
    SenryuKeyCheckTask(void *owner) { mOwner = owner; }
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
