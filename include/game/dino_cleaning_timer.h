#ifndef GAME_DINO_CLEANING_TIMER_H
#define GAME_DINO_CLEANING_TIMER_H

#include "m/archive.h"
#include "m/resource.h"
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

class DinoCleaning;
class MSmartOam;

class DinoCleaningTimer : public MProcess {
public:
    DinoCleaningTimer(DinoCleaning *cleaning, u32 planes, MSmartOam *oam, u32 frames);
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
    MRasterWindow *mRaster; // 0x88: the timer bar
    MResource<struct DinoMessageData> mBar; // 0x8C: the bar's frames
    MMotion *mDigits[2];    // 0x9C: elapsed seconds, ones and tens
};

#endif
