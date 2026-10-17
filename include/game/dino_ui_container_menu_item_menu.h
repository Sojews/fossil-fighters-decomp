#ifndef GAME_DINO_UI_CONTAINER_MENU_ITEM_MENU_H
#define GAME_DINO_UI_CONTAINER_MENU_ITEM_MENU_H

#include <string>
#include <vector>
#include "m/archive.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"
#include "game/dino_ui_container_menu_item.h"

class DinoUiContainerMenu : public DinoHelpBase, public MMenu {
public:
    virtual ~DinoUiContainerMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
    void OnBackDecided();
    BOOL PageRight();
    BOOL PageLeft();
    void OnOKDecided();

    u32 mUnkB4[7];              // 0xB4
    std::vector<void *> mUnkD0; // 0xD0
    u32 mUnkDC[9];              // 0xDC
    std::vector<s32> mUnk100;   // 0x100
    MArchivePtr mArchive;       // 0x10C
    MArchiveFile mFile;         // 0x110
};

class DinoUiContainerBackButton : public DinoUiContainerMenuItem {
public:
    virtual ~DinoUiContainerBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerOKButton : public DinoUiContainerMenuItem {
public:
    virtual ~DinoUiContainerOKButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiContainerPageLButton : public DinoUiContainerMenuItem {
public:
    virtual ~DinoUiContainerPageLButton();
    virtual void OnDecide();
};

class DinoUiContainerPageRButton : public DinoUiContainerMenuItem {
public:
    virtual ~DinoUiContainerPageRButton();
    virtual void OnDecide();
};

#endif
