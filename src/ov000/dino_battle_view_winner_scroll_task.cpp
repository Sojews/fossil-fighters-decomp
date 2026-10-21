// DinoBattleViewWinnerScrollTask, DinoBattleViewBackdrop.
// NONMATCHING: DinoBattleViewBackdrop::Execute (spilled values in other stack slots, registers in the loop).

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#pragma ipa file

#include "game/dino_sound.h"
#include "game/dino_battle_view_winner_scroll_task.h"
#include "types.h"
#include "m/bg_new_task.h"
#include "m/picture.h"
#include "m/motion.h"
#include "game/motion_advantage_arrow.h"
#include "game/dino_battle_view_field.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view2.h"
#include "game/dino_battle_view_backdrop.h"
#include "game/dino_resource.h"
#include "game/dino_battle_view_creature.h"
#include "m/lcd.h"
#include "m/blend.h"
#include "game/dino_static_init.h"
#include "m/screen.h"
#include "game/dino_battle_view_ui_ability_menu_item.h"
#include "game/dino_battle_view_ability_menu_bg.h"

DinoBattleViewWinnerScrollTask::~DinoBattleViewWinnerScrollTask() {
    if (!mUnk78) {
        DinoSoundPlaySe(0x74, 0);
    }
}

void DinoBattleViewLCDFader::Idle() {
}

void DinoBattleViewTutorialRect::Idle() {
}

void DinoBattleViewField::Idle() {
    Dispatch();
}

extern "C" void func_ov000_0218e680(void *);

MotionAdvantageArrow::~MotionAdvantageArrow() {
}

DinoBattleViewAdvantageArrow::~DinoBattleViewAdvantageArrow() {
}

DinoBattleViewTutorialMotion::~DinoBattleViewTutorialMotion() {
}

DinoBattleViewLCDFader::~DinoBattleViewLCDFader() {
}

TutorialWindowCloseTask::~TutorialWindowCloseTask() {
}

WaitUpdateSupportEffectAnimeTask::~WaitUpdateSupportEffectAnimeTask() {
}

DinoBattleViewTutorialRect::~DinoBattleViewTutorialRect() {
}

SetTutorialMotionArrorTask::~SetTutorialMotionArrorTask() {
}

SetTutorialMotionRectTask::~SetTutorialMotionRectTask() {
}

void DinoBattleViewTutorialMotion::Idle() {
}

void SetTutorialMotionRectTask::Execute() {
    mMotion->SetRect(MRect(mTopLeft, mBottomRight));
    Exit();
}

void SetTutorialMotionArrorTask::Execute() {
    if (mDir == 0) {
        mMotion->Hide(mIndex);
    } else {
        mMotion->ShowArrow(mIndex, mDir, mPos);
    }
    Exit();
}

void WaitUpdateSupportEffectAnimeTask::Execute() {
    for (int i = 0; i < 2; i++) {
        if (mView->GetUnkE4(i)->GetUnk108()->IsAnimating()) {
            return;
        }
    }
    Exit();
}

void TutorialWindowCloseTask::Execute() {
    func_ov000_0218e680((void *)mUnk10);
    Exit();
}

// A background load of a text picture (its creator lives in another file).
template class MBgNewTask1<MTextPicture, const char *>;
template MTask *MCreateBgNewTask(MTextPicture **result, const char *a1, int thread);

DinoBattleViewBackdrop::~DinoBattleViewBackdrop() {
    for (int i = 0; i < 5; i++) {
        delete mTables[i];
    }
}

void DinoBattleViewBackdrop::OnVSync() {
    if (mFading) {
        MI_CpuCopy32(mFadeLines, mBack, sizeof(mFadeLines));
    } else {
        MI_CpuCopy32(mTables[mTable], mBack, sizeof(mFadeLines));
    }
    Flip();
}

static inline const DinoRgb &GetBackdropTop(u32 i) {
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    return ((const DinoRgb *)((const u8 *)defs + defs->backdropTop))[i];
}

static inline const DinoRgb &GetBackdropBottom(u32 i) {
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    return ((const DinoRgb *)((const u8 *)defs + defs->backdropBottom))[i];
}

