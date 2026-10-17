#ifndef GAME_DINO_UI_KEYBOARD_H
#define GAME_DINO_UI_KEYBOARD_H

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

class DinoUiKeyboard : public MProcess {
public:
    virtual ~DinoUiKeyboard();
    virtual void Idle();
};

class KasekiPictCache : public MProcess {
public:
    virtual ~KasekiPictCache();
    virtual void Idle();
};

class KeyCheckTask : public MTask {
public:
    virtual ~KeyCheckTask();
    virtual void Execute();
};

class KeyboardMenuItem : public MMenuItem {
public:
    virtual ~KeyboardMenuItem();
    virtual MRect GetFrame();
};

class DinoUiKeyboardMenuItem : public KeyboardMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiKeyboardMenuItem();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x2C];
    u32 mTextId;        // 0xC0
};

class DinoUiKeyboardHideMenuItem : public KeyboardMenuItem {
public:
    virtual ~DinoUiKeyboardHideMenuItem();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
};

class DinoKBBackButton : public KeyboardMenuItem, public DinoHelpItem {
public:
    virtual ~DinoKBBackButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x28];
    MMotion *mMotion;   // 0xBC
    u32 mTextId;        // 0xC0
};

class SetFocusTask : public MTask {
public:
    virtual ~SetFocusTask();
    virtual void Execute();
};

class PageChangeTask : public MTask {
public:
    virtual ~PageChangeTask();
    virtual void Execute();
};

#endif
