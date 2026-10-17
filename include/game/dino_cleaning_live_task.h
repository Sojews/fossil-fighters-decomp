#ifndef GAME_DINO_CLEANING_LIVE_TASK_H
#define GAME_DINO_CLEANING_LIVE_TASK_H

#include "game/dino_common_live.h"

class DinoCleaningTalkWindow;

class DinoCleaningLive : public DinoCommonLive {
public:
    DinoCleaningLive(u32 arg, const std::string &name);
    virtual ~DinoCleaningLive();

    void ShowTutorial(MDisplay *display, const char *text);
    void Unk021580cc();
    void Unk021580d8();

    u32 mUnkD4;                     // 0xD4
    DinoCleaningTalkWindow *mTalk;  // 0xD8
};

#endif
