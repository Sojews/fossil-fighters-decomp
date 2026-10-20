#ifndef GAME_DINO_UI_TOP_MENU_ITEM_H
#define GAME_DINO_UI_TOP_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiTopMenu;

// A top menu button.
class DinoUiTopMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiTopMenuItem(DinoUiTopMenu *owner, const std::string &path, MPoint pos, u32 textId);
    virtual ~DinoUiTopMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiTopMenu *mOwner;  // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

class DinoUiTopMenuBackButton : public DinoUiTopMenuItem {
public:
    DinoUiTopMenuBackButton(DinoUiTopMenu *owner);
    virtual ~DinoUiTopMenuBackButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiTopMenuSingleButton : public DinoUiTopMenuItem {
public:
    DinoUiTopMenuSingleButton(DinoUiTopMenu *owner);
    virtual ~DinoUiTopMenuSingleButton();
    virtual void OnDecide();
};

class DinoUiTopMenuMultiButton : public DinoUiTopMenuItem {
public:
    DinoUiTopMenuMultiButton(DinoUiTopMenu *owner);
    virtual ~DinoUiTopMenuMultiButton();
    virtual void OnDecide();
};

class DinoUiTopMenuMultiDarkButton : public DinoUiTopMenuItem {
public:
    DinoUiTopMenuMultiDarkButton(DinoUiTopMenu *owner);
    virtual ~DinoUiTopMenuMultiDarkButton();
    virtual void OnDecide();
};

#endif
