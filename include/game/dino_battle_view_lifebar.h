#ifndef GAME_DINO_BATTLE_VIEW_LIFEBAR_H
#define GAME_DINO_BATTLE_VIEW_LIFEBAR_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewLifebar : public MProcess {
public:
    virtual ~DinoBattleViewLifebar();
    virtual void Idle();
};

class MMotion;
class MSmartOam;

// A motion that can be swapped for another while keeping its position and visibility.
class LifeMotion : public MProcess {
public:
    LifeMotion(const char *path, MSmartOam *oam, u8 flip, MPoint pos);
    virtual ~LifeMotion();
    virtual void Idle();

    void SetMotion(const char *path);
    void Play(int anime);
    void SetPosition(MPoint pos);
    MPoint GetPosition();
    void Show();
    BOOL IsHidden();
    void Hide();
    MTask *CreateShowTask();
    MTask *CreateHideTask();
    MTask *CreateCoordTask(MPoint pos);
    MTask *CreateCoordAnime(MPoint from, MPoint to, u32 duration, int ease);
    void SetAnchor(int anchor);

    MMotion *mMotion;   // 0x74
    MSmartOam *mOam;    // 0x78
    MPoint mPos;        // 0x7C
    const char *mPath;  // 0x84
    u8 mFlip;           // 0x88
};

class LifebarCoordTask : public MTask {
public:
    LifebarCoordTask(LifeMotion *lifebar, MPoint pos) {
        mPos = pos;
        mLifebar = lifebar;
    }
    virtual ~LifebarCoordTask();
    virtual void Execute();

    MPoint mPos;            // 0x10
    LifeMotion *mLifebar;   // 0x18
};

class LifebarShowTask : public MTask {
public:
    LifebarShowTask(LifeMotion *lifebar, u8 show) {
        mShow = show;
        mLifebar = lifebar;
    }
    virtual ~LifebarShowTask();
    virtual void Execute();

    u8 mShow;               // 0x10
    LifeMotion *mLifebar;   // 0x14
};

class LifebarCoordAnime : public MPointAnime {
public:
    LifebarCoordAnime(LifeMotion *lifebar, MPoint from, MPoint to, u32 duration, int ease) : MPointAnime(from, to) {
        mLifebar = lifebar;
        mEase = ease;
        SetDuration(duration);
    }
    virtual ~LifebarCoordAnime();
    virtual void Apply(int step);

    LifeMotion *mLifebar;   // 0x50
};

#endif
