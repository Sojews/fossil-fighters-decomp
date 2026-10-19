#ifndef GAME_DINO_UI_MULTIPLAY_MENU_ITEM_MENU_H
#define GAME_DINO_UI_MULTIPLAY_MENU_ITEM_MENU_H

#include "m/motion.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_multiplay_menu_item2.h"
#include "game/dino_ui_multiplay_menu_item.h"

class MSmartOam;
class DinoUiMultiplayMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
    DinoUiMultiplayMenu(int *result);
    virtual ~DinoUiMultiplayMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    void Decide(int choice);

    u8 unkB3[1];     // members start in MMenu's tail padding
    MSmartOam *mOam; // 0xB4: the items' sprites
    u8 unkB8[0x18];
    int *mResult;    // 0xD0
    u8 mSilentFocus; // 0xD4
};

class DinoUiMultiplayWirelessButton : public DinoUiMultiplayMenuItem {
public:
    virtual ~DinoUiMultiplayWirelessButton();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

class DinoUiMultiplayMenu;

class DinoUiMultiplayMenuBackButton : public DinoUiMultiplayMenuItem {
public:
    virtual ~DinoUiMultiplayMenuBackButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMultiplayOrganizationButton : public DinoUiMultiplayMenuItem2 {
public:
    virtual ~DinoUiMultiplayOrganizationButton();
    virtual void OnDecide();
};

#endif
