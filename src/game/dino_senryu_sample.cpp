// DinoSenryuSample.
// Only the destructors are decompiled so far.

#include "game/dino_senryu_sample.h"
#include "game/dino_ui_senryu_menu.h"
#include "m/overlay.h"

DinoSenryuSample::~DinoSenryuSample() {
}

void DinoSenryuSample::Idle() {
    (this->*mState)();
}

M_EXTERN_OVERLAY(ov003);
M_EXTERN_OVERLAY(ov007);

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x28];
    void (DinoSenryuSample::*Unk0203467c_28)();
    void (DinoSenryuSample::*Start_30)();
    void (DinoSenryuSample::*Unk02034564_38)();
} data_020bd15c;

DinoSenryuSample::DinoSenryuSample() : MProcess(NULL) {
    mState = data_020bd15c.Start_30;
}

void DinoSenryuSample::StateEnd() {
    if (mResult == -2) {
        MUnloadOverlay(0, M_OVERLAY_ID(ov003));
        MUnloadOverlay(0, M_OVERLAY_ID(ov007));
        Exit();
        return;
    }
    mState = data_020bd15c.Unk0203467c_28;
}

void DinoSenryuSample::StateMenu() {
    mSerial.AddTail(new DinoUiSenryuMenu);
    mState = data_020bd15c.Unk02034564_38;
}
