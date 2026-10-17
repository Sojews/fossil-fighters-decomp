// The fossil cleaning scene.
#ifndef GAME_DINO_CLEANING_H
#define GAME_DINO_CLEANING_H

#include "m/task.h"
#include "m/looper.h"
#include "m/sleep.h"
#include "game/help.h"

class MInput;
class DinoCommonLive;

class DinoCleaning : public DinoHelpBase, public MProcess, public MLooperPreLoopHook, public MSleepHook {
public:
    virtual ~DinoCleaning();
    virtual void Execute();
    virtual void Idle();
    virtual void OnSleep();
    virtual void OnWake();
    virtual void OnPreLoop();

    MInput *GetUnkDC();
    u32 GetUnkE0();
    u32 GetUnkD4();
    u32 GetUnkD8();
    DinoCommonLive *GetUnkE4();
    void SetUnk128(BOOL value);

    void (DinoCleaning::*mState)(); // 0x94
    u8 unk9C[0xD4 - 0x9C];
    u32 mUnkD4;                     // 0xD4
    u32 mUnkD8;                     // 0xD8
    MInput *mUnkDC;                 // 0xDC
    u32 mUnkE0;                     // 0xE0
    DinoCommonLive *mUnkE4;         // 0xE4
    u8 unkE8[0x128 - 0xE8];
    u8 mUnk128;                     // 0x128: a tool task runs instead of the scene
    u8 unk129[0x144 - 0x129];
    MTask *mUnk144;                 // 0x144
    u32 unk148;
    u32 mSleepTimer;                // 0x14C: frames to ignore input after waking
};

#endif
