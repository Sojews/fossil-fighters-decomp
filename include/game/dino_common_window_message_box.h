// Message windows: a box, and two that wait for the player.
#ifndef GAME_DINO_COMMON_WINDOW_MESSAGE_BOX_H
#define GAME_DINO_COMMON_WINDOW_MESSAGE_BOX_H

#include <string>
#include "m/task.h"

class DinoCommonWindowMessageBox : public MProcess {
public:
    virtual ~DinoCommonWindowMessageBox();
    virtual void Idle();

    u32 mUnk74[4];      // 0x74
    std::wstring mText; // 0x84
};

class DinoCommonWindowMessageWait1 : public MProcess {
public:
    virtual ~DinoCommonWindowMessageWait1();
    virtual void Idle();

    std::wstring mText; // 0x74
};

class DinoCommonWindowMessageWait2 : public MProcess {
public:
    virtual ~DinoCommonWindowMessageWait2();
    virtual void Idle();

    std::wstring mText; // 0x74
};

#endif
