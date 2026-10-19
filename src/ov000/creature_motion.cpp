// CreatureMotion.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on
#include "m/display.h"
#include "m/bright.h"
#include "m/lcd.h"

#include "game/creature_motion.h"
#include "game/dino_resource.h"
#include "m/fx32.h"
#include "types.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_message.h"
#include "game/dino_battle_view_class.h"
#include "m/pal.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/view_status_effect_icon.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/geometry.h"
#include "m/motion.h"
#include "game/dino_battle_view_lifebar.h"
#include "game/dino_battle_view_effect_game_begin.h"

CreatureMotion::~CreatureMotion() {
}

NopTask::~NopTask() {
}

CreateCreatureMotionTask::~CreateCreatureMotionTask() {
}

EffectMotionTask::~EffectMotionTask() {
}

SetCretureIconTask::~SetCretureIconTask() {
}

SetCretureIconPriorityTask::~SetCretureIconPriorityTask() {
}

UpdateLifeBarTask::~UpdateLifeBarTask() {
}

UpdateLifePointTask::~UpdateLifePointTask() {
}

SetColorMaskTask::~SetColorMaskTask() {
}

ShuffleMoveTask::~ShuffleMoveTask() {
}

void EffectMotionTask::Idle() {
}

void NopTask::Execute() {
}

void DinoBattleViewCreature::SetCoordTask::Execute() {
    mCreature->SetCoord(mPos);
    mCreature->mMotion->SetPosition(mPos);
    Exit();
}

DinoBattleViewCreature::SetCoordTask::~SetCoordTask() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MCallTask0<DinoBattleViewCreature, DinoBattleViewCreature>;
template MTask *MCreateCallTask(DinoBattleViewCreature *obj, void (DinoBattleViewCreature::*func)());
template class MCallTask1<DinoBattleViewStatusEffectIcon, DinoBattleViewStatusEffectIcon, MPoint>;
template MTask *MCreateCallTask(DinoBattleViewStatusEffectIcon *obj, void (DinoBattleViewStatusEffectIcon::*func)(MPoint), MPoint a0);
#pragma auto_inline reset

// The frame around a creature's position (main's data).
extern "C" const MRect data_020b5aa0;

MRect CreatureMotion::GetFrame() {
    return data_020b5aa0.Moved(GetPosition());
}

void UpdateLifeBarTask::Execute() {
    mLifebar->SetLife(mLife, mMaxLife);
    Exit();
}

DinoBattleViewEffectGameBeginPlayerNamePanel::~DinoBattleViewEffectGameBeginPlayerNamePanel() {
    mFont->Release();
    delete mUnk80;
    delete mUnk84;
}

DinoBattleViewEffectGameBegin::~DinoBattleViewEffectGameBegin() {
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x38];
    void (DinoBattleViewEffectGameBegin::*State_38)();
    u8 unk40[0x48 - 0x40];
    void (DinoBattleViewEffectGameBegin::*Start_48)();
} data_ov000_02193668;

DinoBattleViewEffectGameBegin::DinoBattleViewEffectGameBegin(u32 unk7C, std::string name, std::string name2)
    : MProcess(NULL), mUnk7C(unk7C) {
    mNames[0] = name;
    mNames[1] = name2;
    mState = data_ov000_02193668.Start_48;
}

void DinoBattleViewEffectGameBegin::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.

MRect DinoBattleViewEffectGameBeginPlayerNamePanel::GetFrame() {
    MRect r = MMotion::GetFrame();
    r.right = r.left + 0x88;
    return r;
}

extern "C" void func_ov000_021728d4(DinoBattleViewCreature *view, int arg);
extern "C" void func_ov000_0216fd74(DinoBattleViewCreature *view);

void DinoBattleViewCreature::Idle() {
    MMessageObserver::Dispatch();
    func_ov000_021728d4(this, 0);
    func_ov000_0216fd74(this);
}

MPoint DinoBattleViewEffectGameBeginPlayerNamePanel::GetPosition() {
    return MMotion::GetPosition().Offset(12, 0);
}

void DinoBattleViewEffectGameBeginPlayerNamePanel::Hide() {
    MMotion::Hide();
    mUnk80->Hide();
    mUnk84->Hide();
}

void DinoBattleViewEffectGameBeginPlayerNamePanel::Show() {
    MMotion::Show();
    mUnk80->Show();
    mUnk84->Show();
}

void DinoBattleViewEffectGameBeginPlayerNamePanel::SetPosition(MPoint pos) {
    MMotion::SetPosition(MPoint(pos.x - 12, pos.y));
    mUnk80->SetPosition(MPoint(pos.x + (0x44 - mUnk90 / 2), pos.y + 4));
    int dx = mUnk88 == 0 ? 8 : 0x40;
    int dy = mUnk8C ? -0x40 : 0x18;
    mUnk84->SetPosition(MPoint(pos.x + dx, pos.y + dy));
}

