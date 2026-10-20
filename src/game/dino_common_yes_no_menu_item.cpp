// DinoCommonYesNoMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#define DINO_COMMON_YES_NO_MENU_OWNER
#include "game/dino_common_yes_no_menu_item_menu.h"
#include "game/dino_common_yes_no_menu_item.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "m/lcd.h"
#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "game/common_window_message.h"
#include "m/font.h"
#include "m/smart_oam.h"
#include "m/oam_canvas.h"
#include "m/indicator.h"
#include "game/dino_common_window.h"

DinoCommonWindowYesNoDialog::~DinoCommonWindowYesNoDialog() {
}

extern const u32 data_0208acc8; // "No"
extern const u32 data_0208accc; // "Yes"

DinoCommonWindowYesNoDialog::DinoCommonWindowYesNoDialog(std::string text, BOOL *result)
    : MProcess(NULL), mUnk74(text), mUnk80(gDinoResource->GetText(data_0208acc8)),
      mUnk8C(gDinoResource->GetText(data_0208accc)) {
    mResult = result;
    mPos = MPoint(0x80, 0x60);
    mUnkBC = 0;
}

DinoCommonWindowYesNoDialog::DinoCommonWindowYesNoDialog(MDisplay *display, std::string text, BOOL *result)
    : MProcess(display), mUnk74(text), mUnk80(gDinoResource->GetText(data_0208acc8)),
      mUnk8C(gDinoResource->GetText(data_0208accc)), mUnk98(gDinoResource->GetText(data_0208acc8)),
      mUnkA4(gDinoResource->GetText(data_0208accc)) {
    mResult = result;
    mPos = MPoint(0x80, 0x60);
    mUnkBC = 0;
}

void DinoCommonWindowYesNoDialog::SetLabels(std::string yes, std::string no) {
    mUnk8C = yes;
    mUnk80 = no;
}

void DinoCommonWindowYesNoDialog::SetLabels2(std::string yes, std::string no) {
    mUnkA4 = yes;
    mUnk98 = no;
}

DinoCommonYesNoMenuItem::~DinoCommonYesNoMenuItem() {
}

DinoCommonYesNoMenu::~DinoCommonYesNoMenu() {
}

// The TU's pooled constants (see menu.cpp).
extern "C" struct {
    u32 unk00;
    const char *buttonPath; // 0x04
} data_020bc3f0;

DinoCommonYesNoMenu::DinoCommonYesNoMenu(const std::string &text, BOOL *result, u32 unkD0, u32 unkD4, u32 unkD8)
    : MMenu(0), mUnkC0(text) {
    mResult = result;
    mUnkD0 = unkD0;
    mUnkD4 = unkD4;
    mUnkD8 = unkD8;
    DinoHelpBase::Disable();
    SetDeferStart(TRUE);
}

// NONMATCHING: the game stores the dead copy of pos (MoveTo's argument) before the rect's temporaries.
void DinoCommonYesNoMenu::OnStart() {
    MSmartOam *oam = new MSmartOam(8, 0, TRUE);
    oam->mHidden = TRUE;
    mSerial.AddTail(oam->CreateShowTransfer());
    MPoint size(0x12, 8);
    MPoint pos((0x20 - size.x) / 2, (0x18 - size.y) / 2);
    MRect rect(MPoint(0, 0), size);
    rect.MoveTo(pos);
    mWindow = new DinoCommonWindow(0, TRUE, FALSE);
    mWindow->SetFrame(rect);
    mWindow->SetPriority(0);
    mWindow->SetPosition(MPoint(0, 0));
    mParallel.AddTail(mWindow);
    mSerial.AddTail(mWindow->CreateShowTransfer());
    MFontSet *font = gDinoResource->CreateFont12(0, 1, 10);
    if (mUnkC0.size() != 0) {
        MPoint textSize = font->MeasureText(std::string(mUnkC0), 0, 0);
        MIndicator *text = new MIndicator(new MSmartObjCanvas(oam, textSize, gDinoResource->GetUnkTable(), TRUE), font, 0);
        text->SetPosition(0x80 - textSize.x / 2, pos.y * 8 + 8);
        text->SetPriority(0);
        text->SetText(std::string(mUnkC0));
        text->Show();
        mParallel.AddTail(text);
    }
    for (int i = 0; i < 2; i++) {
        mItems[i] = new DinoCommonYesNoMenuItem(oam, i == 0, i == 0 ? mUnkD0 : mUnkD4);
        AddItem(mItems[i]);
    }
    if (mResult != NULL) {
        FocusItem(mItems[*mResult]);
    } else {
        FocusItem(mItems[mUnkD8]);
    }
    mParallel.AddTail(oam);
    if (mUnkD0 != 0 && mUnkD4 != 0) {
        mSerial.AddTail(DinoHelpBase::CreateEnableTask(&static_cast<DinoHelpBase &>(*this)));
    }
}

