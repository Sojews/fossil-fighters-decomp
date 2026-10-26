// DinoFieldPoint: the money and donation counters on the field.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/lcd.h"
#include "game/dino_field_point.h"
#include "types.h"
#include "game/dino_backup.h"
#include "nitro/os.h"
#include "m/motion.h"
#include "m/smart_oam.h"
#include "game/dino_sound.h"
#include "game/dino_static_init.h"

DinoFieldPoint::~DinoFieldPoint() {
}

DinoFieldMoney::~DinoFieldMoney() {
}

DinoFieldDonate::~DinoFieldDonate() {
}

// The TU's pooled data (the digit position and paths are set by the TU's static initializer).
extern "C" struct {
    const char *mPaths[13];         // 0x00: motion file paths
    u8 unk34[0x3C - 0x34];
    MPoint mDigitPos;               // 0x3C: of the first digit
} data_ov001_0216ab40;
// The digits' motion paths (set by the TU's static initializer).
extern "C" const char *data_ov001_0216abb4[10];
extern const u32 data_0208a214;
extern const u32 data_0208a218;
extern const u32 data_0208a21c;

DinoFieldMoney::DinoFieldMoney() : DinoFieldPoint(data_ov001_0216ab40.mPaths[4], 8, data_0208a21c, data_0208a218, data_0208a214) {
}

DinoFieldDonate::DinoFieldDonate() : DinoFieldPoint(data_ov001_0216ab40.mPaths[2], 5, data_0208a21c, data_0208a218, data_0208a214) {
}

extern "C" const char data_ov001_0216ae40[]; // the money format
extern "C" const char data_ov001_0216ae44[]; // the donation format

void DinoFieldMoney::Format(char *buf) {
    OS_SPrintf(buf, data_ov001_0216ae40, gDinoBackup->GetSystemVar(3));
}

void DinoFieldDonate::Format(char *buf) {
    OS_SPrintf(buf, data_ov001_0216ae44, gDinoBackup->GetSystemVar(0x13));
}

#pragma auto_inline off

MPoint DinoFieldPoint::GetDigitPos(u32 index) {
    return MPoint(data_ov001_0216ab40.mDigitPos.x + index * 8, data_ov001_0216ab40.mDigitPos.y);
}

DinoFieldPoint::DinoFieldPoint(const std::string &path, int digits, u32 a, u32 b, u32 c)
    : MProcess(gLcd->GetBottomDisplay()), mText(path) {
    mDigits = digits;
    mShowSe = a;
    mChangeSe = b;
    mHideSe = c;
}

MRect DinoFieldPoint::GetFrame() {
    return mFrame->GetFrame();
}

// Redraws the digits that changed.
void DinoFieldPoint::Idle() {
    bool changed = false;
    char buf[0xC];
    Format(buf);
    for (u32 i = 0; i < mDigits; i++) {
        if (buf[i] != mChars[i]) {
            changed = true;
            mChars[i] = buf[i];
            if (mDigitMotions[i] != NULL) {
                mTasks->Remove(mDigitMotions[i]);
                delete mDigitMotions[i];
            }
            if (mChars[i] == ' ') {
                mDigitMotions[i] = NULL;
            } else {
                mDigitMotions[i] =
                    new MMotion(std::string(data_ov001_0216abb4[mChars[i] - '0']), mOam, 0, 1);
                mDigitMotions[i]->SetPosition(GetDigitPos(i));
                mDigitMotions[i]->Show();
                mDigitMotions[i]->Play(2, 0);
                mTasks->AddTail(mDigitMotions[i]);
            }
        }
    }
    if (changed && mChangeSe != -1) {
        DinoSoundPlaySe(mChangeSe, 0);
    }
}

MTask *DinoFieldPoint::CreateShowTask() {
    mTasks = new MParallelTaskSet;
    mOam = new MSmartOam(mDigits + 4, 0, TRUE);
    mOam->SetUnk1C(5);
    GetParallel()->AddTail(mTasks, mOam);
    mFrame = new MMotion(std::string(mText), mOam, 0, 1);
    mFrame->SetPosition(128, 96);
    mFrame->SetAnchor(1);
    mFrame->Show();
    mTasks->AddTail(mFrame);
    Format(mChars);
    for (u32 i = 0; i < mDigits; i++) {
        if (mChars[i] == ' ') {
            mDigitMotions[i] = NULL;
        } else {
            mDigitMotions[i] = new MMotion(std::string(data_ov001_0216abb4[mChars[i] - '0']), mOam, 0, 1);
            mDigitMotions[i]->SetPosition(GetDigitPos(i));
            mDigitMotions[i]->Show();
            mTasks->AddTail(mDigitMotions[i]);
        }
    }
    if (mShowSe != -1) {
        DinoSoundPlaySe(mShowSe, 0);
    }
    mFrame->Play(1, 0);
    for (u32 i = 0; i < mDigits; i++) {
        if (mDigitMotions[i] != NULL) {
            mDigitMotions[i]->Play(1, 0);
        }
    }
    return mFrame->CreateWaitStopTask();
}

MTask *DinoFieldPoint::CreateHideTask() {
    if (mHideSe != -1) {
        DinoSoundPlaySe(mHideSe, 0);
    }
    mFrame->Play(3, 0);
    for (u32 i = 0; i < mDigits; i++) {
        if (mDigitMotions[i] != NULL) {
            mDigitMotions[i]->Play(3, 0);
        }
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *wait = mFrame->CreateWaitStopTask();
    MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
    tasks->AddTail(wait);
    tasks->AddTail(exit);
    return tasks;
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov001_0216abc0[];
static struct DinoFieldPointStaticInit {
    DinoFieldPointStaticInit() {
        DINO_POOL_WORD(data_ov001_0216ab40, 0x3c) = 13;
        DINO_POOL_WORD(data_ov001_0216ab40, 0x40) = 16;
        DINO_POOL_WORD(data_ov001_0216ab40, 0x74) = DINO_POOL_WORD(data_ov001_0216ab40, 0xc);
        DINO_POOL_WORD(data_ov001_0216ab40, 0x78) = DINO_POOL_WORD(data_ov001_0216ab40, 0x20);
        DINO_POOL_WORD(data_ov001_0216ab40, 0x7c) = DINO_POOL_WORD(data_ov001_0216ab40, 0x2c);
        DINO_POOL_WORD(data_ov001_0216abc0, 0x0) = DINO_POOL_WORD(data_ov001_0216ab40, 0x18);
        DINO_POOL_WORD(data_ov001_0216abc0, 0x4) = DINO_POOL_WORD(data_ov001_0216ab40, 0x1c);
        DINO_POOL_WORD(data_ov001_0216abc0, 0x8) = DINO_POOL_WORD(data_ov001_0216ab40, 0x14);
        DINO_POOL_WORD(data_ov001_0216abc0, 0xc) = DINO_POOL_WORD(data_ov001_0216ab40, 0x28);
        DINO_POOL_WORD(data_ov001_0216abc0, 0x10) = DINO_POOL_WORD(data_ov001_0216ab40, 0x24);
        DINO_POOL_WORD(data_ov001_0216abc0, 0x14) = DINO_POOL_WORD(data_ov001_0216ab40, 0x30);
        DINO_POOL_WORD(data_ov001_0216abc0, 0x18) = DINO_POOL_WORD(data_ov001_0216ab40, 0x4);
    }
} sDinoFieldPointStaticInit;
