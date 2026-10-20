#ifndef GAME_DINO_UI_REVIVE_STATUS_H
#define GAME_DINO_UI_REVIVE_STATUS_H

#include "m/task.h"
#include "game/dino_ui_rv_information.h"

struct DinoCreatureState;
class DinoUiReviveStatusParts;
class DinoUiReviveStatusDigit;

class DinoUiReviveStatus : public MProcess {
public:
    DinoUiReviveStatus(MDisplay *display, const DinoCreatureInfo &info, u8 flag);
    virtual ~DinoUiReviveStatus();
    virtual void Idle();

    DinoUiReviveStatusParts *GetParts(int i);
    // Shows the creature's numbers, each counting up after its own delay.
    void SetCreature(const DinoCreatureState &creature);

    void (DinoUiReviveStatus::*mState)(); // 0x74
    DinoCreatureInfo mInfo;     // 0x7C: (a DinoCreatureState)
    u8 unk94[0x98 - 0x94];
    DinoUiReviveStatusParts *mParts[4];   // 0x98: head, body, arms, legs
    DinoUiReviveStatusDigit *mDigits[4];  // 0xA8
    u8 unkB8[0xC4 - 0xB8];
    u8 mFlag;                   // 0xC4
    u8 unkC5[0xD0 - 0xC5];
};

#endif
