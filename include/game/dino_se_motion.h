// A motion that plays sound effects.
#ifndef GAME_DINO_SE_MOTION_H
#define GAME_DINO_SE_MOTION_H

#include <vector>
#include "m/motion.h"

class DinoSeMotion : public MMotion {
public:
    virtual ~DinoSeMotion();

    std::vector<void *> mUnk7C; // 0x7C
};

#endif
