// DinoUiTitle, DinoUiTitleSkip, DinoUiOpeningCloud, DinoUiOpeningChara, DinoUiTitleMenuGameItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "m/seed_generator.h"
#include "m/bright.h"
#include "m/picture.h"
#include "game/backup_validater.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/smart_oam.h"
#include "m/motion.h"
#include "m/input.h"
#include "m/menu.h"
#include "game/dino_ui_top_menu_loop.h"
#include "game/dino_ui_save_slot_select.h"
#include "game/dino_sound.h"
#include "game/dino_ui_title_menu.h"
#include "game/dino_ui_title.h"
#include "game/dino_ui_title_skip.h"
#include "game/dino_ui_opening_cloud.h"
#include "game/dino_ui_opening_chara.h"
#include "game/dino_ui_title_menu_game_item.h"
#include "m/bg.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiTitle::~DinoUiTitle() {
}

DinoUiTitleSkip::~DinoUiTitleSkip() {
}

DinoUiOpeningCloud::~DinoUiOpeningCloud() {
}

DinoUiOpeningChara::DinoUiOpeningChara(MSmartOam *oam, std::string path, MPoint pos) : MProcess(NULL) {
    mPos = pos;
    mMotion = new MMotion(std::string(path), oam, 2, 1);
    mMotion->SetPriority(0);
    mMotion->SetAnchor(1);
    mMotion->Play(1, 0);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

static inline void AddSerial(DinoUiOpeningChara *chara, MTask *a, MTask *b) {
    MSerialTaskSet *serial = &chara->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
}

// Sways from side to side, at a random pace.
void DinoUiOpeningChara::Idle() {
    AddSerial(this,
              mMotion->CreateCoordAnime(mPos.Offset(-16, 0), mPos.Offset(8, 0),
                                        gDinoResource->GetRandom()->Next(20) + 80, 0),
              mMotion->CreateCoordAnime(mPos.Offset(8, 0), mPos.Offset(-16, 0),
                                        gDinoResource->GetRandom()->Next(20) + 40, 11));
}

DinoUiOpeningChara::~DinoUiOpeningChara() {
}

DinoUiTitleMenuGameItem::~DinoUiTitleMenuGameItem() {
}

DinoUiTitleMenu::~DinoUiTitleMenu() {
}

void DinoUiTitle::Idle() {
    (this->*mState)();
}

DinoUiOpening::~DinoUiOpening() {
}

void DinoUiOpening::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MBgCallTask0<DinoUiTitle>;
template MTask *MCreateBgCallTask(DinoUiTitle *obj, void (DinoUiTitle::*func)(), int thread);
template class MCallTask1<MBg, MBg, bg_mode>;
template MTask *MCreateCallTask(MBg *obj, void (MBg::*func)(bg_mode), bg_mode a0);
template class MBgCallTask0<DinoUiOpening>;
template MTask *MCreateBgCallTask(DinoUiOpening *obj, void (DinoUiOpening::*func)(), int thread);
#pragma auto_inline reset

void DinoUiTitleMenu::OnFocus(MMenuItem *item) {
    mIdleTime = 0;
    if (mSilentFocus) {
        mSilentFocus = FALSE;
    } else {
        DinoSoundPlaySe(0x38E, 0);
    }
}

void DinoUiTitleMenu::OnDecide(MMenuItem *item) {
    if (item == mStartItem) {
        DinoSoundPlaySe(0x390, 0);
        *mResult = 3;
        mMotion->Play(2, 0);
        mSerial.AddTail(mMotion->CreateWaitStopTask());
    }
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

MRect DinoUiTitleMenuGameItem::GetFrame() {
    return MRect(0, 0, 256, 192);
}

void DinoUiTitleMenu::OnIdle() {
    if (mOam->mOffset.y == 0) {
        mMotion->Play(1, 0);
        if (mIdleTime++ >= 1200) {
            *mResult = 8;
            Exit();
        }
    }
}

void DinoUiOpening::Execute() {
    if (mInput->GetPressed() & 0x1C0B) { // A, B, Start, X, Y or a touch
        mSkipped = TRUE;
    }
    MProcess::Execute();
}

void DinoUiOpeningCloud::Idle() {
    mSerial.AddTail(mCloud->CreateCoordAnime(MPoint(-512, 0), MPoint(0, 0), 600, 0));
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
// The TU's pooled pointer-to-member constants (see menu.cpp). The game's pool is one block; dsd splits it every 0x80
// bytes (data_ov004_02172ff8, data_ov004_02173078), and tools/anon_names.py maps the offsets back.
extern "C" struct {
    u8 unk00[0x8];
    const char *mScreenPaths[3]; // 0x08: the screens before the opening
    const char *mTouchPath; // 0x14: the title menu's "touch" prompt
    u8 unk18[0x24 - 0x18];
    const char *mLogoPath; // 0x24
    u8 unk28[0x2C - 0x28];
    void (DinoUiTitle::*StateMenu_2C)(); // 0x2c
    u8 unk34[0x3C - 0x34];
    void (DinoUiTitle::*Unk0215ee60_3C)(); // 0x3c
    void (DinoUiTitle::*Unk0215f4ec_44)(); // 0x44
    u8 unk4C[0x54 - 0x4C];
    void (DinoUiTitle::*StateToSaveSlot_54)(); // 0x54
    u8 unk5C[0x6C - 0x5C];
    void (DinoUiTitle::*Unk0215d818_6C)(); // 0x6c
    void (DinoUiTitle::*StateOpening_74)(); // 0x74
    void (DinoUiTitle::*StateSaveSlot_7C)(); // 0x7c
    void (DinoUiTitle::*StateTopMenuEnd_84)(); // 0x84
    u8 unk8C[0x94 - 0x8C];
    void (DinoUiTitle::*LoadLogo_94)(); // 0x94
    void (DinoUiTitle::*Unk0215f1e0_9C)(); // 0x9c
    u8 unkA4[0xAC - 0xA4];
    void (DinoUiTitle::*Unk0215e03c_AC)(); // 0xac
    void (DinoUiTitle::*Start_B4)(); // 0xb4
    u8 unkBC[0xC4 - 0xBC];
    void (DinoUiTitle::*Unk0215d818_C4)(); // 0xc4
    u8 unkCC[0xE4 - 0xCC];
    void (DinoUiTitle::*StateAdvice2_E4)(); // 0xe4
    void (DinoUiTitle::*Unk0215e03c_EC)(); // 0xec
    void (DinoUiTitle::*Unk0215f5ac_F4)(); // 0xf4
    u8 unkFC[0x114 - 0xFC];
    void (DinoUiOpening::*OpeningStart_114)(); // 0x114
    u8 unk11C[0x124 - 0x11C];
    void (DinoUiTitle::*Unk0215e03c_124)(); // 0x124
    void (DinoUiTitle::*Unk0215f420_12C)(); // 0x12c
    void (DinoUiTitle::*Unk0215d6b4_134)(); // 0x134
    void (DinoUiTitle::*Unk0215d818_13C)(); // 0x13c
    void (DinoUiTitle::*Unk0215e9e4_144)(); // 0x144
    void (DinoUiTitle::*StateWarning_14C)(); // 0x14c
    void (MBg::*SetMode_154)(bg_mode); // 0x154
    void (DinoUiTitle::*StateSaveSlot_15C)(); // 0x15c
    void (DinoUiTitle::*StateSaveSlotEnd_164)(); // 0x164
} data_ov004_02172f78;

inline DinoUiOpening::DinoUiOpening(MInput *input) : MProcess(NULL), mInput(input) {
    mState = data_ov004_02172f78.OpeningStart_114;
}

DinoUiTitle::DinoUiTitle(s32 *result) : MProcess(NULL), mResult(result) {
    mState = data_ov004_02172f78.Start_B4;
}

// Slides the title away and the save slots in, then shows the slot select.
void DinoUiTitle::StateToSaveSlot() {
    GetSerial()->AddTail((new MParallelTaskSet(TRUE))
                             ->Add(mUnk8C->CreateCoordAnime(MPoint(0, 0), MPoint(0, -0xA0), 60, 12))
                             ->Add(mUnk90->CreateCoordAnime(MPoint(0, 0xC0), MPoint(0, 0x10), 60, 12)),
                         MTaskExitTransfer::Create(mUnk8C), new MTimer(1),
                         MCreateCallTask(gLcd->GetBottomDisplay()->mBg, data_ov004_02172f78.SetMode_154, (bg_mode)0));
    mState = data_ov004_02172f78.StateSaveSlot_15C;
}

void DinoUiTitleMenu::OnStart() {
    mIdleTime = 0;
    mSilentFocus = TRUE;
    DinoSaveSummary summary;
    for (u32 i = 0; i < 2; i++) {
        gDinoBackup->GetSummary(i, &summary);
        if (summary.valid) {
            break;
        }
    }
    mOam = new MSmartOam(gLcd->GetBottomDisplay(), 0x3C, 0, TRUE);
    mOam->SetUnk1C(7);
    mMotion = new MMotion(std::string(data_ov004_02172f78.mTouchPath), mOam, 0, 1);
    mMotion->SetPosition(128, 96);
    mMotion->Show();
    GetParallel()->AddTail(mMotion, mOam);
    mStartItem = new DinoUiTitleMenuGameItem;
    AddItem(mStartItem);
    if (*mResult == 3) {
        FocusItem(mStartItem);
    }
}

void DinoUiTitle::StateOpening() {
    mSerial.AddTail(new DinoUiOpening(mInput));
    mState = data_ov004_02172f78.Unk0215e03c_AC;
}

void DinoUiTitle::StateMenu() {
    mParallel.Remove(mMenu);
    mMenu->Activate();
    mSerial.AddTail(mMenu);
    mState = data_ov004_02172f78.Unk0215e9e4_144;
}

void DinoUiTitle::StateSaveSlot() {
    *mResult = 4;
    mSerial.AddTail(new DinoUiSaveSlotSelect(mInput, &mSlotChoice));
    mState = data_ov004_02172f78.StateSaveSlotEnd_164;
}

void DinoUiTitle::StateSaveSlotEnd() {
    switch (mSlotChoice) {
    case 1:
        mState = data_ov004_02172f78.Unk0215ee60_3C;
        break;
    case 0:
        mState = data_ov004_02172f78.Unk0215f1e0_9C;
        break;
    }
}

void DinoUiTitle::StateTopMenu() {
    mSerial.AddTail(new DinoUiTopMenuLoop(&mTopResult));
    mState = data_ov004_02172f78.StateTopMenuEnd_84;
}

void DinoUiTitle::StateTopMenuEnd() {
    switch (mTopResult) {
    case 1:
        mState = data_ov004_02172f78.Unk0215f420_12C;
        break;
    case 0:
        mState = data_ov004_02172f78.StateSaveSlot_7C;
        break;
    case 2:
        mState = data_ov004_02172f78.Unk0215f4ec_44;
        break;
    case 3:
        mState = data_ov004_02172f78.Unk0215f5ac_F4;
        break;
    }
}

// How the title starts: 0 normally, 4 after the opening, 5 and 7 (the backup checked first) to the top menu, 6 to
// the save slots.
void DinoUiTitle::StateStart() {
    MSingleInputHub *hub = new MSingleInputHub(1);
    hub->EnableTouch();
    mInput = hub->GetInput(0);
    mParallel.AddTail(hub);
    switch (*mResult) {
    case 0:
        mTopResult = 1;
        mState = data_ov004_02172f78.Unk0215d6b4_134;
        break;
    case 4:
        mTopResult = 1;
        mState = data_ov004_02172f78.Unk0215d818_13C;
        break;
    case 7:
        mSerial.AddTail(new DinoBackupReadValidater(NULL, NULL));
    case 5:
        mTopResult = 2;
        mState = data_ov004_02172f78.Unk0215d818_C4;
        break;
    case 6:
        DinoSoundPlayBgm(0x77, 0);
        mTopResult = 3;
        mState = data_ov004_02172f78.Unk0215d818_6C;
        break;
    }
}

#pragma dont_inline on
void DinoUiTitle::StateShowLogo() {
    gLcd->mMain->mBg->SetMode(3);
    mOam = new MSmartOam(10, 0, 1);
    mLogo = new MMotion(data_ov004_02172f78.mLogoPath, mOam, 0, 1);
    mLogo->SetPriority(1);
    mLogo->SetPosition(0x80, 0x60);
    mLogo->Play(3, 0);
    mLogo->Show();
    MSmartOam *oam = mOam;
    MTaskSet &parallel = mParallel;
    parallel.AddTail(mLogo);
    parallel.AddTail(oam);
    mSerial.AddTail(MCreateBgCallTask(this, data_ov004_02172f78.LoadLogo_94, 1));
    mState = data_ov004_02172f78.StateMenu_2C;
}
#pragma dont_inline reset

// The health and safety warning (and the backup check).
void DinoUiTitle::StateWarning() {
    MTextPicture *screen = new MTextPicture(data_ov004_02172f78.mScreenPaths[1], TRUE, true);
    screen->Show();
    mParallel.AddTail(screen);
    MTask *validate = new DinoBackupReadValidater(NULL, NULL);
    MTask *exit = MTaskExitTransfer::Create(screen);
    MTask *skip = new DinoUiTitleSkip(&mSkipped, mInput, 30, 30, 1, 0);
    MTask *wait = new MTimer(60);
    MTask *fade = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(30, 0);
    MTaskSet &serial = mSerial;
    serial.AddTail(fade);
    serial.AddTail(wait);
    serial.AddTail(skip);
    serial.AddTail(exit);
    serial.AddTail(validate);
    mState = data_ov004_02172f78.StateWarning_14C;
}

void DinoUiTitle::StateAdvice() {
    if (mSkipped) {
        mState = data_ov004_02172f78.Unk0215e03c_EC;
        return;
    }
    MTextPicture *screen = new MTextPicture(data_ov004_02172f78.mScreenPaths[2], TRUE, false);
    screen->Show();
    mParallel.AddTail(screen);
    MTask *exit = MTaskExitTransfer::Create(screen);
    MTask *skip = new DinoUiTitleSkip(&mSkipped, mInput, 60, 15, 1, 0);
    MTask *fade = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(15, 0);
    MTaskSet &serial = mSerial;
    serial.AddTail(fade);
    serial.AddTail(skip);
    serial.AddTail(exit);
    mState = data_ov004_02172f78.StateAdvice2_E4;
}

void DinoUiTitle::StateAdvice2() {
    if (mSkipped) {
        mState = data_ov004_02172f78.Unk0215e03c_124;
        return;
    }
    MTextPicture *screen = new MTextPicture(data_ov004_02172f78.mScreenPaths[0], TRUE, false);
    screen->Show();
    mParallel.AddTail(screen);
    MTask *exit = MTaskExitTransfer::Create(screen);
    MTask *skip = new DinoUiTitleSkip(&mSkipped, mInput, 60, 15, 1, 1);
    MTask *fade = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(15, 0);
    MTaskSet &serial = mSerial;
    serial.AddTail(fade);
    serial.AddTail(skip);
    serial.AddTail(exit);
    mState = data_ov004_02172f78.StateOpening_74;
}

// Leaves for the game: fades both screens out to white.
void DinoUiTitle::StateEnd() {
    *mResult = 3;
    DinoSoundStopBgm(-1, 60);
    MTask *fadeTop;
    MTask *fadeBottom;
    MTask *exit;
    exit = MTaskExitTransfer::Create(this);
    fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToWhite(60, 0);
    fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToWhite(60, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeTop);
    fades->AddTail(fadeBottom);
    MTaskSet &serial = mSerial;
    serial.AddTail(fades);
    serial.AddTail(exit);
}

// (Run in the background.)
void DinoUiTitle::ResetBackup() {
    gDinoBackup->Reset(TRUE);
    gDinoBackup->WriteSlotRaw();
    gDinoBackup->WriteHeader();
}

void DinoUiTitle::LoadBackup() {
    gDinoBackup->Load();
    gDinoBackup->WriteHeader();
}

// Leaves for a wireless battle.
void DinoUiTitle::StateWirelessBattle() {
    *mResult = 5;
    MTask *fadeTop;
    MTask *fadeBottom;
    MTask *exit;
    exit = MTaskExitTransfer::Create(this);
    fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToWhite(60, 0);
    fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToWhite(60, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeTop);
    fades->AddTail(fadeBottom);
    MTaskSet &serial = mSerial;
    serial.AddTail(fades);
    serial.AddTail(exit);
}

// Leaves for the ATM (the download play).
void DinoUiTitle::StateATM() {
    *mResult = 6;
    DinoSoundStopBgm(-1, 60);
    MTask *fadeTop;
    MTask *fadeBottom;
    MTask *exit;
    exit = MTaskExitTransfer::Create(this);
    fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(60, 0);
    fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(60, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeTop);
    fades->AddTail(fadeBottom);
    MTaskSet &serial = mSerial;
    serial.AddTail(fades);
    serial.AddTail(exit);
}

// Counts the screen's frames down; A, B, Start, X, Y or a touch cuts it short (Start skips the rest).
void DinoUiTitleSkip::Idle() {
    u32 pressed = mInput->GetPressed();
    if (pressed & 0x1C0B) {
        if (pressed & 0x8) {
            *mSkipped = TRUE;
        }
        mFrames = 0;
        mFadeFrames = 15;
    }
    if (mFrames != 0) {
        mFrames--;
        return;
    }
    if (mUnk85 || (*mSkipped && mUnk84)) {
        MTask *fadeTop;
        MTask *fadeBottom;
        fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToWhite(mFadeFrames, 0);
        fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToWhite(mFadeFrames, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        mSerial.AddTail(fades);
    } else {
        mSerial.AddTail(gCurrentDisplay->mBright->CreateFadeOutToBlack(mFadeFrames, 0));
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    MTaskSet &serial = mSerial;
    serial.AddTail(exit);
}
