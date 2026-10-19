// DinoBattleViewWinnerScrollTask, DinoBattleViewBackdrop.
// NONMATCHING: DinoBattleViewBackdrop::Execute (spilled values in other stack slots, registers in the loop).

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

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
