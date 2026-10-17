#ifndef GAME_DINO_UI3_PARTS_CURSOR_H
#define GAME_DINO_UI3_PARTS_CURSOR_H

#include "m/view.h"

class DinoUi3PartsCursor : public MView {
public:
    virtual ~DinoUi3PartsCursor();
    virtual void Execute();

    MView *mUnk10;  // 0x10
};

#endif
