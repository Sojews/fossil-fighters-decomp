#ifndef GAME_DINO_UI_CLEANING_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CLEANING_MENU_ITEM_MENU_H

#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_cleaning_menu_item.h"

class DinoEventLoop;

class DinoUiCleaningMenu : public DinoHelpBase, public MMenu {
public:
    DinoUiCleaningMenu(DinoEventLoop *event);
    virtual ~DinoUiCleaningMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();

    u32 mUnkB4[3];              // 0xB4
    std::vector<void *> mUnkC0; // 0xC0
    u32 mUnkCC[7];              // 0xCC
    std::vector<u32> mUnkE8;    // 0xE8
    MArchivePtr mArchive;       // 0xF4
    MArchiveFile mFile;         // 0xF8
    u32 mUnk104[5];             // 0x104
};

class DinoUiCleaningBackButton : public DinoUiCleaningMenuItem {
public:
    virtual ~DinoUiCleaningBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiCleaningPageLButton : public DinoUiCleaningMenuItem {
public:
    virtual ~DinoUiCleaningPageLButton();
    virtual void OnDecide();
};

class DinoUiCleaningPageRButton : public DinoUiCleaningMenuItem {
public:
    virtual ~DinoUiCleaningPageRButton();
    virtual void OnDecide();
};

class DinoUiCleaningTest : public MProcess {
public:
    virtual ~DinoUiCleaningTest();
    virtual void Idle();

    void (DinoUiCleaningTest::*mState)(); // 0x74
    std::vector<void *> mUnk7C; // 0x7C
};

#endif