DinoCommonYesNoMenuItem::DinoCommonYesNoMenuItem(MSmartOam *oam, u8 isYes, u32 textId) {
    mIsYes = isYes;
    mTextId = textId;
    mMotion = new MMotion(data_020bc3f0.buttonPath, oam, 0, 1);
    mMotion->Show();
    mMotion->SetPriority(0);
    mMotion->SetPosition(MPoint(mIsYes ? 0x60 : 0xA0, 0x68));
    mParallel.AddTail(mMotion);
    OnFocusChanged(FALSE);
    if (!isYes) {
        SetKeys(2);
    }
}

MRect DinoCommonYesNoMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoCommonYesNoMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

MRect DinoCommonYesNoMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoCommonYesNoMenuItem::GetDisplay() {
    return gLcd->GetBottomDisplay();
}

void DinoCommonYesNoMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(mIsYes ? 1 : 4, 0);
    } else {
        mMotion->Play(mIsYes ? 2 : 5, 0);
    }
}

#pragma auto_inline off

MMotion *DinoCommonYesNoMenuItem::GetMotion() {
    return mMotion;
}

u8 DinoCommonYesNoMenuItem::IsYes() {
    return mIsYes;
}

#pragma auto_inline on

void DinoCommonYesNoMenu::OnFocus(MMenuItem *item) {
    DinoHelpItem *help = dynamic_cast<DinoHelpItem *>(item);
    if (help) {
        SetSelectedItem(help);
    }
}

void DinoCommonYesNoMenu::OnDecide(MMenuItem *item) {
    DinoCommonYesNoMenuItem *decided = static_cast<DinoCommonYesNoMenuItem *>(item);
    mSerial.AddTail(decided->GetMotion()->CreateWaitStopTask());
    Disable();
    if (mResult) {
        *mResult = item == mItems[1];
        mSerial.AddTail(MTaskExitTransfer::Create(this));
    }
}


void DinoCommonYesNoMenuItem::OnDecide() {
    mMotion->Play(mIsYes ? 3 : 6, 1);
}

#pragma auto_inline off

void DinoCommonWindowYesNoDialog::SetUnkBC(u8 value) {
    mUnkBC = value;
}

#pragma auto_inline off

void DinoCommonWindowYesNoDialog::SetPosition(MPoint pos) {
    mPos = pos;
}

void DinoCommonWindowYesNoDialog::Idle() {
    DinoCommonWindowMessage *message = new DinoCommonWindowMessage(mPos, std::string(mUnk74), 6, 0);
    mParallel.AddTail(message);
    if (mUnkBC) {
        mUnk80 = "*" + mUnk80;
    }
    DinoCommonWindowMenu *menu = new DinoCommonWindowMenu(mResult, mPos, NULL, 5);
    menu->AddChoice(1, std::string(mUnk8C), std::string(mUnkA4));
    menu->AddChoice(0, std::string(mUnk80), std::string(mUnk98));
    GetSerial()->AddTail(new MTimer(1), MTaskSetWaitEmptyTask::Create(&message->mSerial), menu,
                         MTaskExitTransfer::Create(this));
}
