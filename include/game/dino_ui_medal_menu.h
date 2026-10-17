// The medal list menu.
#ifndef GAME_DINO_UI_MEDAL_MENU_H
#define GAME_DINO_UI_MEDAL_MENU_H

#include "game/dino_resource.h"
#include <vector>
#include "m/menu.h"
#include "m/motion.h"
#include "game/help.h"

class DinoUiMedalMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiMedalMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    void PageRight();
    void PageLeft();

    u32 mUnkB4[58];                     // 0xB4
    std::vector<void *> mUnk19C;        // 0x19C
    std::vector<void *> mUnk1A8;        // 0x1A8
    std::vector<void *> mUnk1B4;        // 0x1B4
    u32 mUnk1C0[17];                    // 0x1C0
    u8 mUnk204;                         // 0x204
    std::vector<MMotionFile> mMotions;  // 0x208
};

class DinoUiSortMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiSortMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u32 mTextId;        // 0x94
};

class DinoUiMedalList : public MProcess {
public:
    DinoUiMedalList();
    virtual ~DinoUiMedalList();
    virtual void Idle();

    void (DinoUiMedalList::*mState)(); // 0x74
    u32 mUnk7C;                         // 0x7C
    u32 mUnk80;                         // 0x80
    u8 mUnk84;                          // 0x84
    u8 mUnk85;                          // 0x85
};

class DinoUiMedalMenu;

class DinoUiMedalMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMedalMenuItem();
    virtual MRect GetFrame();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiMedalMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiMedalBackButton : public DinoUiMedalMenuItem {
public:
    virtual ~DinoUiMedalBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiMedalPageLButton : public DinoUiMedalMenuItem {
public:
    virtual ~DinoUiMedalPageLButton();
    virtual void OnDecide();
};

class DinoUiMedalPageRButton : public DinoUiMedalMenuItem {
public:
    virtual ~DinoUiMedalPageRButton();
    virtual void OnDecide();
};

class DinoUiMedalSlotCreateTask : public MProcess {
public:
    virtual ~DinoUiMedalSlotCreateTask();
    virtual void Idle();
};

#endif
