#ifndef GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_MENU_H
#define GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_window_choice.h"

// One of the menu's entries.
struct DinoCommonWindowMenuEntry {
    u32 id;             // 0x00
    MRect rect;         // 0x04
    std::string text;   // 0x14
};

class DinoCommonWindowMenu : public DinoHelpBase, public MMenu {
public:
    DinoCommonWindowMenu(void *unkE4, MPoint pos, const std::vector<DinoWindowChoice> *choices, u32 unkFC);
    DinoCommonWindowMenu(MDisplay *display, void *unkE4, MPoint pos, const std::vector<DinoWindowChoice> *choices,
                         u32 unkFC);
    virtual ~DinoCommonWindowMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    virtual void OnIdle();

    void AddChoice(u32 id, const std::string &a, const std::string &b);
    void AddEntry(u32 id, MRect rect, const std::string &text);

    u32 mUnkB4[3];                                  // 0xB4
    std::vector<void *> mUnkC0;                     // 0xC0
    std::vector<DinoCommonWindowMenuEntry> mEntries;// 0xCC
    u32 mUnkD8[3];                                  // 0xD8
    void *mUnkE4;                                   // 0xE4
    MPoint mPos;                                    // 0xE8
    std::vector<DinoWindowChoice> mChoices;         // 0xF0
    u32 mUnkFC;                                     // 0xFC
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

    u32 mUnk94;         // 0x94
    MRect mFrame;       // 0x98
    std::string mText;  // 0xA8
};

#endif
