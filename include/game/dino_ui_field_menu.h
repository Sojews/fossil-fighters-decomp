#ifndef GAME_DINO_UI_FIELD_MENU_H
#define GAME_DINO_UI_FIELD_MENU_H

#include "m/task.h"

class DinoUiFieldMenu : public MProcess {
public:
    DinoUiFieldMenu(u32 arg);
    virtual ~DinoUiFieldMenu();
    virtual void Idle();

    void (DinoUiFieldMenu::*mState)();  // 0x74
    u32 mUnk7C;                         // 0x7C
    u8 unk80[0x58];
};

#endif
