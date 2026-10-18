// The fossil rock found screen.
#ifndef GAME_DINO_EVENT_KASEKI_ROCK_FOUND_H
#define GAME_DINO_EVENT_KASEKI_ROCK_FOUND_H

#include "m/task.h"
#include "m/archive.h"

class DinoEventLoop;

class DinoEventKasekiRockFound : public MProcess {
public:
    virtual ~DinoEventKasekiRockFound();
    virtual void Idle();

    MTask *CreateFoundMessage();
    MTask *CreateYesNo(u32 message);
    void StateFound();
    void StateFoundEnd();
    void StateAsk();
    void StateAnswered();

    void (DinoEventKasekiRockFound::*mState)(); // 0x74
    DinoEventLoop *mEvent;      // 0x7C
    u32 mUnk80[12];         // 0x80
    MArchivePtr mArchive;   // 0xB0
    MArchiveFile mFile;     // 0xB4
    MArchivePtr mArchive2;  // 0xC0
    MArchiveFile mFile2;    // 0xC4
    u32 unkD0[2];
    int mAnswer;            // 0xD8: the yes/no dialog's
};

#endif
