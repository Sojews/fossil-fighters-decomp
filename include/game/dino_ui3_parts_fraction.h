#ifndef GAME_DINO_UI3_PARTS_FRACTION_H
#define GAME_DINO_UI3_PARTS_FRACTION_H

#include "m/motion.h"
#include "m/task.h"

class DinoUi3PartsFraction : public MProcess {
public:
    virtual ~DinoUi3PartsFraction();
    virtual void Idle();
    void SetNumerator(u8 n);
    void SetDenominator(u8 n);
    void Show();
    void Hide();

    MMotion *mNumerator;    // 0x74: one digit each
    MMotion *mDenominator;  // 0x78
};

#endif
