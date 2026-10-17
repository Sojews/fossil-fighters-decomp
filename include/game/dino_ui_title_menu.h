#ifndef GAME_DINO_UI_TITLE_MENU_H
#define GAME_DINO_UI_TITLE_MENU_H

#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoUiTitleMenu : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoUiTitleMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u32 mUnkB4[5];
    u32 mUnkC8;      // 0xC8
    u8 mSilentFocus; // 0xCC
};

class DinoUiOpening : public MProcess {
public:
    virtual ~DinoUiOpening();
    virtual void Execute();
    virtual void Idle();

    u8 unk74[4];
    void (DinoUiOpening::*mState)(); // 0x78
    u8 unk80[40];
    std::vector<void *> mUnkA8; // 0xA8
};

#endif