// Fades from one backdrop's colors to another's: the top and bottom colors are faded, the lines in between blend them.
void DinoBattleViewBackdrop::Execute() {
    MLooperVSyncTask::Execute();
    if (!mFading) {
        return;
    }
    mFrame++;
    if (mFrame >= mDuration) {
        mTable = mTo;
        mFading = FALSE;
        return;
    }
    const DinoRgb &topFrom = GetBackdropTop(mFrom);
    const DinoRgb &topTo = GetBackdropTop(mTo);
    const DinoRgb &bottomFrom = GetBackdropBottom(mFrom);
    const DinoRgb &bottomTo = GetBackdropBottom(mTo);
    int r = topFrom.r + (topTo.r - topFrom.r) * mFrame / mDuration;
    int g = topFrom.g + (topTo.g - topFrom.g) * mFrame / mDuration;
    int b = topFrom.b + (topTo.b - topFrom.b) * mFrame / mDuration;
    int dr = bottomFrom.r + (bottomTo.r - bottomFrom.r) * mFrame / mDuration - r;
    int dg = bottomFrom.g + (bottomTo.g - bottomFrom.g) * mFrame / mDuration - g;
    int db = bottomFrom.b + (bottomTo.b - bottomFrom.b) * mFrame / mDuration - b;
    u16 *line = mFadeLines;
    int ar = 0;
    int ag = 0;
    int ab = 0;
    for (int i = 0; i < M_RASTER_LINES; i++) {
        *line++ = 0x8000 | ((u32)(b + ab / (M_RASTER_LINES - 1)) >> 3 << 10) |
                  ((u32)(g + ag / (M_RASTER_LINES - 1)) >> 3 << 5) | ((u32)(r + ar / (M_RASTER_LINES - 1)) >> 3);
        ar += dr;
        ag += dg;
        ab += db;
    }
}

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until DinoBattleViewField::OnMessage, which uses them,
// is decompiled).
template void std::__vec_deleter<MMotionFile, std::allocator<MMotionFile> >::sub_push_back(const MMotionFile &x, Metrowerks::int2type<false>);
template std::__vec_deleter<MMotionFile, std::allocator<MMotionFile> >::~__vec_deleter();
template void std::__vec_deleter<MMotionFile, std::allocator<MMotionFile> >::erase_at_end(size_t n, Metrowerks::int2type<false>);
template void std::__vec_deleter<MPictureFile, std::allocator<MPictureFile> >::sub_push_back(const MPictureFile &x, Metrowerks::int2type<false>);
template std::__vec_deleter<MPictureFile, std::allocator<MPictureFile> >::~__vec_deleter();
template void std::__vec_deleter<MPictureFile, std::allocator<MPictureFile> >::erase_at_end(size_t n, Metrowerks::int2type<false>);

#pragma auto_inline off

// Fades the backdrop over to the table.
extern "C" void func_ov000_0216147c(DinoBattleViewUnkE0 *field, u32 table) {
    DinoBattleViewBackdrop *backdrop = field->mBackdrop;
    if (backdrop != NULL) {
        backdrop->mFrom = backdrop->mTable;
        backdrop->mTo = table;
        backdrop->mDuration = 30;
        backdrop->mFrame = 0;
        backdrop->mFading = TRUE;
    }
}

extern "C" void func_ov000_021614b4(DinoBattleViewUnkE0 *field) {
    for (int side = 0; side < 2; side++) {
        DinoBattleViewPlayer *player = field->mView->GetUnkE4(side);
        for (int i = 0; i < player->GetCreatureCount(); i++) {
            DinoBattleViewCreature *creature = player->GetCreature(i);
            creature->SetUnitModes(FALSE);
            creature->SetMotionPriority(2);
            creature->SetMotionAnchor(0);
        }
    }
}

DinoBattleViewLCDFader::DinoBattleViewLCDFader(u32 arg) : MProcess(NULL) {
    mUnk74 = arg;
    mDisplays[0] = gLcd->mMain;
    mDisplays[1] = gLcd->mSub;
    mRatios[0] = 0;
    mRatios[1] = 0;
}

// Fades the screen's blend ratio (screen 1 only).
extern "C" void func_ov000_02161550(DinoBattleViewLCDFader *fader, int lcd, int ratio, u32 duration, int ease) {
    if (lcd != 0) {
        MBlend *blend = fader->mDisplays[lcd]->mBlend;
        blend->SetMode(3);
        blend->SetAlpha(16, 0);
        blend->SetTarget1(0x39);
        blend->SetTarget2(0x3F);
        fader->mParallel.AddTail(blend->CreateFadeRatioAnime(fader->mRatios[lcd], ratio, duration, ease));
        fader->mRatios[lcd] = ratio;
    }
}

// Runs the two tasks (ending the ones before).
extern "C" void func_ov000_021615b4(DinoBattleViewUnkE0 *field, MTask *task, MTask *task2, u32 arg) {
    if (field->mTask128 != NULL) {
        field->mTask128->Exit();
    }
    field->mTask128 = task;
    field->mTasksB8->AddTail(field->mTask128);
    if (field->mTask12C != NULL) {
        field->mTask12C->Exit();
    }
    field->mTask12C = task2;
    if (field->mTask12C != NULL) {
        field->mTasksB8->AddTail(field->mTask12C);
        field->mUnk130 = arg;
    }
}

extern "C" u32 data_ov000_02191b1c[];   // the TU's pooled statics (+0x70: the attack bars' x)
extern "C" MTask *func_ov000_0217a080(AttackMenuBarMotion *bar, MPoint from, MPoint to, int frames, int ease);

