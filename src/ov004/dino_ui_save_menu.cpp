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
    u8 unk1C[0x24 - 0x1C];
    void (DinoUiSaveMenu::*Unk02169db8_24)();
    u8 unk2C[0x44 - 0x2C];
    void (DinoUiSaveMenu::*StateAskSave_44)();
} data_ov004_02175130;
// "/image/m_back00" (the TU's string literals are pooled data too).
extern "C" const char data_ov004_021751fc[];
// Text ids.
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
        mState = data_ov004_02175130.Unk02169db8_24;
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
