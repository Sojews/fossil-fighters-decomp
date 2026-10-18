#ifndef GAME_DINO_EVENT_REVIVE_EFFECT_H
#define GAME_DINO_EVENT_REVIVE_EFFECT_H

#include "m/task.h"

// What the revive effect shows of a creature.
struct DinoEventReviveCreature {
    u32 unk[6];
};

class DinoEventReviveEffect : public MProcess {
public:
    DinoEventReviveEffect(const DinoEventReviveCreature &creature, const DinoEventReviveCreature &creature2, u32 *done)
        : MProcess(NULL), mCreature(creature), mCreature2(creature2), mDone(done), mStep(0) {}
    virtual ~DinoEventReviveEffect();
    virtual void Idle();

    DinoEventReviveCreature mCreature;  // 0x74
    DinoEventReviveCreature mCreature2; // 0x8C
    u32 *mDone;                         // 0xA4
    u32 mStep;                          // 0xA8
    u32 unkAC;
};

#endif