void DinoBattleViewEffectGameBeginPlayerNamePanel::Execute() {
    MMotion::Execute();
    if (mUnk84->mState == MTask::STATE_RUN) {
        mUnk84->Execute();
    }
    if (mUnk80->mState == MTask::STATE_RUN) {
        mUnk80->Execute();
    }
}

void SetCretureIconPriorityTask::Execute() {
    (*mMotion)->SetPriority(mPriority);
    (*mMotion)->SetAnchor(mAnchor);
    Exit();
}

void SetColorMaskTask::Execute() {
    mTarget->mColorMask = mColorMask;
    Exit();
}

void CreateCreatureMotionTask::Execute() {
    (*mSlot)->Exit();
    *mSlot = new CreatureMotion(mPath.c_str(), mOam, mArg);
    func_ov000_021728d4(mView, 1);
    Exit();
}

#pragma auto_inline off

template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewWaitActionInfoBarRequest *msg);

extern "C" void (DinoBattleViewCreature::*const data_ov000_0219304c)();

// Runs the creature's pending call once its motion stops; returns a task that waits for it too.
MTask *DinoBattleViewCreature::CreateMotionCallTask() {
    MTask *wait, *call;
    call = MCreateCallTask(this, data_ov000_0219304c);
    wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    set->AddTail(wait);
    set->AddTail(call);
    mParallel.AddTail(set);
    return mMotion->CreateWaitStopTask();
}

struct DinoAttackHit {
    u16 unk0;
    u16 weight;
};

// Splits the damage between the attack's hits by their weights (the rounding goes to the first).
// NONMATCHING: 90%, the game keeps the loop counter in a register (ours spills it and strength-reduces the index).
void DinoSplitDamage(int *out, u32 attack, int total) {
    MI_CpuFill8(out, 0, 12);
    const DinoAttackDef *def = gDinoResource->GetAttackDef(attack);
    int count = def->hitCount;
    MFx32 sum = MFx32(DinoOffsetPtr<u16>(def, def->cost)[1]);
    int assigned = 0;
    int *p = out;
    for (int i = 0; i < count; i++) {
        int part = MFx32ToInt(MFx32(total) * MFx32(((const DinoAttackHit *)((const u8 *)def + def->hits))[i].weight) / sum);
        *p++ = part;
        assigned += part;
    }
    while (assigned < total) {
        out[0]++;
        assigned++;
    }
}

// Both screens fade in from white.
void DinoBattleViewEffectGameBegin::StateFadeIn() {
    MTask *top, *bottom;
    bottom = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromWhite(30, 0);
    top = gLcd->GetTopDisplay()->mBright->CreateFadeInFromWhite(30, 0);
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(top);
    set->AddTail(bottom);
    mSerial.AddTail(set);
    mState = data_ov000_02193668.State_38;
}

void SetCretureIconTask::Idle() {
    (*mMotion)->SetPriority(2);
    (*mMotion)->SetAnchor(0);
    (*mMotion)->Show();
    (*mMotion)->SetPosition(mPos);
    (*mMotion)->SetFlip(mFlip, 0);
    mTasks->AddTail(*mMotion);
    GetSerial()->AddTail((*mMotion)->CreatePlayTransfer(12, 0), (*mMotion)->CreateWaitStopTask(),
                         (*mMotion)->CreatePlayTransfer(mAnime, 0), MTaskExitTransfer::Create(this));
}

void DinoBattleViewCreature::SetUnitModes(BOOL on) {
    mMotion->GetUnit()->SetMode(on ? 1 : 0);
    mMotion140->GetUnit()->SetMode(on ? 1 : 0);
}

// Hides the creature's icons, lifebar and motion, and plays its marker's closing animation.
void DinoBattleViewCreature::HideAll() {
    mStatusIcons->HideIcons();
    ((DinoBattleViewLifebar *)mUnkE0)->Hide();
    mMotion->Hide();
    MTask *hide = mMotion140->CreateHideTransfer();
    MTask *wait = mMotion140->CreateWaitStopTask();
    MTask *play = mMotion140->CreatePlayTransfer(2, 0);
    MTaskSet *set = mUnk13C;
    set->AddTail(play);
    set->AddTail(wait);
    set->AddTail(hide);
}

// Plays the motion's down animation (14) and waits for it to stop.
MTask *DinoBattleViewCreature::CreateDownTask() {
    MTask *play;
    MTask *wait;
    wait = mMotion->CreateWaitStopTask();
    play = mMotion->CreatePlayTransfer(14, 0);
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    set->AddTail(play);
    set->AddTail(wait);
    return set;
}

// A palette animation that blends the motion's colors toward a color (from them, if withBase), looping.
// NONMATCHING: the game spills weight first thing; ours after the first loads.
MPalAnime *DinoBattleViewCreature::CreatePalBlend(BOOL withBase, u16 color, int weight, u32 duration) {
    MMotionFile *file = &mMotion->mFile;
    MPalUnit *palette = mMotion->GetPalette(0);
    MPalAnime *anime = new MPalAnime(palette, 0, 0x7FFF);
    anime->SetLoadRange(2, 0xFE);
    if (withBase) {
        anime->AddFrame((const u16 *)((u8 *)file->GetPalette(0, NULL) + 4));
    }
    anime->AddBlend((const u16 *)((u8 *)file->GetPalette(0, NULL) + 4), color, weight);
    anime->SetLoop();
    anime->SetDuration(duration);
    return anime;
}

