#ifndef GAME_DINO_UI_FIELD_MENU_CURSOR_H
#define GAME_DINO_UI_FIELD_MENU_CURSOR_H

#include "m/view.h"

class DinoUiFieldMenuCursor : public MView {
public:
    virtual ~DinoUiFieldMenuCursor();
    virtual void Execute();

    MView *mUnk10;  // 0x10
    MView *mUnk14;  // 0x14
};

#endif
