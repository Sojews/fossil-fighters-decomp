// Blowing into the microphone (the cleaning blower): the samples are read every frame, and loud enough ones count.
#ifndef GAME_DINO_BLOWER_H
#define GAME_DINO_BLOWER_H

#include "m/task.h"
#include "m/looper.h"
#include "m/sleep.h"

#define DINO_BLOWER_SAMPLES 0x400

// Accumulates sample energy; blowing is detected each time it passes a threshold.
class DinoBlower : public MTask, public MLooperPreLoopHook, public MSleepHook {
public:
    DinoBlower();
    virtual ~DinoBlower();
    virtual void Execute();
    virtual void OnSleep();
    virtual void OnWake();
    virtual void OnPreLoop();

    u8 IsBlowing();

    u32 mUnk24;
    u8 mBufferSpace[DINO_BLOWER_SAMPLES + 32];  // 0x028
    s8 *mBuffer;            // 0x448: 32-byte aligned
    u32 mEnergy;            // 0x44C
    u8 mBlowing;            // 0x450
    u32 mSettleFrames;      // 0x454: samples are ignored for a while after waking
};

// Reports how hard the player is blowing (0 to 4) from the loudest sample.
class DinoBlower2 : public MTask, public MLooperPreLoopHook, public MSleepHook {
public:
    DinoBlower2();
    virtual ~DinoBlower2();
    virtual void Execute();
    virtual void OnSleep();
    virtual void OnWake();
    virtual void OnPreLoop();

    u32 GetStrength();

    u32 mUnk24;
    u8 mBufferSpace[DINO_BLOWER_SAMPLES + 32];  // 0x028
    s8 *mBuffer;            // 0x448
    u32 mStrength;          // 0x44C
    u32 mSettleFrames;      // 0x450
};

#endif
