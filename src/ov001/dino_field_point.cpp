// DinoFieldPoint.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_field_point.h"
#include "types.h"

DinoFieldPoint::~DinoFieldPoint() {
}

DinoFieldMoney::~DinoFieldMoney() {
}

DinoFieldDonate::~DinoFieldDonate() {
}

extern const char *data_ov001_0216ab40[]; // motion file paths
extern u32 data_0208a214;
extern u32 data_0208a218;
extern u32 data_0208a21c;

// NONMATCHING (both): the game loads a before storing b; scheduling only.
DinoFieldMoney::DinoFieldMoney() : DinoFieldPoint(data_ov001_0216ab40[4], 8, data_0208a21c, data_0208a218, data_0208a214) {
}

DinoFieldDonate::DinoFieldDonate() : DinoFieldPoint(data_ov001_0216ab40[2], 5, data_0208a21c, data_0208a218, data_0208a214) {
}
