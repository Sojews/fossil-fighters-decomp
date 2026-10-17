// Moves the player character from the pad and touch screen.
#ifndef GAME_DINO_FIELD_PLAYER_CONTROL_H
#define GAME_DINO_FIELD_PLAYER_CONTROL_H

#include "m/task.h"
#include "m/archive.h"

class DinoFieldPlayerControl : public MProcess {
public:
    virtual ~DinoFieldPlayerControl();
    virtual void Idle();

    u32 mUnk74[16];         // 0x74
    MArchivePtr mArchive;   // 0xB4
    MArchiveFile mFile;     // 0xB8
};

#endif
