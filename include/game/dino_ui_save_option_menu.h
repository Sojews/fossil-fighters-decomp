#ifndef GAME_DINO_UI_SAVE_OPTION_MENU_H
#define GAME_DINO_UI_SAVE_OPTION_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_save_option_menu_item.h"

class DinoUiSaveOptionMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiSaveOptionMenu(u32 arg);
    virtual ~DinoUiSaveOptionMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    void RemoveItem();
    void AppendItem(u8 kind);
    void OnBackDecided();
    void OnOptionDecided();
    void OnSaveDecided();

    u8 unkB3[1];     // members start in MMenu's tail padding
    u32 mUnkB4;      // 0xB4
    u32 mUnkB8;      // 0xB8
    MMenuItem *mItems[3]; // 0xBC
    u32 mUnkC8;      // 0xC8
    u8 mSilentFocus; // 0xCC
};

class DinoUiSaveOptionSaveButton : public DinoUiSaveOptionMenuItem {
public:
    virtual ~DinoUiSaveOptionSaveButton();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

class DinoUiSaveOptionOptionButton : public DinoUiSaveOptionMenuItem {
public:
    virtual ~DinoUiSaveOptionOptionButton();
    virtual MRect GetFrame();
    virtual void OnDecide();
};

class DinoUiSaveOptionBackButton : public DinoUiSaveOptionMenuItem {
public:
    virtual ~DinoUiSaveOptionBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
