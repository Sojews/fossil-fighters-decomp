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
#include "m/oam_canvas.h"
#include "m/font.h"
#include "m/picture.h"
#include "game/info_panel_window.h"
#include "game/dino_sound.h"

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

// Pops the damage (or the miss) and the life left over the creature, then updates its life bar.
void UpdateLifePointTask::Idle() {
    DinoBattleViewPlayer *player = (DinoBattleViewPlayer *)mCreature->GetUnkDC();
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    MPoint pos(defs->unk140[0], defs->unk140[1]);
    if (player->GetUnkAC() == 1) {
        pos.x = -pos.x;
    }
    pos += mCreature->GetMotion()->GetPosition();
    switch (mKind) {
    case 0: {
        MTask *life = new DinoBattleViewPopNumber(((DinoBattleViewPlayer *)mCreature->GetUnkDC())->GetView(),
                                                  (MSmartOam *)mCreature->GetUnkD8(), pos, mCreature->GetUnkF8(), 0, 3);
        MTask *damage = new DinoBattleViewPopNumber(((DinoBattleViewPlayer *)mCreature->GetUnkDC())->GetView(),
                                                    (MSmartOam *)mCreature->GetUnkD8(), pos, mValue, 0, 0);
        player->AddTimedTask(damage, life);
        break;
    }
    case 1: {
        MTask *life = new DinoBattleViewPopNumber(((DinoBattleViewPlayer *)mCreature->GetUnkDC())->GetView(),
                                                  (MSmartOam *)mCreature->GetUnkD8(), pos, mCreature->GetUnkF8(), 0, 3);
        MTask *miss = new DinoBattleViewPopNumber(((DinoBattleViewPlayer *)mCreature->GetUnkDC())->GetView(),
                                                  (MSmartOam *)mCreature->GetUnkD8(), pos, 0, 1, 0);
        player->AddTimedTask(miss, life);
        break;
    }
    }
    GetSerial()->AddTail(new UpdateLifeBarTask((DinoBattleViewLifebar *)mCreature->GetUnkE0(), mCreature->GetUnkF8(),
                                          mCreature->GetUnkF4()),
                    MTaskExitTransfer::Create(this));
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
    u8 unk00[0x04];
    const char *mBackPath;      // 0x04
    const char *mFontPath;      // 0x08
    const char *mMarkPath2;     // 0x0C: (side 1)
    const char *mMarkPath;      // 0x10: (side 0)
    const char *mPanelPath;     // 0x14
    u8 unk18[0x38 - 0x18];
    void (DinoBattleViewEffectGameBegin::*State_38)();
    u8 unk40[0x48 - 0x40];
    void (DinoBattleViewEffectGameBegin::*Start_48)();
    void (DinoBattleViewEffectGameBegin::*StateFadeIn_50)();
} data_ov000_02193668;

extern "C" u8 data_ov000_02193690[];  // the name's palette

// A player's name panel: the panel, the name drawn on a canvas and the side's mark.
DinoBattleViewEffectGameBeginPlayerNamePanel::DinoBattleViewEffectGameBeginPlayerNamePanel(MSmartOam *oam,
                                                                                         std::string name, u32 arg2,
                                                                                         u32 arg3, bool arg4)
    : MMotion(std::string(data_ov000_02193668.mPanelPath), oam, 0, 1) {
    mUnk88 = arg2;
    mUnk8C = arg4;
    Play(arg2 == 0 ? 3 : 4, 0);
    mFont = new MFontSet(std::string(data_ov000_02193668.mFontPath), 0, 1, 3);
    mFont->AddRef();
    MPoint size = mFont->MeasureText(std::string(name.c_str()), 0, 0);
    if ((u32)size.x > 128) {
        size.x = 128;
    }
    mUnk90 = size.x;
    mUnk80 = new MSmartObjCanvas(oam, size, data_ov000_02193690, TRUE);
    mUnk80->DrawText(mFont, MPoint(0, 0), 0, std::string(name.c_str()));
    mUnk84 = new MMotion(std::string(arg2 == 0 ? data_ov000_02193668.mMarkPath : data_ov000_02193668.mMarkPath2), oam,
                         3, 1);
    mUnk84->Play(arg3, 0);
}

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

extern "C" void func_ov000_0216fd74(DinoBattleViewCreature *view);

