#ifndef GAME_DINO_CLEANING_TIMER_H
#define GAME_DINO_CLEANING_TIMER_H

#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoCleaningTimer : public MProcess {
public:
    virtual ~DinoCleaningTimer();
    virtual void Idle();

    u32 mUnk74[6];          // 0x74
    MArchivePtr mArchive;   // 0x8C
    MArchiveFile mFile;     // 0x90
};

#endif
