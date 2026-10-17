#ifndef GAME_DINO_CLEANING_XRAY_H
#define GAME_DINO_CLEANING_XRAY_H

#include "game/dino_cleaning_tool.h"

class DinoCleaningXray : public DinoCleaningTool {
public:
    DinoCleaningXray(DinoCleaning *cleaning);
    virtual ~DinoCleaningXray();
    virtual void Unk18();
    virtual void Unk1C();
};

#endif
