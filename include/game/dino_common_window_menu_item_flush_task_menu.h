#ifndef GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_MENU_H
#define GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_window_choice.h"

class DinoCommonWindow;
class DinoCommonWindowMenuCursor;


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

    DinoCommonWindow *mWindow;                      // 0xB4
    u32 mWidth;                                     // 0xB8: of the window, in tiles
    u32 mUnkBC;                                     // 0xBC
    std::vector<void *> mUnkC0;                     // 0xC0
    std::vector<DinoCommonWindowMenuEntry> mEntries;// 0xCC
    u32 mUnkD8;                                     // 0xD8
    MTaskSet *mCursorTasks;                         // 0xDC: the cursor's moves
    DinoCommonWindowMenuCursor *mCursor;            // 0xE0
    void *mUnkE4;                                   // 0xE4
    MPoint mPos;                                    // 0xE8
    std::vector<DinoWindowChoice> mChoices;         // 0xF0
    u32 mUnkFC;                                     // 0xFC
    MPoint mUnk100;                                 // 0x100: the window's offset from mPos
    MMenuItem *mCancelItem;                         // 0x108: chosen by B, if any
};

class DinoCommonWindowMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoCommonWindowMenuItem(MFont *font, MPoint pos, std::string text, std::string name, int width);
    virtual ~DinoCommonWindowMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    MView *mUnk94;      // 0x94: its text
    MRect mFrame;       // 0x98
    std::string mText;  // 0xA8
};

#endif
