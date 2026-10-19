// DinoCommonLive.

#define DINO_COMMON_LIVE_OWNER
#include "game/dino_common_live.h"
#include "m/resource.h"
#include "nitro/mi.h"

// The TU's pooled data (from 0x020bb324).
extern "C" struct {
    u32 unk0;
    const char *mPathPrefix;    // 0x04
} data_020bb324;
extern "C" const u32 data_020bb32c; // 0

DinoCommonLive::DinoCommonLive(std::string name, u32 arg1, u32 arg2)
    : MProcess(NULL), mData(data_020bb324.mPathPrefix + name), mUnk94(((u32 *) mData.mFile.GetData())[1]),
      mUnkA0(0xB8), mUnkAC(0xB8), mUnkB8(13, data_020bb32c) {
    mUnkC4 = 0;
    mUnkC8 = 0;
    mOpen = 0;
    mUnkD0 = arg1;
    mUnkD1 = arg2;
    mUnkD2 = 1;
    MI_CpuFill8(mUnkA0.mBytes, 0, (mUnkA0.mBits + 7) >> 3);
    MI_CpuFill8(mUnkAC.mBytes, 0, (mUnkAC.mBits + 7) >> 3);
    for (u32 i = 0; i < ((u32 *) mData.mFile.GetData())[1]; i++) {
        u8 *data = (u8 *) mData.mFile.GetData();
        u8 *line = data + ((u32 *) (data + ((u32 *) data)[2]))[i];
        mUnk94[i].unk00 = -1;
        mUnk94[i].mUnk04 = *(u32 *) (line + 0x18);
        mUnk94[i].unk08 = 0;
    }
}

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
    mUnkB8[index] = (u32) value;
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


#pragma auto_inline off

// Rereads each entry's value from the commentary data.
void DinoCommonLive::RefreshEntries() {
    for (u32 i = 0; i < ((u32 *)mData.mFile.GetData())[1]; i++) {
        u8 *data = (u8 *)mData.mFile.GetData();
        u8 *line = data + ((u32 *)(data + ((u32 *)data)[2]))[i];
        mUnk94[i].mUnk04 = *(u32 *)(line + 0x18);
    }
}
