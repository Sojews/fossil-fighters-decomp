// DinoCommonLive.
// Only the destructor is decompiled so far.

#define DINO_COMMON_LIVE_OWNER
#include "game/dino_common_live.h"
#include "m/resource.h"
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

#pragma auto_inline off

void DinoCommonLive::Signal(u32 id) {
}

BOOL DinoCommonLive::IsBusy() {
    return FALSE;
}

void DinoCommonLive::CloseIfOpen() {
    if (mOpen) {
        mOpen = 0;
        Close();
        mUnkC8 = 0;
    }
}

void DinoCommonLive::SetUnkD2(u8 value) {
    mUnkD2 = value;
}

u8 DinoCommonLive::GetUnkD2() {
    return mUnkD2;
}

// Resources the file loads (their data types are unknown); until the code that constructs them is decompiled,
// these keep out-of-line copies of the constructors.
struct DinoCommonLiveData;
#pragma dont_inline on
MResource<DinoCommonLiveData> DinoCommonLiveDataLoad(std::string path) {
    return MResource<DinoCommonLiveData>(path);
}
#pragma dont_inline reset
