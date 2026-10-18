#ifndef GAME_DINO_UI_PARTS_NUMBER8_H
#define GAME_DINO_UI_PARTS_NUMBER8_H

#include "m/indicator.h"
#include "m/task.h"

// A number of up to 8 digits in text.
class DinoUiPartsNumber8 : public MProcess {
public:
    DinoUiPartsNumber8(MPoint pos);
    virtual ~DinoUiPartsNumber8();
    virtual void Idle();
    void SetValue(u32 value);

    MIndicator *mText; // 0x74
};

#endif
