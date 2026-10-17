#ifndef GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_MENU_H
#define GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_window_choice.h"

// One of the menu's entries.
struct DinoCommonWindowMenuEntry {
    u32 unk00[5];
    std::string text;   // 0x14
};

class DinoCommonWindowMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoCommonWindowMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    virtual void OnIdle();

    u32 mUnkB4[3];                                  // 0xB4
    std::vector<void *> mUnkC0;                     // 0xC0
    std::vector<DinoCommonWindowMenuEntry> mEntries;// 0xCC
    u32 mUnkD8[6];                                  // 0xD8
    std::vector<DinoWindowChoice> mChoices;         // 0xF0
};

class DinoCommonWindowMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoCommonWindowMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u32 mUnk94[5];      // 0x94
    std::string mText;  // 0xA8
};

#endif
