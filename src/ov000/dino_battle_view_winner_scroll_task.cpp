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
#include "game/dino_battle_view_ui_change_menu.h"
#include "game/dino_battle_view_ui_attacker_menu.h"
#include "game/dino_battle_view_ui_defender_menu.h"
#include "game/dino_battle_view_ui_select_all.h"
#include "game/dino_battle_view.h"
#include "game/dino_battle_view3.h"
#include "game/dino_battle_view_effect_game_begin.h"
#include "game/dino_battle_view_effect_telop.h"
#include "game/wait_action_info_bar_task.h"
#include "game/camera_wipe.h"
#include "game/view_status_effect_icon.h"
#include "game/dino_battle_tutorial.h"
#include "game/dino_battle_view_field_player_bg.h"
#include "game/dino_message.h"
#include "game/dino_battle_view_kaseki_point.h"
#include "game/dino_battle_view_ability_text_window.h"

// (Emitted here, with the vtable and the OnMessage thunk.)
DinoBattleViewField::~DinoBattleViewField() {
}

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
    int ar = 0;
    u16 *line = mFadeLines;
    int ag = 0;
    int ab = 0;
    for (int i = 0; i < M_RASTER_LINES; i++) {
        *line++ = 0x8000 | ((u32)(r + ar / (M_RASTER_LINES - 1)) >> 3) | ((u32)(b + ab / (M_RASTER_LINES - 1)) >> 3 << 10) |
                  ((u32)(g + ag / (M_RASTER_LINES - 1)) >> 3 << 5);
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
// (append_realloc's temporary, emitted ahead of the growth that uses it.)
template std::__vec_deleter_helper<MPictureFile, std::allocator<MPictureFile> &>::~__vec_deleter_helper();

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


// The view's battle field: its messages.
extern "C" const u32 data_0208a0f0;     // the winner scroll's sound delay
extern "C" const u32 data_0208a0fc;     // the turn telop's delay
extern "C" const char *const data_ov000_02191b9c[]; // the turn telop's texts (own, the other's)
extern "C" const MPoint data_ov000_02191b8c;        // the attack bar's position
extern "C" volatile u32 data_ov000_021978d0[2];     // the effect tables' guards
extern "C" const char *data_ov000_02191dd4[];       // the effects' motions and their animes (pairs)
extern "C" int data_ov000_02191dd8[];
extern "C" MPoint data_ov000_02191de4[];            // the effects' positions, by side
extern "C" int data_ov000_02191dec[];              // (the second)
extern "C" u32 data_ov000_02191d9c[];
extern "C" void func_ov000_0218b4c0(class DinoBattleViewActionInfoBar *bar, u32 textId);
extern "C" void func_ov000_0218b928(class DinoBattleViewFocusLCDFrame *frame, u8 top);
extern "C" void func_ov000_0217dafc(u8 value);
extern "C" void func_ov000_0218e600(DinoBattleViewTutorialWindow *window, const char *text);
extern "C" MTask *func_ov000_0218e6b0(DinoBattleViewTutorialWindow *window);
extern "C" void func_ov000_0218c3d4(class DinoBattleTalkWindow *window, const char *text);
extern "C" void func_ov000_0218c960(class DinoBattleTalkWindow *window);
extern "C" DinoBattleViewAbilityTextWindow *func_ov000_0217c0b8(DinoBattleViewAbilityMenuBG *bg, BOOL rows);

inline DinoBattleViewWinnerScrollTask::DinoBattleViewWinnerScrollTask(u32 field, u8 scroll) : MProcess(NULL) {
    mUnk74 = field;
    mUnk78 = scroll;
    if (scroll) {
        MTask *se = DinoSoundCreatePlaySeTask(0x73, 0);
        MTimer *timer = new MTimer(data_0208a0f0);
        MSerialTaskSet *set = new MSerialTaskSet(TRUE);
        set->AddTail(timer);
        set->AddTail(se);
        mParallel.AddTail(set);
    }
}

// The view's requests: menus, effects, tutorial marks and the attack bar, each answered when done.
// NONMATCHING: 80.5%, call sequence identical. The game keeps this on the stack (reloading it in every case) where ours keeps it in a register,
// which moves the spills; the cases' code is otherwise laid out as the game's.
#define DINO_FIELD ((DinoBattleViewUnkE0 *)this)
void DinoBattleViewField::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    case 0x10009: {
        DinoBattleViewEffectGameBeginRequest *req = (DinoBattleViewEffectGameBeginRequest *)msg;
        GetSerial()->AddTail(new DinoBattleViewEffectGameBegin((u32)DINO_FIELD->mView, DINO_FIELD->mView->GetPlayerByAC(0)->GetUnkC0(),
                                                               DINO_FIELD->mView->GetPlayerByAC(1)->GetUnkC0()),
                             ((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewEffectGameBeginResult()));
        if (DINO_FIELD->mView->GetUnk130() != 0) {
            DinoSoundPlayBgm(DINO_FIELD->mView->GetUnk130(), 0);
        }
        break;
    }
    case 0x1000B: {
        DinoBattleViewEffectTurnBeginRequest *req = (DinoBattleViewEffectTurnBeginRequest *)msg;
        DINO_FIELD->unkA8 = req->mUnk10;
        BOOL own = DINO_FIELD->mView->GetUnkE4(req->mUnk10)->GetUnkAC() == 0;
        func_ov000_0216147c(DINO_FIELD, own ? 1 : 2);
        DinoBattleViewPlayer *player = DINO_FIELD->mView->GetUnkE4(req->mUnk10);
        DINO_FIELD->mPlayerBG->SetSide(player->GetUnkAC());
        for (int side = 0; side < 2; side++) {
            BOOL same = side == DINO_FIELD->mView->GetUnkE4(req->mUnk10)->GetUnkAC();
            int ease = same ? 1 : -1;
            int kind = same ? 2 : 3;
            DinoBattleViewFieldSlot *slot = DINO_FIELD->mSlotViewsC0[side];
            mParallel.AddTail(slot->CreateCoordAnime(slot->GetPosition(), func_ov000_0215f22c(kind, side), 40, ease));
            BOOL turn = side == req->mUnk10;
            DinoBattleViewPlayer *p = DINO_FIELD->mView->GetUnkE4(side);
            for (int i = 0; i < p->GetCreatureCount(); i++) {
                p->GetCreature(i);
                p->GetCreature(i)->MoveHome(turn);
            }
            p->GetKasekiPoint()->UpdateAnime(DINO_FIELD->mView->GetUnkE4(req->mUnk10)->GetUnkAC());
        }
        DINO_FIELD->mArrow->PointAt(player->GetUnkAC());
        MSerialTaskSet *set = new MSerialTaskSet(TRUE);
        set->AddTail(new MTimer(data_0208a0fc),
                     new DinoBattleViewEffectTelop((u32)DINO_FIELD->mView, std::string(data_ov000_02191b9c[own ? 0 : 1]),
                                                   own ? 3 : 2, req->mUnk0C, (u32) new DinoBattleViewEffectTurnBeginResult(),
                                                   1),
                     MTaskRunTransfer::Create((MTask *)DINO_FIELD->mArrow));
        mParallel.AddTail(set);
        break;
    }
    case 0x10010: {
        DinoBattleViewUiRootMenuRequest *req = (DinoBattleViewUiRootMenuRequest *)msg;
        mParallel.AddTail(new DinoBattleViewUiRootSelect(DINO_FIELD->mView, (MMessageObserver *)req->mUnk0C, req->mUnk10,
                                                         req->mUnk14, req->mUnk18, req->mUnk1C, req->mUnk20));
        break;
    }
    case 0x1001B: {
        DinoBattleViewUiChangeMenuRequest *req = (DinoBattleViewUiChangeMenuRequest *)msg;
        mParallel.AddTail(new DinoBattleViewUiChangeMenu(DINO_FIELD->mView, req->mUnk10, req->mUnk14,
                                                         (MMessageObserver *)req->mUnk0C, (const u32 *)req->mUnk18,
                                                         req->mUnk24, (MSmartOam *)DINO_FIELD->unkB0));
        break;
    }
    case 0x1004B: {
        DinoBattleViewUiTurnEndRequest *req = (DinoBattleViewUiTurnEndRequest *)msg;
        mParallel.AddTail(new DinoBattleViewTurnEndDialog((u32)DINO_FIELD->mView, (MMessageObserver *)req->mUnk0C, req->mUnk14));
        break;
    }
    case 0x1001D: {
        DinoBattleViewUiCreatureInfoMenuRequest *req = (DinoBattleViewUiCreatureInfoMenuRequest *)msg;
        mParallel.AddTail(new DinoBattleViewUiCreatureInfoMenu(DINO_FIELD->mView, req->mUnk10, (MMessageObserver *)req->mUnk0C));
        break;
    }
    case 0x10017: {
        DinoBattleViewUiAttackerMenuRequest *req = (DinoBattleViewUiAttackerMenuRequest *)msg;
        mParallel.AddTail(new DinoBattleViewUiAttackerMenu(DINO_FIELD->mView, req->mUnk10, (MMessageObserver *)req->mUnk0C,
                                                           (const u32 *)req->mUnk14, req->mUnk20, req->mUnk24,
                                                           req->mUnk28));
        break;
    }
    case 0x10019: {
        DinoBattleViewUiDefenderMenuRequest *req = (DinoBattleViewUiDefenderMenuRequest *)msg;
        if (!req->mUnk24) {
            mParallel.AddTail(new DinoBattleViewUiDefenderMenu(DINO_FIELD->mView, req->mUnk10, (MMessageObserver *)req->mUnk0C,
                                                               (const u32 *)req->mUnk14, req->mUnk20));
        } else {
            mParallel.AddTail(new DinoBattleViewUiSelectAllMenu(DINO_FIELD->mView, req->mUnk10, (MMessageObserver *)req->mUnk0C,
                                                                (const u32 *)req->mUnk14, req->mUnk20));
        }
        break;
    }
    case 0x10023: {
        DinoBattleViewUiAbilityMenuRequest *req = (DinoBattleViewUiAbilityMenuRequest *)msg;
        DinoBattleViewCreature *creature = DINO_FIELD->mView->GetUnkE4(req->mUnk10)->FindCreatureByAC(req->mUnk14);
        mParallel.AddTail(new DinoBattleViewUiAbilityMenu(req->mUnk0C, creature->GetUnkA8(), req->mUnk14, req->mUnk18,
                                                          req->mUnk1C, *(const DinoBattleViewUiAbilityMenuArgs *)req->mUnk20,
                                                          req->mUnk38, req->mUnk3C, DINO_FIELD->mView));
        break;
    }
    case 0x10036: {
        DinoBattleViewEffectGameEndRequest *req = (DinoBattleViewEffectGameEndRequest *)msg;
        MSerialTaskSet *set = new MSerialTaskSet(TRUE);
        set->AddTail(new WaitActionInfoBarTask((DinoBattleViewActionInfoBar *)DINO_FIELD->mView->GetUnk104()));
        BOOL won = DINO_FIELD->mView->GetUnkE4(req->mUnk10)->GetUnkAC() == 0;
        func_ov000_0216147c(DINO_FIELD, won ? 3 : 4);
        if (won) {
            set->AddTail(new DinoBattleViewWinnerScrollTask((u32)this, TRUE),
                         new DinoBattleViewEffectTelop((u32)DINO_FIELD->mView,
                                                       std::string(((const char *const *)data_ov000_02191b1c)[0x30 / 4]), 4,
                                                       0, 0, 0));
        } else {
            set->AddTail(new DinoBattleViewWinnerScrollTask((u32)this, FALSE),
                         new DinoBattleViewEffectTelop((u32)DINO_FIELD->mView,
                                                       std::string(((const char *const *)data_ov000_02191b1c)[0x34 / 4]), 5,
                                                       0, 0, 0));
        }
        set->AddTail(((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewEffectGameEndResult()));
        mParallel.AddTail(set);
        break;
    }
    case 0x10061: {
        DinoBattleViewWaitUpdateSupportEffectAnimeRequest *req = (DinoBattleViewWaitUpdateSupportEffectAnimeRequest *)msg;
        GetSerial()->AddTail(new WaitUpdateSupportEffectAnimeTask(DINO_FIELD->mView),
                             ((MMessageObserver *)req->mUnk0C)
                                 ->CreatePostTask(new DinoBattleViewWaitUpdateSupportEffectAnimeResult()));
        break;
    }
    case 0x10063: {
        DinoBattleViewActionInfoBarUpdateRequest *req = (DinoBattleViewActionInfoBarUpdateRequest *)msg;
        func_ov000_0218b4c0((DinoBattleViewActionInfoBar *)DINO_FIELD->mView->GetUnk104(), req->mUnk10);
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewActionInfoBarUpdateResult());
        }
        break;
    }
    case 0x10065: {
        DinoBattleViewFocusLCDFrameRequest *req = (DinoBattleViewFocusLCDFrameRequest *)msg;
        func_ov000_0218b928((DinoBattleViewFocusLCDFrame *)DINO_FIELD->mView->GetUnk108(), req->mUnk10);
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewFocusLCDFrameResult());
        }
        break;
    }
    case 0x10067: {
        DinoBattleViewCreatureSetBlendRequest *req = (DinoBattleViewCreatureSetBlendRequest *)msg;
        if (req->mUnk2C) {
            func_ov000_021614b4(DINO_FIELD);
        }
        for (int i = 0; i < (int)req->mUnk10; i++) {
            DinoBattleViewCreature *creature = DINO_FIELD->mView->FindCreatureByAC(((const u32 *)req->mUnk14)[i]);
            if (creature != NULL) {
                creature->SetUnitModes(TRUE);
                creature->SetMotionPriority(2);
                creature->SetMotionAnchor(0);
            }
        }
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewCreatureSetBlendResult());
        }
        break;
    }
    case 0x10069: {
        DinoBattleViewLCDFadeRequest *req = (DinoBattleViewLCDFadeRequest *)msg;
        if (req->mUnk10 != -1) {
            func_ov000_02161550(DINO_FIELD->mFader, 0, req->mUnk10, req->mUnk14, req->mUnk18);
            if (DINO_FIELD->mTask128 != NULL) {
                func_ov000_0216161c(DINO_FIELD, 0x8C, 30, 12);
            }
        }
        if (req->mUnk1C != -1) {
            func_ov000_02161550(DINO_FIELD->mFader, 1, req->mUnk1C, req->mUnk20, req->mUnk24);
        }
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewLCDFadeResult());
        }
        break;
    }
    case 0x1006B: {
        DinoBattleViewWaitActionInfoBarRequest *req = (DinoBattleViewWaitActionInfoBarRequest *)msg;
        GetSerial()->AddTail(new WaitActionInfoBarTask((DinoBattleViewActionInfoBar *)DINO_FIELD->mView->GetUnk104()),
                             ((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewWaitActionInfoBarResult()));
        break;
    }
    case 0x10055: {
        DinoBattleViewFieldEffectWaitRequest *req = (DinoBattleViewFieldEffectWaitRequest *)msg;
        MTask *post = ((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewFieldEffectWaitResult());
        MTask *wait = MTaskSetWaitEmptyTask::Create(DINO_FIELD->mTasksBC);
        MSerialTaskSet *set = new MSerialTaskSet(TRUE);
        set->AddTail(wait);
        set->AddTail(post);
        GetSerial()->AddTail(set);
        break;
    }
    case 0x1006F: {
        DinoBattleViewShowAttackNameBarRequest *req = (DinoBattleViewShowAttackNameBarRequest *)msg;
        AttackMenuBarMotion *bar =
            new AttackMenuBarMotion(DINO_FIELD->mView, DINO_FIELD->mView->mUnk10C, req->mUnk10, req->mUnk14, 0, 0, 0);
        DinoBattleViewAbilityMenuBG *bg = new DinoBattleViewAbilityMenuBG();
        DinoBattleViewAbilityTextWindow *window = func_ov000_0217c0b8(bg, FALSE);
        window->Show();
        window->SetRect(MRect(-1, 0, 32, 3));
        MPoint offset(0, 16 - ((const int *)data_ov000_02191b1c)[0x74 / 4]);
        if (bg->mUnk10D4) {
            ((MScreen *)bg->mText)->SetOffset(offset);
        } else {
            bg->mOffset = offset;
        }
        bar->Play(2);
        bar->SetPosition(data_ov000_02191b8c);
        bg->SetPriority(1);
        bar->SetPriority(1);
        func_ov000_021615b4(DINO_FIELD, bar, bg, 0);
        DINO_FIELD->mUnk130 = 16;
        if (req->mUnk15) {
            func_ov000_0216161c(DINO_FIELD, 0x8C, 30, 12);
        }
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewShowAttackNameBarResult());
        }
        break;
    }
    case 0x10070: {
        DinoBattleViewMoveAttackNameBarRequest *req = (DinoBattleViewMoveAttackNameBarRequest *)msg;
        func_ov000_0216161c(DINO_FIELD, 0x8C, 30, 12);
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewMoveAttackNameBarResult());
        }
        break;
    }
    case 0x1006D: {
        DinoBattleViewClearAttackBarRequest *req = (DinoBattleViewClearAttackBarRequest *)msg;
        func_ov000_0216170c(DINO_FIELD);
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewClearAttackBarResult());
        }
        break;
    }
    case 0x1007D: {
        DinoBattleViewSetCreaturePriorityRequest *req = (DinoBattleViewSetCreaturePriorityRequest *)msg;
        for (int side = 0; side < 2; side++) {
            DinoBattleViewPlayer *player = DINO_FIELD->mView->GetUnkE4(side);
            for (int i = 0; i < player->GetCreatureCount(); i++) {
                player->GetCreature(i)->SetMotionPriority(2);
                player->GetCreature(i)->SetMotionAnchor(3);
            }
        }
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewSetCreaturePriorityResult());
        }
        break;
    }
    case 0x10081: {
        DinoBattleViewChangeStatusEffectIconRequest *req = (DinoBattleViewChangeStatusEffectIconRequest *)msg;
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleViewChangeStatusEffectIconResult());
        }
        break;
    }
    case 0x10085: {
        DinoBattleViewTutorialWindowRequest *req = (DinoBattleViewTutorialWindowRequest *)msg;
        if (req->mUnk10 != 0) {
            MResource<DinoMessageData> messages = gDinoResource->LoadMessage(gDinoResource->GetTalkMessageMatch(req->mUnk10));
            for (u32 i = 0; i < ((const u32 *)messages.mFile.GetData())[1]; i++) {
                const u8 *data = (const u8 *)messages.mFile.GetData();
                if (req->mUnk10 == *(const u32 *)(data + ((const u32 *)(data + ((const u32 *)data)[2]))[i])) {
                    const u8 *data2 = (const u8 *)messages.mFile.GetData();
                    const u8 *entry = data2 + ((const u32 *)(data2 + ((const u32 *)data2)[2]))[i];
                    func_ov000_0218e600(DINO_FIELD->mTutorialWindow, (const char *)entry + ((const u32 *)entry)[1]);
                    break;
                }
            }
            GetSerial()->AddTail(func_ov000_0218e6b0(DINO_FIELD->mTutorialWindow));
        } else {
            GetSerial()->AddTail(new TutorialWindowCloseTask((u32)DINO_FIELD->mTutorialWindow), new MTimer(1));
        }
        if (req->mUnk0C != 0) {
            GetSerial()->AddTail(((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewTutorialWindowResult()));
        }
        break;
    }
    case 0x10087: {
        DinoBattleViewTutorialSetArroeRequest *req = (DinoBattleViewTutorialSetArroeRequest *)msg;
        GetSerial()->AddTail(new SetTutorialMotionArrorTask(DINO_FIELD->mTutorialMotion, req->mUnk10, req->mUnk14,
                                                            *(const MPoint *)req->mUnk18));
        if (req->mUnk0C != 0) {
            GetSerial()->AddTail(((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewTutorialSetArroeResult()));
        }
        break;
    }
    case 0x10089: {
        DinoBattleViewTutorialSetRectRequest *req = (DinoBattleViewTutorialSetRectRequest *)msg;
        GetSerial()->AddTail(new SetTutorialMotionRectTask(DINO_FIELD->mTutorialMotion, *(const MPoint *)req->mUnk10,
                                                           *(const MPoint *)req->mUnk18));
        if (req->mUnk0C != 0) {
            GetSerial()->AddTail(((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewTutorialSetRectResult()));
        }
        break;
    }
    case 0x1008D: {
        DinoBattleViewLiveMessageSignal *req = (DinoBattleViewLiveMessageSignal *)msg;
        if (((const u8 *)gDinoResource->GetBattleAdjustDefs())[0x191]) {
            break;
        }
        if (req->mUnk0C != 0) {
            MResource<DinoMessageData> messages = gDinoResource->LoadMessage(gDinoResource->GetTalkMessageMatch(req->mUnk0C));
            for (u32 i = 0; i < ((const u32 *)messages.mFile.GetData())[1]; i++) {
                const u8 *data = (const u8 *)messages.mFile.GetData();
                if (req->mUnk0C == *(const u32 *)(data + ((const u32 *)(data + ((const u32 *)data)[2]))[i])) {
                    const u8 *data2 = (const u8 *)messages.mFile.GetData();
                    const u8 *entry = data2 + ((const u32 *)(data2 + ((const u32 *)data2)[2]))[i];
                    func_ov000_0218c3d4((DinoBattleTalkWindow *)DINO_FIELD->mUnk138, (const char *)entry + ((const u32 *)entry)[1]);
                    break;
                }
            }
        } else {
            func_ov000_0218c960((DinoBattleTalkWindow *)DINO_FIELD->mUnk138);
        }
        break;
    }
    case 0x1008E: {
        DinoBattleViewDecideFirstAttackTeamRequest *req = (DinoBattleViewDecideFirstAttackTeamRequest *)msg;
        GetSerial()->AddTail(new DinoBattleViewDecideFirstAttackTeam((u32)DINO_FIELD->mView, req->mUnk10, ((const u32 *)req->mUnk14)[0],
                                                                     ((const u32 *)req->mUnk14)[1]));
        if (req->mUnk0C != 0) {
            GetSerial()->AddTail(
                ((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewDecideFirstAttackTeamResult()));
        }
        break;
    }
    case 0x10095: {
        DinoBattleViewSetAttackBarUpperLCDRequest *req = (DinoBattleViewSetAttackBarUpperLCDRequest *)msg;
        if (req->mUnk0C != 0) {
            GetSerial()->AddTail(
                ((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewSetAttackBarUpperLCDResult()));
        }
        break;
    }
    case 0x10084: {
        for (int side = 0; side < 2; side++) {
            DinoBattleViewPlayer *player = DINO_FIELD->mView->GetUnkE4(side);
            for (int i = 0; i < player->GetCreatureCount(); i++) {
                player->GetCreature(i)->mStatusIcons->ResetIconModes();
            }
        }
        break;
    }
    case 0x10059: {
        DinoBattleViewFreeCoordEffectRequest *req = (DinoBattleViewFreeCoordEffectRequest *)msg;
        // (Two function-local static tables: the effects' motions, then their positions.)
        if (!(data_ov000_021978d0[0] & 1)) {
            ((const char **)data_ov000_02191d9c)[0x38 / 4] = ((const char *const *)data_ov000_02191b1c)[0x4 / 4];
            ((const char **)data_ov000_02191d9c)[0x40 / 4] = ((const char *const *)data_ov000_02191b1c)[0x18 / 4];
            data_ov000_021978d0[0] |= 1;
        }
        if (!(data_ov000_021978d0[1] & 1)) {
            ((int *)data_ov000_02191d9c)[0x48 / 4] = 32;
            ((int *)data_ov000_02191d9c)[0x4C / 4] = 160;
            data_ov000_02191dec[0] = 223;
            data_ov000_02191dec[1] = 160;
            data_ov000_021978d0[1] |= 1;
        }
        MParallelTaskSet *set = new MParallelTaskSet();
        MSmartOam *oam = new MSmartOam(8, 2, TRUE);
        MMotion *motion = new MMotion(std::string(data_ov000_02191dd4[req->mUnk14 * 2]), oam, 0, 1);
        motion->Play(data_ov000_02191dd8[req->mUnk14 * 2], 0);
        motion->SetPriority(0);
        motion->SetPosition(data_ov000_02191de4[req->mUnk10]);
        motion->Show();
        set->AddTail(motion);
        set->AddTail(oam);
        MTask *exit = MTaskExitTransfer::Create(set);
        MTask *wait = motion->CreateWaitStopTask();
        MSerialTaskSet *after = new MSerialTaskSet(TRUE);
        after->AddTail(wait);
        after->AddTail(exit);
        MTaskSet *tasks = DINO_FIELD->mTasksBC;
        tasks->AddTail(set);
        tasks->AddTail(after);
        if (req->mUnk0C != 0) {
            GetSerial()->AddTail(((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleViewFreeCoordEffectResult()));
        }
        break;
    }
    case 0x10082: {
        DinoBattleViewPopNumberRedIfZeroSignal *req = (DinoBattleViewPopNumberRedIfZeroSignal *)msg;
        func_ov000_0217dafc(req->mUnk0C);
        break;
    }
    case 0x10097: {
        DinoBattleViewCountPlayerThinkTimeSignal *req = (DinoBattleViewCountPlayerThinkTimeSignal *)msg;
        DINO_FIELD->mUnk150 = req->mUnk0C;
        break;
    }
    }
}

#undef DINO_FIELD

// The file's static initialization (see dino_static_init.h). The pool is one object: stores past its first 0x80 bytes go
// through the first anchor.
extern "C" u32 data_ov000_02191dcc[];
static struct DinoBattleViewWinnerScrollTaskStaticInit {
    DinoBattleViewWinnerScrollTaskStaticInit() {
        DINO_POOL_WORD(data_ov000_02191b1c, 0x70) = 128;
        DINO_POOL_WORD(data_ov000_02191b1c, 0x74) = 176;
        DINO_POOL_WORD(data_ov000_02191b9c, 0x10) = 8;
        DINO_POOL_WORD(data_ov000_02191b9c, 0x14) = 150;
        DINO_POOL_WORD(data_ov000_02191b1c, 0x78) = 64;
        DINO_POOL_WORD(data_ov000_02191b1c, 0x7c) = 112;
        DINO_POOL_WORD(data_ov000_02191b1c, 0x2a8) = 64;
        DINO_POOL_WORD(data_ov000_02191b1c, 0x2ac) = 96;
        volatile s32 *p = (volatile s32 *)data_ov000_02191dcc;
        p[0] = 112 * 4;
        p[1] = 96;
    }
} sDinoBattleViewWinnerScrollTaskStaticInit;