void DinoBattleViewCreature::Idle() {
    MMessageObserver::Dispatch();
    UpdateStatusColor(FALSE);
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
    mView->UpdateStatusColor(TRUE);
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
// Shows the "VS" mark and both players' name panels over the backdrop, then fades in.
// NONMATCHING: the hoisted &mParallel sits at sp+0x18 where the game has it at sp+0x10 (below two temporaries).
void DinoBattleViewEffectGameBegin::Start() {
    mOam = new MSmartOam(22, 0, TRUE);
    mVs = new MMotion(std::string(data_ov000_02193668.mPanelPath), mOam, 0, 1);
    mVs->SetPosition(0x80, 0x60);
    MParallelTaskSet *parallel = &mParallel;
    for (int i = 0; i < 2; i++) {
        mPanels[i] = new DinoBattleViewEffectGameBeginPlayerNamePanel(
            mOam, mNames[i], i, ((DinoBattleView *)mUnk7C)->GetPlayerByAC(i)->GetUnkCC(), i ? true : false);
        parallel->AddTail(mPanels[i]);
        mPanels[i]->SetPriority(0);
    }
    mBack = new MTextPicture(std::string(data_ov000_02193668.mBackPath), 1, false);
    GetParallel()->AddTail(mVs, mBack, mOam);
    mState = data_ov000_02193668.StateFadeIn_50;
}

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

// The four in a row.
static inline MTask *DinoGameBeginSerial(MTask *a, MTask *b, MTask *c, MTask *d) {
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    set->AddTail(a);
    set->AddTail(b);
    set->AddTail(c);
    set->AddTail(d);
    return set;
}

// The three at once.
static inline MTask *DinoGameBeginParallel(MTask *a, MTask *b, MTask *c) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(a);
    set->AddTail(b);
    set->AddTail(c);
    return set;
}

// Shows "VS": the players' names fly in from the sides and hold, then slide away (the top one up, the bottom one
// down) with the backdrop. (One expression: the game gives the points' temporaries stack slots in this order.)
void DinoBattleViewEffectGameBegin::StateNames() {
    mVs->Show();
    mVs->Play(1, 0);
    int topY = 48 - mPanels[0]->GetHeight() / 2;
    int topX = 128 - mPanels[0]->GetWidth() / 2;
    int bottomY = 144 - mPanels[1]->GetHeight() / 2;
    int bottomX = 128 - mPanels[1]->GetWidth() / 2;
    mPanels[0]->Hide();
    mPanels[1]->Hide();
    mSerial.AddTail(
        new MTimer(10),
        DinoGameBeginParallel(
            DinoGameBeginSerial(mBack->CreateShowTransfer(),
                                mBack->CreateCoordAnime(MPoint(0, 192), MPoint(0, 0), 20, -2), new MTimer(80),
                                mBack->CreateCoordAnime(MPoint(0, 0), MPoint(0, -192), 20, 2)),
            DinoGameBeginSerial(mPanels[0]->CreateShowTransfer(),
                                mPanels[0]->CreateCoordAnime(MPoint(topX - 304, topY), MPoint(topX - 64, topY), 20, -2),
                                mPanels[0]->CreateCoordAnime(MPoint(topX - 64, topY), MPoint(topX - 32, topY), 80, 0),
                                mPanels[0]->CreateCoordAnime(MPoint(topX - 32, topY), MPoint(topX - 32, topY - 128), 20,
                                                             2)),
            DinoGameBeginSerial(
                mPanels[1]->CreateShowTransfer(),
                mPanels[1]->CreateCoordAnime(MPoint(bottomX + 304, bottomY), MPoint(bottomX + 64, bottomY), 20, -2),
                mPanels[1]->CreateCoordAnime(MPoint(bottomX + 64, bottomY), MPoint(bottomX + 32, bottomY), 80, 0),
                mPanels[1]->CreateCoordAnime(MPoint(bottomX + 32, bottomY), MPoint(bottomX + 32, bottomY + 128), 20,
                                             2))),
        MTaskExitTransfer::Create(this));
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

extern "C" MPoint func_ov000_0216f320(int side, u32 kind, BOOL own);
extern "C" MPoint func_ov000_0216f4c0(int side, u32 kind, BOOL own);
extern "C" int func_ov000_0216f698(int arg0, BOOL arg1, BOOL arg2);
extern "C" MTask *func_ov000_0216ff18(DinoBattleViewCreature *creature, MPoint from, MPoint to, int frames, int ease,
                                       BOOL shuffle, int arg);

extern "C" const u32 data_0208a2b0;    // the action bar's pinch text
extern "C" const u32 data_0208a2b8[];  // (word 2) its leaving-the-pinch text
// The TU's pooled pinch mark path (word 1).
extern "C" const char *const data_ov000_02193030[];

inline MTask *DinoBattleView::CreateWaitActionInfoBarTask() {
    return DinoCreatePostAndWaitTask(this, new DinoBattleViewWaitActionInfoBarRequest(0));
}

inline MTask *DinoBattleView::CreateActionInfoBarUpdateTask(u32 text) {
    return CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(text));
}

