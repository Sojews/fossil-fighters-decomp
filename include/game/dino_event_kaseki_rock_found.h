// The fossil rock found screen.
#ifndef GAME_DINO_EVENT_KASEKI_ROCK_FOUND_H
#define GAME_DINO_EVENT_KASEKI_ROCK_FOUND_H

#include "m/task.h"
#include "m/resource.h"
#include "game/dino_backup.h"
#include "m/geometry.h"

class DinoEventLoop;

struct DinoMessageData;
class DinoFieldHole;

class DinoEventKasekiRockFound : public MProcess {
public:
    DinoEventKasekiRockFound(DinoEventLoop *event, u32 arg, MPoint pos);
    virtual ~DinoEventKasekiRockFound();
    virtual void Idle();

    void Start();
    MTask *CreateFoundMessage();
    // The message (with the argument put into it) in a box.
    MTask *CreateMessageBox(u32 index, const std::string &arg = "");
    // Gives the player the rock.
    void StateGet();
    // The rock isn't taken: says so, and fades the field out and back in.
    void StateLeave();
    // Fights for the rock.
    void StateBattle();
    void State02037204();
    MTask *CreateYesNo(u32 message);
    void StateFound();
    void StateFoundEnd();
    void StateAsk();
    void StateAnswered();

    void (DinoEventKasekiRockFound::*mState)(); // 0x74
    DinoEventLoop *mEvent;      // 0x7C
    MPoint mPos;            // 0x80
    u32 mArg;               // 0x88
    u32 mBattle;            // 0x8C: the battle the rock starts, or 0
    u32 mUnk90;             // 0x90
    DinoBackupEntry mEntry; // 0x94: the rock found
    u32 mUnkA8;             // 0xA8
    DinoFieldHole *mHole;   // 0xAC: the hole the rock was dug out of
    MResource<DinoMessageData> mMessages;   // 0xB0: the digging messages
    MResource<DinoMessageData> mUnkC0;      // 0xC0
    u32 unkD0[2];
    int mAnswer;            // 0xD8: the yes/no dialog's
    u32 unkDC[2];
};

#endif
