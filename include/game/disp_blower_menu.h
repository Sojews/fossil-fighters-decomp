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
    DinoUiOptionSpeedMenuItem(DinoUiOptionMenu *owner, u8 speed);
    virtual ~DinoUiOptionSpeedMenuItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    u8 mSpeed;      // 0xA0: the message speed this item picks
};

class BGMVolumeButton : public DinoUiOptionMenuItem {
public:
    BGMVolumeButton(DinoUiOptionMenu *owner, u8 level, u32 textId);
    virtual ~BGMVolumeButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u8 mLevel;      // 0xA0: the volume shown, 0-10
    MPoint mPos;    // 0xA4
};

class SEVolumeButton : public DinoUiOptionMenuItem {
public:
    SEVolumeButton(DinoUiOptionMenu *owner, int level, u32 textId);
    virtual ~SEVolumeButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    u8 mLevel;      // 0xA0: the volume shown, 0-10
    MPoint mPos;    // 0xA4
};

class MicTestButton : public DinoUiOptionMenuItem {
public:
    MicTestButton(DinoUiOptionMenu *owner, u32 textId);
    virtual ~MicTestButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    MMotion *mMotionA0;     // 0xA0: the "speak" prompt
    MMotion *mMotionA4;     // 0xA4
    MMotion *mMotionA8;     // 0xA8
    MMotion *mMeter;        // 0xAC
    MTaskSet *mTasks;       // 0xB0
    MTaskSet *mMessages;    // 0xB4: the "wait" message while the mic starts
    u32 mCount;             // 0xB8: samples taken
    u8 mLevels[0x40];       // 0xBC: the last samples' strengths
};

class DinoUiOptionBackButton : public DinoUiOptionMenuItem {
public:
    DinoUiOptionBackButton(DinoUiOptionMenu *owner, u32 textId);
    virtual ~DinoUiOptionBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOptionOkButton : public DinoUiOptionMenuItem {
public:
    DinoUiOptionOkButton(DinoUiOptionMenu *owner, u32 textId);
    virtual ~DinoUiOptionOkButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOptionMenu;
class DinoBlower2;
class MTextPicture;

class DinoUiOptionPower : public MTask, public MSleepHook {
public:
    DinoUiOptionPower(DinoUiOptionMenu *menu);
    virtual ~DinoUiOptionPower();
    virtual void Execute();
    virtual void OnSleep();
    virtual void OnWake();

    DinoUiOptionMenu *mMenu;    // 0x18
};

class DinoUiOptionMenu : public DinoHelpBase, public MLooperPreLoopHook, public MMenu {
public:
    DinoUiOptionMenu();
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
    void SetSpeed(u8 speed);
    void Sleep();
    BOOL IsWaiting();
    u8 GetMicStrength();
    void CheckKeys();

    u8 unkBF[1];     // members start in MMenu's tail padding
    MSmartOam *mOam; // 0xC0
    MMenuItem *mTopItems[3];    // 0xC4: the row above the BGM volume row
    MMenuItem *mBgmItems[11];   // 0xD0: one per volume level
    MMenuItem *mSeItems[11];    // 0xFC
    MMenuItem *mUnk128;         // 0x128
    MMenuItem *mBackButton;     // 0x12C: B goes to it
    MicTestButton *mMicTest;    // 0x130: the item under the SE volume row
    MTextPicture *mUnk134;      // 0x134: the title
    DinoBlower2 *mBlower;       // 0x138: the microphone
    MTask *mUnk13C;             // 0x13C
    MMotion *mSpeedMarks[3];    // 0x140
    MMotion *mBgmMotion;        // 0x14C: shows the BGM volume
    MMotion *mSeMotion;         // 0x150
    u8 unk154[4];               // 0x154
    DinoHelpItem *mHelps[8];    // 0x158: speed items, top-screen areas, OK, back
    u8 mSpeed;                  // 0x178
    u8 mBgmVolume;              // 0x179
    u8 mSeVolume;               // 0x17A
    u8 mOldSpeed;       // 0x17B: the settings when the menu opened, put back on Back
    u8 mOldBgmVolume;   // 0x17C
    u8 mOldSeVolume;    // 0x17D
    u8 unk17E[2];
    int mDelay;      // 0x180: frames left before input is taken
    u8 mSilentFocus; // 0x184
};

#endif
