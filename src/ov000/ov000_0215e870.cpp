// Ov0000215e870.
// Only the destructors are decompiled so far.

#include "game/dino_sound.h"
#include "types.h"
#include "game/motion_advantage_arrow.h"

#include "m/motion.h"

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

void MotionAdvantageArrow::Unk44(int arg) {
    if (arg == 0x16 && !mUnk7C) {
        DinoSoundPlaySe(0x5C, 0);
    }
}
