// DinoCleaningScore: the score screen at the end of a cleaning.

// (The game's -ipa file build: func_ov002_0215bce8's std::max compare copies sit in source order among the other
// temporaries only under it.)
#pragma ipa file

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
#include "game/dino_static_init.h"

// A colour, 0-255 a channel.
struct DinoCleaningScoreRgb {
    u32 r, g, b;
};

struct DinoScoreLiterals;
extern "C" DinoScoreLiterals data_ov002_02164a58;

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
// The TU's literal block (data_ov002_02164a58, 0x00-0x63): under ipa its 4-byte objects are laid out by mwcc's
// heapsort of the objects in reverse creation order (the strings and the class typeinfos/vtable take part), and this
// declaration order puts each one at the game's offset (+0x24 digit path, +0x3C arrow, +0x50 failure, +0x58 success,
// +0x20/+0x4C Idle's limits, the rest the channels' 0xFF clamps; the 0xFF words are the game's std::min literals,
// written as statics named after their game symbols so their order can be chosen).
// sScorePad stands for the game's word at +0x00 (a 0, probably a dead literal temporary): a 4-byte object created
// here that Show keeps alive without code ((void)sScorePad). Its value is not the game's (= 0 would go to .bss).
static u32 data_ov002_02164a6c = 0xFF;
static const char *sDigitPath = "/motion/cleaning/score_digit.bin";
static u32 data_ov002_02164a80 = 0xFF;
static u32 data_ov002_02164a68 = 0xFF;
static u32 data_ov002_02164a74 = 0xFF;
static u32 data_ov002_02164a5c = 0xFF;
static u32 data_ov002_02164a60 = 0xFF;
static u32 data_ov002_02164a64 = 0xFF;
static int sScorePad = 1;
static u32 data_ov002_02164ab8 = 0xFF;
static const char *sSuccessPath = "/motion/cleaning/success_window.bin";
static const char *sFailurePath = "/motion/cleaning/failure_window.bin";
static int data_ov002_02164aa4 = 126;
static u32 data_ov002_02164aa0 = 0xFF;
static u32 data_ov002_02164a9c = 0xFF;
static u32 data_ov002_02164a98 = 0xFF;
static u32 data_ov002_02164a70 = 0xFF;
static u32 data_ov002_02164a90 = 0xFF;
static u32 data_ov002_02164a8c = 0xFF;
static u32 data_ov002_02164a88 = 0xFF;
static u32 data_ov002_02164a84 = 0xFF;
static u32 data_ov002_02164ab4 = 0xFF;
static u32 data_ov002_02164aac = 0xFF;
static int data_ov002_02164a78 = 66;
static const char *sArrowPath = "/motion/cleaning/yajirusi.bin";
// A channel clamped to 0xFF.
#define SCORE_FF(lim, x) std::min(lim, (u32)(x))

// t percent of the way from one channel to the other.
#define LERP(from, to, t) (((from) * (100 - (t)) + (to) * (t)) / 100)
#define SCORE_RGB(r, g, b) ((((b) >> 3) << 10) | ((((g) >> 3) << 5) | (0x8000 | ((r) >> 3))))

