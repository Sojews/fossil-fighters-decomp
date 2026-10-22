// DinoCleaningScore: the score screen at the end of a cleaning.

#include <algorithm>
#include <string>
#include "m/motion.h"
#include "m/raster.h"
#include "nitro/mi.h"
#include "game/dino_cleaning.h"
#include "game/dino_cleaning_kaseki_rock.h"
#include "game/dino_cleaning_rock.h"
#include "game/dino_cleaning_score.h"
#include "game/dino_common_live.h"
#include "game/dino_resource.h"
#include "game/help.h"
#include "game/dino_sound.h"

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

// Works out the score from how clean the rock is (mUnk84, what's left of the fossil and mUnk80, the score; the
// second mode scores the two rocks' dirt) and shows both on the commentary; the first non-zero score moves on.
// NONMATCHING: 93%, the MFx32 temporaries' stack copies differ and the second clamp branches the other way round.
extern "C" void func_ov002_0215bce8(DinoCleaningScore *score) {
    if (score->mCleaning->GetKasekiRock()) {
        if (!score->mFlag) {
            MFx32 ratio = score->mCleaning->GetKasekiRock()->GetRock8C()->GetRatio();
            MFx32 left = MFx32(100) - (MFx32(100) - ratio) * MFx32(2);
            score->mUnk84 = MFx32(100 - MFx32ToInt(std::max(MFx32(0), left)));
            MFx32 diff = score->mCleaning->GetKasekiRock()->GetRatio() - score->mUnk84;
            score->mUnk80 = std::max(MFx32(0), MFx32::FromRaw(diff.v & 0xFFFFF000));
        } else {
            score->mUnk84 = MFx32(0);
            MFx32 q = ((MFx32(100) - score->mCleaning->GetKasekiRock()->GetRock8C()->GetRatio()) * MFx32(2) +
                       (MFx32(100) - score->mCleaning->GetKasekiRock()->GetRock90()->GetRatio()) * MFx32(3)) /
                      MFx32(5);
            score->mUnk80 = MFx32::FromRaw(q.v & 0xFFFFF000);
        }
    } else {
        score->mUnk84 = MFx32(0);
        score->mUnk80 = MFx32(0);
    }
    score->mCleaning->GetUnkE4()->SetUnkB8(3, (void *)MFx32ToInt(score->mUnk84));
    score->mCleaning->GetUnkE4()->SetUnkB8(2, (void *)MFx32ToInt(score->mUnk80));
    switch (score->mUnk9C8) {
    case 0:
        if (score->mUnk80 != MFx32(0)) {
            score->mUnk9C8 = 2;
        } else {
            score->mUnk9C8 = 1;
        }
        break;
    case 1:
        if (score->mUnk80 != MFx32(0)) {
            score->mUnk9C8 = 2;
            if (score->mUnk9CC) {
                score->mCleaning->GetUnkE4()->SetUnkAC(5);
            }
        }
        break;
    }
}

// The TU's pooled literals (std::min/std::max operands): the value read through the pool, the operand returned by its own
// symbol.
extern "C" struct {
    u8 unk00[0x20];
    int mTopLimit;              // 0x20: 66
    u8 unk24[0x4C - 0x24];
    int mBottomLimit;           // 0x4C: 126
} data_ov002_02164a58;
extern "C" const int data_ov002_02164a78, data_ov002_02164aa4;

static inline const int &DinoScoreMin(const int &a, const int &named, const int &b) {
    return b < a ? b : named;
}
static inline const int &DinoScoreMax(const int &a, const int &named, const int &b) {
    return a < b ? b : named;
}

extern "C" u8 data_0208a22c;                    // shows the windows' numbers
extern "C" MPoint data_ov002_02164ba4[3];       // the success window's digits, from its corner
extern "C" MPoint data_ov002_02164b8c[3];       // the failure window's

// Follows the score: sounds as the score passes the target either way, the arrow's motion, the windows sliding to
// the two scores (with their digits) and the backdrop's gradients between them.
// NONMATCHING: 99.9%, the last gradient loop's line, source and offset registers rotate (r2/r3/r4); tried separate,
// swapped and while-loop counters.
void DinoCleaningScore::Idle() {
    mPrev80 = mUnk80;
    mPrev84 = mUnk84;
    func_ov002_0215bce8(this);
    mUnk91 = mUnk84 > mPrev84;
    mUnk90 = mUnk80 > mPrev80;
    bool up = mPrev80 < MFx32(mScore) && mUnk80 >= MFx32(mScore);
    if (up) {
        DinoSoundPlaySe(0x44, 0);
    }
    bool down = (!mUnk9C4 && mPrev80 >= MFx32(mScore)) && mUnk80 < MFx32(mScore);
    if (down) {
        mUnk9C4 = TRUE;
        DinoSoundPlaySe(0x40, 0);
    }
    if (GetUnk91()) {
        mArrow->Play(4, 0);
    } else if (GetUnk90()) {
        mArrow->Play(3, 0);
    } else if (!mArrow->IsPlaying()) {
        mArrow->Play(2, 0);
    }
    int top = MFx32ToInt(MFx32(46) + MFx32(100) * mUnk84 / MFx32(100));
    int bottom = MFx32ToInt(MFx32(146) - MFx32(100) * mUnk80 / MFx32(100));
    mFailureWindow->SetPosition(0x80, DinoScoreMin(data_ov002_02164a58.mTopLimit, data_ov002_02164a78, top));
    mSuccessWindow->SetPosition(0x80, DinoScoreMax(data_ov002_02164a58.mBottomLimit, data_ov002_02164aa4, bottom));
    if (data_0208a22c) {
        u32 n = MFx32ToInt(mUnk80);
        if (n != 0) {
            mSuccessWindow->Show();
        }
        if (!mSuccessWindow->IsHidden()) {
            for (u32 i = 0; i < 3; i++) {
                u32 digit = n % 10;
                n /= 10;
                mDigits[i]->Play(digit + 1, 0);
                mDigits[i]->SetPosition(data_ov002_02164ba4[i] + mSuccessWindow->GetPosition());
                if (n != 0 || digit != 0 || i == 0) {
                    mDigits[i]->Show();
                } else {
                    mDigits[i]->Hide();
                }
            }
        }
        n = MFx32ToInt(mUnk84);
        if (n != 0) {
            mFailureWindow->Show();
        }
        if (!mFailureWindow->IsHidden()) {
            for (u32 i = 0; i < 3; i++) {
                u32 digit = n % 10;
                n /= 10;
                mDigits2[i]->Play(digit + 1, 0);
                mDigits2[i]->SetPosition(data_ov002_02164b8c[i] + mFailureWindow->GetPosition());
                if (n != 0 || digit != 0 || i == 0) {
                    mDigits2[i]->Show();
                } else {
                    mDigits2[i]->Hide();
                }
            }
        }
    }
    for (u32 i = 0; i < 2; i++) {
        u32 k, line;
        for (line = top + 1, k = 0; line < bottom; line++, k++) {
            (&mRasterFront)[i][line] = mGradients[0][i][k + (top - 45)];
        }
        for (line = bottom, k = 0; line < 146; line++, k++) {
            (&mRasterFront)[i][line] = mGradients[1][i][k];
        }
        for (line = top, k = 0; line >= 46; line--, k++) {
            (&mRasterFront)[i][line] = mGradients[2][i][k];
        }
    }
}
