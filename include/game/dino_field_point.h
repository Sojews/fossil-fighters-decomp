#ifndef GAME_DINO_FIELD_POINT_H
#define GAME_DINO_FIELD_POINT_H

#include "m/geometry.h"
#include <string>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MMotion;
class MSmartOam;

class DinoFieldPoint : public MProcess {
public:
    DinoFieldPoint(const std::string &path, int digits, u32 a, u32 b, u32 c);
    virtual ~DinoFieldPoint();
    virtual void Idle();
    virtual void Format(char *buf) = 0;  // the shown value
    MTask *CreateShowTask();
    MTask *CreateHideTask();
    // Where the digit's sprite goes.
    static MPoint GetDigitPos(u32 index);

    std::string mText;          // 0x74: the frame's motion
    u32 mDigits;                // 0x80
    u32 mShowSe;                // 0x84: (-1: none)
    u32 mChangeSe;              // 0x88
    u32 mHideSe;                // 0x8C
    char mChars[0xC];           // 0x90: the shown value, one digit or a space each
    MTaskSet *mTasks;           // 0x9C
    MMotion *mFrame;            // 0xA0
    MMotion *mDigitMotions[8];  // 0xA4: NULL for spaces
    MSmartOam *mOam;            // 0xC4
};

class DinoFieldMoney : public DinoFieldPoint {
public:
    DinoFieldMoney();
    virtual ~DinoFieldMoney();
    virtual void Format(char *buf);
};

class DinoFieldDonate : public DinoFieldPoint {
public:
    DinoFieldDonate();
    virtual ~DinoFieldDonate();
    virtual void Format(char *buf);
};

#endif
