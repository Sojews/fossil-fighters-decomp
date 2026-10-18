#ifndef GAME_DINO_UI_TITLE_H
#define GAME_DINO_UI_TITLE_H

#include "m/task.h"

class MInput;
class MMenu;

class DinoUiTitle : public MProcess {
public:
    DinoUiTitle(s32 *result);
    virtual ~DinoUiTitle();
    virtual void Idle();

    void StateOpening();
    void StateMenu();
    void StateSaveSlot();
    void StateSaveSlotEnd();
    void StateTopMenu();
    void StateTopMenuEnd();

    void (DinoUiTitle::*mState)(); // 0x74
    s32 *mResult;                  // 0x7C: what the player chose
    MInput *mInput;                // 0x80
    u32 unk84[4];
    MMenu *mMenu;                  // 0x94: the title menu
    u32 unk98[2];
    u32 mSlotChoice;               // 0xA0: the save slot select's
    int mTopResult;                // 0xA4: the top menu loop's
};

#endif
