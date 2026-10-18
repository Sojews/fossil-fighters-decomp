// DinoMain, DinoMainLoop.

#pragma ipa file

#include "game/dino_main.h"
#include "game/dino_main_loop.h"
#include "game/battle_select.h"
#include "game/dino_atm_sample.h"
#include "game/dino_field.h"
#include "game/dino_field_menu_sample.h"
#include "game/dino_help_sample.h"
#include "game/dino_net_battle_sample.h"
#include "game/dino_sound.h"
#include "game/dino_ui_atm.h"
#include "game/dino_ui_cleaning_menu_item_menu.h"
#include "game/dino_ui_keyboard.h"
#include "game/dino_ui_mask_equip_test.h"
#include "game/dino_ui_museum.h"
#include "game/dino_ui_save_option.h"
#include "game/dino_ui_title.h"
#include "game/dino_ui_top_menu_loop.h"
#include "game/dino_wireless_battle.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/overlay.h"
#include "m/soft_reset.h"

M_EXTERN_OVERLAY(ov000);
M_EXTERN_OVERLAY(ov003);
M_EXTERN_OVERLAY(ov004);
M_EXTERN_OVERLAY(ov005);
M_EXTERN_OVERLAY(ov006);
M_EXTERN_OVERLAY(ov007);

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoMainLoop::*StateGame_04)();
    void (DinoMainLoop::*StateTitleEnd_0C)();
    void (DinoMainLoop::*StateTitleEnd_14)();
    void (DinoMainLoop::*StateTitle_1C)();
    u8 unk24[0x8];
    void (DinoMainLoop::*StateLoad_2C)();
    u8 unk34[0x8];
    void (DinoMainLoop::*StateATMSampleEnd_3C)();
    void (DinoMainLoop::*StateDebugMenu_44)();
    void (DinoMainLoop::*StateDebugMenu_4C)();
    void (DinoMainLoop::*StateFieldMenuSampleEnd_54)();
    void (DinoMainLoop::*StateMapSelectEnd_5C)();
    void (DinoMainLoop::*StateDebugMenu_64)();
    void (DinoMainLoop::*StateNetBattleSampleEnd_6C)();
    void (DinoMainLoop::*StateDebugMenu_74)();
    void (DinoMainLoop::*StateDebugMenu_7C)();
} data_020bff70;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoMainLoop::*StateHelpSample_04)();
    u8 unk0C[0x18];
    void (DinoMainLoop::*StateAutoBattleEnd_24)();
    u8 unk2C[0x8];
    void (DinoMainLoop::*State02048a1c_34)();
    void (DinoMainLoop::*State0204897c_3C)();
    u8 unk44[0x10];
    void (DinoMainLoop::*StateDebugMenu_54)();
    void (DinoMainLoop::*State020483fc_5C)();
    void (DinoMainLoop::*State02048a1c_64)();
    void (DinoMainLoop::*StateDebugMenu_6C)();
    void (DinoMainLoop::*StateDebugMenu_74)();
} data_020bfff0;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoMainLoop::*StateDebugMenu_04)();
    void (DinoMainLoop::*StateMaskEquipTestEnd_0C)();
    void (DinoMainLoop::*StateDebugMenu_14)();
    void (DinoMainLoop::*StateMuseumEnd_1C)();
    void (DinoMainLoop::*StateDebugMenu_24)();
    void (DinoMainLoop::*StateCleaningTestEnd_2C)();
    void (DinoMainLoop::*StateDebugMenu_34)();
    void (DinoMainLoop::*StateSaveOptionEnd_3C)();
    void (DinoMainLoop::*StateDebugMenu_44)();
    u8 unk4C[0x8];
    void (DinoMainLoop::*StateDebugMenu_54)();
    void (DinoMainLoop::*StateAutoBattleEnd_5C)();
    void (DinoMainLoop::*StateDebugMenu_64)();
    void (DinoMainLoop::*StateTopMenuEnd_6C)();
    void (DinoMainLoop::*StateKeyboard_74)();
    void (DinoMainLoop::*StateKeyboardEnd_7C)();
} data_020c0070;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoMainLoop::*StateNetBattleSample_04)();
    void (DinoMainLoop::*StateMaskEquipTest_0C)();
    void (DinoMainLoop::*StateMuseum_14)();
    void (DinoMainLoop::*StateCleaningTest_1C)();
    void (DinoMainLoop::*StateSaveOption_24)();
    void (DinoMainLoop::*StateFieldMenuSample_2C)();
    void (DinoMainLoop::*StateATMSample_34)();
    void (DinoMainLoop::*StateTopMenu_3C)();
    void (DinoMainLoop::*StateAutoBattle_44)();
    void (DinoMainLoop::*State020488a4_4C)();
    void (DinoMainLoop::*State02048a1c_54)();
    u8 unk5C[0x8];
    void (DinoMainLoop::*StateStart_64)();
    void (DinoMainLoop::*StateDebugMenu_6C)();
    void (DinoMainLoop::*StateDebugMenu_74)();
} data_020c00f0;
extern "C" void (DinoMainLoop::*data_020c016c)(); // LoadResources

