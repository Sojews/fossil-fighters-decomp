#ifndef GAME_DINO_FIELD_CAPTURE_FADE_OUT_TASK_H
#define GAME_DINO_FIELD_CAPTURE_FADE_OUT_TASK_H

#include "m/task.h"
#include "game/help.h"

class DinoField;
class MDirectBitmap;
class MRasterWindow;

class DinoFieldCaptureFadeOutTask : public MProcess, public DinoHelpDisabler {
public:
    DinoFieldCaptureFadeOutTask(void *field) : MProcess(NULL), mField((DinoField *) field), mStep(0) {}
    virtual ~DinoFieldCaptureFadeOutTask();
    virtual void Idle();

    DinoField *mField;          // 0x78
    u32 mStep;                  // 0x7C
    MDirectBitmap *mTopBitmap;  // 0x80: the capture, moved to the top screen
    MDirectBitmap *mBitmap;     // 0x84: the bottom screen captured
    MRasterWindow *mRaster;     // 0x88: the closing circle
    u32 mWipe;                  // 0x8C: frames of it left
};

#endif
