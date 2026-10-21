#ifndef GAME_DINO_UI_LICENSE_CARD_H
#define GAME_DINO_UI_LICENSE_CARD_H

#include "m/task.h"

class MFont;

class MInput;
class MTextPicture;
class MSmartOam;

class DinoUiLicenseCard : public MProcess {
public:
    DinoUiLicenseCard(u8 arg);
    virtual ~DinoUiLicenseCard();
    virtual void Idle();
    void Close();
    void StateStart();
    void StateWait();

    void (DinoUiLicenseCard::*mState)(); // 0x74
    u8 mUnk7C;                           // 0x7C
    MInput *mInput;                      // 0x80
    MTextPicture *mPicture;              // 0x84: the horider level
    MSmartOam *mOam;                     // 0x88
    MFont *mFont;                        // 0x8C
};

#endif
