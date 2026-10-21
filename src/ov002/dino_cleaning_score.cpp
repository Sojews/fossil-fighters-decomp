// DinoCleaningScore: the score screen at the end of a cleaning.

#include <algorithm>
#include <string>
#include "m/motion.h"
#include "m/raster.h"
#include "nitro/mi.h"
#include "game/dino_cleaning_score.h"
#include "game/dino_resource.h"
#include "game/help.h"

// A colour, 0-255 a channel.
struct DinoCleaningScoreRgb {
    u32 r, g, b;
};

static const char *sDigitPath = "/motion/cleaning/score_digit.bin";
static const char *sArrowPath = "/motion/cleaning/yajirusi.bin";
static const char *sFailurePath = "/motion/cleaning/failure_window.bin";
static const char *sSuccessPath = "/motion/cleaning/success_window.bin";

// The statics this TU's __sinit builds.
extern "C" struct {
    u8 unk00[0x10];
    DinoCleaningScoreRgb mColors[6]; // 0x10: the gradients' ends (dark red, blue, light blue, grey, red, dark grey)
    u8 unk58[0x64 - 0x58];
    MRect mHelpRect;            // 0x64
} data_ov002_02164ad8;

extern "C" const u32 data_0208ad34;  // the help text over the score
extern "C" void func_ov002_0215bce8(DinoCleaningScore *score);

#define SCORE_DATA data_ov002_02164ad8
// t percent of the way from one channel to the other.
#define LERP(from, to, t) (((from) * (100 - (t)) + (to) * (t)) / 100)
#define SCORE_RGB(r, g, b) ((((b) >> 3) << 10) | ((((g) >> 3) << 5) | (0x8000 | ((r) >> 3))))

// Builds the backdrop gradients (grey, blue and red, each in a normal and a brighter version, every channel
// clamped to 0xFF), the backdrop's raster, the success and failure windows with their score digits, and the arrow.
// NONMATCHING: 97.9%, the game keeps the digit loops' counters on the stack (its frame is 8 bytes bigger), and the
// clamp literals' offsets in the TU's data block differ until the rest of the TU's data is written.
DinoCleaningScore::DinoCleaningScore(DinoCleaning *cleaning, MSmartOam *oam, u32 score, u8 flag) : MProcess(NULL) {
    mCleaning = cleaning;
    mScore = score;
    mFlag = flag;
    mUnk9C4 = 0;
    mUnk9C8 = 0;
    mUnk9CC = 0;
    for (u32 k = 0; k < 2; k++) {
        for (u32 j = 0; j < 100; j++) {
            u32 r = LERP(std::min((u32)0xFF, SCORE_DATA.mColors[5].r + k * 4),
                         std::min((u32)0xFF, SCORE_DATA.mColors[3].r + k * 4), j);
            u32 g = LERP(std::min((u32)0xFF, SCORE_DATA.mColors[5].g + k * 4),
                         std::min((u32)0xFF, SCORE_DATA.mColors[3].g + k * 4), j);
            u32 b = LERP(std::min((u32)0xFF, SCORE_DATA.mColors[5].b + k * 4),
                         std::min((u32)0xFF, SCORE_DATA.mColors[3].b + k * 4), j);
            mGradients[0][k][j] = SCORE_RGB(r, g, b);
            r = LERP(std::min((u32)0xFF, SCORE_DATA.mColors[1].r + k * 4),
                     std::min((u32)0xFF, SCORE_DATA.mColors[2].r + k * 4), j);
            g = LERP(std::min((u32)0xFF, SCORE_DATA.mColors[1].g + k * 4),
                     std::min((u32)0xFF, SCORE_DATA.mColors[2].g + k * 4), j);
            b = LERP(std::min((u32)0xFF, SCORE_DATA.mColors[1].b + k * 4),
                     std::min((u32)0xFF, SCORE_DATA.mColors[2].b + k * 4), j);
            mGradients[1][k][j] = SCORE_RGB(r, g, b);
            r = LERP(std::min((u32)0xFF, SCORE_DATA.mColors[4].r + k * 4),
                     std::min((u32)0xFF, SCORE_DATA.mColors[0].r + k * 4), j);
            g = LERP(std::min((u32)0xFF, SCORE_DATA.mColors[4].g + k * 4),
                     std::min((u32)0xFF, SCORE_DATA.mColors[0].g + k * 4), j);
            b = LERP(std::min((u32)0xFF, SCORE_DATA.mColors[4].b + k * 4),
                     std::min((u32)0xFF, SCORE_DATA.mColors[0].b + k * 4), j);
            mGradients[2][k][j] = SCORE_RGB(r, g, b);
        }
    }
    mRaster = new MRasterPal(-1, NULL);
    mParallel.AddTail(mRaster);
    mRasterFront = (u16 *)mRaster->mFront;
    mRasterBack = (u16 *)mRaster->mBack;
    mRaster->SetFlipOnVBlank(TRUE);
    MI_CpuFill8(mRasterFront, 0, 0xC0 * sizeof(u16));
    MI_CpuFill8(mRasterBack, 0, 0xC0 * sizeof(u16));

    mSuccessWindow = new MMotion(std::string(sSuccessPath), oam, 0, 1);
    mSuccessWindow->Play(1, 0);
    mSuccessWindow->SetPriority(2);
    mSuccessWindow->SetAnchor(2);
    mParallel.AddTail(mSuccessWindow);
    for (u32 i = 0; i < 3; i++) {
        mDigits[i] = new MMotion(std::string(sDigitPath), oam, 0, 1);
        mDigits[i]->SetPriority(2);
        mDigits[i]->SetAnchor(1);
        mParallel.AddTail(mDigits[i]);
    }
    mParallel.AddTail(new DinoHelpDummyItem(gCurrentDisplay, SCORE_DATA.mHelpRect,
                                            std::string(gDinoResource->GetText(data_0208ad34))));

    mFailureWindow = new MMotion(std::string(sFailurePath), oam, 0, 1);
    mFailureWindow->Play(1, 0);
    mFailureWindow->SetPriority(2);
    mFailureWindow->SetAnchor(2);
    mParallel.AddTail(mFailureWindow);
    for (u32 i = 0; i < 3; i++) {
        mDigits2[i] = new MMotion(std::string(sDigitPath), oam, 0, 1);
        mDigits2[i]->SetPriority(2);
        mDigits2[i]->SetAnchor(1);
        mParallel.AddTail(mDigits2[i]);
    }
    func_ov002_0215bce8(this);
    mUnk90 = mUnk91 = 0;

    mArrow = new MMotion(std::string(sArrowPath), oam, 0, 1);
    mArrow->SetPriority(2);
    mArrow->SetAnchor(3);
    mParallel.AddTail(mArrow);
}

DinoCleaningScore::~DinoCleaningScore() {
}

#pragma auto_inline off

MFx32 DinoCleaningScore::GetUnk80() {
    return mUnk80;
}

MFx32 DinoCleaningScore::GetUnk84() {
    return mUnk84;
}

u8 DinoCleaningScore::GetUnk90() {
    return mUnk90;
}

u8 DinoCleaningScore::GetUnk91() {
    return mUnk91;
}

void DinoCleaningScore::SetUnk9CC() {
    mUnk9CC = TRUE;
}

// Shows the score's gauge, rising to the score.
void DinoCleaningScore::Show() {
    mArrow->Play(1, 0);
    mArrow->Show();
    mSerial.AddTail(mArrow->CreateCoordAnime(MPoint(0x80, 0x92), MPoint(0x80, (100 - mScore) * 100 / 100 + 0x2E), 90, -1));
}
