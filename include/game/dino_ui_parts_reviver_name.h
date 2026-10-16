#ifndef GAME_DINO_UI_PARTS_REVIVER_NAME_H
#define GAME_DINO_UI_PARTS_REVIVER_NAME_H

#include "m/task.h"
#include "game/help.h"

class DinoUiPartsReviverName : public MProcess, public DinoHelpHook {
public:
    virtual ~DinoUiPartsReviverName();
};

#endif