// Builds the backdrop gradients (grey, blue and red, each in a normal and a brighter version, every channel
// clamped to 0xFF), the backdrop's raster, the success and failure windows with their score digits, and the arrow.
// NONMATCHING: 97.9%, the game keeps the digit loops' counters on the stack and this + i * 4 in a register across
// the path string's destructor (its frame is 8 bytes bigger): the known "this+i*4 across a string dtor" loop family.
DinoCleaningScore::DinoCleaningScore(DinoCleaning *cleaning, MSmartOam *oam, u32 score, u8 flag) : MProcess(NULL) {
    mCleaning = cleaning;
    mScore = score;
    mFlag = flag;
    mUnk9C4 = 0;
    mUnk9C8 = 0;
    mUnk9CC = 0;
    for (u32 k = 0; k < 2; k++) {
        for (u32 j = 0; j < 100; j++) {
            u32 r = LERP(SCORE_FF(data_ov002_02164a64, SCORE_DATA.mColors[5].r + k * 4),
                         SCORE_FF(data_ov002_02164a5c, SCORE_DATA.mColors[3].r + k * 4), j);
            u32 g = LERP(SCORE_FF(data_ov002_02164ab8, SCORE_DATA.mColors[5].g + k * 4),
                         SCORE_FF(data_ov002_02164a9c, SCORE_DATA.mColors[3].g + k * 4), j);
            u32 b = LERP(SCORE_FF(data_ov002_02164aa0, SCORE_DATA.mColors[5].b + k * 4),
                         SCORE_FF(data_ov002_02164a6c, SCORE_DATA.mColors[3].b + k * 4), j);
            mGradients[0][k][j] = SCORE_RGB(r, g, b);
            r = LERP(SCORE_FF(data_ov002_02164aac, SCORE_DATA.mColors[1].r + k * 4),
                     SCORE_FF(data_ov002_02164a70, SCORE_DATA.mColors[2].r + k * 4), j);
            g = LERP(SCORE_FF(data_ov002_02164ab4, SCORE_DATA.mColors[1].g + k * 4),
                     SCORE_FF(data_ov002_02164a88, SCORE_DATA.mColors[2].g + k * 4), j);
            b = LERP(SCORE_FF(data_ov002_02164a8c, SCORE_DATA.mColors[1].b + k * 4),
                     SCORE_FF(data_ov002_02164a98, SCORE_DATA.mColors[2].b + k * 4), j);
            mGradients[1][k][j] = SCORE_RGB(r, g, b);
            r = LERP(SCORE_FF(data_ov002_02164a90, SCORE_DATA.mColors[4].r + k * 4),
                     SCORE_FF(data_ov002_02164a68, SCORE_DATA.mColors[0].r + k * 4), j);
            g = LERP(SCORE_FF(data_ov002_02164a60, SCORE_DATA.mColors[4].g + k * 4),
                     SCORE_FF(data_ov002_02164a80, SCORE_DATA.mColors[0].g + k * 4), j);
            b = LERP(SCORE_FF(data_ov002_02164a84, SCORE_DATA.mColors[4].b + k * 4),
                     SCORE_FF(data_ov002_02164a74, SCORE_DATA.mColors[0].b + k * 4), j);
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
    (void)sScorePad; // keeps the literal block's +0x00 word (see the statics above)
    mArrow->Play(1, 0);
    mArrow->Show();
    mSerial.AddTail(mArrow->CreateCoordAnime(MPoint(0x80, 0x92), MPoint(0x80, (100 - mScore) * 100 / 100 + 0x2E), 90, -1));
}

// The score rounded down to a whole number (by value: the game copies it).
static inline MFx32 DinoScoreFloor(MFx32 x) {
    return MFx32::FromRaw(x.v & 0xFFFFF000);
}

// Works out the score from how clean the rock is (mUnk84, what's left of the fossil and mUnk80, the score; the
// second mode scores the two rocks' dirt) and shows both on the commentary; the first non-zero score moves on.
// The rock's ratio read inside lost's expression and the floor applied to the second mode's quotient (its own temp
// and copy) give the game's frame.
// The first std::max's compare param copies follow source order among the other temporaries only under the file's ipa.
// The second max: the game keeps &b (passed as a pointer) as the default and switches to &a (std::max's ternary
// picks the other way round, and a reference parameter takes b's address after a is built).
static inline const MFx32 &DinoScoreFxMax(const MFx32 &a, const MFx32 *p) {
    const MFx32 &b = *p;
    if (!(a < b)) {
        p = &a;
    }
    return *p;
}

extern "C" void func_ov002_0215bce8(DinoCleaningScore *score) {
    if (score->mCleaning->GetKasekiRock()) {
        if (!score->mFlag) {
            MFx32 lost = MFx32(100) - score->mCleaning->GetKasekiRock()->GetRock8C()->GetRatio();
            score->mUnk84 = MFx32(100 - MFx32ToInt(std::max(MFx32(0), MFx32(100) - lost * MFx32(2))));
            score->mUnk80 = DinoScoreFxMax(MFx32(0), &(const MFx32 &)DinoScoreFloor(score->mCleaning->GetKasekiRock()->GetRatio() - score->mUnk84));
        } else {
            score->mUnk84 = MFx32(0);
            score->mUnk80 =
                DinoScoreFloor(((MFx32(100) - score->mCleaning->GetKasekiRock()->GetRock8C()->GetRatio()) * MFx32(2) +
                                (MFx32(100) - score->mCleaning->GetKasekiRock()->GetRock90()->GetRatio()) * MFx32(3)) /
                               MFx32(5));
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

// The literal block's type: sized 0x80 so the static init's stores through it stay in bounds (see the sinit).
extern "C" struct DinoScoreLiterals {
    u8 unk00[0x80];
};

extern "C" u8 data_0208a22c;                    // shows the windows' numbers
extern "C" MPoint data_ov002_02164ba4[3];       // the success window's digits, from its corner
extern "C" MPoint data_ov002_02164b8c[3];       // the failure window's

// Follows the score: sounds as the score passes the target either way, the arrow's motion, the windows sliding to
// the two scores (with their digits) and the backdrop's gradients between them.
// (The last gradient loop has its own line variable, declared before the loop: it takes the game's registers.)
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
    mFailureWindow->SetPosition(0x80, std::min(data_ov002_02164a78, top));
    mSuccessWindow->SetPosition(0x80, std::max(data_ov002_02164aa4, bottom));
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
        u32 up;
        for (up = top, k = 0; up >= 46; up--, k++) {
            (&mRasterFront)[i][up] = mGradients[2][i][k];
        }
    }
}

// The file's static initialization (see dino_static_init.h): the common layout, the gradients' colours, the windows'
// digit positions and the help rect.
// Everything goes through the game's three pool anchors (0x00-0x7f via the literal block, 0x80-0xff via
// data_ov002_02164ad8, past 0x100 also via ad8 so the literal is shared); the literal block's struct must be 0x80
// bytes, or mwcc reloads the screen size's x it just stored (an access past the end of the declared object).
// NONMATCHING: 91.8%, the game hoists the screen rect's first zero (and loads data_ov002_02164ad8 first) above the
// screen size's stores, and the digit points' anchor and value registers swap.
extern "C" u32 data_ov002_02164b58[], data_ov002_02164b94[], data_ov002_02164b9c[], data_ov002_02164bac[],
    data_ov002_02164bb4[];
#define SCORE_POOL(T, off) \
    (*(T *)((off) < 0x80 ? (u8 *)&data_ov002_02164a58 + (off) \
            : (off) < 0x100 ? (u8 *)&data_ov002_02164ad8 + ((off) - 0x80) : (u8 *)&data_ov002_02164ad8 + ((off) - 0x80)))
#pragma always_inline on
static struct DinoCleaningScoreStaticInit {
    DinoCleaningScoreStaticInit() {
        DINO_POOL_NV(MPoint, data_ov002_02164a58, 0x64) = MPoint(256, 192);
        SCORE_POOL(MRect, 0xF4) = DinoSizeRect(DINO_POOL_NV(MPoint, data_ov002_02164a58, 0x64));
        DINO_POOL_NV(MPoint, data_ov002_02164a58, 0x6C) = MPoint(2, 2);
        DINO_POOL_NV(MPoint, data_ov002_02164a58, 0x74) = MPoint(36, 28);
        SCORE_POOL(MRect, 0x104) = DinoSizeRect(DINO_POOL_NV(MPoint, data_ov002_02164a58, 0x74));
        SCORE_POOL(MPoint, 0x114) = MPoint(0, 24);
        SCORE_POOL(MPoint, 0x11C) = MPoint(256, 192);
        SCORE_POOL(MRect, 0x124) = MRect(0, 0, 256, 24);
        SCORE_DATA.mColors[4].r = 0xA1;
        SCORE_DATA.mColors[4].g = 0;
        SCORE_DATA.mColors[4].b = 0xC;
        SCORE_DATA.mColors[0].r = 0x43;
        SCORE_DATA.mColors[0].g = 0;
        SCORE_DATA.mColors[0].b = 0;
        SCORE_DATA.mColors[1].r = 0;
        SCORE_DATA.mColors[1].g = 0x10;
        SCORE_DATA.mColors[1].b = 0xFF;
        SCORE_DATA.mColors[2].r = 0x72;
        SCORE_DATA.mColors[2].g = 0xC7;
        SCORE_DATA.mColors[2].b = 0xFF;
        SCORE_DATA.mColors[3].r = 0x62;
        SCORE_DATA.mColors[3].g = 0x62;
        SCORE_DATA.mColors[3].b = 0x62;
        SCORE_DATA.mColors[5].r = 0x1A;
        SCORE_DATA.mColors[5].g = 0x1A;
        SCORE_DATA.mColors[5].b = 0x1A;
        SCORE_POOL(MPoint, 0x134) = MPoint(72, -3);
        DINO_POOL(MPoint, data_ov002_02164b94, 0) = MPoint(56, -3);
        DINO_POOL(MPoint, data_ov002_02164b9c, 0) = MPoint(40, -3);
        SCORE_POOL(MPoint, 0x14C) = MPoint(-34, 3);
        DINO_POOL(MPoint, data_ov002_02164bac, 0) = MPoint(-50, 3);
        DINO_POOL(MPoint, data_ov002_02164bb4, 0) = MPoint(-66, 3);
        SCORE_DATA.mHelpRect = MRect(37, 37, 220, 157);
    }
} sDinoCleaningScoreStaticInit;
