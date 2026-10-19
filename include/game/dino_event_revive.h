// The revival event (cleaning a fossil rock and reviving the vivosaur).
#ifndef GAME_DINO_EVENT_REVIVE_H
#define GAME_DINO_EVENT_REVIVE_H

#include <string>
#include <vector>
#include "m/task.h"
#include "m/resource.h"
#include "game/dino_event_revive_cleaning.h"
#include "game/dino_event_revive_effect.h"

class DinoEventLoop;
struct DinoMessageData;

class DinoEventRevive : public MProcess {
public:
    DinoEventRevive(DinoEventLoop *event);
    virtual ~DinoEventRevive();
    virtual void Idle();

    MTask *CreateMessage(u32 message, const std::string &extra);
    void AddMessage(u32 message);
    void StateMode();
    void StateDeposit();
    void State02037dbc();
    void StateCheckRocks();
    void State02038544();
    void StateLevelMessage();
    void StateNoneMessage();
    void StateStart();
    void StateContainerSelect();
    void StateSelected();
    void StateCleaning();
    void StateCleaned();
    void StateEffect();
    void StateEffectEnd();

    void (DinoEventRevive::*mState)(); // 0x74
    DinoEventLoop *mEvent;      // 0x7C
    s32 mRock;                  // 0x80: the one chosen, or -1
    DinoEventReviveResult mResult;          // 0x84: mOutcome is at 0x94
    DinoEventReviveCreature mCreature;      // 0x98
    DinoEventReviveCreature mCreature2;     // 0xB0
    MResource<DinoMessageData> mMessages; // 0xC8: the cleaning messages
    u32 mUnkD8;                 // 0xD8
    u32 mEffectDone;            // 0xDC
    std::vector<void *> mUnkE0; // 0xE0
    int mScene;                 // 0xEC: the sound scene to load
    int mBgm;                   // 0xF0
};

#endif
