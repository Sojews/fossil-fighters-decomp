#ifndef GAME_DINO_UI_KEYBOARD_H
#define GAME_DINO_UI_KEYBOARD_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include <vector>
#include "m/picture.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "game/dino_ui_keyboard_menu.h"

class DinoUiKeyboard : public MProcess {
public:
    DinoUiKeyboard(int mode, int arg, u8 flag);
    DinoUiKeyboard(int mode, int arg);
    virtual ~DinoUiKeyboard();
    virtual void Idle();

    int mMode;      // 0x74
    int mArg;       // 0x78
    u8 mFlag;       // 0x7C
};

class KasekiPictCache : public MProcess {
public:
    virtual ~KasekiPictCache();
    virtual void Idle();

    u32 unk74;
    std::vector<MPictureFile> mPictures;    // 0x78
};

class KeyCheckTask : public MTask {
public:
    virtual ~KeyCheckTask();
    virtual void Execute();

    void *mOwner;   // 0x10
};

class KeyboardMenuItem : public MMenuItem {
public:
    virtual ~KeyboardMenuItem();
    virtual MRect GetFrame();

    u32 unk88[5];
    MRect mFrame;   // 0x9C
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

    MMotion *mMotion;           // 0xB8
    MSerialTaskSet *mTasks;     // 0xBC
    u32 mTextId;                // 0xC0
};

class DinoUiKeyboardHideMenuItem : public KeyboardMenuItem {
public:
    virtual ~DinoUiKeyboardHideMenuItem();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
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

    u32 unkB8;
    MMotion *mMotion;   // 0xBC
    u32 mTextId;        // 0xC0
};

class SetFocusTask : public MTask {
public:
    virtual ~SetFocusTask();
    virtual void Execute();

    DinoUiKeyboardMenu *mMenu;  // 0x10
    MMenuItem *mItem;           // 0x14
};

class PageChangeTask : public MTask {
public:
    virtual ~PageChangeTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

#endif
