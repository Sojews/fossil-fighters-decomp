#ifndef GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_MENU_H
#define GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_MENU_H

#include "game/dino_cleaning.h"
#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/motion.h"
#include "game/help.h"

class DinoCleaningStopConfirmMenu : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoCleaningStopConfirmMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u8 mSilentFocus;        // 0xB4
    std::string mUnkB8;     // 0xB8
    u8 *mResult;            // 0xC4: TRUE if the player stops
    u32 mUnkC8;
    MMotion *mMotion;       // 0xCC
    MMenuItem *mYesItem;    // 0xD0
    MMenuItem *mNoItem;     // 0xD4
};

class DinoCleaningStopConfirm : public MProcess, public DinoHelpBase {
public:
    virtual ~DinoCleaningStopConfirm();
    virtual void Idle();

    void StateClose();

    void (DinoCleaningStopConfirm::*mState)(); // 0x80
    DinoCleaning *mCleaning;    // 0x88
    BOOL *mResult;              // 0x8C: the answer
    MMotion *mMotion;           // 0x90
};

class DinoCleaningSleep : public MProcess, public DinoHelpBase {
public:
    virtual ~DinoCleaningSleep();
    virtual void Idle();

    void (DinoCleaningSleep::*mState)(); // 0x80
    DinoCleaning *mCleaning;    // 0x88
};

#endif
