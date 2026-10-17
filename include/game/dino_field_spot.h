#ifndef GAME_DINO_FIELD_SPOT_H
#define GAME_DINO_FIELD_SPOT_H

#include "m/task.h"

// Fades both screens out to (or in from) a spot.
class DinoFieldSpotOut : public MProcess {
public:
    virtual ~DinoFieldSpotOut();
    virtual void Idle();
};

class DinoFieldSpotIn : public MProcess {
public:
    virtual ~DinoFieldSpotIn();
    virtual void Idle();
};

class DinoFieldWhiteOut : public MProcess {
public:
    virtual ~DinoFieldWhiteOut();
    virtual void Idle();

    int mDuration;  // 0x74
};

class DinoFieldWhiteIn : public MProcess {
public:
    virtual ~DinoFieldWhiteIn();
    virtual void Idle();

    int mDuration;  // 0x74
};

class DinoFieldBlackOut : public MProcess {
public:
    virtual ~DinoFieldBlackOut();
    virtual void Idle();

    int mDuration;  // 0x74
};

class DinoFieldBlackIn : public MProcess {
public:
    virtual ~DinoFieldBlackIn();
    virtual void Idle();

    int mDuration;  // 0x74
};

#endif
