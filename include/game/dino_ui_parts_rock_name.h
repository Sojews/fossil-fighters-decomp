#ifndef GAME_DINO_UI_PARTS_ROCK_NAME_H
#define GAME_DINO_UI_PARTS_ROCK_NAME_H

#include "m/task.h"
#include "m/motion.h"
#include "m/indicator.h"

class DinoUiPartsRockName : public MProcess {
public:
    virtual ~DinoUiPartsRockName();
    virtual void Idle();
    void Clear();

    MMotion *mMotion;       // 0x74
    MIndicator *mText;      // 0x78
};

#endif
