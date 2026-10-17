#ifndef GAME_DINO_UI_LICENSE_CARD_H
#define GAME_DINO_UI_LICENSE_CARD_H

#include "m/task.h"

class DinoUiLicenseCard : public MProcess {
public:
    DinoUiLicenseCard(u8 arg);
    virtual ~DinoUiLicenseCard();
    virtual void Idle();

    void (DinoUiLicenseCard::*mState)(); // 0x74
    u8 mUnk7C;                           // 0x7C
    u8 unk7D[0x13];
};

#endif
