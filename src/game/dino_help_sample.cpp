// DinoHelpSample: the help practice screen.

#include "game/dino_help_sample.h"
#include <string>
#include "m/blend.h"
#include "m/console.h"
#include "m/display.h"
#include "m/input.h"
#include "m/lcd.h"
#include "m/motion.h"
#include "m/oam_canvas.h"
#include "m/obj.h"
#include "m/picture.h"
#include "m/smart_oam.h"
#include "game/dino_common_label.h"
#include "game/dino_resource.h"
#include "game/dino_static_init.h"

#define PAD_BUTTON_START 0x0008

static const char *sFiles[] = {
    NULL,
    "/image/sample_bg1",
    "/motion/common/sample_button_3.bin",
    "/motion/common/sample_button_2.bin",
    "/motion/common/sample_button_1.bin",
    "/image/sample_console",
    "/motion/common/sample_button_yes.bin",
    "/motion/common/sample_button_4.bin",
    "/image/sample_bg2",
    "/motion/common/sample_button_no.bin",
};


// Text IDs.
extern const u32 kHelpSampleNoText;
extern const u32 kHelpSampleYesText;
extern const u32 kHelpSampleQuitText;
extern const u32 kHelpSampleConsoleText;
extern const u32 kHelpSampleButton4Help;
extern const u32 kHelpSampleButton3Help;
extern const u32 kHelpSampleButton2Help;
extern const u32 kHelpSampleButton1Help;
extern const u32 kHelpSampleButton4Desc;
extern const u32 kHelpSampleButton3Desc;
extern const u32 kHelpSampleButton2Desc;
extern const u32 kHelpSampleButton1Desc;

DinoHelpSampleYesNoItem::DinoHelpSampleYesNoItem(MSmartOam *oam, MPoint pos, const char *path, u32 textId) {
    mTextId = textId;
    DinoHelpItem::Enable();
    mMotion = new MMotion(path, oam, 0, 1);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mMotion->Play(2, 0);
    mParallel.AddTail(mMotion);
}

MRect DinoHelpSampleYesNoItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoHelpSampleYesNoItem::OnDecide() {
}

void DinoHelpSampleYesNoItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        DinoHelpItem::Select();
        mMotion->Play(1, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

MRect DinoHelpSampleYesNoItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoHelpSampleYesNoItem::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoHelpSampleYesNoItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

DinoHelpSampleYesNoMenu::DinoHelpSampleYesNoMenu(MTask *sample) : MMenu(gLcd->GetBottomDisplay(), NULL, 0) {
    mSample = sample;
}

void DinoHelpSampleYesNoMenu::OnStart() {
    DinoHelpBase::Enable();
    mBg = new MTextPicture(sFiles[5], TRUE, false);
    mBg->Show();
    mParallel.AddTail(mBg);
    MBlend *blend = GetDisplay()->mBlend;
    blend->SetMode(1);
    blend->SetAlpha(12, 4);
    blend->SetTarget1(mBg->GetPlaneMask());
    blend->SetTarget2(0x3f);
    mOam = new MSmartOam(30, 0, TRUE);
    MFont *font = gDinoResource->CreateFont12(0, 10, 1);
    DinoCommonLabel *label = new DinoCommonLabel(mOam, font, gDinoResource->GetUnkTable(), kHelpSampleQuitText, MPoint(0, 0));
    MPoint pos((256 - label->GetWidth()) / 2, 64);
    label->SetPosition(pos);
    label->Show();
    mParallel.AddTail(label);
    mYes = new DinoHelpSampleYesNoItem(mOam, MPoint(80, 128), sFiles[6], kHelpSampleYesText);
    mNo = new DinoHelpSampleYesNoItem(mOam, MPoint(176, 128), sFiles[9], kHelpSampleNoText);
    AddItem(mYes);
    AddItem(mNo);
    mParallel.AddTail(mOam);
    FocusNearestItem(MPoint(0, 0));
}

void DinoHelpSampleYesNoMenu::OnDecide(MMenuItem *item) {
    if (item == mYes) {
        Exit();
        mSample->Exit();
    }
    if (item == mNo) {
        Exit();
    }
}

// The file's pooled statics (the game addresses sConsoleRect as a slot of this block; see menu.cpp).
extern "C" struct {
    u8 unk00[0x2C];
    MRect consoleRect;  // 0x2C: sConsoleRect
} data_020bc804;

DinoHelpSampleConsole::DinoHelpSampleConsole(MDisplay *display) : MProcess(display) {
    DinoHelpItem::Enable();
    MDisplayScope scope(display);
    mBg = new MTextPicture(sFiles[5], TRUE, false);
    mBg->Show();
    mParallel.AddTail(mBg);
    MBlend *blend = MProcess::GetDisplay()->mBlend;
    blend->SetMode(1);
    blend->SetAlpha(8, 8);
    blend->SetTarget1(mBg->GetPlaneMask());
    blend->SetTarget2(0x3f);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay,
                                        MPoint(data_020bc804.consoleRect.right - data_020bc804.consoleRect.left,
                                               data_020bc804.consoleRect.bottom - data_020bc804.consoleRect.top),
                                        gDinoResource->GetUnkTable(), TRUE);
    MFont *font = gDinoResource->CreateFont12(0, 10, 1);
    mConsole = new MConsole(canvas, font, MPoint(0, 3), MFx32(3));
    mConsole->SetPosition(MPoint(data_020bc804.consoleRect.left, data_020bc804.consoleRect.top));
    mConsole->Show();
    mParallel.AddTail(mConsole);
}

