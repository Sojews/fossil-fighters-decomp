// The cleaning screen's talk window.
#ifndef GAME_DINO_CLEANING_TALK_WINDOW_H
#define GAME_DINO_CLEANING_TALK_WINDOW_H

#include "m/task.h"
#include "m/archive.h"
#include "m/motion.h"

class DinoCleaningTalkWindow : public MProcess {
public:
    virtual ~DinoCleaningTalkWindow();
    virtual void Idle();

    u32 mUnk74;             // 0x74
    MArchivePtr mArchive;   // 0x78
    MArchiveFile mFile7C;   // 0x7C
    MArchiveFile mFile88;   // 0x88
    MArchiveFile mFile94;   // 0x94
    MArchiveFile mFileA0;   // 0xA0
    MMotionFile mMotion;    // 0xAC
};

#endif
