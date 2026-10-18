#ifndef GAME_DRAW_FONT_H
#define GAME_DRAW_FONT_H

#include "m/task.h"
#include "m/geometry.h"

class MConsole;

class DrawFont : public MProcess {
public:
    virtual ~DrawFont();
    virtual void Idle();
    void SetCenter(const MPoint &pos);
    void Show();

    MConsole *mConsole; // 0x74
};

#endif
