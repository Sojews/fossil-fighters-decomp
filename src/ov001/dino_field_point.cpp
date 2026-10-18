// DinoFieldPoint.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_field_point.h"
#include "types.h"
#include "game/dino_backup.h"
#include "nitro/os.h"

DinoFieldPoint::~DinoFieldPoint() {
}

DinoFieldMoney::~DinoFieldMoney() {
}

DinoFieldDonate::~DinoFieldDonate() {
}

extern const char *const data_ov001_0216ab40[]; // motion file paths
extern const u32 data_0208a214;
extern const u32 data_0208a218;
extern const u32 data_0208a21c;

DinoFieldMoney::DinoFieldMoney() : DinoFieldPoint(data_ov001_0216ab40[4], 8, data_0208a21c, data_0208a218, data_0208a214) {
}

DinoFieldDonate::DinoFieldDonate() : DinoFieldPoint(data_ov001_0216ab40[2], 5, data_0208a21c, data_0208a218, data_0208a214) {
}

extern "C" const char data_ov001_0216ae40[]; // the money format
extern "C" const char data_ov001_0216ae44[]; // the donation format

void DinoFieldMoney::Format(char *buf) {
    OS_SPrintf(buf, data_ov001_0216ae40, gDinoBackup->GetSystemVar(3));
}

void DinoFieldDonate::Format(char *buf) {
    OS_SPrintf(buf, data_ov001_0216ae44, gDinoBackup->GetSystemVar(0x13));
}