// The help sample menu's text ids (its items' ids too), and which one was chosen.
extern "C" const u32 data_0208a464;
extern "C" u32 data_020cc048;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoMainLoop::*StateDebugMenu_04)();
    void (DinoMainLoop::*State02048398_0C)();
    u8 unk14[0x8];
    void (DinoMainLoop::*State02047a1c_1C)();
    void (DinoMainLoop::*StateDebugMenu_24)();
    void (DinoMainLoop::*State02048030_2C)();
    void (DinoMainLoop::*StateTitle_34)();
    void (DinoMainLoop::*StateATMEnd_3C)();
    void (DinoMainLoop::*StateTitle_44)();
    void (DinoMainLoop::*StateWirelessBattleEnd_4C)();
    void (DinoMainLoop::*StateTitle_54)();
    void (DinoMainLoop::*StateTitle_5C)();
    void (DinoMainLoop::*StateVolume_64)();
    void (DinoMainLoop::*StateATM_6C)();
    void (DinoMainLoop::*StateWirelessBattle_74)();
} data_020c0170;

DinoMainLoop::DinoMainLoop(u32 slot) : MProcess(NULL), mSlot(slot) {
    mState = data_020c00f0.StateStart_64;
}

DinoMainLoop::~DinoMainLoop() {
}

void DinoMainLoop::Idle() {
    (this->*mState)();
}

void DinoMainLoop::StateStart() {
    mTitleResult = 0;
    mUnk7C = 0;
    mUnk80 = 0;
    mUnkA0 = -1;
    if (MGetResetParameter() == 1) {
        mTitleResult = 7;
    }
    if (MGetResetParameter() >= 2) {
        mUnkA0 = MGetResetParameter() - 2;
    }
    mState = data_020bff70.StateLoad_2C;
}

void DinoMainLoop::StateLoad() {
    MLoadOverlay(0, M_OVERLAY_ID(ov004));
    mParallel.AddTail(MCreateBgCallTask(this, data_020c016c, 2));
    mSerial.AddTail(new DinoUiTitle(&mTitleResult));
    mState = data_020bff70.StateTitleEnd_14;
}

void DinoMainLoop::StateTitle() {
    if (mTitleResult != 5) {
        DinoSoundLoadScene(1);
    }
    MLoadOverlay(0, M_OVERLAY_ID(ov004));
    mSerial.AddTail(new DinoUiTitle(&mTitleResult));
    mState = data_020bff70.StateTitleEnd_0C;
}

void DinoMainLoop::StateWirelessBattle() {
    mSerial.AddTail(new DinoWirelessBattle);
    mState = data_020c0170.StateWirelessBattleEnd_4C;
}

void DinoMainLoop::StateWirelessBattleEnd() {
    mTitleResult = 5;
    mState = data_020c0170.StateTitle_44;
}

void DinoMainLoop::StateATM() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    DinoSoundLoadScene(0x29);
    mSerial.AddTail(new DinoUiATM(1));
    mState = data_020c0170.StateATMEnd_3C;
}

void DinoMainLoop::StateATMEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov003));
    MUnloadOverlay(0, M_OVERLAY_ID(ov007));
    mTitleResult = 6;
    mState = data_020c0170.StateTitle_34;
}

void DinoMainLoop::StateTopMenu() {
    MLoadOverlay(0, M_OVERLAY_ID(ov004));
    mSerial.AddTail(new DinoUiTopMenuLoop(0));
    mState = data_020c0070.StateTopMenuEnd_6C;
}

void DinoMainLoop::StateTopMenuEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov004));
    mState = data_020c0070.StateDebugMenu_64;
}

