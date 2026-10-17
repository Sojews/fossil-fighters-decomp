// DinoCommonLive.
// Only the destructor is decompiled so far.

#define DINO_COMMON_LIVE_OWNER
#include "game/dino_common_live.h"
#include "nitro/mi.h"

void DinoCommonLive::ClearUnkA0() {
    MI_CpuFill8(mUnkA0.mBytes, 0, (mUnkA0.mBits + 7) >> 3);
}

void DinoCommonLive::SetUnkAC(u32 index) {
    mUnkAC.Set(index);
}

void DinoCommonLive::SetUnkA0(u32 index, BOOL value) {
    mUnkA0[index] = value;
}

void DinoCommonLive::SetUnkB8(int index, void *value) {
    mUnkB8[index] = value;
}

DinoCommonLive::~DinoCommonLive() {
}
