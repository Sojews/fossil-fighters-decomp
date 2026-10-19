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

#pragma auto_inline off

// An entry of mUnk94 (12 bytes).
struct DinoCommonLiveEntry {
    u32 unk00;
    u32 mUnk04;     // 0x04: from the data's line
    u32 unk08;
};

// Rereads each entry's value from the commentary data.
void DinoCommonLive::RefreshEntries() {
    for (u32 i = 0; i < ((u32 *)mFile.GetData())[1]; i++) {
        u8 *data = (u8 *)mFile.GetData();
        u8 *line = data + ((u32 *)(data + ((u32 *)data)[2]))[i];
        ((DinoCommonLiveEntry *)&mUnk94[0])[i].mUnk04 = *(u32 *)(line + 0x18);
    }
}
