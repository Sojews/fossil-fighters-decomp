// Ov0000215e870.
// Only the destructors are decompiled so far.

#include "game/dino_sound.h"
#include "types.h"
#include "game/motion_advantage_arrow.h"

void DinoBattleViewAdvantageArrow::Idle() {
}

void MotionAdvantageArrow::Unk44(int arg) {
    if (arg == 0x16 && !mUnk7C) {
        DinoSoundPlaySe(0x5C, 0);
    }
}
