// The fossil rock found screen.
#ifndef GAME_DINO_EVENT_KASEKI_ROCK_FOUND_H
#define GAME_DINO_EVENT_KASEKI_ROCK_FOUND_H

#include "m/task.h"
#include "m/resource.h"
#include "game/dino_backup.h"
#include "m/geometry.h"
#include "m/fx32.h"

class DinoEventLoop;

struct DinoMessageData;
struct DinoBattleData;
class DinoFieldHole;
class DinoFieldChara;
class MPolygonChar;

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
    // After the battle: on a win, what the rival says and on to the rock; on a loss, what they say and the end.
    void StateBattleEnd();
    void State020374d4();
    MTask *CreateYesNo(u32 message);
    void StateFound();
    void StateFoundEnd();
    // The battle's rival walks up beside the rock (when it has one) and speaks, then the yes/no.
    void StateRival();
    void StateAsk();
    void StateAnswered();
    // The finder digs the rock out (its effect playing over the hole), turns to the camera and shows a mark.
    void StateDig();

    void (DinoEventKasekiRockFound::*mState)(); // 0x74
    DinoEventLoop *mEvent;      // 0x7C
    MFx32Point mPos;        // 0x80: where the rock was found
    u32 mArg;               // 0x88
    u32 mBattle;            // 0x8C: the battle the rock starts, or 0
    u32 mUnk90;             // 0x90
    DinoBackupEntry mEntry; // 0x94: the rock found
    DinoFieldChara *mChara; // 0xA8: the one who found the rock
    DinoFieldHole *mHole;   // 0xAC: the hole the rock was dug out of
    MResource<DinoMessageData> mMessages;   // 0xB0: the digging messages
    MResource<DinoBattleData> mBattleData;  // 0xC0: the battle's, after it
    DinoFieldChara *mRival; // 0xD0: the one who wants the rock too (the battle's chara), or NULL
    MPolygonChar *mEffect;  // 0xD4: the digging effect
    int mAnswer;            // 0xD8: the yes/no dialog's
    u8 mUnkDC;              // 0xDC
    u8 unkDD[3];
    u32 mAngle;             // 0xE0: to turn the finder to
};

#endif
