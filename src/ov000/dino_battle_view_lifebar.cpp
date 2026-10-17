// DinoBattleViewLifebar.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_lifebar.h"
#include "types.h"
#include "m/motion.h"

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

#pragma auto_inline off

LifeMotion::LifeMotion(const char *path, MSmartOam *oam, u8 flip, MPoint pos) : MProcess(NULL) {
    mMotion = NULL;
    mOam = oam;
    mPos = pos;
    mPath = NULL;
    mFlip = flip;
    SetMotion(path);
}

// NONMATCHING: `shown` and the new-expression's constructed flag get r6/r7 swapped
void LifeMotion::SetMotion(const char *path) {
    if (mPath == path) {
        return;
    }
    mPath = path;
    MPoint pos = mPos;
    BOOL shown = FALSE;
    if (mMotion != NULL) {
        pos = mMotion->GetPosition();
        if (!mMotion->IsHidden()) {
            shown = TRUE;
        }
        mParallel.Remove(mMotion);
        delete mMotion;
    }
    mMotion = new MMotion(std::string(path), mOam, 2, 1);
    mMotion->SetOverride(0);
    mMotion->SetPriority(2);
    mMotion->SetAnchor(mFlip ? 2 : 1);
    SetPosition(pos);
    if (shown) {
        Show();
    } else {
        Hide();
    }
    mParallel.AddTail(mMotion);
}

void LifeMotion::Play(int anime) {
    mMotion->Play(anime, 0);
}

void LifeMotion::SetPosition(MPoint pos) {
    mMotion->SetPosition(pos);
}

MPoint LifeMotion::GetPosition() {
    return mMotion->GetPosition();
}

void LifeMotion::Show() {
    mMotion->Show();
}

BOOL LifeMotion::IsHidden() {
    return mMotion->IsHidden();
}

void LifeMotion::Hide() {
    mMotion->Hide();
}

void LifeMotion::Idle() {
}

MTask *LifeMotion::CreateShowTask() {
    return new LifebarShowTask(this, TRUE);
}

MTask *LifeMotion::CreateHideTask() {
    return new LifebarShowTask(this, FALSE);
}

MTask *LifeMotion::CreateCoordTask(MPoint pos) {
    return new LifebarCoordTask(this, pos);
}

MTask *LifeMotion::CreateCoordAnime(MPoint from, MPoint to, u32 duration, int ease) {
    return new LifebarCoordAnime(this, from, to, duration, ease);
}

void LifeMotion::SetAnchor(int anchor) {
    mMotion->SetAnchor(anchor);
}

#pragma auto_inline on

void LifebarCoordTask::Execute() {
    mLifebar->SetPosition(mPos);
    Exit();
}

void LifebarShowTask::Execute() {
    if (mShow) {
        mLifebar->Show();
    } else {
        mLifebar->Hide();
    }
    Exit();
}

void LifebarCoordAnime::Apply(int step) {
    MPointAnime::Apply(step);
    MPoint pos = mValue;
    mLifebar->SetPosition(pos);
}
