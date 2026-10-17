#ifndef GAME_DINO_COMMON_LIVE_H
#define GAME_DINO_COMMON_LIVE_H

#include <vector>
#include "m/task.h"
#include "m/archive.h"
#include "m/bit_vector.h"

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

    MArchivePtr mArchive;               // 0x74
    MArchiveFile mFile;                 // 0x78
    MArchivePtr mArchive2;              // 0x84
    MArchiveFile mFile2;                // 0x88
    std::vector<unsigned char> mUnk94;  // 0x94: (the element type is a guess, distinct from other modules')
    MBitVector mUnkA0;                  // 0xA0
    MBitVector mUnkAC;                  // 0xAC
    std::vector<void *> mUnkB8;         // 0xB8
};

#endif
