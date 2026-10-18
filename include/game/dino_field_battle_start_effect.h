#ifndef GAME_DINO_FIELD_BATTLE_START_EFFECT_H
#define GAME_DINO_FIELD_BATTLE_START_EFFECT_H

#include "m/task.h"

class DinoField;

class DinoFieldBattleStartEffect : public MProcess {
public:
    DinoFieldBattleStartEffect(DinoField *field);
    virtual ~DinoFieldBattleStartEffect();
    virtual void Idle();

    void (DinoFieldBattleStartEffect::*mState)(); // 0x74
    DinoField *mField;                            // 0x7C
    u8 unk80[8];
};

#endif
