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
class DinoCleaningStatus;
class MTextPicture;

class DinoCleaning : public DinoHelpBase, public MProcess, public MLooperPreLoopHook, public MSleepHook {
public:
    DinoCleaning(u32 unk9C, void *unkA0, u32 unkA8, u32 unkAC, int mode, u32 unk140);
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
    void StateQuit();
    void State02160638();
    void State02160e78();
    void State02160f54();
    void State021610b0();
    void State02161164();
    void State02161260();
    void StateStopAnswered();
    void State02161d48();

    void (DinoCleaning::*mState)(); // 0x94
    u32 mUnk9C;                     // 0x9C
    void *mUnkA0;                   // 0xA0
    u32 unkA4;
    u32 mUnkA8;                     // 0xA8
    u32 mUnkAC;                     // 0xAC
    int mMode;                      // 0xB0
    u8 unkB4[0xD4 - 0xB4];
    u32 mUnkD4;                     // 0xD4
    DinoCleaningKasekiRock *mKasekiRock; // 0xD8
    MInput *mUnkDC;                 // 0xDC
    u32 mUnkE0;                     // 0xE0
    DinoCommonLive *mUnkE4;         // 0xE4
    DinoCleaningStatus *mStatus;    // 0xE8
    DinoCleaningToolMenu *mToolMenu;    // 0xEC
    u32 mTool;                      // 0xF0: the tool in use
    u32 mNextTool;                  // 0xF4: the tool to switch to
    DinoCleaningTool *mTools[5];    // 0xF8
    MTask *mUnk10C;                 // 0x10C
    u32 mUnk110;                    // 0x110: a count shown as text
    u32 mUnk114;                    // 0x114
    MTextPicture *mUnk118;          // 0x118: the count's text
    MTaskSet *mUnk11C;              // 0x11C
    u32 unk120;
    u32 mUnk124;                    // 0x124
    u8 mUnk128;                     // 0x128: a tool task runs instead of the scene
    u8 unk129[0x138 - 0x129];
    u32 mUnk138;                    // 0x138
    u32 unk13C;
    u32 mUnk140;                    // 0x140
    MTaskSet *mUnk144;              // 0x144: tasks run instead of the scene while mUnk128 is set
    u8 mStop;                       // 0x148: the answer to the stop confirmation
    u32 mSleepTimer;                // 0x14C: frames to ignore input after waking
};

#endif
