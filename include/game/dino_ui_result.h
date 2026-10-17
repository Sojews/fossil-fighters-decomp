#ifndef GAME_DINO_UI_RESULT_H
#define GAME_DINO_UI_RESULT_H

#include <string>
#include "m/task.h"

class DinoUiResult : public MProcess {
public:
    virtual ~DinoUiResult();
    virtual void Idle();

    u32 mUnk74[4];      // 0x74
    std::string mText;  // 0x84
};

#endif
