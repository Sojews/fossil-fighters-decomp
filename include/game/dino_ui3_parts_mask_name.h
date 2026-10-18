#ifndef GAME_DINO_UI3_PARTS_MASK_NAME_H
#define GAME_DINO_UI3_PARTS_MASK_NAME_H

#include "m/task.h"
#include "m/motion.h"
#include "m/indicator.h"

class DinoUi3PartsMaskName : public MProcess {
public:
    virtual ~DinoUi3PartsMaskName();
    virtual void Idle();
    void Clear();

    MMotion *mMotion;       // 0x74
    MIndicator *mText;      // 0x78
};

#endif
