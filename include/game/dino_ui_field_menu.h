#ifndef GAME_DINO_UI_FIELD_MENU_H
#define GAME_DINO_UI_FIELD_MENU_H

#include "m/task.h"
#include "game/dino_backup.h"

class DinoUiFieldMenu : public MProcess {
public:
    DinoUiFieldMenu(u32 arg);
    virtual ~DinoUiFieldMenu();
    virtual void Idle();

    void StateStart();
    void StateMenu();

    void (DinoUiFieldMenu::*mState)();  // 0x74
    u32 mUnk7C;                         // 0x7C
    int mChoice;                        // 0x80: the menu's
    DinoTeam mTeam;                     // 0x84
    DinoTeam mTeam2;                    // 0xAC
    u8 mHasCreature;                    // 0xD4: the team's first slot is used
    u8 unkD5[3];
};

#endif
