#ifndef GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_MENU_H
#define GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_MENU_H

#include "game/dino_cleaning.h"
#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/motion.h"
#include "game/help.h"

class DinoSeMotion;
class MSmartOam;

class DinoCleaningStopConfirmMenu : public MMenu, public DinoHelpBase {
public:
    DinoCleaningStopConfirmMenu(std::string motion, u8 *result, u32 yesText, u32 noText);
    virtual ~DinoCleaningStopConfirmMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u8 mSilentFocus;        // 0xB4
    std::string mUnkB8;     // 0xB8
    u8 *mResult;            // 0xC4: TRUE if the player stops
    MSmartOam *mOam;        // 0xC8
    DinoSeMotion *mMotion;  // 0xCC
    MMenuItem *mYesItem;    // 0xD0
    MMenuItem *mNoItem;     // 0xD4
    u32 mYesText;           // 0xD8
    u32 mNoText;            // 0xDC
};

class DinoCleaningStopConfirm : public MProcess, public DinoHelpBase {
public:
    DinoCleaningStopConfirm(DinoCleaning *cleaning, u8 *result);
    virtual ~DinoCleaningStopConfirm();
    virtual void Idle();

    void StateAsk();
    void StateDecided();
    void StateAskAgain();
    void StateLeave();

    void (DinoCleaningStopConfirm::*mState)(); // 0x80
    DinoCleaning *mCleaning;    // 0x88
    u8 *mResult;                // 0x8C: the answer
};

// Shown after the DS wakes, until the cleaning takes input again.
class DinoCleaningSleep : public MProcess, public DinoHelpBase {
public:
    DinoCleaningSleep(DinoCleaning *cleaning, u32 *timer);
    virtual ~DinoCleaningSleep();
    virtual void Idle();

    void Start();
    void StateClose();

    void (DinoCleaningSleep::*mState)(); // 0x80
    DinoCleaning *mCleaning;    // 0x88
    u32 *mTimer;                // 0x8C: the cleaning's sleep timer
    DinoSeMotion *mMotion;      // 0x90
};

#endif
