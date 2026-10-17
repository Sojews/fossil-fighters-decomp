// DinoBattleViewLifebar.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_lifebar.h"
#include "types.h"
#include "m/motion.h"
#include "game/dino_battle_view_creature.h"

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

// Pooled file statics.
struct LifebarStatics {
    u8 unk00[0x38];
    const char *barPath;    // 0x38
};
extern "C" LifebarStatics data_ov000_02194270;
// Frame motions by creature type: shrinking, growing, settled.
extern "C" const char *const data_ov000_02194340[][3];
extern "C" const char *const data_ov000_02194344[][3];
extern "C" const char *const data_ov000_02194348[][3];

DinoBattleViewLifebar::DinoBattleViewLifebar(DinoBattleViewCreature *creature, MSmartOam *oam, MPoint pos, int life, int maxLife)
    : MProcess(NULL) {
    mOam = oam;
    mCreature = creature;
    mShownLevel = 1;
    mFrame = new LifeMotion(data_ov000_02194344[mCreature->GetDefUnk0D()][0], mOam, FALSE, pos);
    mFrame->Show();
    mFrame->Play(1);
    mFrame->SetAnchor(2);
    mBar = new LifeMotion(data_ov000_02194270.barPath, mOam, TRUE, MPoint(pos.x, pos.y));
    mBar->Show();
    mBar->Play(1);
    mBar->SetAnchor(1);
    SetLife(life, maxLife);
    MTask *bar = mBar;
    MTaskSet *parallel = &mParallel;
    parallel->AddTail(mFrame);
    parallel->AddTail(bar);
}

void DinoBattleViewLifebar::SetLife(int life, int maxLife) {
    mTimer = 3;
    mLife = life;
    mMaxLife = maxLife;
    if (maxLife == 0) {
        mBar->Play(1);
        mLevel = -1;
        return;
    }
    mLevel = mLife * 40 / mMaxLife + 1;
    if (life > 0 && mLevel == 1) {
        mLevel = 2;
    }
}

void DinoBattleViewLifebar::Idle() {
    if (mFrame->IsHidden()) {
        return;
    }
    if (mShownLevel != mLevel) {
        mTimer--;
        if (mTimer <= 0) {
            mTimer = 3;
            if (mShownLevel >= mLevel) {
                mShownLevel--;
            } else {
                mShownLevel++;
            }
            if (mShownLevel == mLevel) {
                mBar->Hide();
                mBar->Play(mLevel);
                mFrame->SetMotion(data_ov000_02194348[mCreature->GetDefUnk0D()][0]);
                mFrame->Play(mLevel);
            } else if (mLevel > mShownLevel) {
                mBar->Show();
                mBar->Play(mShownLevel);
                mFrame->SetMotion(data_ov000_02194344[mCreature->GetDefUnk0D()][0]);
                mFrame->Play(mLevel);
            } else {
                mBar->Show();
                mBar->Play(mLevel);
                mFrame->SetMotion(data_ov000_02194340[mCreature->GetDefUnk0D()][0]);
                mFrame->Play(mShownLevel);
            }
        }
    }
}

void DinoBattleViewLifebar::Hide() {
    mFrame->Hide();
    mBar->Hide();
}

MTask *DinoBattleViewLifebar::CreateShowTask() {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    MTask *bar = mBar->CreateShowTask();
    set->AddTail(mFrame->CreateShowTask());
    set->AddTail(bar);
    return set;
}

MTask *DinoBattleViewLifebar::CreateHideTask() {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    MTask *bar = mBar->CreateHideTask();
    set->AddTail(mFrame->CreateHideTask());
    set->AddTail(bar);
    return set;
}

MTask *DinoBattleViewLifebar::CreateCoordTask(MPoint pos) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    MTask *bar = mBar->CreateCoordTask(pos);
    set->AddTail(mFrame->CreateCoordTask(pos));
    set->AddTail(bar);
    return set;
}

MTask *DinoBattleViewLifebar::CreateCoordAnime(MPoint from, MPoint to, u32 duration, int ease) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    MTask *bar = mBar->CreateCoordAnime(from, to, duration, ease);
    set->AddTail(mFrame->CreateCoordAnime(from, to, duration, ease));
    set->AddTail(bar);
    return set;
}

MPoint DinoBattleViewLifebar::GetPosition() {
    return mBar->GetPosition();
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
