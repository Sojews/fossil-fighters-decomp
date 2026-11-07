#ifndef GAME_DINO_COMMON_LIVE_H
#define GAME_DINO_COMMON_LIVE_H

#include <vector>
#include "m/task.h"
#include "m/archive.h"
#include "m/bit_vector.h"
#include "m/resource.h"

// The commentary data (a count of lines, then their offsets).
struct DinoCommonLiveData;

// An entry of mUnk94 (12 bytes).
struct DinoCommonLiveEntry {
    int unk00;
    u32 mUnk04;     // 0x04: from the data's line
    u32 unk08;
};

class MDisplay;

class DinoCommonLive : public MProcess {
public:
    // The game's destructor is inline. Its vtable's owner (dino_common_live.cpp) defines DINO_COMMON_LIVE_OWNER and
    // emits it out of line until the key function there is decompiled.
#ifdef DINO_COMMON_LIVE_OWNER
    virtual ~DinoCommonLive();
#else
    virtual ~DinoCommonLive() {}
#endif
    // Says the highest-priority line whose conditions hold (a random one of the ties if mUnkD1), then counts down
    // until it closes.
    virtual void Idle();
    virtual void Open(const char *text) = 0;
    virtual void Signal(u32 id);
    virtual void Close() = 0;
    virtual void ShowTutorial(MDisplay *display, const char *text) = 0;
    virtual BOOL IsBusy();

    DinoCommonLive(std::string name, u32 arg1, u32 arg2);
    void CloseIfOpen();
    void ClearUnkA0();
    void SetUnkAC(u32 index);
    void SetUnkA0(u32 index, BOOL value);
    void SetUnkB8(int index, void *value);
    // Whether either flag vector has the flag.
    BOOL IsFlagSet(u32 flag) {
        BOOL set = TRUE;
        u32 mask = 1 << (flag & 7);
        if (!(mUnkA0.mBytes[flag >> 3] & mask)) {
            if (!(mUnkAC.mBytes[flag >> 3] & mask)) {
                set = FALSE;
            }
        }
        return set;
    }
    // Whether a line's condition holds: the flags (0: all set, 1: any set, 2: none set, 3: not all set), two
    // variables compared (4-9), or a variable and a constant (10-15).
    BOOL CheckCondition(const struct DinoCommonLiveCondition *cond);
    void SetUnkD2(u8 value);
    u8 GetUnkD2();
    void RefreshEntries();

    MResource<DinoCommonLiveData> mData;    // 0x74
    MResource<struct DinoMessageData> mMessages; // 0x84: the messages of the line last said
    std::vector<DinoCommonLiveEntry> mUnk94;    // 0x94: one per line
    MBitVector mUnkA0;                  // 0xA0
    MBitVector mUnkAC;                  // 0xAC
    std::vector<u32> mUnkB8;            // 0xB8
    u8 mUnkC4;                          // 0xC4
    u32 mUnkC8;                         // 0xC8
    u32 mOpen;                          // 0xCC
    u8 mUnkD0;                          // 0xD0
    u8 mUnkD1;                          // 0xD1
    u8 mUnkD2;                          // 0xD2
};

#endif
