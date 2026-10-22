#ifndef GAME_DINO_CLEANING_SCORE_H
#define GAME_DINO_CLEANING_SCORE_H

#include "m/task.h"
#include "m/fx32.h"

class DinoCleaning;
class MMotion;
class MRasterPal;
class MSmartOam;

class DinoCleaningScore : public MProcess {
public:
    DinoCleaningScore(DinoCleaning *cleaning, MSmartOam *oam, u32 score, u8 flag);
    void Show();
    void SetUnk9CC();
    virtual ~DinoCleaningScore();
    virtual void Idle();

    u8 GetUnk90();
    u8 GetUnk91();
    MFx32 GetUnk80();
    MFx32 GetUnk84();

    DinoCleaning *mCleaning;    // 0x74
    u32 mScore;                 // 0x78: out of 100
    u8 mFlag;                   // 0x7C
    MFx32 mUnk80;               // 0x80
    MFx32 mUnk84;               // 0x84
    MFx32 mPrev80;              // 0x88: last frame's
    MFx32 mPrev84;              // 0x8C
    u8 mUnk90;                  // 0x90
    u8 mUnk91;                  // 0x91
    MRasterPal *mRaster;        // 0x94: the backdrop colour per line
    u16 *mRasterFront;          // 0x98
    u16 *mRasterBack;           // 0x9C
    u16 mGradients[3][2][0xC0]; // 0xA0: three backdrop gradients, in two brightnesses
    MMotion *mSuccessWindow;    // 0x9A0
    MMotion *mFailureWindow;    // 0x9A4
    MMotion *mDigits[3];        // 0x9A8
    MMotion *mDigits2[3];       // 0x9B4
    MMotion *mArrow;            // 0x9C0
    u8 mUnk9C4;                 // 0x9C4
    u32 mUnk9C8;                // 0x9C8
    u8 mUnk9CC;                 // 0x9CC
};

#endif
