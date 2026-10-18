// DinoFieldHole.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_field_hole.h"
#include "types.h"

DinoFieldHole::~DinoFieldHole() {
}

void DinoFieldHole::Idle() {
    (this->*mState)();
}

u8 DinoFieldHole::GetUnk110() {
    return mUnk110;
}

void DinoFieldHole::SetUnk114() {
    mUnk114 = 0xB1;
}

MPoint DinoFieldHole::GetPos() {
    return mPos;
}
