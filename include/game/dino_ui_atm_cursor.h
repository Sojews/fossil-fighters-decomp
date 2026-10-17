#ifndef GAME_DINO_UI_ATM_CURSOR_H
#define GAME_DINO_UI_ATM_CURSOR_H

#include "m/view.h"

class DinoUiATMCursor : public MView {
public:
    virtual ~DinoUiATMCursor();
    virtual void Execute();

    MView *mUnk10;  // 0x10
};

#endif
