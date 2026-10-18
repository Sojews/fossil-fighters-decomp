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

extern "C" int func_ov000_0216ecc0(int a, int b); // the type advantage of a over b

void DinoBattleViewCreature::SetAdvantage(DinoBattleViewCreature *other) {
    if (other != NULL) {
        mAdvantage = func_ov000_0216ecc0(GetDefUnk0D(), other->GetDefUnk0D());
    } else {
        mAdvantage = 0;
    }
}

void DinoBattleViewCreature::ClearAdvantage() {
    mAdvantage = 0;
}

void DinoBattleViewCreature::SetCoord(MPoint pos) {
    mCoord = pos;
    mMotion->SetPosition(mCoord + mCoordOffset);
}

MPoint DinoBattleViewCreature::GetCoord() {
    return mCoord;
}

MPoint DinoBattleViewCreature::GetMotionPosition() {
    return mMotion->GetPosition();
}
