// DinoBattleViewLifebar.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_lifebar.h"
#include "types.h"

DinoBattleViewLifebar::~DinoBattleViewLifebar() {
}

LifeMotion::~LifeMotion() {
}

LifebarCoordTask::~LifebarCoordTask() {
}

LifebarShowTask::~LifebarShowTask() {
}

LifebarCoordAnime::~LifebarCoordAnime() {
}

void LifeMotion::Idle() {
}

extern "C" void func_ov000_02179334(void *lifebar, MPoint pos);

void LifebarCoordTask::Execute() {
    func_ov000_02179334(mLifebar, mPos);
    Exit();
}

extern "C" void func_ov000_02179358(void *lifebar);
extern "C" void func_ov000_02179370(void *lifebar);

void LifebarShowTask::Execute() {
    if (mShow) {
        func_ov000_02179358(mLifebar);
    } else {
        func_ov000_02179370(mLifebar);
    }
    Exit();
}

void LifebarCoordAnime::Apply(int step) {
    MPointAnime::Apply(step);
    MPoint pos = mValue;
    func_ov000_02179334(mLifebar, pos);
}