// The number of the creature's attacks its level allows.
// NONMATCHING (and GetAttack): as DinoBattleModelCreature's, the game indexes the tables with i * 4 every time.
u32 DinoBattleViewCreature::CountAttacks() {
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(mUnkA8);
    u32 count = 0;
    int i = 0;
    if (i < def->attackCount) {
        const u32 *levels = (const u32 *) ((const u8 *) def + def->attackLevels);
        for (; i < def->attackCount; i++) {
            if (levels[i] <= mLevel) {
                count++;
            }
        }
    }
    return count;
}

// The index'th attack its level allows; 0 if none.
u32 DinoBattleViewCreature::GetAttack(u32 index) {
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(mUnkA8);
    for (int i = 0; i < def->attackCount; i++) {
        if (((const u32 *) ((const u8 *) def + def->attackLevels))[i] <= mLevel && index-- == 0) {
            return ((const u32 *) ((const u8 *) def + def->attackIds))[i];
        }
    }
    return 0;
}

// The creature's special attack, if the whole team qualifies and its level allows.
u32 DinoBattleViewCreature::GetSpecialAttack() {
    if (((DinoBattleViewPlayer *)mUnkDC)->HasCommonCreatureFlags() && ((DinoBattleViewPlayer *)mUnkDC)->mUnk114) {
        const DinoCreatureDef *def = gDinoResource->GetCreatureDef(mUnkA8);
        if (def->specialAttackLevel <= mLevel) {
            return def->specialAttack;
        }
    }
    return 0;
}

u32 DinoBattleViewCreature::GetUnkAC() {
    return mUnkAC;
}

u32 DinoBattleViewCreature::GetUnkB0() {
    return mUnkB0;
}

u32 DinoBattleViewCreature::GetUnkB4() {
    return mUnkB4;
}

u32 DinoBattleViewCreature::GetUnkA8() {
    return mUnkA8;
}

u32 DinoBattleViewCreature::GetUnkF4() {
    return mUnkF4;
}

u32 DinoBattleViewCreature::GetUnkF8() {
    return mUnkF8;
}

u32 DinoBattleViewCreature::GetUnk10C() {
    return mUnk10C;
}

u32 DinoBattleViewCreature::GetUnkFC() {
    return mUnkFC;
}

u32 DinoBattleViewCreature::GetUnk100() {
    return mUnk100;
}

u32 DinoBattleViewCreature::GetUnk104() {
    return mUnk104;
}

u32 DinoBattleViewCreature::GetUnk108() {
    return mUnk108;
}

u8 DinoBattleViewCreature::GetDefUnk0D() {
    return mDef->mUnk0D;
}

u8 DinoBattleViewCreature::GetDefUnk61() {
    return mDef->mUnk61;
}

// At or below the pinch share of its life.
BOOL DinoBattleViewCreature::IsInPinch() {
    if (MFx32(mUnkF8) / MFx32(mUnkF4) <= MFx32(gDinoResource->GetBattleAdjustDefs()->pinchRatio)) {
        return TRUE;
    }
    return FALSE;
}

u32 DinoBattleViewCreature::GetUnkE0() {
    return mUnkE0;
}

u32 DinoBattleViewCreature::GetUnkDC() {
    return mUnkDC;
}

u32 DinoBattleViewCreature::GetUnkD8() {
    return mUnkD8;
}

MMotion *DinoBattleViewCreature::GetMotion() {
    return mMotion;
}

u32 DinoBattleViewCreature::GetUnk118() {
    return mUnk118;
}

void DinoBattleViewCreature::SetUnk118(u32 value) {
    mUnk118 = value;
}

void DinoBattleViewCreature::SetMotionPriority(int priority) {
    mMotion->SetPriority(priority);
}

void DinoBattleViewCreature::SetMotionAnchor(int anchor) {
    mMotion->SetAnchor(anchor);
}

void DinoBattleViewCreature::PlayMotion3() {
    mMotion->Play(3, 0);
}

void DinoBattleViewCreature::SetFlag144(int bit) {
    mFlags144 |= 1 << bit;
}

void DinoBattleViewCreature::ClearFlag144(int bit) {
    mFlags144 &= ~(1 << bit);
}

#pragma auto_inline on

#pragma auto_inline off

// A player name panel for the game's opening.
extern "C" MMotion *func_ov000_02173d64(MSmartOam *oam, const std::string &name, u32 arg2, u32 arg3, u8 arg4) {
    return new DinoBattleViewEffectGameBeginPlayerNamePanel(oam, name, arg2, arg3, arg4);
}
