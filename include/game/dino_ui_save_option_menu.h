#ifndef GAME_DINO_UI_SAVE_OPTION_MENU_H
#define GAME_DINO_UI_SAVE_OPTION_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_save_option_menu_item.h"

class MSmartOam;
class MTextPicture;

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
    MSmartOam *GetOam();

    u8 unkB3[1];     // members start in MMenu's tail padding
    u32 mUnkB4;      // 0xB4
    MSmartOam *mOam; // 0xB8: the items' sprites
    MMenuItem *mItems[3]; // 0xBC
    MTextPicture *mBg;  // 0xC8
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
