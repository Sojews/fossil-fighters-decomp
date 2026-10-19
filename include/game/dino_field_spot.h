#ifndef GAME_DINO_FIELD_SPOT_H
#define GAME_DINO_FIELD_SPOT_H

#include "m/task.h"

// Fades both screens out to (or in from) a spot.
class DinoFieldSpotOut : public MProcess {
public:
    DinoFieldSpotOut(int duration) : MProcess(NULL) { mDuration = duration; }
    virtual ~DinoFieldSpotOut();
    virtual void Idle();

    int mDuration;  // 0x74
};

class DinoFieldSpotIn : public MProcess {
public:
    DinoFieldSpotIn(int duration) : MProcess(NULL) { mDuration = duration; }
    virtual ~DinoFieldSpotIn();
    virtual void Idle();

    int mDuration;  // 0x74
};

class DinoFieldWhiteOut : public MProcess {
public:
    DinoFieldWhiteOut(int duration) : MProcess(NULL) { mDuration = duration; }
    virtual ~DinoFieldWhiteOut();
    virtual void Idle();

    int mDuration;  // 0x74
};

class DinoFieldWhiteIn : public MProcess {
public:
    DinoFieldWhiteIn(int duration) : MProcess(NULL) { mDuration = duration; }
    virtual ~DinoFieldWhiteIn();
    virtual void Idle();

    int mDuration;  // 0x74
};

class DinoFieldBlackOut : public MProcess {
public:
    DinoFieldBlackOut(int duration) : MProcess(NULL) { mDuration = duration; }
    virtual ~DinoFieldBlackOut();
    virtual void Idle();

    int mDuration;  // 0x74
};

class DinoFieldBlackIn : public MProcess {
public:
    DinoFieldBlackIn(int duration) : MProcess(NULL) { mDuration = duration; }
    virtual ~DinoFieldBlackIn();
    virtual void Idle();

    int mDuration;  // 0x74
};

#endif
