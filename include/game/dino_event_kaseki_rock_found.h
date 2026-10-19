// The fossil rock found screen.
#ifndef GAME_DINO_EVENT_KASEKI_ROCK_FOUND_H
#define GAME_DINO_EVENT_KASEKI_ROCK_FOUND_H

#include "m/task.h"
#include "m/archive.h"
#include "m/geometry.h"

class DinoEventLoop;

class DinoEventKasekiRockFound : public MProcess {
public:
    DinoEventKasekiRockFound(DinoEventLoop *event, u32 arg, MPoint pos);
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
    MPoint mPos;            // 0x80
    u32 mArg;               // 0x88
    u32 mUnk8C[9];          // 0x8C
    MArchivePtr mArchive;   // 0xB0
    MArchiveFile mFile;     // 0xB4
    MArchivePtr mArchive2;  // 0xC0
    MArchiveFile mFile2;    // 0xC4
    u32 unkD0[2];
    int mAnswer;            // 0xD8: the yes/no dialog's
    u32 unkDC[2];
};

#endif