// NONMATCHING: the game reads the creature's side for the bar text after allocating the request (here before).
void DinoBattleViewCreature::SetLife(int life) {
    if (mUnkF8 == 0 || life == 0) {
        mUnkF8 = life;
        return;
    }
    BOOL pinch = IsInPinch();
    mUnkF8 = life;
    if (GetDefUnk61() != 5 || pinch == IsInPinch()) {
        return;
    }
    u32 text = *(pinch ? &data_0208a2b0 : &data_0208a2b8[2]);
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    ((DinoBattleViewPlayer *)mUnkDC)->mView->IncUnk13C();
    MTask *enable = ((DinoBattleViewPlayer *)mUnkDC)->mView->CreateEnableRootSelectTask();
    MTask *wait = ((DinoBattleViewPlayer *)mUnkDC)->mView->CreateWaitActionInfoBarTask();
    MTask *post = ((DinoBattleViewPlayer *)mUnkDC)->mView->CreateActionInfoBarUpdateTask(text + (mUnkB0 == 1 ? 1 : 0));
    set->AddTail(DinoBattleViewPopNumber::CreateWaitTask(), post, wait, enable);
    if (!pinch) {
        MSmartOam *oam = new MSmartOam(10, 5, TRUE);
        oam->SetOffset(((MSmartOam *)mUnkD8)->GetOffset());
        MMotion *mark = new MMotion(std::string(data_ov000_02193030[1]), oam, 0, 1);
        mark->SetPosition(func_ov000_0216f320(mUnkB0, mUnkB4, FALSE));
        mark->SetPriority(1);
        mark->Show();
        mark->GetUnit()->SetMode(1);
        MTaskSet *tasks = ((DinoBattleViewPlayer *)mUnkDC)->mView->mUnkE0->mTasksBC;
        tasks->AddTail(mark);
        tasks->AddTail(oam);
        MTask *exitOam = MTaskExitTransfer::Create(oam);
        MTask *exitMark = MTaskExitTransfer::Create(mark);
        MTask *se = DinoSoundCreatePlaySeTask(0x56C, 0);
        GetSerial()->AddTail(mark->CreatePlayTransfer(6, 0), se, set, exitMark, exitOam);
        SetFlag144(7);
    } else {
        mSerial.AddTail(set);
        ClearFlag144(7);
    }
}

void DinoBattleViewCreature::MoveHome(BOOL own) {
    MPoint home = func_ov000_0216f320(mUnkB0, mUnkB4, own);
    if (own) {
        MPoint coord = GetCoord();
        mParallel.AddTail(func_ov000_0216ff18(this, coord, home, 40, -1, FALSE, 0));
        DinoBattleViewLifebar *lifebar = (DinoBattleViewLifebar *)mUnkE0;
        MPoint from = lifebar->GetPosition();
        MPoint to = func_ov000_0216f4c0(mUnkB0, mUnkB4, TRUE);
        mParallel.AddTail(((DinoBattleViewLifebar *)mUnkE0)->CreateCoordAnime(from, to, 40, -1));
    } else {
        MPoint coord = GetCoord();
        mParallel.AddTail(func_ov000_0216ff18(this, coord, home, 40, -1, FALSE, 0));
        DinoBattleViewLifebar *lifebar = (DinoBattleViewLifebar *)mUnkE0;
        MPoint from = lifebar->GetPosition();
        MPoint to = func_ov000_0216f4c0(mUnkB0, mUnkB4, FALSE);
        mParallel.AddTail(((DinoBattleViewLifebar *)mUnkE0)->CreateCoordAnime(from, to, 40, -1));
    }
    mMotion->Play(func_ov000_0216f698(mUnkB4, own, mUnk159), 0);
    mCoord = home;
}

