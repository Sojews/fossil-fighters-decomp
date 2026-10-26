// MultiPlayErrorDialog.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "nitro/mi.h"
#include "game/dino_battle.h"
#include "game/multi_play_error_dialog_menu.h"
#include "m/multi_play.h"
#include "m/font.h"
#include "m/smart_oam.h"
#include "m/oam_canvas.h"
#include "m/indicator.h"
#include "game/dino_resource.h"
#include "m/ulti_play_error_dialog.h"
#include "game/dino_static_init.h"

MultiPlayErrorDialog::~MultiPlayErrorDialog() {
}

void MultiPlayErrorDialog::Idle() {
    (this->*mState)();
}

DinoMpGame::~DinoMpGame() {
}

void DinoMpGame::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoMpGame::*Start_04)();
    u8 unk0C[0x2C - 0xC];
    void (DinoMpGame::*StateWaitExit_2C)();
    u8 unk34[0x3C - 0x34];
    void (DinoMpGame::*Unk_3C)();
    void (MultiPlayErrorDialog::*ErrorStart_44)();
} data_020c02fc;

inline MultiPlayErrorDialog::MultiPlayErrorDialog() : MProcess(NULL) {
    mState = data_020c02fc.ErrorStart_44;
}

DinoMpGame::DinoMpGame(MMultiPlay *multiPlay) : MProcess(NULL), mMultiPlay(multiPlay) {
    mState = data_020c02fc.Start_04;
}

// Waits three seconds, then ends.
void DinoMpGame::StateWaitExit() {
    GetSerial()->AddTail(new MTimer(180), MTaskExitTransfer::Create(this));
}

void DinoMpGame::StateFinish() {
    mSerial.AddTail(mMultiPlay->CreateFinishTask());
    if (mFailed) {
        mSerial.AddTail(new MultiPlayErrorDialog);
    }
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

// A wireless battle: player 0 is the parent.
// NONMATCHING: the game keeps the 0 in a register (from the loop counter's start).
void DinoMpGame::StateStart() {
    mParallel.AddTail(mMultiPlay);
    DinoBattleParams params;
    MI_CpuFill8(&params, 0, sizeof(params));
    params.unk00 = 2;
    params.unk04 = (u32)mMultiPlay;
    for (int i = 0; i < 2; i++) {
        (&params.unk08)[i] = i == mMultiPlay->GetPlayerIndex() ? 0 : 1;
    }
    if (mMultiPlay->GetPlayerIndex() != 1) {
        params.unk10 = 0;
        params.unk14 = 1;
    } else {
        params.unk10 = 1;
        params.unk14 = 0;
    }
    params.unk18 = 1;
    params.unk1C = mMultiPlay->GetPlayerIndex() == 0 ? TRUE : FALSE;
    mSerial.AddTail(new DinoBattleDebugSelect(params, &mFailed));
    mState = data_020c02fc.Unk_3C;
}

extern "C" const u32 data_0208a438; // the error's text

void MultiPlayErrorDialog::ErrorStart() {
    MSmartOam *oam = new MSmartOam(8, 0, TRUE);
    MFont *font = gDinoResource->CreateFont12(0, 10, 1);
    u32 text = data_0208a438;
    MPoint size = font->MeasureText(std::string(gDinoResource->GetText(text)), 0, 0);
    MIndicator *indicator =
        new MIndicator(new MSmartObjCanvas(oam->mDisplay, oam, size, gDinoResource->GetUnkTable(), TRUE), font, 0);
    indicator->SetPosition(MPoint(128 - size.x / 2, 96 - size.y / 2));
    indicator->Show();
    indicator->SetText(std::string(gDinoResource->GetText(text)));
    MTaskSet *parallel = &mParallel;
    parallel->AddTail(indicator);
    parallel->AddTail(oam);
    mState = reinterpret_cast<void (MultiPlayErrorDialog::*)()>(data_020c02fc.StateWaitExit_2C);
}

// The file's static initialization (see dino_static_init.h).
static struct MultiPlayErrorDialogStaticInit {
    MultiPlayErrorDialogStaticInit() {
        DINO_POOL_WORD(data_020c02fc, 0x34) = 128;
        DINO_POOL_WORD(data_020c02fc, 0x38) = 176;
        DINO_POOL_WORD(data_020c02fc, 0x1c) = 8;
        DINO_POOL_WORD(data_020c02fc, 0x20) = 150;
        DINO_POOL_WORD(data_020c02fc, 0xc) = 64;
        DINO_POOL_WORD(data_020c02fc, 0x10) = 112;
        DINO_POOL_WORD(data_020c02fc, 0x14) = 240;
        DINO_POOL_WORD(data_020c02fc, 0x18) = 176;
    }
} sMultiPlayErrorDialogStaticInit;
