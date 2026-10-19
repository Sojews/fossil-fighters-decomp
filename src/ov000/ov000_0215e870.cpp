// Ov0000215e870.
// Only some functions are decompiled so far.

#include "game/dino_sound.h"
#include "types.h"
#include "game/motion_advantage_arrow.h"

#include "m/motion.h"
#include "game/dino_resource.h"

DinoBattleViewTutorialMotion::DinoBattleViewTutorialMotion(MSmartOam *oam) : MProcess(NULL) {
    mOam = oam;
    for (int i = 0; i < 3; i++) {
        mArrows[i] = NULL;
    }
    mRect = NULL;
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

// The pooled constants the code reads.
extern "C" struct {
    u8 unk00[0x2C];
    const char *mArrowPath;     // 0x2C
} data_ov000_02191b1c;

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
