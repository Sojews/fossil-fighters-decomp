// DinoBattleViewCreature.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/creature_motion.h"
#include "game/dino_battle_view_creature.h"
#include "types.h"

DinoBattleViewCreature::~DinoBattleViewCreature() {
}

void CreatureMotion::Unk44(int arg) {
    switch (arg) {
    case 1:
        DinoSoundPlaySe(0x47, 0);
        return;
    case 2:
        DinoSoundPlaySe(0x48, 0);
        return;
    }
}