// The status colors and their blends' weights and lengths.
extern "C" const u16 data_020b5a90, data_020b5a92, data_020b5a94, data_020b5a96, data_020b5a98, data_020b5a9a;
extern "C" const u32 data_0208a124, data_0208a1ac, data_0208a1b0, data_0208a1b8, data_0208a1c4, data_0208a1cc,
    data_0208a1d4, data_0208a1dc;

void DinoBattleViewCreature::UpdateStatusColor(BOOL force) {
    int color = 0;
    u32 flags = mFlags144 & ~mMask148;
    if ((flags & 0x14) == 0x14) {
        color = 6;
    } else if (flags & 0x10) {
        color = 4;
    } else if (flags & 8) {
        color = 3;
    } else if (flags & 4) {
        color = 2;
    } else if (flags & 0x20) {
        color = 5;
    } else if (flags & 2) {
        color = 1;
    } else if (flags & 0x80) {
        color = 7;
    }
    if (mColor == color && !force) {
        return;
    }
    mColor = color;
    if (mBlend != NULL) {
        mParallel.Remove(mBlend);
        delete mBlend;
        mBlend = NULL;
    }
    MMotionFile *file = &mMotion->mFile;
    MPalUnit *palette = mMotion->GetPalette(0);
    switch (mColor) {
    case 0:
        palette->LoadColors((const u8 *)file->GetPalette(0, NULL) + 8, 2, 0xFE);
        return;
    case 1:
        mBlend = CreatePalBlend(FALSE, data_020b5a94, (u16)data_0208a1cc, 30);
        mParallel.AddTail(mBlend);
        return;
    case 2:
        mBlend = CreatePalBlend(TRUE, data_020b5a98, (u16)data_0208a124, data_0208a1c4);
        mParallel.AddTail(mBlend);
        return;
    case 3:
        mBlend = CreatePalBlend(FALSE, data_020b5a96, (u16)data_0208a1d4, 30);
        mParallel.AddTail(mBlend);
        return;
    case 4:
        mBlend = CreatePalBlend(FALSE, data_020b5a9a, (u16)data_0208a1dc, 30);
        mParallel.AddTail(mBlend);
        return;
    case 5:
        mBlend = CreatePalBlend(FALSE, data_020b5a92, (u16)data_0208a1b8, 30);
        mParallel.AddTail(mBlend);
        return;
    case 6:
        mBlend = CreatePalBlend(TRUE, data_020b5a9a, (u16)data_0208a1dc, 30);
        mParallel.AddTail(mBlend);
        return;
    case 7:
        mBlend = CreatePalBlend(TRUE, data_020b5a90, (u16)data_0208a1ac, data_0208a1b0);
        mParallel.AddTail(mBlend);
        break;
    }
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
extern "C" MMotion *func_ov000_02173d64(MSmartOam *oam, const std::string &name, u32 arg2, u32 arg3, bool arg4) {
    return new DinoBattleViewEffectGameBeginPlayerNamePanel(oam, name, arg2, arg3, arg4);
}

static inline int DinoLerp(int from, int to, int t, int n) {
    return from + (to - from) * t / n;
}

static inline fx32 DinoFxMul(fx32 a, fx16 b) {
    return (fx32)(((fx64)a * b + 0x800) >> FX32_SHIFT);
}

// Moves the motion from mFrom to mTo, wobbling around the way (the wobble grows for a second, then shrinks).
// NONMATCHING: 92%, the game spills the duration (ours spills the start's y).
void ShuffleMoveTask::Execute() {
    u32 frame = ++mFrame;
    if (frame >= mDuration) {
        mMotion->SetPosition(mTo);
        Exit();
        return;
    }
    int fromY = mFrom.y;
    int fromX = mFrom.x;
    int y = DinoLerp(fromY, mTo.y, frame, mDuration);
    int x = DinoLerp(fromX, mTo.x, frame, mDuration);
    if (mRadius != 0 || frame < 60) {
        mRadius += frame < 60 ? 0xC00 : -0xC00;
    }
    fx32 radius = mRadius;
    fx32 dx = DinoFxMul(radius, FX_CosIdx(mAngle));
    fx32 dy = DinoFxMul(radius, FX_SinIdx(mAngle));
    mAngle += ((DinoBattleViewCreature *)mArg)->GetUnkB0() ? 0x400 : -0x400;
    mMotion->SetPosition(MPoint(x + (dx >> FX32_SHIFT), y + (dy >> FX32_SHIFT)));
}
