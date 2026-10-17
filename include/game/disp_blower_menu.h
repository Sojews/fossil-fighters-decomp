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

    u8 mLevel;      // 0xA0: the volume shown, 0-10
    MPoint mPos;    // 0xA4
};

class SEVolumeButton : public DinoUiOptionMenuItem {
public:
    virtual ~SEVolumeButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u8 mLevel;      // 0xA0: the volume shown, 0-10
    MPoint mPos;    // 0xA4
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

class DinoUiOptionMenu;

class DinoUiOptionPower : public MTask, public MSleepHook {
public:
    virtual ~DinoUiOptionPower();
    virtual void Execute();
    virtual void OnSleep();
    virtual void OnWake();

    DinoUiOptionMenu *mMenu;    // 0x18
};

class DinoUiOptionMenu : public DinoHelpBase, public MLooperPreLoopHook, public MMenu {
public:
    virtual ~DinoUiOptionMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnIdle();
    virtual void OnPreLoop();
    void OnOKDecided();
    void OnBackDecided();
    void SetBgmVolume(u8 level);
    MSmartOam *GetOam() { return mOam; }
    void SetSeVolume(u8 level);
    void Sleep();
    void CheckKeys();

    u8 unkBF[1];     // members start in MMenu's tail padding
    MSmartOam *mOam; // 0xC0
    u8 unkC4[0x78];
    MTask *mUnk13C;  // 0x13C
    u8 unk140[0x40];
    int mDelay;      // 0x180: frames left before input is taken
    u8 mSilentFocus; // 0x184
};

#endif
