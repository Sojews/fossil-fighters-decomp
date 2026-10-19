#ifndef GAME_DINO_UI_TOP_MENU_ITEM2_H
#define GAME_DINO_UI_TOP_MENU_ITEM2_H

#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "m/motion.h"
#include "game/help.h"

class DinoUiTopMenu;

// The other kind of top menu button.
class DinoUiTopMenuItem2 : public MMenuItem, public DinoHelpItem {
public:
    DinoUiTopMenuItem2(DinoUiTopMenu *owner, const std::string &path, const std::string &path2, MPoint pos,
                       u32 textId);
    virtual ~DinoUiTopMenuItem2();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiTopMenu *mOwner;  // 0x94
    MMotion *mMotion;       // 0x98
    MMotion *mMotion2;      // 0x9C
    u32 mTextId;            // 0xA0
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
