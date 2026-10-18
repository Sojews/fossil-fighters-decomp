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
    void Start();
    void Stop();
    void Pause();
    int GetSeconds();
    int GetRemainingFrames();
    virtual ~DinoCleaningTimer();
    virtual void Idle();

    u32 mUnk74[3];          // 0x74
    int mFrames;            // 0x80
    int mLimit;             // 0x84: frames allowed
    u32 mUnk88;             // 0x88
    MArchivePtr mArchive;   // 0x8C
    MArchiveFile mFile;     // 0x90
};

#endif
