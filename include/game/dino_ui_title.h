#ifndef GAME_DINO_UI_TITLE_H
#define GAME_DINO_UI_TITLE_H

#include "m/task.h"

class MInput;
class MMenu;
class MMotion;
class MSmartOam;

class DinoUiTitle : public MProcess {
public:
    DinoUiTitle(s32 *result);
    virtual ~DinoUiTitle();
    virtual void Idle();

    void StateStart();
    void StateEnd();
    void StateWirelessBattle();
    void StateATM();
    void ResetBackup();
    void LoadBackup();
    void StateWarning();
    void StateAdvice();
    void StateAdvice2();
    void StateShowLogo();
    void LoadLogo();
    void StateOpening();
    void StateMenu();
    void StateSaveSlot();
    void StateSaveSlotEnd();
    void StateTopMenu();
    void StateTopMenuEnd();

    void (DinoUiTitle::*mState)(); // 0x74
    s32 *mResult;                  // 0x7C: what the player chose
    MInput *mInput;                // 0x80
    u8 mSkipped;                   // 0x84
    MSmartOam *mOam;               // 0x88
    u32 unk8C[2];
    MMenu *mMenu;                  // 0x94: the title menu
    MMotion *mLogo;                // 0x98
    u32 unk9C;
    u32 mSlotChoice;               // 0xA0: the save slot select's
    int mTopResult;                // 0xA4: the top menu loop's
};

#endif
