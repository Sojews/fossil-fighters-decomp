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

class DinoBattleViewTutorialWindow : public MProcess {
public:
    virtual ~DinoBattleViewTutorialWindow();
    virtual void Idle();
};

class KeyWaitTask : public MProcess {
public:
    virtual ~KeyWaitTask();
    virtual void Idle();
};

class DinoBattleTutorialConsoleFilter : public MConsoleFilter {
public:
    virtual ~DinoBattleTutorialConsoleFilter();
};

class DinoBattleTutorial {
public:
    virtual ~DinoBattleTutorial();

    u32 unk04[3];
    MArchivePtr mArchive;                       // 0x10
    MArchiveFile mFile;                         // 0x14
    u32 unk20[2];
    DinoBattleTutorialConsoleFilter *mFilter;   // 0x28
};

#endif
