#ifndef GAME_DINO_UI2_FIELD_MENU_CURSOR_H
#define GAME_DINO_UI2_FIELD_MENU_CURSOR_H

#include "m/view.h"

class DinoUi2FieldMenuCursor : public MView {
public:
    virtual ~DinoUi2FieldMenuCursor();
    virtual void Execute();

    MView *mUnk10;  // 0x10
    MView *mUnk14;  // 0x14
};

#endif
