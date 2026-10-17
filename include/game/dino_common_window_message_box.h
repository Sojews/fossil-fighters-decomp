// Message windows: a box, and two that wait for the player.
#ifndef GAME_DINO_COMMON_WINDOW_MESSAGE_BOX_H
#define GAME_DINO_COMMON_WINDOW_MESSAGE_BOX_H

#include "m/geometry.h"
#include <string>
#include "m/task.h"

class DinoCommonWindowMessageBox : public MProcess {
public:
    DinoCommonWindowMessageBox(MPoint pos, const std::string &text, int frames);
    virtual ~DinoCommonWindowMessageBox();
    virtual void Idle();

    u32 mUnk74;         // 0x74
    u32 mUnk78;         // 0x78
    MPoint mPos;        // 0x7C
    std::string mText;  // 0x84
    int mFrames;        // 0x90
    u32 mUnk94;         // 0x94
    u8 mUnk98;          // 0x98
};

class DinoCommonWindowMessageWait1 : public MProcess {
public:
    virtual ~DinoCommonWindowMessageWait1();
    virtual void Idle();

    std::string mText;  // 0x74
};

class DinoCommonWindowMessageWait2 : public MProcess {
public:
    virtual ~DinoCommonWindowMessageWait2();
    virtual void Idle();

    std::string mText;  // 0x74
};

#endif
