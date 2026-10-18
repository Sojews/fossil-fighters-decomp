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

CreatureMotion::CreatureMotion(const char *path, MSmartOam *oam, int arg) : MMotion(std::string(path), oam, 3, 1) {
    mArg = arg;
    mUnk80 = 0;
    mUnk84 = 0;
    mUnk88 = 0;
    mOam = oam;
}

#pragma auto_inline off

void CreatureMotion::SetArg(int arg, BOOL flag) {
    mArg = arg | (flag ? 2 : 0);
}

BOOL DinoBattleViewCreature::IsUnkCC2() {
    return (mUnkCC & 2) ? TRUE : FALSE;
}

void DinoBattleViewCreature::ClearUnk160() {
    mUnk160 = 0;
}

MPoint DinoBattleViewCreature::GetUnk124() {
    return mUnk124;
}

MPoint DinoBattleViewCreature::GetMotionPosition() {
    return mMotion->GetPosition();
}
