#ifndef GAME_DINO_HELP_SIGN_H
#define GAME_DINO_HELP_SIGN_H

#include "m/menu_item.h"

class DinoHelpItem;

// A menu item standing for a help item: touching it picks that item's help.
class DinoHelpSign : public MMenuItem {
public:
    DinoHelpSign(DinoHelpItem *item);
    virtual ~DinoHelpSign();
    virtual MRect GetFrame();
    virtual void OnDecide();

    DinoHelpItem *GetItem();

    DinoHelpItem *mItem;    // 0x88
};

#endif
