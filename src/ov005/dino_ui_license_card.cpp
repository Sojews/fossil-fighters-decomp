// DinoUiLicenseCard.

#include "game/dino_static_init.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/input.h"
#include "m/lcd.h"
#include "game/dino_ui_license_card.h"
#include "m/font.h"
#include "m/obj.h"
#include "m/smart_oam.h"
#include "m/oam_canvas.h"
#include "m/motion.h"
#include "m/picture.h"
#include "m/indicator.h"
#include "m/seed_generator.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "game/dino_sound.h"
#include <nitro/rtc.h>

DinoUiLicenseCard::~DinoUiLicenseCard() {
}

void DinoUiLicenseCard::Close() {
    mFont->Release();
    Exit();
}

void DinoUiLicenseCard::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x8];
    const char *mFramePath;     // 0x08
    const char *mCardPath;      // 0x0C: the card, one anime per rank
    u16 mPalette[8];            // 0x10: the name's
    void (DinoUiLicenseCard::*Close_20)();
    void (DinoUiLicenseCard::*Start_28)();
    void (DinoUiLicenseCard::*Wait_30)();
} data_ov005_021654e0;

DinoUiLicenseCard::DinoUiLicenseCard(u8 arg) : MProcess(gLcd->mMain), mUnk7C(arg) {
    mState = data_ov005_021654e0.Start_28;
}

extern "C" const char data_ov005_021655e0[]; // the picture of the player's horider level
extern "C" const char data_ov005_021655f8[]; // the date's font
extern "C" const char data_ov005_02165610[]; // the date's format

// Four tasks into one set (the arguments are evaluated last to first; the set is kept in a register).
static inline void DinoLicenseAddTail(MTaskSet *set, MTask *a, MTask *b, MTask *c, MTask *d) {
    MTaskSet *self = set;
    self->AddTail(a);
    self->AddTail(b);
    self->AddTail(c);
    self->AddTail(d);
}

// The card: the player's name and the date it was issued (stamped now, for a new card), then shaken into place
// with a stamp.
void DinoUiLicenseCard::StateStart() {
    if (mUnk7C) {
        gLcd->mMain->ResetHooks();
        gLcd->mSub->ResetHooks();
        gLcd->mMain->Setup(5, 1, 4, 1, 0, 0);
        gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
        gLcd->VSyncNow();
        gLcd->mMain->mObj->Show();
        gLcd->mSub->mObj->Show();
        gLcd->SetBottomDisplay(gLcd->mMain);
    }
    MSingleInputHub *hub = new MSingleInputHub(1);
    hub->EnableTouch();
    mInput = hub->GetInput(0);
    mParallel.AddTail(hub);
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, 0);
    if (mUnk7C) {
        RTC_Init();
        RTC_GetDateTime((RTCDate *)player.unk3c, (RTCTime *)&player.unk4c);
        gDinoBackup->SetPlayer(&player);
    }
    char text[0x100];
    OS_SPrintf(text, data_ov005_021655e0, player.horiderLevel);
    mPicture = new MTextPicture(std::string(text), TRUE, false);
    mPicture->Show();
    mParallel.AddTail(mPicture);
    mOam = new MSmartOam(0x28, 0, TRUE);
    MMotion *card = new MMotion(std::string(data_ov005_021654e0.mCardPath), mOam, 0, 1);
    card->Play(player.rank + 1, 0);
    card->SetPosition(0x80, 0x60);
    card->Show();
    mParallel.AddTail(card);
    MMotion *frame = new MMotion(std::string(data_ov005_021654e0.mFramePath), mOam, 0, 1);
    frame->SetPosition(0x80, 0x60);
    frame->Show();
    mParallel.AddTail(frame);
    mFont = gDinoResource->CreateFont24(0, 0, 0);
    mFont->AddRef();
    MSmartObjCanvas *name = new MSmartObjCanvas(
        mOam, mFont->MeasureText(std::wstring((const wchar_t *)player.name), 0, 0) + MPoint(2, 2),
        data_ov005_021654e0.mPalette, TRUE);
    name->Clear();
    mFont->SetColors(-1, 2, 2);
    name->DrawText(mFont, MPoint(0, 1), 0, std::wstring((const wchar_t *)player.name));
    name->DrawText(mFont, MPoint(1, 0), 0, std::wstring((const wchar_t *)player.name));
    mFont->SetColors(-1, 1, -1);
    name->DrawText(mFont, MPoint(1, 1), 0, std::wstring((const wchar_t *)player.name));
    name->SetPosition(MPoint(0xB0, 0x48) - name->GetSize() / 2);
    name->Show();
    mParallel.AddTail(name);
    MFontSet *font = new MFontSet(std::string(data_ov005_021655f8), 0, 0, 0);
    OS_SPrintf(text, data_ov005_02165610, player.unk3c[1], player.unk3c[2], player.unk3c[0] + 2000);
    MPoint size = font->MeasureText(std::string(text), 0, 0);
    MIndicator *issued = new MIndicator(new MSmartObjCanvas(mOam, size, font->GetPalette(), TRUE), font, 0);
    issued->SetText(std::string(text));
    issued->SetPosition(0x93 - size.x, 0x9E);
    issued->Show();
    mParallel.AddTail(issued);
    mParallel.AddTail(mOam);
    mSerial.AddTail(gCurrentDisplay->mBright->CreateFadeInFromBlack(60, 0));
    if (!mUnk7C) {
        frame->Play(3, 0);
    } else {
        DinoLicenseAddTail(&mSerial, new MTimer(60), frame->CreatePlayTransfer(1, 0),
                           DinoSoundCreatePlaySeTask(0x392, 0), frame->CreateWaitStopTask());
        for (int i = 30; i >= 0; i--) {
            int half = i / 2;
            MPoint offset(gDinoResource->GetRandom()->Next(i) - half, gDinoResource->GetRandom()->Next(i) - half);
            mSerial.AddTail(mPicture->CreateCoordTransfer(MPoint(-offset.x, -offset.y)), mOam->CreateOffsetTransfer(offset), new MTimer(1));
        }
    }
    mState = data_ov005_021654e0.Wait_30;
}

// Until A, B, Start or a touch, then fades out.
void DinoUiLicenseCard::StateWait() {
    if (mInput->GetPressed() & 0x1007) {
        mSerial.AddTail(gCurrentDisplay->mBright->CreateFadeOutToBlack(60, 0));
        mState = data_ov005_021654e0.Close_20;
    }
}

// The file's static initialization (see dino_static_init.h).
static struct DinoUiLicenseCardStaticInit {
    DinoUiLicenseCardStaticInit() {
        *(u16 *)((u8 *)&data_ov005_021654e0 + 0x12) = 0xFFFF;
        *(u16 *)((u8 *)&data_ov005_021654e0 + 0x14) = 0x996E;
    }
} sDinoUiLicenseCardStaticInit;
