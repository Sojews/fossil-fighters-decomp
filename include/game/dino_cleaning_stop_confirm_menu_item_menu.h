#ifndef GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_MENU_H
#define GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_MENU_H

#include "game/dino_cleaning_kaseki_rock.h"
#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCleaningStopConfirmMenu : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoCleaningStopConfirmMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u32 mUnkB4;             // 0xB4
    std::string mUnkB8;     // 0xB8
};

class DinoCleaningStopConfirm : public MProcess, public DinoHelpBase {
public:
    virtual ~DinoCleaningStopConfirm();
    virtual void Idle();

    void (DinoCleaningStopConfirm::*mState)(); // 0x80
    DinoCleaningKasekiRock *mRock;  // 0x88
};

class DinoCleaningSleep : public MProcess, public DinoHelpBase {
public:
    virtual ~DinoCleaningSleep();
    virtual void Idle();

    void (DinoCleaningSleep::*mState)(); // 0x80
    DinoCleaningKasekiRock *mRock;  // 0x88
};

#endif
