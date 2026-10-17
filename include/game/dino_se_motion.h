// A motion that plays sound effects.
#ifndef GAME_DINO_SE_MOTION_H
#define GAME_DINO_SE_MOTION_H

#include <vector>
#include "m/motion.h"

class DinoSeMotion : public MMotion {
public:
    virtual ~DinoSeMotion();
    virtual void Unk44(int arg);

    std::vector<void *> mSes;   // 0x7C: sound effects the motion's commands play (1-based)
};

#endif
