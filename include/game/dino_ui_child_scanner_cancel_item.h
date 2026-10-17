#ifndef GAME_DINO_UI_CHILD_SCANNER_CANCEL_ITEM_H
#define GAME_DINO_UI_CHILD_SCANNER_CANCEL_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiChildScannerCancelItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiChildScannerCancelItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MDisplay *GetDisplay();
    virtual MRect GetRect();
    virtual const char *GetText();
};

#endif
