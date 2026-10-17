#ifndef GAME_DINO_UI_TITLE_SKIP_H
#define GAME_DINO_UI_TITLE_SKIP_H

#include "m/task.h"

class DinoUiTitleSkip : public MProcess {
public:
    virtual ~DinoUiTitleSkip();
    virtual void Idle();
};

#endif
