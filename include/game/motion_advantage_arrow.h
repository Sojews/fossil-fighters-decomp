#ifndef GAME_MOTION_ADVANTAGE_ARROW_H
#define GAME_MOTION_ADVANTAGE_ARROW_H

#include "m/menu_item.h"
#include "m/motion.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MotionAdvantageArrow : public MMotion {
public:
    MotionAdvantageArrow(const std::string &path, MSmartOam *oam);
    virtual ~MotionAdvantageArrow();
    virtual void Unk44(int arg);

    u8 mUnk7C;  // 0x7C
};

// The arrow between the players' fields showing who has the advantage.
class DinoBattleViewAdvantageArrow : public MProcess {
public:
    DinoBattleViewAdvantageArrow(u32 unk74, MSmartOam *oam);
    virtual ~DinoBattleViewAdvantageArrow();
    virtual void Idle();

    // Moves the arrow to point at the side (at once while suspended).
    void PointAt(int side);

    u32 mUnk74;                     // 0x74
    MSmartOam *mOam;                // 0x78
    MotionAdvantageArrow *mArrow;   // 0x7C
    int mUnk80;                     // 0x80
    MPoint mPoints[2];              // 0x84: per side
};

class MSmartOam;
class MMotion;

// The tutorial's pointing arrows and highlight rectangle.
class DinoBattleViewTutorialMotion : public MProcess {
public:
    DinoBattleViewTutorialMotion(MSmartOam *oam);
    virtual ~DinoBattleViewTutorialMotion();
    virtual void Idle();

    void ShowArrow(int index, int dir, MPoint pos);
    void SetRect(MRect rect);
    // Removes one arrow, or with -1 every arrow and the rectangle.
    void Hide(int index);

    MSmartOam *mOam;        // 0x74
    MMotion *mArrows[3];    // 0x78
    class DinoBattleViewTutorialRect *mRect;   // 0x84
};

// Fades the screens (by blend ratio).
class DinoBattleViewLCDFader : public MProcess {
public:
    DinoBattleViewLCDFader(u32 arg);
    virtual ~DinoBattleViewLCDFader();
    virtual void Idle();

    u32 mUnk74;             // 0x74
    MDisplay *mDisplays[2]; // 0x78: main, sub
    int mRatios[2];         // 0x80
};

class TutorialWindowCloseTask : public MTask {
public:
    TutorialWindowCloseTask(u32 window) { mUnk10 = window; }
    virtual ~TutorialWindowCloseTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
};

class DinoBattleView;

// Waits until both players' ability icons stop counting.
class WaitUpdateSupportEffectAnimeTask : public MTask {
public:
    WaitUpdateSupportEffectAnimeTask(DinoBattleView *view) { mView = view; }
    virtual ~WaitUpdateSupportEffectAnimeTask();
    virtual void Execute();

    DinoBattleView *mView;  // 0x10
};

// Four corner marks around a rectangle.
class DinoBattleViewTutorialRect : public MProcess {
public:
    DinoBattleViewTutorialRect(MSmartOam *oam, MRect rect);
    virtual ~DinoBattleViewTutorialRect();
    virtual void Idle();

    MRect mRect;                // 0x74
    MParallelTaskSet *mTasks;   // 0x84: the marks
};

class SetTutorialMotionArrorTask : public MTask {
public:
    SetTutorialMotionArrorTask(DinoBattleViewTutorialMotion *motion, int index, int dir, MPoint pos) {
        mMotion = motion;
        mIndex = index;
        mDir = dir;
        mPos = pos;
    }
    virtual ~SetTutorialMotionArrorTask();
    virtual void Execute();

    DinoBattleViewTutorialMotion *mMotion;  // 0x10
    int mIndex;                             // 0x14
    int mDir;                               // 0x18: 0 hides the arrow
    MPoint mPos;                            // 0x1C
};

class SetTutorialMotionRectTask : public MTask {
public:
    SetTutorialMotionRectTask(DinoBattleViewTutorialMotion *motion, MPoint topLeft, MPoint bottomRight) {
        mMotion = motion;
        mTopLeft = topLeft;
        mBottomRight = bottomRight;
    }
    virtual ~SetTutorialMotionRectTask();
    virtual void Execute();

    DinoBattleViewTutorialMotion *mMotion;  // 0x10
    MPoint mTopLeft;                        // 0x14
    MPoint mBottomRight;                    // 0x1C
};

#endif
