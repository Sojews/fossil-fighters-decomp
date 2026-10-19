// DinoSenryuSample.
// Only the destructors are decompiled so far.

#include "nitro/os.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/font.h"
#include "m/simple_menu.h"
#include "m/text_codec.h"
#include "game/dino_resource.h"
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
    u8 unk00[0x18];
    void (DinoSenryuSample::*StatePages_18)();
    void (DinoSenryuSample::*StateEnd_20)();
    void (DinoSenryuSample::*Unk0203467c_28)();
    void (DinoSenryuSample::*Start_30)();
    void (DinoSenryuSample::*Unk02034564_38)();
} data_020bd15c;
// "PAGE%d", "CANCEL" (the TU's string literals are pooled data too), the title's text id.
extern "C" const char data_020bd1e8[];
extern "C" const char data_020bd1f0[];
extern "C" const u32 data_0208ac7c;

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

void DinoSenryuSample::StateStart() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    mResult = 0;
    mDefs = gDinoResource->LoadSenryuDefs();
    mText = MDecodeUtf8(gDinoResource->GetText(data_0208ac7c));
    mPageCount = (mDefs.Get()->count + 41) / 42;
    mState = data_020bd15c.StatePages_18;
}

void DinoSenryuSample::StatePages() {
    gLcd->mMain->mBright->Reset();
    gLcd->mSub->mBright->Reset();
    MSimpleMenu *menu = new MSimpleMenu(&mResult, gDinoResource->CreateFont12(0, 0, 0), NULL, mResult);
    for (u32 i = 0; i < mPageCount; i++) {
        char text[0x100];
        OS_SPrintf(text, data_020bd1e8, i + 1);
        menu->AddItem(i, text);
    }
    menu->AddItem(-2, data_020bd1f0);
    mSerial.AddTail(menu);
    mState = data_020bd15c.StateEnd_20;
}

// The text with every tag replaced by the value (unless the value has the tag itself).
// NONMATCHING: ours keeps an unreachable epilogue after the loop (as ov003's copy).
std::string DinoReplaceTag(std::string text, std::string tag, std::string value) {
    if (value.find(tag, 0) != std::string::npos) {
        return text;
    }
    while (true) {
        std::string::size_type pos = text.find(tag, 0);
        if (pos == std::string::npos) {
            return text;
        }
        text = text.replace(pos, tag.size(), value);
    }
}
