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
#include "m/input.h"
#include "m/console.h"
#include "m/picture.h"

class DinoBattleViewTutorialWindow : public MProcess {
public:
    virtual ~DinoBattleViewTutorialWindow();
    virtual void Idle();

    MPictureFile mPicture;  // 0x74
    u32 unkA8;
    MConsole *mConsole;     // 0xAC
    u32 unkB0[2];
    MView *mCursor;         // 0xB8
};

class KeyWaitTask : public MProcess {
public:
    virtual ~KeyWaitTask();
    virtual void Idle();

    DinoBattleViewTutorialWindow *mWindow;  // 0x74
    MInputHub *mHub;                        // 0x78
};

class DinoBattleTutorialConsoleFilter : public MConsoleFilter {
public:
    virtual ~DinoBattleTutorialConsoleFilter();
};

class DinoBattleTutorial {
public:
    virtual ~DinoBattleTutorial();

    void SetUnk24(u32 value);

    u32 unk04[3];
    MArchivePtr mArchive;                       // 0x10
    MArchiveFile mFile;                         // 0x14
    u32 unk20;
    u32 mUnk24;                                 // 0x24
    DinoBattleTutorialConsoleFilter *mFilter;   // 0x28
};

#endif
