// DinoBattleViewCreature.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/creature_motion.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_resource.h"
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

// The adjust defs' layout points.
#define DINO_ADJUST_POINT(name, field)                                          \
    static inline MPoint name(int i) {                                          \
        const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs(); \
        return MPoint(defs->field[i].x, defs->field[i].y);                       \
    }
DINO_ADJUST_POINT(SlotPos98, slotPos98)
DINO_ADJUST_POINT(SlotPosB8, slotPosB8)
DINO_ADJUST_POINT(SlotOffsetB8, slotOffsetB8)
// The same, set field by field.
#define DINO_ADJUST_POINT2(name, field)                                         \
    static inline MPoint name(int i) {                                          \
        const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs(); \
        MPoint pos;                                                             \
        pos.x = defs->field[i].x;                                               \
        pos.y = defs->field[i].y;                                               \
        return pos;                                                             \
    }
DINO_ADJUST_POINT2(SlotPos, slotPos)
DINO_ADJUST_POINT2(SlotPosAlt, slotPosAlt)
DINO_ADJUST_POINT2(SlotOffset, slotOffset)
DINO_ADJUST_POINT2(SlotOffsetAlt, slotOffsetAlt)

// Where things go for a slot kind, by the battle's adjust defs; mirrored for side 1.
extern "C" MPoint func_ov000_0216f294(int side, u32 kind) {
    MPoint pos;
    switch (kind) {
    case 0:
        pos = SlotPos98(0);
        break;
    case 1:
        pos = SlotPos98(2);
        break;
    case 2:
        pos = SlotPos98(1);
        break;
    case 3:
        pos = SlotPos98(3);
        break;
    }
    if (side == 1) {
        pos.x = -pos.x;
    }
    return pos;
}

// Where the slot is.
extern "C" MPoint func_ov000_0216f320(int side, u32 kind, BOOL own) {
    MPoint pos;
    switch (kind) {
    case 0:
        pos = own ? SlotPos(0) : SlotPosAlt(0);
        break;
    case 1:
        pos = own ? SlotPos(2) : SlotPosAlt(2);
        break;
    case 2:
        pos = own ? SlotPos(1) : SlotPosAlt(1);
        break;
    case 3:
        pos = own ? SlotPos(3) : SlotPosAlt(3);
        break;
    }
    if (side == 1) {
        pos.x = -pos.x;
    }
    return pos;
}

extern "C" MPoint func_ov000_0216f434(int side, u32 kind) {
    MPoint pos;
    switch (kind) {
    case 0:
        pos = SlotPosB8(0);
        break;
    case 1:
        pos = SlotPosB8(2);
        break;
    case 2:
        pos = SlotPosB8(1);
        break;
    case 3:
        pos = SlotPosB8(3);
        break;
    }
    if (side == 1) {
        pos.x = -pos.x;
    }
    return pos;
}

// The slot's position moved by its offset.
extern "C" MPoint func_ov000_0216f4c0(int side, u32 kind, BOOL own) {
    MPoint offset;
    switch (kind) {
    case 0:
        offset = own ? SlotOffset(0)
                     : SlotOffsetAlt(0);
        break;
    case 1:
        offset = own ? SlotOffset(2)
                     : SlotOffsetAlt(2);
        break;
    case 2:
        offset = own ? SlotOffset(1)
                     : SlotOffsetAlt(1);
        break;
    case 3:
        offset = own ? SlotOffset(3)
                     : SlotOffsetAlt(3);
        break;
    }
    if (side == 1) {
        offset.x = -offset.x;
    }
    return offset + func_ov000_0216f320(side, kind, own);
}

extern "C" MPoint func_ov000_0216f600(int side, u32 kind) {
    MPoint offset;
    switch (kind) {
    case 0:
        offset = SlotOffsetB8(0);
        break;
    case 1:
        offset = SlotOffsetB8(2);
        break;
    case 2:
        offset = SlotOffsetB8(1);
        break;
    case 3:
        offset = SlotOffsetB8(3);
        break;
    }
    if (side == 1) {
        offset.x = -offset.x;
    }
    return offset + func_ov000_0216f434(side, kind);
}

extern "C" int func_ov000_0216f698(int arg0, BOOL arg1, BOOL arg2) {
    if (arg0 == 3) {
        return 5;
    }
    if (arg1) {
        if (arg2) {
            return 2;
        }
        return 3;
    }
    return 4;
}

void CreatureMotion::SetArg(int arg, BOOL flag) {
    mArg = arg | (flag ? 2 : 0);
}

BOOL DinoBattleViewCreature::IsUnkCC2() {
    return (mUnkCC & 2) ? TRUE : FALSE;
}

int DinoGetTypeAdvantage(int type, int other); // 1: type beats other, 2: other beats type

void DinoBattleViewCreature::SetAdvantage(DinoBattleViewCreature *other) {
    if (other != NULL) {
        mAdvantage = DinoGetTypeAdvantage(GetDefUnk0D(), other->GetDefUnk0D());
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
