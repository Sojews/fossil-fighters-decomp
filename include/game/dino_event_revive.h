// The revival event (cleaning a fossil rock and reviving the vivosaur).
#ifndef GAME_DINO_EVENT_REVIVE_H
#define GAME_DINO_EVENT_REVIVE_H

#include <vector>
#include "m/task.h"
#include "m/archive.h"

class DinoEventRevive : public MProcess {
public:
    virtual ~DinoEventRevive();
    virtual void Idle();

    void (DinoEventRevive::*mState)(); // 0x74
    u32 mUnk7C[19];
    MArchivePtr mArchive;       // 0xC8
    MArchiveFile mFile;         // 0xCC
    u32 mUnkD8[2];              // 0xD8
    std::vector<void *> mUnkE0; // 0xE0
};

#endif
