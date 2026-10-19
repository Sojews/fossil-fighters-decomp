// BattleSelect.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/battle_select.h"
#include "types.h"
#include "m/resource.h"
#include "game/dino_battle_view2.h"
#include "game/dino_battle_view_3d.h"

#pragma auto_inline off

BOOL DinoBattleViewAbilityIcon::IsAnimating() {
    for (int i = 0; i < 4; i++) {
        if ((mMask & (1 << i)) && mValue[i] != mTarget[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// NONMATCHING: the game keeps the position on the stack below the offset point; ours swaps the two slots.
MRect DinoBattleViewAbilityIcon::GetRect() {
    MPoint pos = mMotion->GetPosition();
    MPoint p(pos.x - mOam->mOffset.x, pos.y - mOam->mOffset.y);
    MRect r(-20, -20, 18, 18);
    r.Offset(p);
    return r;
}

BOOL DinoBattleViewAbilityIcon::IsHidden() {
    return mMotion->IsHidden();
}

#pragma auto_inline on

DebugAutoBattle::~DebugAutoBattle() {
    delete mRandom;
}

void DebugAutoBattle::Idle() {
    Exit();
}

BattleSelect::~BattleSelect() {
}

UseCreatureSelect::~UseCreatureSelect() {
}

PlayerLevelSelect::~PlayerLevelSelect() {
}

IncValTask::~IncValTask() {
}

TimeCountTask::~TimeCountTask() {
}

void TimeCountTask::Idle() {
}

void UseCreatureSelect::Idle() {
    (this->*mState)();
}

void PlayerLevelSelect::Idle() {
    (this->*mState)();
}

void IncValTask::Execute() {
    (*mValue)++;
}

// A resource the debug battle select loads (its data type is unknown).
struct DinoBattleSelectData;
// (Not decompiled yet: the select screen's setup constructs it; this keeps an out-of-line copy of the constructor.)
#pragma dont_inline on
MResource<DinoBattleSelectData> DinoBattleSelectLoadData(std::string path) {
    return MResource<DinoBattleSelectData>(path);
}
#pragma dont_inline reset

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x38];
    void (UseCreatureSelect::*Start_38)();
    u8 unk40[0x48 - 0x40];
    void (PlayerLevelSelect::*Start_48)();
} data_ov000_02195e50;

UseCreatureSelect::UseCreatureSelect(void *params, u32 unk7C, u32 unk80, u8 *cancelled)
    : MProcess(NULL), mUnk7C(unk7C), mUnk80(unk80), mUnk88(0), mParams(params), mCancelled(cancelled) {
    *mCancelled = FALSE;
    mState = data_ov000_02195e50.Start_38;
}

PlayerLevelSelect::PlayerLevelSelect(u32 *level, BOOL unk7C, u32 unk80)
    : MProcess(NULL), mUnk7C(unk7C), mUnk80(unk80), mUnk9C(0), mUnkA8(0), mUnkAC(1), mLevel(level) {
    mState = data_ov000_02195e50.Start_48;
}

#pragma auto_inline off
// Vector members the game emits in this file (made explicitly until the code that uses them is decompiled).
template void std::__vec_deleter<DinoBattleView3DRecord, std::allocator<DinoBattleView3DRecord> >::sub_push_back(const DinoBattleView3DRecord &x, Metrowerks::int2type<true>);
template std::__vec_deleter<DinoBattleView3DRecord, std::allocator<DinoBattleView3DRecord> >::~__vec_deleter();
template void std::__vec_deleter<DinoBattleView3DEntry, std::allocator<DinoBattleView3DEntry> >::append(size_t n, Metrowerks::int2type<true>);
template std::__vec_deleter<DinoBattleView3DEntry, std::allocator<DinoBattleView3DEntry> >::~__vec_deleter();

// A debug switch, always off.
extern "C" BOOL func_ov000_0218472c(int flag) {
    return FALSE;
}
