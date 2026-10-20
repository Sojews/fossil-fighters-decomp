// DinoUiResult.

#include "game/dino_ui_result.h"
#include "m/lcd.h"
#include "game/dino_static_init.h"
#include "game/dino_backup.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_resource.h"
#include "game/dino_sound.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/indicator.h"
#include "m/motion.h"
#include "m/oam_canvas.h"
#include "m/obj.h"
#include "m/picture.h"
#include "m/smart_oam.h"

DinoUiResult::DinoUiResult(u32 arg0, u8 arg1, u32 count, u32 count2, const std::string &text)
    : MProcess(gLcd->mSub), mUnk74(arg0), mUnk78(arg1), mCount(count), mCount2(count2), mText(text) {
    if (mCount > 99) {
        mCount = 99;
    }
    if (mCount2 > 99) {
        mCount2 = 99;
    }
}

DinoUiResult::~DinoUiResult() {
}

// The file's pooled statics: the digits' motions ([2] the first count's, [3] the second's) and the labels' places.
extern "C" const char *data_ov004_02177148[];
extern "C" const MPoint data_ov004_02177158, data_ov004_02177168;
extern "C" const char data_ov004_02177244[], data_ov004_02177254[];  // the pictures
extern "C" const u32 data_0208aae4;     // "again?"

// Shows the result (the player's name, the text and the two counts) and asks whether to go again, then fades out.
#pragma opt_common_subs off
void DinoUiResult::Idle() {
    DinoSoundLoadScene(0x28);
    DinoSoundPlayBgm(0x78, 0);
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mMain->Setup(5, 4, 0, 0);
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetBottomDisplay()->mBright->Reset();
    MTextPicture *picture = new MTextPicture(gLcd->GetTopDisplay(), std::string(data_ov004_02177244), TRUE, false);
    picture->Show();
    mParallel.AddTail(picture);
    mParallel.AddTail(picture = new MTextPicture(std::string(data_ov004_02177254), TRUE, false));
    picture->Show();
    picture->SetPriority(3);
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, TRUE);
    MFont *font = gDinoResource->CreateFont12(0, 10, 1);
    MIndicator *label = new MIndicator(new MObjCanvas(gCurrentDisplay, MPoint(0x40, 0x10), gDinoResource->GetUnkTable(), TRUE), font, 0);
    mParallel.AddTail(label);
    label->SetPosition(data_ov004_02177168);
    label->SetPriority(3);
    label->Show();
    label->SetText(std::wstring((const wchar_t *)player.name));
    label = new MIndicator(new MObjCanvas(gCurrentDisplay, MPoint(0x40, 0x10), gDinoResource->GetUnkTable(), TRUE), font, 0);
    mParallel.AddTail(label);
    label->SetPosition(data_ov004_02177158);
    label->SetPriority(3);
    label->Show();
    label->SetText(std::string(mText));
    MSmartOam *oam = new MSmartOam(0x28, 0, TRUE);
    MMotion *digit;
    if (mCount >= 10) {
        digit = new MMotion(data_ov004_02177148[2], oam, 2, 1);
        digit->Play(mCount / 10 + 9, 0);
        digit->SetPosition(0x80, 0x60);
        digit->SetPriority(2);
        digit->Show();
        mParallel.AddTail(digit);
    }
    if (mCount % 10 != 0) {
        digit = new MMotion(data_ov004_02177148[2], oam, 2, 1);
        digit->Play(mCount % 10, 0);
        digit->SetPosition(0x80, 0x60);
        digit->SetPriority(2);
        digit->Show();
        mParallel.AddTail(digit);
    }
    if (mCount2 >= 10) {
        digit = new MMotion(data_ov004_02177148[3], oam, 2, 1);
        digit->Play(mCount2 / 10 + 9, 0);
        digit->SetPosition(0x80, 0x60);
        digit->SetPriority(2);
        digit->Show();
        mParallel.AddTail(digit);
    }
    if (mCount2 % 10 != 0) {
        digit = new MMotion(data_ov004_02177148[3], oam, 2, 1);
        digit->Play(mCount2 % 10, 0);
        digit->SetPosition(0x80, 0x60);
        digit->SetPriority(2);
        digit->Show();
        mParallel.AddTail(digit);
    }
    mParallel.AddTail(oam);
    MTask *main;
    MParallelTaskSet *fades;
    if (mUnk78) {
        (fades = new MParallelTaskSet(TRUE))->AddTail(gLcd->mMain->mBright->CreateFadeInFromWhite(30, 0), gLcd->mSub->mBright->CreateFadeInFromWhite(30, 0));
        mSerial.AddTail(fades);
    } else {
        (fades = new MParallelTaskSet(TRUE))->AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(30, 0), gLcd->mSub->mBright->CreateFadeInFromBlack(30, 0));
        mSerial.AddTail(fades);
    }
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(std::string(gDinoResource->GetText(data_0208aae4)), (BOOL *)mUnk74);
    dialog->SetPosition(MPoint(0x80, 0x90));
    mSerial.AddTail(dialog);
    MTask *sub = gLcd->mSub->mBright->CreateFadeOutToWhite(30, 0);
    main = gLcd->mMain->mBright->CreateFadeOutToWhite(30, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(main);
    fades->AddTail(sub);
    mSerial.AddTail(fades);
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}
#pragma opt_common_subs reset

// The file's static initialization (see dino_static_init.h).
static struct DinoUiResultStaticInit {
    DinoUiResultStaticInit() {
        DINO_POOL_WORD(data_ov004_02177148, 0x20) = 60;
        DINO_POOL_WORD(data_ov004_02177148, 0x24) = 9;
        DINO_POOL_WORD(data_ov004_02177148, 0x10) = 148;
        DINO_POOL_WORD(data_ov004_02177148, 0x14) = 9;
    }
} sDinoUiResultStaticInit;
