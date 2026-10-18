#ifndef GAME_DINO_UI_LICENSE_CARD_H
#define GAME_DINO_UI_LICENSE_CARD_H

#include "m/task.h"

class MFont;

class MInput;

class DinoUiLicenseCard : public MProcess {
public:
    DinoUiLicenseCard(u8 arg);
    virtual ~DinoUiLicenseCard();
    virtual void Idle();
    void Close();
    void StateWait();

    void (DinoUiLicenseCard::*mState)(); // 0x74
    u8 mUnk7C;                           // 0x7C
    MInput *mInput;                      // 0x80
    u8 unk84[0x8C - 0x84];
    MFont *mFont;                        // 0x8C
};

#endif
