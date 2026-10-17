#ifndef GAME_DINO_UI_TOP_MENU_ITEM2_H
#define GAME_DINO_UI_TOP_MENU_ITEM2_H

#include "m/menu_item.h"
#include "game/help.h"

// The other kind of top menu button.
class DinoUiTopMenuItem2 : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiTopMenuItem2();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

class DinoUiTopMenuPassButton : public DinoUiTopMenuItem2 {
public:
    virtual ~DinoUiTopMenuPassButton();
    virtual void OnDecide();
};

class DinoUiTopMenuPassDarkButton : public DinoUiTopMenuItem2 {
public:
    virtual ~DinoUiTopMenuPassDarkButton();
    virtual void OnDecide();
};

class DinoUiTopMenuOptionButton : public DinoUiTopMenuItem2 {
public:
    virtual ~DinoUiTopMenuOptionButton();
    virtual void OnDecide();
};

#endif
