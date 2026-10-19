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
    virtual void Idle();
    virtual void Open() = 0;
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
    void SetUnkD2(u8 value);
    u8 GetUnkD2();
    void RefreshEntries();

    MResource<DinoCommonLiveData> mData;    // 0x74
    MResource<DinoCommonLiveData> mData2;   // 0x84
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
