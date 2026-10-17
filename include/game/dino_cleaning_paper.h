#ifndef GAME_DINO_CLEANING_PAPER_H
#define GAME_DINO_CLEANING_PAPER_H

#include "m/task.h"

class MSmartOam;
class MRandom;

class DinoCleaningPaper : public MProcess {
public:
    DinoCleaningPaper(MSmartOam *oam0, MSmartOam *oam1, MRandom *random);
    virtual ~DinoCleaningPaper();
    virtual void Idle();
    void SetDone();

    MSmartOam *mOams[2];    // 0x74
    MRandom *mRandom;       // 0x7C
    u8 mDone;               // 0x80
};

#endif
