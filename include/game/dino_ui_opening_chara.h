#ifndef GAME_DINO_UI_OPENING_CHARA_H
#define GAME_DINO_UI_OPENING_CHARA_H

#include <string>
#include "m/task.h"

class MMotion;
class MSmartOam;

// A character in the opening.
class DinoUiOpeningChara : public MProcess {
public:
    DinoUiOpeningChara(MSmartOam *oam, std::string path, u32 unk78, u32 unk7C);
    virtual ~DinoUiOpeningChara();
    virtual void Idle();

    MMotion *mMotion;   // 0x74
    u32 mUnk78;         // 0x78
    u32 mUnk7C;         // 0x7C
};

#endif
