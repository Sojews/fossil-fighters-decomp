// DinoSenryuSample.

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
#include "game/dino_backup.h"
#include "nitro/mi.h"
#include <algorithm>

DinoSenryuSample::~DinoSenryuSample() {
}

void DinoSenryuSample::Idle() {
    (this->*mState)();
}

M_EXTERN_OVERLAY(ov003);
M_EXTERN_OVERLAY(ov007);

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[8];
    u32 mNameMax;       // 0x08: characters of a poet's name kept
    u32 mTextMax;       // 0x0C: bytes of a poem kept
    u8 unk10[8];
    void (DinoSenryuSample::*StatePages_18)();
    void (DinoSenryuSample::*StateEnd_20)();
    void (DinoSenryuSample::*StateFillRecords_28)();
    void (DinoSenryuSample::*Start_30)();
    void (DinoSenryuSample::*StatePages_38)();
    void (DinoSenryuSample::*StateMenu_40)();
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
    mState = data_020bd15c.StateFillRecords_28;
}

// Fills the records with the chosen page's sample poems (marked and shown), clears the rest, then shows the menu.
void DinoSenryuSample::StateFillRecords() {
    u32 i = 0;
    for (u32 j = mResult * 42; i < 42 && j < mDefs.Get()->count; i++, j++) {
        DinoBackupRecord record;
        MI_CpuFill8(&record, 0, sizeof(record));
        record.unk78 = 1;
        record.unk7a = 1;
        const u8 *data = (const u8 *)mDefs.Get();
        const u32 *entry = (const u32 *)(data + ((const u32 *)(data + ((const u32 *)data)[2]))[j]);
        std::wstring name = MDecodeUtf8(std::string((const char *)entry + entry[0]));
        for (u32 k = 0; k < std::min(name.size(), data_020bd15c.mNameMax); k++) {
            record.name[k] = name[k];
        }
        std::string text((const char *)entry + entry[1]);
        for (u32 k = 0; k < std::min(text.size(), data_020bd15c.mTextMax); k++) {
            record.data[k] = text[k];
        }
        gDinoBackup->SetRecord(i, &record);
    }
    for (; i < 42; i++) {
        DinoBackupRecord empty;
        MI_CpuFill8(&empty, 0, sizeof(empty));
        gDinoBackup->SetRecord(i, &empty);
    }
    mState = data_020bd15c.StateMenu_40;
}

void DinoSenryuSample::StateMenu() {
    mSerial.AddTail(new DinoUiSenryuMenu);
    mState = data_020bd15c.StatePages_38;
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
