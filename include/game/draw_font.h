#ifndef GAME_DRAW_FONT_H
#define GAME_DRAW_FONT_H

#include "m/task.h"

class DrawFont : public MProcess {
public:
    virtual ~DrawFont();
    virtual void Idle();
};

#endif
