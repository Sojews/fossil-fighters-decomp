#ifndef GAME_DRAW_FONT_H
#define GAME_DRAW_FONT_H

#include "m/task.h"
#include "m/geometry.h"

class MConsole;

// A text centred on the top screen.
class DrawFont : public MProcess {
public:
    // text: an index into the island map's texts.
    DrawFont(int text);
    virtual ~DrawFont();
    virtual void Idle();
    void SetCenter(const MPoint &pos);
    void Show();

    MConsole *mConsole; // 0x74
};

#endif
