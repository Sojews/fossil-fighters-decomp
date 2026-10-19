// DinoBattleViewField.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_field.h"
#include "game/dino_resource.h"
#include "types.h"

DinoBattleViewField::~DinoBattleViewField() {
}


static inline const MPoint *DinoFieldPoints() {
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    return (const MPoint *)((const u8 *)defs + defs->fieldPoints);
}

static inline MPoint DinoFieldPoint(int i) {
    return DinoFieldPoints()[i];
}

// Where a side's view sits for a slot kind (mirrored for the second side).
extern "C" MPoint func_ov000_0215f22c(int kind, int side) {
    MPoint pos = DinoFieldPoint(1);
    switch (kind) {
    case 0:
        pos.x = -128;
        break;
    case 1:
        pos = DinoFieldPoint(0);
        break;
    case 2:
        break;
    case 3:
        pos = DinoFieldPoint(2);
        break;
    case 4:
        pos = DinoFieldPoint(3);
        break;
    case 5:
        pos = DinoFieldPoint(4);
        break;
    }
    if (side != 0) {
        pos.x = 256 - pos.x;
    }
    return pos;
}
