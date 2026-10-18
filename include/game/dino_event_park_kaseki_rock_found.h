#ifndef GAME_DINO_EVENT_PARK_KASEKI_ROCK_FOUND_H
#define GAME_DINO_EVENT_PARK_KASEKI_ROCK_FOUND_H

#include "m/task.h"
#include "game/dino_backup.h"

class DinoEventLoop;
class DinoFieldChara;

class DinoEventParkKasekiRockFound : public MProcess {
public:
    DinoEventParkKasekiRockFound(DinoEventLoop *event, u32 index);
    virtual ~DinoEventParkKasekiRockFound();
    virtual void Idle();

    void StateStart();

    void (DinoEventParkKasekiRockFound::*mState)(); // 0x74
    DinoEventLoop *mEvent;      // 0x7C
    u32 mIndex;                 // 0x80: of the rock in the park
    DinoBackupEntry28 mEntry;   // 0x84: the rock, taken out of the park
    u32 unkAC;
    DinoFieldChara *mChara;     // 0xB0: the one the camera follows
    u32 unkB4;
};

#endif
