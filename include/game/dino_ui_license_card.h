#ifndef GAME_DINO_UI_LICENSE_CARD_H
#define GAME_DINO_UI_LICENSE_CARD_H

#include "m/task.h"

class DinoUiLicenseCard : public MProcess {
public:
    virtual ~DinoUiLicenseCard();
    virtual void Idle();

    void (DinoUiLicenseCard::*mState)(); // 0x74
};

#endif
