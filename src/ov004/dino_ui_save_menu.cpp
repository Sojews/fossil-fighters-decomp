// DinoUiSaveMenu.
// Only the destructors are decompiled so far.

#include "m/bright.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/picture.h"
#include "game/dino_ui_save_slot_confirm.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_ui_save_menu.h"
#include "game/dino_common_window_message_box.h"
#include "game/backup_validater.h"
#include "game/dino_backup.h"

DinoUiSaveMenu::~DinoUiSaveMenu() {
}

void DinoUiSaveMenu::Idle() {
    (this->*mState)();
}

// Ends the window, answering 1 for yes (mUnk8C 1) and 2 otherwise.
void DinoUiSaveMenu::StateFinish() {
    mUnk94->Exit();
    if (mUnk8C == 1) {
        *mResult = 1;
        Exit();
    } else {
        *mResult = 2;
        Exit();
    }
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiSaveMenu::*StateConfirmed_04)();
    void (DinoUiSaveMenu::*Start_0C)();
    void (DinoUiSaveMenu::*StateFinish_14)();
    void (DinoBackup::*Save_1C)();
    void (DinoUiSaveMenu::*StateSave_24)();
    void (DinoUiSaveMenu::*StateAskOverwrite_2C)();
    u8 unk34[0x44 - 0x34];
    void (DinoUiSaveMenu::*StateAskSave_44)();
} data_ov004_02175130;
// (MCreateBgCallTask is only declared here: the game calls the copy in dino_ui_exchange.cpp.)
template <class T> MTask *MCreateBgCallTask(T *obj, void (T::*func)(), int thread);

// "/image/m_back00" (the TU's string literals are pooled data too).
extern "C" const char data_ov004_021751fc[];
// Text ids.
extern "C" const u32 data_0208ab54;
extern "C" const u32 data_0208acb0;
extern "C" const u32 data_0208acb4;
extern "C" const u32 data_0208acb8;
extern "C" const u32 data_0208add0;
extern "C" const u32 data_0208add4;
extern "C" const u32 data_0208add8;
extern "C" const u32 data_0208addc;

DinoUiSaveMenu::DinoUiSaveMenu(u32 *result, u8 arg) : MProcess(NULL), mResult(result), mUnk90(arg) {
    mState = data_ov004_02175130.Start_0C;
}

void DinoUiSaveMenu::StateConfirmed() {
    if (mUnk8C == 1) {
        mState = data_ov004_02175130.StateSave_24;
        return;
    }
    *mResult = 0;
    Exit();
}

void DinoUiSaveMenu::StateStart() {
    if (mUnk90) {
        MTextPicture *back = new MTextPicture(data_ov004_021751fc, TRUE, false);
        back->SetPriority(3);
        back->Show();
        mParallel.AddTail(back);
        MTask *fadeTop;
        MTask *fadeBottom;
        fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(30, 0);
        fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeInFromBlack(30, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        mSerial.AddTail(fades);
    }
    mUnk94 = new DinoUiSaveSlotConfirm;
    mParallel.AddTail(mUnk94);
    mState = data_ov004_02175130.StateAskSave_44;
}

void DinoUiSaveMenu::StateAskSave() {
    mUnk8C = TRUE;
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(gDinoResource->GetText(mUnk90 ? data_0208acb4 : data_0208acb8), &mUnk8C);
    dialog->SetLabels2(gDinoResource->GetText(data_0208addc), gDinoResource->GetText(data_0208add8));
    dialog->SetPosition(MPoint(0x80, 0x90));
    dialog->SetUnkBC(1);
    AddDialogHelp(dialog);
    mSerial.AddTail(dialog);
    mState = data_ov004_02175130.StateConfirmed_04;
}

// Saves (with a window up meanwhile), then fades out and leaves, or asks whether to overwrite.
void DinoUiSaveMenu::StateSave() {
    mUnk94->Unk02159590();
    DinoCommonWindowMessageWait1 *wait =
        new DinoCommonWindowMessageWait1(std::string(gDinoResource->GetText(data_0208ab54)), MPoint(0x80, 0x8C), 4, 0);
    mParallel.AddTail(wait);
    MTask *close = MTaskExitTransfer::Create(wait);
    GetSerial()->AddTail(new DinoBackupWriteValidater(wait, NULL),
                         MCreateBgCallTask(gDinoBackup, data_ov004_02175130.Save_1C, 1),
                         new DinoBackupWriteValidater(wait, NULL), close);
    if (mUnk90) {
        MTask *fadeTop;
        MTask *fadeBottom;
        MTask *exit;
        exit = MTaskExitTransfer::Create(this);
        fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(30, 0);
        fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(30, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fades);
        serial->AddTail(exit);
    } else {
        mState = data_ov004_02175130.StateAskOverwrite_2C;
    }
}

void DinoUiSaveMenu::StateAskOverwrite() {
    mUnk8C = TRUE;
    DinoCommonWindowYesNoDialog *dialog = new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208acb0), &mUnk8C);
    dialog->SetLabels2(gDinoResource->GetText(data_0208add4), gDinoResource->GetText(data_0208add0));
    dialog->SetPosition(MPoint(0x80, 0x90));
    dialog->SetUnkBC(1);
    AddDialogHelp(dialog);
    mSerial.AddTail(dialog);
    mState = data_ov004_02175130.StateFinish_14;
}
