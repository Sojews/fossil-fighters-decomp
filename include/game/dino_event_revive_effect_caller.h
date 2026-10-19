#ifndef GAME_DINO_EVENT_REVIVE_EFFECT_CALLER_H
#define GAME_DINO_EVENT_REVIVE_EFFECT_CALLER_H

#include "game/dino_event_ui_task.h"

// Plays the revive effect for a creature from storage, then shows its status.
class DinoEventReviveEffectCaller : public DinoEventUiTask {
public:
    virtual ~DinoEventReviveEffectCaller();
    virtual MTask *CreateUi();

    u32 mCreature;  // 0x84
    u32 mResult;    // 0x88
};

#endif
