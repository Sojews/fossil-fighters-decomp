#ifndef GAME_DINO_BATTLE_TUTORIAL_H
#define GAME_DINO_BATTLE_TUTORIAL_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"
#include "m/console_filter.h"
#include "m/archive.h"
#include "m/resource.h"
#include "m/input.h"
#include "m/console.h"
#include "m/picture.h"

class DinoBattleViewTutorialWindow : public MProcess {
public:
    DinoBattleViewTutorialWindow();
    virtual ~DinoBattleViewTutorialWindow();
    virtual void Idle();

    MPictureFile mPicture;  // 0x74
    MView *mFrame;          // 0xA8
    MConsole *mConsole;     // 0xAC
    u32 unkB0[2];
    MView *mCursor;         // 0xB8
};

class KeyWaitTask : public MProcess {
public:
    KeyWaitTask(DinoBattleViewTutorialWindow *window) : MProcess(NULL), mWindow(window) {
        mHub = new MSingleInputHub(1);
        mHub->EnableTouch();
        mParallel.AddTail(mHub);
    }
    virtual ~KeyWaitTask();
    virtual void Idle();

    DinoBattleViewTutorialWindow *mWindow;  // 0x74
    MInputHub *mHub;                        // 0x78
};

class MConsole;
class DinoCommonFont;

class DinoBattleTutorialConsoleFilter : public MConsoleFilter {
public:
    DinoBattleTutorialConsoleFilter(DinoCommonFont *font, DinoBattleViewTutorialWindow *window)
        : mFont(font), mWindow(window), mUnk10(1), mUnk11(0) {}
    virtual ~DinoBattleTutorialConsoleFilter();
    // Handles a '$' escape (W: quiet, s: sound, f: clear, wN: rate, nN: font size, cX: color); other characters
    // play the typing sounds and are printed.
    virtual BOOL Filter(MConsole *console, int c);

    DinoCommonFont *mFont;  // 0x04
    DinoBattleViewTutorialWindow *mWindow;  // 0x08
    u32 unk0C;
    u8 mUnk10;              // 0x10: the high typing sound
    u8 mUnk11;              // 0x11: the speed sound
};

struct DinoBattleTutorialData;

class DinoBattleTutorial {
public:
    // With id 0, no tutorial.
    DinoBattleTutorial(u32 id);
    virtual ~DinoBattleTutorial();

    void SetView(class DinoBattleView *view);
    void SetUnk24(u32 value);
    // A number a tutorial text shows (by kind: the creatures' life, points, attack costs, ...).
    u32 GetValue(u32 kind);

    u32 mId;                                    // 0x04
    u32 mUnk08;                                 // 0x08
    u8 mUnk0C;                                  // 0x0C
    u8 mUnk0D;                                  // 0x0D
    MResource<DinoBattleTutorialData> mData;    // 0x10
    class DinoBattleView *mView;                // 0x20
    u32 mUnk24;                                 // 0x24
    DinoHelpDisabler *mDisabler;                // 0x28: keeps the help off during the tutorial
};

#endif
