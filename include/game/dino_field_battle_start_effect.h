#ifndef GAME_DINO_FIELD_BATTLE_START_EFFECT_H
#define GAME_DINO_FIELD_BATTLE_START_EFFECT_H

#include "m/task.h"

class DinoField;

class MRasterWindow;

class DinoFieldBattleStartEffect : public MProcess {
public:
    DinoFieldBattleStartEffect(DinoField *field);
    virtual ~DinoFieldBattleStartEffect();
    virtual void Idle();

    void Start();
    // Closes a circle on the bottom screen over 90 frames, then ends.
    void StateClose();

    void (DinoFieldBattleStartEffect::*mState)(); // 0x74
    DinoField *mField;                            // 0x7C
    MRasterWindow *mWindow;                       // 0x80
    u32 mFrame;                                   // 0x84
};

#endif
