// DinoCommonLive.

#define DINO_COMMON_LIVE_OWNER
#include "game/dino_common_live.h"
#include "m/resource.h"
#include "nitro/mi.h"
#include "game/dino_resource.h"
#include "m/seed_generator.h"
#include "m/lcd.h"
#include <string.h>

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

// A line's condition: its kind, then count operands at offset.
struct DinoCommonLiveCondition {
    u32 kind;       // 0x00
    u32 count;      // 0x04
    u32 offset;     // 0x08: the operands (u32 each)

    const u32 *GetArgs() const { return (const u32 *)((const u8 *)this + offset); }
};

// NONMATCHING: the flag loops step a pointer through the operands (the game indexes them), and the flag test is
// materialized rather than kept as TRUE-then-cleared.
BOOL DinoCommonLive::CheckCondition(const DinoCommonLiveCondition *cond) {
    switch (cond->kind) {
    default:
        return FALSE;
    case 0:
        return mUnkB8[cond->GetArgs()[0]] == mUnkB8[cond->GetArgs()[1]] ? TRUE : FALSE;
    case 1:
        return mUnkB8[cond->GetArgs()[0]] != mUnkB8[cond->GetArgs()[1]] ? TRUE : FALSE;
    case 2:
        return (int)mUnkB8[cond->GetArgs()[0]] < (int)mUnkB8[cond->GetArgs()[1]] ? TRUE : FALSE;
    case 3:
        return (int)mUnkB8[cond->GetArgs()[0]] <= (int)mUnkB8[cond->GetArgs()[1]] ? TRUE : FALSE;
    case 4:
        return (int)mUnkB8[cond->GetArgs()[0]] > (int)mUnkB8[cond->GetArgs()[1]] ? TRUE : FALSE;
    case 5:
        return (int)mUnkB8[cond->GetArgs()[0]] >= (int)mUnkB8[cond->GetArgs()[1]] ? TRUE : FALSE;
    case 6:
        return mUnkB8[cond->GetArgs()[0]] == cond->GetArgs()[1] ? TRUE : FALSE;
    case 7:
        return mUnkB8[cond->GetArgs()[0]] != cond->GetArgs()[1] ? TRUE : FALSE;
    case 8:
        return (int)mUnkB8[cond->GetArgs()[0]] < (int)cond->GetArgs()[1] ? TRUE : FALSE;
    case 9:
        return (int)mUnkB8[cond->GetArgs()[0]] <= (int)cond->GetArgs()[1] ? TRUE : FALSE;
    case 10:
        return (int)mUnkB8[cond->GetArgs()[0]] > (int)cond->GetArgs()[1] ? TRUE : FALSE;
    case 11:
        return (int)mUnkB8[cond->GetArgs()[0]] >= (int)cond->GetArgs()[1] ? TRUE : FALSE;
    case 12:
        for (u32 i = 0; i < cond->count; i++) {
            u32 flag = cond->GetArgs()[i];
            BOOL set = IsFlagSet(flag);
            if (!set) {
                return FALSE;
            }
        }
        return TRUE;
    case 13:
        for (u32 i = 0; i < cond->count; i++) {
            u32 flag = cond->GetArgs()[i];
            BOOL set = IsFlagSet(flag);
            if (set) {
                return TRUE;
            }
        }
        return FALSE;
    case 14:
        for (u32 i = 0; i < cond->count; i++) {
            u32 flag = cond->GetArgs()[i];
            BOOL set = IsFlagSet(flag);
            if (set) {
                return FALSE;
            }
        }
        return TRUE;
    case 15:
        for (u32 i = 0; i < cond->count; i++) {
            u32 flag = cond->GetArgs()[i];
            BOOL set = IsFlagSet(flag);
            if (!set) {
                return TRUE;
            }
        }
        return FALSE;
    }
}

// The data's line (a table of offsets after the header).
static inline u8 *DinoLiveLine(MResource<DinoCommonLiveData> &res, u32 i) {
    u8 *data = (u8 *)res.mFile.GetData();
    return data + ((u32 *)(data + ((u32 *)data)[2]))[i];
}

