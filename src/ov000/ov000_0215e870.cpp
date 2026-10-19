// Ov0000215e870.
// Only some functions are decompiled so far.

#include "game/dino_sound.h"
#include "types.h"
#include "game/motion_advantage_arrow.h"

#include "m/motion.h"
#include "game/dino_resource.h"
#include "game/dino_battle_view_field_player_bg.h"
#include "game/dino_battle_view_class.h"
#include "m/picture.h"

// The pooled constants the code reads.
extern "C" struct {
    u8 unk00[0x08];
    const char *mPointerPath;   // 0x08: the tutorial's pointing arrow
    u8 unk0C[0x28 - 0x0C];
    const char *mPointerPath2;  // 0x28: its other kind (directions above 4)
    const char *mArrowPath;     // 0x2C: the advantage arrow
} data_ov000_02191b1c;

DinoBattleViewTutorialMotion::DinoBattleViewTutorialMotion(MSmartOam *oam) : MProcess(NULL) {
    mOam = oam;
    for (int i = 0; i < 3; i++) {
        mArrows[i] = NULL;
    }
    mRect = NULL;
}

// Points an arrow (replacing the one there) at pos.
void DinoBattleViewTutorialMotion::ShowArrow(int index, int dir, MPoint pos) {
    if (mArrows[index] != NULL) {
        mArrows[index]->Exit();
    }
    MMotion *arrow;
    if (dir > 4) {
        DinoSoundPlaySe(0x397, 0);
        arrow = new MMotion(std::string(data_ov000_02191b1c.mPointerPath2), mOam, 0, 1);
        dir -= 4;
    } else {
        DinoSoundPlaySe(0x397, 0);
        arrow = new MMotion(std::string(data_ov000_02191b1c.mPointerPath), mOam, 0, 1);
    }
    arrow->Show();
    arrow->SetPosition(pos);
    arrow->SetPriority(0);
    arrow->Play(dir, 0);
    mParallel.AddTail(arrow);
    mArrows[index] = arrow;
}

// NONMATCHING: register allocation in the -1 loop (the game reuses the -1 register as the counter) and the
// rectangle's NULL is rematerialized.
void DinoBattleViewTutorialMotion::Hide(int index) {
    if (index == -1) {
        for (int i = 0; i < 3; i++) {
            if (mArrows[i] != NULL) {
                mArrows[i]->Exit();
            }
            mArrows[i] = NULL;
        }
        if (mRect != NULL) {
            mRect->Exit();
            mRect = NULL;
        }
    } else if (mArrows[index] != NULL) {
        mArrows[index]->Exit();
        mArrows[index] = NULL;
    }
}

void DinoBattleViewAdvantageArrow::Idle() {
}

MotionAdvantageArrow::MotionAdvantageArrow(const std::string &path, MSmartOam *oam)
    : MMotion(std::string(path), oam, 0, 1) {
}

void MotionAdvantageArrow::Unk44(int arg) {
    if (arg == 0x16 && !mUnk7C) {
        DinoSoundPlaySe(0x5C, 0);
    }
}


// NONMATCHING: 98%, the game halves the sums straight from registers (ours stores them first).
DinoBattleViewAdvantageArrow::DinoBattleViewAdvantageArrow(u32 unk74, MSmartOam *oam) : MProcess(NULL) {
    mUnk74 = unk74;
    mOam = oam;
    mUnk80 = 2;
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    mPoints[0] = MPoint(defs->advantageArrowX0, defs->advantageArrowY0);
    defs = gDinoResource->GetBattleAdjustDefs();
    mPoints[1] = MPoint(defs->advantageArrowX1, defs->advantageArrowY1);
    mArrow = new MotionAdvantageArrow(std::string(data_ov000_02191b1c.mArrowPath), mOam);
    MPoint mid;
    mid.y = mPoints[0].y + mPoints[1].y;
    mid.x = mPoints[0].x + mPoints[1].x;
    mid.x /= 2;
    mid.y /= 2;
    mArrow->SetPosition(mid);
    mArrow->SetPriority(2);
    mArrow->SetAnchor(0);
    mArrow->Hide();
    mParallel.AddTail(mArrow);
}

void DinoBattleViewAdvantageArrow::PointAt(int side) {
    MPoint to = mPoints[side == 1 ? 1 : 0];
    if (mState == STATE_SUSPEND) {
        mArrow->SetPosition(to);
    }
    mParallel.AddTail(mArrow->CreateCoordAnime(mArrow->GetPosition(), to, 40, 1));
}

// The player BG's pooled constants (see ov000_0215ef04.cpp).
extern "C" struct {
    u8 unk00[0x20];
    void (DinoBattleViewFieldPlayerBG::*StateDone_20)();
} data_ov000_02191b9c;
extern "C" const char data_ov000_02192484[];    // the default background

DinoBattleViewFieldPlayerBG::DinoBattleViewFieldPlayerBG(DinoBattleView *view) : MProcess(NULL) {
    mSide = 2;
    mPicture = NULL;
    mChanging = FALSE;
    mView = view;
    mFixed = gDinoResource->GetBattleAdjustDefs()->fixedFieldBg;
    if (mFixed != 0) {
        const DinoBattleBgDefs *defs = gDinoResource->GetBattleBgDefs();
        for (u32 i = 0; i < defs->count; i++) {
            u32 battle = defs->Get(i)->battle;
            u32 cur = mView->GetUnk134();
            if (cur == battle) {
                mPicture = new MTextPicture(std::string(defs->GetPath(i)), TRUE, true);
                break;
            }
        }
        if (mPicture == NULL) {
            mPicture = new MTextPicture(std::string(data_ov000_02192484), TRUE, true);
        }
        mParallel.AddTail(mPicture);
        mPicture->Show();
        mPicture->SetPriority(3);
    }
    mState = data_ov000_02191b9c.StateDone_20;
}
