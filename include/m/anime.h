// Animations: tasks that step a value from one end to the other over a number of frames, optionally eased,
// and hand each step to Apply().
#ifndef M_ANIME_H
#define M_ANIME_H

#include "m/task.h"
#include "m/geometry.h"

class MAnime : public MTask {
public:
    MAnime() {
        mStarted = FALSE;
        mFinished = FALSE;
        mStepsPerFrame = FALSE;
        mSteps = -1;
        mDuration = -1;
        mLoops = 1;
        mEase = 0;
    }
    virtual ~MAnime() {}
    virtual void Execute();
    virtual void Apply(int step) = 0;

    void Reset();

    void SetDuration(u32 frames) {
        mDuration = frames;
        if (mStepsPerFrame) {
            if (frames == 0) {
                mSteps = 1;
            } else {
                mSteps = frames * 16;
            }
        }
    }

    u8 mStarted;        // 0x10
    u8 mFinished;       // 0x11: the last step has been applied; exits next frame
    u8 mStepsPerFrame;  // 0x12: mSteps is set from the duration (16 per frame) rather than the range
    int mLastStep;      // 0x14
    int mSteps;         // 0x18
    u32 mFrame;         // 0x1C
    u32 mDuration;      // 0x20: frames
    int mLoopsLeft;     // 0x24: -1: loop forever
    int mLoops;         // 0x28
    int mEase;          // 0x2C: >0 ease in, <0 ease out, beyond +-10 both; the size is the power
};

class MValueAnime : public MAnime {
public:
    MValueAnime(int from, int to);
    virtual void Apply(int step);

    int mValue;     // 0x30
    int mFrom;      // 0x34
    int mTo;        // 0x38
    int mDir;       // 0x3C
};

class MPointAnime : public MAnime {
public:
    MPointAnime(MPoint from, MPoint to);
    virtual void Apply(int step);

    MPoint mValue;  // 0x30
    MPoint mFrom;   // 0x38
    MPoint mTo;     // 0x40
    int mDeltaX;    // 0x48: per step, 16.16
    int mDeltaY;    // 0x4C
};

class MRectAnime : public MAnime {
public:
    MRectAnime(MRect from, MRect to);
    virtual void Apply(int step);

    MRect mValue;   // 0x30
    MRect mFrom;    // 0x40
    MRect mTo;      // 0x50
    int mDelta[4];  // 0x60: per step, 16.16
};

#endif
