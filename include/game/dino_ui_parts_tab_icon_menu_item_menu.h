#ifndef GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_MENU_H
#define GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_status_menu_item.h"

struct DinoTeam;
class MSmartOam;
class DinoUiStatusMenu : public DinoHelpBase, public MMenu {
public:
    MSmartOam *GetOam() { return mOam; }
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

    u8 unkB3[1];                // members start in MMenu's tail padding
    MSmartOam *mOam;            // 0xB4: the items' sprites
    u8 unkB8[8];
    MMenuItem *mTabItems[3];    // 0xC0
    u32 mUnkCC[2];              // 0xCC
    MMenuItem *mMembers[5];     // 0xD4
    u8 unkE8[0x2C];
    int *mResult;               // 0x114
    struct Team {
        u8 unk00[0x22];
        u8 pickable[5];         // 0x22
    } *mTeam;                   // 0x118
    u32 mUnk11C;                // 0x11C
    u32 mUnk120;                // 0x120
    int mSelected;              // 0x124
    int mMode;                  // 0x128
    u8 unk12C;
    u8 mUnk12D;                 // 0x12D
    u8 unk12E[0x138 - 0x12E];
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