void DinoCommonLive::Idle() {
    u8 found = FALSE;
    u32 priority = mUnkC8;
    u32 pick = found;
    std::vector<void *> ties;
    if (mUnkD2) {
        for (u32 i = found; i < ((u32 *)mData.mFile.GetData())[1]; i++) {
            DinoCommonLiveEntry *entry = &mUnk94[i];
            if (entry->mUnk04 == 0) {
                continue;
            }
            if (entry->unk08 != 0 && --entry->unk08 != 0) {
                continue;
            }
            u8 *line = DinoLiveLine(mData, i);
            if (mUnkD1) {
                if (priority > *(u32 *)(line + 0x10) || mUnkC8 >= *(u32 *)(line + 0x10)) {
                    continue;
                }
            } else if (priority >= *(u32 *)(line + 0x10)) {
                continue;
            }
            BOOL ok = TRUE;
            for (u32 j = 0; j < *(u32 *)line; j++) {
                if (!CheckCondition(
                        (const DinoCommonLiveCondition *)(line + ((u32 *)(line + *(u32 *)(line + 4)))[j]))) {
                    ok = FALSE;
                    break;
                }
            }
            if (!ok) {
                continue;
            }
            if (mUnkD1) {
                if (priority < *(u32 *)(line + 0x10)) {
                    ties.clear();
                }
                ties.push_back((void *const &)i);
            }
            found = TRUE;
            priority = *(u32 *)(line + 0x10);
            pick = i;
        }
    }
    if (found) {
        u32 count;
        if (mUnkD1 && (count = ties.size()) != 0) {
            pick = (u32)ties[gDinoResource->GetRandom()->Next(count)];
        }
        u8 *data = (u8 *)mData.mFile.GetData();
        u8 *line = data + ((u32 *)(data + ((u32 *)data)[2]))[pick];
        u32 variant;
        if (*(u32 *)(line + 8) == 1) {
            variant = 0;
        } else {
            do {
                variant = gDinoResource->GetRandom()->Next(*(u32 *)(line + 8));
            } while (variant == mUnk94[pick].unk00);
        }
        mUnk94[pick].unk00 = variant;
        u32 message = ((u32 *)(line + *(u32 *)(line + 0xC)))[variant];
        mUnkC8 = priority;
        mUnk94[pick].unk08 = *(u32 *)(line + 0x14);
        mUnk94[pick].mUnk04--;
        if (mUnkD0) {
            Signal(message);
            mOpen = 200;
        } else {
            mMessages = gDinoResource->LoadMessage(gDinoResource->GetTalkMessageMatch(message));
            const char *text = "";
            for (u32 i = 0; i < ((u32 *)mMessages.mFile.GetData())[1]; i++) {
                u8 *messages = (u8 *)mMessages.mFile.GetData();
                if (message == *(u32 *)(messages + ((u32 *)(messages + ((u32 *)messages)[2]))[i])) {
                    u8 *messages2 = (u8 *)mMessages.mFile.GetData();
                    u8 *entry = messages2 + ((u32 *)(messages2 + ((u32 *)messages2)[2]))[i];
                    text = (const char *)(entry + ((u32 *)entry)[1]);
                    break;
                }
            }
            if (strncmp((char *)text, "$upper", 6) == 0) {
                ShowTutorial(gLcd->GetTopDisplay(), text + 6);
                mUnkC8 = 0;
                mOpen = 0;
            } else if (strncmp((char *)text, "$lower", 6) == 0) {
                ShowTutorial(gLcd->GetBottomDisplay(), text + 6);
                mUnkC8 = 0;
                mOpen = 0;
            } else {
                Open(text);
                mOpen = 180;
            }
        }
    }
    if (mOpen != 0 && IsBusy()) {
        mOpen++;
    }
    if (mOpen != 0 && --mOpen == 0) {
        Close();
        mUnkC8 = 0;
    }
    MI_CpuFill8(mUnkAC.mBytes, 0, (mUnkAC.mBits + 7) >> 3);
}

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