// Slides the attack bar to y and the ability menu's BG with it.
extern "C" void func_ov000_0216161c(DinoBattleViewUnkE0 *field, int y, int frames, int ease) {
    AttackMenuBarMotion *bar = (AttackMenuBarMotion *)field->mTask128;
    MPoint from = bar->GetPosition();
    MPoint to(((const int *)data_ov000_02191b1c)[0x70 / 4], y);
    MPoint scrollFrom;
    MPoint scrollTo(0, field->mUnk130 - y);
    DinoBattleViewAbilityMenuBG *bg = (DinoBattleViewAbilityMenuBG *)field->mTask12C;
    scrollFrom = bg->mUnk10D4 ? ((MScreen *)bg->mText)->GetOffset() : bg->mOffset;
    MTask *scroll;
    if (((DinoBattleViewAbilityMenuBG *)field->mTask12C)->mUnk10D4) {
        scroll = ((MScreen *)((DinoBattleViewAbilityMenuBG *)field->mTask12C)->mText)
                     ->CreateOffsetAnime(scrollFrom, scrollTo, frames, ease);
    } else {
        scroll = new MTimer(1);
    }
    MTask *move = func_ov000_0217a080((AttackMenuBarMotion *)field->mTask128, from, to, frames, ease);
    MTaskSet *tasks = field->mTasksB4;
    tasks->AddTail(scroll);
    tasks->AddTail(move);
}

extern "C" void func_ov000_0216170c(DinoBattleViewUnkE0 *field) {
    if (field->mTask128 != NULL) {
        field->mTask128->Exit();
        field->mTask128 = NULL;
    }
    if (field->mTask12C != NULL) {
        field->mTask12C->Exit();
        field->mTask12C = NULL;
    }
}

// Clears every creature's type advantage mark.
extern "C" void func_ov000_02161748(DinoBattleViewUnkE0 *field) {
    for (u32 side = 0; side < 2; side++) {
        DinoBattleViewPlayer *player = field->mView->GetUnkE4(side);
        for (u32 i = 0; i < player->GetCreatureCount(); i++) {
            player->GetCreature(i)->ClearAdvantage();
        }
    }
}

extern "C" MPoint func_ov000_0215f22c(int kind, int side);

// Sets a side's creatures back to their idle motion.
static inline void ResetCreatures(DinoBattleViewPlayer *player) {
    for (u32 i = 0; i < player->GetCreatureCount(); i++) {
        DinoBattleViewCreature *creature = player->GetCreature(i);
        creature->ClearFlag144(5);
        creature->UpdateStatusColor(TRUE);
        creature->PlayMotion3();
    }
}

// A side's slot. (Through a non-member inline, the slot gets the game's stack slot.)
static inline DinoBattleViewFieldSlot *DinoWinnerSlot(u32 field, int side) {
    return ((DinoBattleViewUnkE0 *)field)->mSlotViewsC0[side];
}

// Without the scroll, resets the second side's creatures and exits; otherwise slides both sides' slots away.
void DinoBattleViewWinnerScrollTask::Idle() {
    MParallelTaskSet *tasks;
    if (!mUnk78) {
        for (int side = 0; side < 2; side++) {
            DinoBattleViewFieldSlot *slot = ((DinoBattleViewUnkE0 *)mUnk74)->mSlotViewsC0[side];
            if (side == 1) {
                ResetCreatures(slot->mPlayer);
            }
        }
        Exit();
        return;
    }
    tasks = new MParallelTaskSet(TRUE);
    for (int side = 0; side < 2; side++) {
        DinoBattleViewFieldSlot *slot = DinoWinnerSlot(mUnk74, side);
        if (side == 0) {
            ResetCreatures(slot->mPlayer);
        }
        tasks->AddTail(slot->CreateCoordAnime(slot->GetPosition(), func_ov000_0215f22c(side == 0 ? 4 : 5, side), 120, -1));
    }
    GetSerial()->AddTail(tasks, MTaskExitTransfer::Create(this));
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_02191b9c[];
extern "C" u32 data_ov000_02191d9c[];
extern "C" u32 data_ov000_02191dcc[];
// NONMATCHING: the game loads the last two anchors' addresses together, before the first group's stores.
static struct DinoBattleViewWinnerScrollTaskStaticInit {
    DinoBattleViewWinnerScrollTaskStaticInit() {
        DINO_POOL_WORD(data_ov000_02191b1c, 0x70) = 128;
        DINO_POOL_WORD(data_ov000_02191b1c, 0x74) = 176;
        DINO_POOL_WORD(data_ov000_02191b9c, 0x10) = 8;
        DINO_POOL_WORD(data_ov000_02191b9c, 0x14) = 150;
        DINO_POOL_WORD(data_ov000_02191b1c, 0x78) = 64;
        DINO_POOL_WORD(data_ov000_02191b1c, 0x7c) = 112;
        DINO_POOL_WORD(data_ov000_02191d9c, 0x28) = 64;
        DINO_POOL_WORD(data_ov000_02191d9c, 0x2c) = 96;
        volatile s32 *p = (volatile s32 *)data_ov000_02191dcc;
        p[0] = 112 * 4;
        p[1] = 96;
    }
} sDinoBattleViewWinnerScrollTaskStaticInit;
