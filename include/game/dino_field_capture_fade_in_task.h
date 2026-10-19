#ifndef GAME_DINO_FIELD_CAPTURE_FADE_IN_TASK_H
#define GAME_DINO_FIELD_CAPTURE_FADE_IN_TASK_H

#include "m/task.h"
#include "game/help.h"

class DinoFieldCaptureFadeInTask : public MProcess, public DinoHelpDisabler {
public:
    DinoFieldCaptureFadeInTask(void *field) : MProcess(NULL), mField(field), mUnk7C(0) {}
    virtual ~DinoFieldCaptureFadeInTask();
    virtual void Idle();

    void *mField;   // 0x78
    u32 mUnk7C;     // 0x7C
    u32 unk80[4];
};

#endif
