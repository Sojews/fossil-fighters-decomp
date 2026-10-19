#ifndef GAME_DINO_UI_OPENING_CHARA_H
#define GAME_DINO_UI_OPENING_CHARA_H

#include <string>
#include "m/task.h"

class MMotion;
class MSmartOam;

// A character in the opening.
class DinoUiOpeningChara : public MProcess {
public:
    DinoUiOpeningChara(MSmartOam *oam, std::string path, MPoint pos);
    virtual ~DinoUiOpeningChara();
    virtual void Idle();

    MMotion *mMotion;   // 0x74
    MPoint mPos;        // 0x78
};

#endif