void DinoMainLoop::StateATMSample() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    mSerial.AddTail(new DinoATMSample);
    mState = data_020bff70.StateATMSampleEnd_3C;
}

void DinoMainLoop::StateATMSampleEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov003));
    MUnloadOverlay(0, M_OVERLAY_ID(ov007));
    mState = data_020c0070.StateDebugMenu_54;
}

void DinoMainLoop::StateFieldMenuSample() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov006));
    mSerial.AddTail(new DinoFieldMenuSample);
    mState = data_020bff70.StateFieldMenuSampleEnd_54;
}

void DinoMainLoop::StateFieldMenuSampleEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov003));
    MUnloadOverlay(0, M_OVERLAY_ID(ov006));
    mState = data_020c0070.StateDebugMenu_44;
}

void DinoMainLoop::StateSaveOption() {
    MLoadOverlay(0, M_OVERLAY_ID(ov004));
    mSerial.AddTail(new DinoUiSaveOption(0));
    mState = data_020c0070.StateSaveOptionEnd_3C;
}

void DinoMainLoop::StateSaveOptionEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov004));
    mState = data_020c0070.StateDebugMenu_34;
}

void DinoMainLoop::StateCleaningTest() {
    MLoadOverlay(0, M_OVERLAY_ID(ov005));
    mSerial.AddTail(new DinoUiCleaningTest);
    mState = data_020c0070.StateCleaningTestEnd_2C;
}

void DinoMainLoop::StateCleaningTestEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov005));
    mState = data_020c0070.StateDebugMenu_24;
}

void DinoMainLoop::StateMuseum() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov006));
    mSerial.AddTail(new DinoUiMuseum);
    mState = data_020c0070.StateMuseumEnd_1C;
}

void DinoMainLoop::StateMuseumEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov003));
    MUnloadOverlay(0, M_OVERLAY_ID(ov006));
    mState = data_020c0070.StateDebugMenu_14;
}

void DinoMainLoop::StateMaskEquipTest() {
    MLoadOverlay(0, M_OVERLAY_ID(ov005));
    mSerial.AddTail(new DinoUiMaskEquipTest);
    mState = data_020c0070.StateMaskEquipTestEnd_0C;
}

void DinoMainLoop::StateMaskEquipTestEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov005));
    mState = data_020c0070.StateDebugMenu_04;
}

void DinoMainLoop::StateNetBattleSample() {
    mSerial.AddTail(new DinoNetBattleSample);
    mState = data_020bff70.StateNetBattleSampleEnd_6C;
}

void DinoMainLoop::StateNetBattleSampleEnd() {
    mState = data_020bfff0.StateDebugMenu_74;
}

void DinoMainLoop::StateMapSelect() {
    s32 result;
    MLoadOverlay(0, M_OVERLAY_ID(ov004));
    result = 8;
    mSerial.AddTail(new DinoMapSelectMenu(&result, 0x291));
    mState = data_020bff70.StateMapSelectEnd_5C;
}

void DinoMainLoop::StateMapSelectEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov004));
    mState = data_020bff70.StateDebugMenu_64;
}

void DinoMainLoop::StateKeyboard() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    mSerial.AddTail(new DinoUiKeyboard(1, 0));
    mSerial.AddTail(new DinoUiKeyboard(0));
    mState = data_020c0070.StateKeyboardEnd_7C;
}

void DinoMainLoop::StateKeyboardEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov003));
    MUnloadOverlay(0, M_OVERLAY_ID(ov007));
    mState = data_020bfff0.StateDebugMenu_54;
}

void DinoMainLoop::StateAutoBattle() {
    MLoadOverlay(0, M_OVERLAY_ID(ov000));
    mSerial.AddTail(new DebugAutoBattle);
    mState = data_020c0070.StateAutoBattleEnd_5C;
}

void DinoMainLoop::StateAutoBattleEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov000));
    mState = data_020bff70.StateDebugMenu_4C;
}

void DinoMainLoop::StateHelpSample() {
    data_020cc048 = mUnk80 - data_0208a464;
    mSerial.AddTail(new DinoHelpSample);
    mState = data_020bff70.StateDebugMenu_7C;
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoMainLoop>;

DinoMain::DinoMain() : MProcess(NULL) {
}

DinoMain::~DinoMain() {
}
