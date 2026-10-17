#ifndef GAME_DINO_EVENT_CANON_PUTIN_H
#define GAME_DINO_EVENT_CANON_PUTIN_H

#include "game/dino_event_capture_fade_ui_task.h"

class DinoEventCanonPutin : public DinoEventCaptureFadeUiTask {
public:
    virtual ~DinoEventCanonPutin();
    virtual MTask *CreateUi();

    u32 mUnk74[4];  // 0x74
    u32 mCount;     // 0x84: the containers in use when the menu opened
};

#endif
