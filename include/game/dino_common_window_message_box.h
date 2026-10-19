// Message windows: a box, and two that wait for the player.
#ifndef GAME_DINO_COMMON_WINDOW_MESSAGE_BOX_H
#define GAME_DINO_COMMON_WINDOW_MESSAGE_BOX_H

#include "m/geometry.h"
#include <string>
#include "m/task.h"

class MSingleInputHub;

class DinoCommonWindowMessageBox : public MProcess {
public:
    DinoCommonWindowMessageBox(MPoint pos, const std::string &text, int frames);
    DinoCommonWindowMessageBox(MPoint pos, const std::string &text, u32 unk78, int frames);
    DinoCommonWindowMessageBox(MDisplay *display, MPoint pos, const std::string &text, int frames);
    virtual ~DinoCommonWindowMessageBox();
    virtual void Idle();

    MSingleInputHub *mInput;    // 0x74
    u32 mUnk78;         // 0x78: frames until it closes by itself (0: never)
    MPoint mPos;        // 0x7C
    std::string mText;  // 0x84
    int mFrames;        // 0x90
    u32 mUnk94;         // 0x94
    u8 mUnk98;          // 0x98
};

class DinoCommonWindowMessageWait1 : public MProcess {
public:
    DinoCommonWindowMessageWait1(std::string text, MPoint pos, int arg, u8 flag);
    virtual ~DinoCommonWindowMessageWait1();
    virtual void Idle();

    std::string mText;  // 0x74
    u32 mUnk80;         // 0x80
    MPoint mPos;        // 0x84
    int mUnk8C;         // 0x8C
    u8 mUnk90;          // 0x90
};

class DinoCommonWindowMessageWait2 : public MProcess {
public:
    DinoCommonWindowMessageWait2(std::string text, MPoint pos, int arg, u8 flag);
    virtual ~DinoCommonWindowMessageWait2();
    virtual void Idle();

    std::string mText;  // 0x74
    u32 mUnk80;         // 0x80
    MPoint mPos;        // 0x84
    int mUnk8C;         // 0x8C
    u8 mUnk90;          // 0x90
};

#endif
