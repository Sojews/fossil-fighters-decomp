#ifndef GAME_DINO_UI_REVIVE_STATUS_H
#define GAME_DINO_UI_REVIVE_STATUS_H

#include "m/task.h"
#include "game/dino_ui_rv_information.h"

struct DinoCreatureState;
class DinoUiReviveStatusParts;
class DinoUiReviveStatusDigit;
class MSmartOam;
class MIndicator;
class MMotion;

class DinoUiReviveStatus : public MProcess {
public:
    DinoUiReviveStatus(MDisplay *display, const DinoCreatureInfo &info, u8 flag);
    virtual ~DinoUiReviveStatus();
    virtual void Idle();

    DinoUiReviveStatusParts *GetParts(int i);
    // Shows the creature's numbers, each counting up after its own delay.
    void SetCreature(const DinoCreatureState &creature);

    void Start();

    void (DinoUiReviveStatus::*mState)(); // 0x74
    DinoCreatureInfo mInfo;     // 0x7C: (a DinoCreatureState)
    MSmartOam *mOam;            // 0x94
    DinoUiReviveStatusParts *mParts[4];   // 0x98: head, body, arms, legs
    DinoUiReviveStatusDigit *mDigits[4];  // 0xA8
    MIndicator *mName;          // 0xB8
    MIndicator *mVoice;         // 0xBC
    MMotion *mMotion;           // 0xC0
    u8 mFlag;                   // 0xC4: shows the picture, the name and the voice
    u8 mUncleanable;            // 0xC5: the creature's fossil is one piece
    u32 mUnkC8;                 // 0xC8
    u32 mPartCount;             // 0xCC
};

#endif
