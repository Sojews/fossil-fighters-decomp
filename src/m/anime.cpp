// Animations.

#include <algorithm>
#include <stdlib.h>
#include "m/anime.h"

// base * (base / denom)^n
static inline int EasePow(int base, u32 n, int denom) {
    int v = base;

    for (u32 i = 0; i < n; i++) {
        v = v * base / denom;
    }
    return v;
}

void MAnime::Reset() {
    mStarted = TRUE;
    mFrame = 0;
    mLastStep = -1;
    mLoopsLeft = mLoops;
}

void MAnime::Execute() {
    int last;
    int step;
    int max;
    int count;
    int ease;

    if (!mStarted) {
        Reset();
    }
    if (mFinished) {
        mFinished = FALSE;
        Exit();
        return;
    }
    if (++mFrame >= mDuration) {
        if (mLoopsLeft == -1 || --mLoopsLeft != 0) {
            mFrame = 0;
            mLastStep = -1;
            Apply(0);
            return;
        }
        mFinished = TRUE;
    }
    last = mLastStep;
    count = mSteps;
    max = count - 1;
    step = (int)(mFrame * max) / (int)mDuration;
    ease = mEase;
    if (ease != 0) {
        if (ease > 10) {
            u32 half = (u32)count >> 1;
            if ((u32)step < half) {
                step = EasePow(step, ease - 10, half - 1);
            } else {
                int rest = max - half;
                int v = EasePow(rest - (step - (int)half), ease - 10, rest);
                step = half + (rest - v);
            }
        } else if (ease < -10) {
            u32 half = (u32)count >> 1;
            if ((u32)step < half) {
                int end = half - 1;
                int v = EasePow(end - step, -(ease + 10), end);
                step = (half - 1) - v;
            } else {
                int v = EasePow(step - (int)half, -(ease + 10), max - half);
                step = half + v;
            }
        } else if (ease > 0) {
            step = EasePow(step, ease, max);
        } else {
            step = max - EasePow(max - step, -ease, max);
        }
    }
    mLastStep = step;
    if (step != last) {
        Apply(step);
    }
}

MValueAnime::MValueAnime(int from, int to) {
    mFrom = from;
    mTo = to;
    mSteps = abs(from - to) + 1;
    mDir = from < to ? 1 : -1;
}

void MValueAnime::Apply(int step) {
    mValue = mFrom + step * mDir;
}

MPointAnime::MPointAnime(MPoint from, MPoint to) {
    mFrom = from;
    mTo = to;
    int dy = abs(from.y - to.y);
    int dx = abs(from.x - to.x);
    mSteps = std::max(dx, dy) + 1;
    mDeltaX = ((to.x - from.x) << 16) / (mSteps - 1);
    mDeltaY = ((to.y - from.y) << 16) / (mSteps - 1);
}

void MPointAnime::Apply(int step) {
    if (step != mSteps - 1) {
        mValue = MPoint(mFrom.x + ((mDeltaX * step) >> 16), mFrom.y + ((mDeltaY * step) >> 16));
    } else {
        mValue.x = mTo.x;
        mValue.y = mTo.y;
    }
}

void MRectAnime::Apply(int step) {
    if (step != mSteps - 1) {
        mValue = MRect(mFrom.left + ((mDelta[0] * step) >> 16), mFrom.top + ((mDelta[1] * step) >> 16),
                       mFrom.right + ((mDelta[2] * step) >> 16), mFrom.bottom + ((mDelta[3] * step) >> 16));
    } else {
        mValue.left = mTo.left;
        mValue.top = mTo.top;
        mValue.right = mTo.right;
        mValue.bottom = mTo.bottom;
    }
}
