#ifndef GAME_DINO_CLEANING_TIMER_H
#define GAME_DINO_CLEANING_TIMER_H

#include "m/archive.h"
#include "m/raster.h"
#include "m/motion.h"
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
    BOOL IsAtStart();
    int GetSeconds();
    int GetRemainingFrames();
    virtual ~DinoCleaningTimer();
    virtual void Idle();

    void StateCountdown();
    void StateIdle();
    void StateExtend();

    void (DinoCleaningTimer::*mState)(); // 0x74
    class DinoCleaning *mOwner; // 0x7C
    int mFrames;            // 0x80
    int mLimit;             // 0x84: frames allowed
    MRasterIO *mRaster;     // 0x88: the timer bar
    MArchivePtr mArchive;   // 0x8C
    MArchiveFile mFile;     // 0x90: the bar's frames
    MMotion *mDigits[2];    // 0x9C: elapsed seconds, ones and tens
};

#endif
