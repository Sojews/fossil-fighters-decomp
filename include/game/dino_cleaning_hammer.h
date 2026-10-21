#ifndef GAME_DINO_CLEANING_HAMMER_H
#define GAME_DINO_CLEANING_HAMMER_H

#include "game/dino_cleaning_tool.h"

class DinoCleaningHammer : public DinoCleaningTool {
public:
    DinoCleaningHammer(DinoCleaning *cleaning, u8 arg1);
    virtual ~DinoCleaningHammer();
    virtual void Unk18();
    virtual void Unk1C();
    virtual void Unk20();

    u32 mUnk78;     // 0x78
    u32 mUnk7C;     // 0x7C
    u8 mUnk80;      // 0x80
};

#endif
