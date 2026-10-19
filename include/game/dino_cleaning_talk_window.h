// The cleaning screen's talk window.
#ifndef GAME_DINO_CLEANING_TALK_WINDOW_H
#define GAME_DINO_CLEANING_TALK_WINDOW_H

#include "m/picture.h"

#include "m/task.h"
#include "m/fx32.h"
#include "m/archive.h"
#include "m/motion.h"

class MConsole;

class DinoCleaningTalkWindow : public MProcess {
public:
    void SetUnkD0(u32 value);
    void SetUnkCC(u32 value);
    void SetRate(MFx32 rate);
    void Reset();
    DinoCleaningTalkWindow(u32 arg);
    virtual ~DinoCleaningTalkWindow();
    virtual void Idle();

    u32 mUnk74;             // 0x74
    MPictureFile mPicture;  // 0x78: the window
    MMotionFile mMotion;    // 0xAC
    u32 unkB0;
    MSerialTaskSet *mTasksB4;   // 0xB4
    MParallelTaskSet *mTasksB8; // 0xB8
    u32 unkBC[3];
    MConsole *mConsole;     // 0xC8
    u32 mUnkCC;             // 0xCC
    u32 mUnkD0;             // 0xD0
    u32 unkD4;
    MFx32 mRate;            // 0xD8
};

#endif