void DinoHelpSampleConsole::SetText(const char *text) {
    mConsole->Clear();
    mConsole->ClearText();
    mConsole->Print(std::string(text));
}

MRect DinoHelpSampleConsole::GetRect() {
    return data_020bc804.consoleRect;
}

MDisplay *DinoHelpSampleConsole::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoHelpSampleConsole::GetText() {
    return gDinoResource->GetText(kHelpSampleConsoleText);
}

void DinoHelpSampleConsole::Idle() {
}

DinoHelpSampleMenuItem::DinoHelpSampleMenuItem(MSmartOam *oam, MPoint pos, const char *path,
                                               DinoHelpSampleConsole *console, u32 descTextId, u32 helpTextId) {
    mConsole = console;
    mDescTextId = descTextId;
    mHelpTextId = helpTextId;
    mMotion = new MMotion(path, oam, 0, 1);
    mMotion->SetPriority(3);
    mMotion->SetPosition(pos);
    mMotion->Play(2, 0);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

MRect DinoHelpSampleMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoHelpSampleMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(1, 0);
        DinoHelpItem::Select();
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoHelpSampleMenuItem::OnDecide() {
    mMotion->Play(3, 1);
    mConsole->SetText(gDinoResource->GetText(mDescTextId));
}

MRect DinoHelpSampleMenuItem::GetRect() {
    return GetFrame();
}

const char *DinoHelpSampleMenuItem::GetText() {
    return gDinoResource->GetText(mHelpTextId);
}

MDisplay *DinoHelpSampleMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

DinoHelpSampleMenu::DinoHelpSampleMenu() : MMenu(gLcd->GetBottomDisplay(), NULL, 0) {
}

void DinoHelpSampleMenu::OnStart() {
    MTextPicture *top = new MTextPicture(gLcd->GetTopDisplay(), sFiles[1], TRUE, false);
    top->SetPriority(3);
    top->Show();
    mParallel.AddTail(top);
    MTextPicture *bottom = new MTextPicture(gLcd->GetBottomDisplay(), sFiles[8], TRUE, false);
    bottom->SetPriority(3);
    bottom->Show();
    mParallel.AddTail(bottom);
    DinoHelpSampleConsole *console = new DinoHelpSampleConsole(gLcd->GetTopDisplay());
    mParallel.AddTail(console);
    mOam = new MSmartOam(30, 0, TRUE);
    mParallel.AddTail(mOam);
    AddItem(new DinoHelpSampleMenuItem(mOam, MPoint(64, 48), sFiles[4], console, kHelpSampleButton1Desc,
                                       kHelpSampleButton1Help));
    AddItem(new DinoHelpSampleMenuItem(mOam, MPoint(192, 48), sFiles[3], console, kHelpSampleButton2Desc,
                                       kHelpSampleButton2Help));
    AddItem(new DinoHelpSampleMenuItem(mOam, MPoint(64, 144), sFiles[2], console, kHelpSampleButton3Desc,
                                       kHelpSampleButton3Help));
    AddItem(new DinoHelpSampleMenuItem(mOam, MPoint(192, 144), sFiles[7], console, kHelpSampleButton4Desc,
                                       kHelpSampleButton4Help));
    FocusNearestItem(MPoint(0, 0));
    DinoHelpBase::Enable();
}

void DinoHelpSampleMenu::OnIdle() {
    MInput *input = GetInput();
    u32 buttons = input->mButtons;
    if (buttons & (input->mPrevButtons ^ buttons) & PAD_BUTTON_START) {
        mSerial.AddTail(new DinoHelpSampleYesNoMenu(this));
    }
}

DinoHelpSample::DinoHelpSample() : MProcess(NULL) {
}

void DinoHelpSample::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MMenu *menu = new DinoHelpSampleMenu();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

// The file's static initialization (see dino_static_init.h).
static struct DinoHelpSampleStaticInit {
    DinoHelpSampleStaticInit() {
        DINO_POOL_WORD(data_020bc804, 0x2c) = 40;
        DINO_POOL_WORD(data_020bc804, 0x30) = 52;
        DINO_POOL_WORD(data_020bc804, 0x34) = 216;
        DINO_POOL_WORD(data_020bc804, 0x38) = 146;
    }
} sDinoHelpSampleStaticInit;
