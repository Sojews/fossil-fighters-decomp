#ifndef GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_MENU_H
#define GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_status_menu_item.h"

struct DinoTeam;
class DinoUiStatusMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiStatusMenu(int *choice, DinoTeam *team, DinoTeam *team2);
    virtual ~DinoUiStatusMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnOKDecided();
    void OnBackDecided();
    void SelectTab(int tab);
    void DecideMember(int index);
    void SelectMember(int index);

    u8 unkB3[0xD];              // members start in MMenu's tail padding
    MMenuItem *mTabItems[3];    // 0xC0
    u32 mUnkCC[2];              // 0xCC
    MMenuItem *mMembers[5];     // 0xD4
    u8 unkE8[0x30];
    struct Team {
        u8 unk00[0x22];
        u8 pickable[5];         // 0x22
    } *mTeam;                   // 0x118
    u32 mUnk11C;                // 0x11C
    u32 mUnk120;                // 0x120
    int mSelected;              // 0x124
    int mMode;                  // 0x128
    u8 unk12C[0x138 - 0x12C];
};

class DinoUiStatusBackButton : public DinoUiStatusMenuItem {
public:
    virtual ~DinoUiStatusBackButton();
    virtual void OnDecide();
};

class DinoUiStatusOKButton : public DinoUiStatusMenuItem {
public:
    virtual ~DinoUiStatusOKButton();
    virtual void OnDecide();
};

#endif
