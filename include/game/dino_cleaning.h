// The fossil cleaning scene.
#ifndef GAME_DINO_CLEANING_H
#define GAME_DINO_CLEANING_H

#include "m/task.h"
#include "m/looper.h"
#include "m/sleep.h"
#include "game/help.h"

class MInput;
class DinoCommonLive;
class DinoCleaningKasekiRock;
class DinoCleaningTool;
class DinoCleaningToolMenu;

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
    DinoCleaningKasekiRock *GetKasekiRock();
    DinoCommonLive *GetUnkE4();
    void SetUnk128(BOOL value);

    void StateEnableToolMenu();
    void StateStopTool();
    void StateSwitchTool();
    void StateCheckDone();
    void StateSleep();
    void StateWake();
    void StateStopConfirm();

    void (DinoCleaning::*mState)(); // 0x94
    u8 unk9C[0xD4 - 0x9C];
    u32 mUnkD4;                     // 0xD4
    DinoCleaningKasekiRock *mKasekiRock; // 0xD8
    MInput *mUnkDC;                 // 0xDC
    u32 mUnkE0;                     // 0xE0
    DinoCommonLive *mUnkE4;         // 0xE4
    u32 unkE8;
    DinoCleaningToolMenu *mToolMenu;    // 0xEC
    u32 mTool;                      // 0xF0: the tool in use
    u32 mNextTool;                  // 0xF4: the tool to switch to
    DinoCleaningTool *mTools[11];   // 0xF8
    u32 mUnk124;                    // 0x124
    u8 mUnk128;                     // 0x128: a tool task runs instead of the scene
    u8 unk129[0x144 - 0x129];
    MTaskSet *mUnk144;              // 0x144: tasks run instead of the scene while mUnk128 is set
    u8 mStop;                       // 0x148: the answer to the stop confirmation
    u32 mSleepTimer;                // 0x14C: frames to ignore input after waking
};

#endif
