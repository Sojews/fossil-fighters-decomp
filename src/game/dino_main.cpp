// DinoMain, DinoMainLoop.

#pragma ipa file

#include "game/dino_main.h"
#include "game/dino_main_loop.h"
#include "game/dino_multi_play_scanner.h"
#include "game/battle_select.h"
#include "game/dino_battle.h"
#include "game/dino_atm_sample.h"
#include "game/dino_backup.h"
#include "game/dino_event_exec.h"
#include "game/dino_resource.h"
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
#include "game/multi_play_error_dialog_menu.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/display.h"
#include "m/font.h"
#include "m/lcd.h"
#include "m/multi_play.h"
#include "m/obj.h"
#include "m/overlay.h"
#include "m/simple_menu.h"
#include "m/soft_reset.h"
#include "game/help.h"
#include "m/bright.h"
#include "m/sleep.h"
#include "m/file.h"
#include "m/dma.h"
#include "game/dino_message.h"

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
    void (DinoMainLoop::*StateDebugMenu_0C)();
    u8 unk14[0x10];
    void (DinoMainLoop::*StateAutoBattleEnd_24)();
    void (DinoMainLoop::*StateMapSelect_2C)();
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

void DinoMainLoop::LoadResources() {
    DinoResource::Create();
    {
        MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
        const DinoExcavateDefs *data = defs.Get();
        DinoSoundInit(*DinoOffsetPtr<u8>(data, data->soundOffset));
    }
    DinoBackup::Create(mSlot, TRUE);
}
extern "C" MPoint data_020c003c;                   // (0, 0)

// The help sample menu's text ids (its items' ids too), and which one was chosen.
extern "C" const u32 data_0208a464;
extern "C" const u32 data_0208a460;
extern "C" const u32 data_0208a45c;
extern "C" const u32 data_0208a458;
extern "C" const u32 data_0208a454;
extern "C" const u32 data_0208a450;
extern "C" const u32 data_0208a44c;
extern "C" const u32 data_0208a448;
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
    mMenuChoice = 0;
    mHelpChoice = 0;
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

void DinoMainLoop::StateTitleEnd() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov004));
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mMain->Setup(5, 4, 0, 0);
    gLcd->mSub->Setup(5, 4, 0, 0);
    gLcd->VSyncNow();
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->mSub->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    switch (mTitleResult) {
    case 0:
        mState = data_020bff70.StateTitle_1C;
        return;
    case 8:
        mState = data_020bff70.StateGame_04;
        return;
    case 3:
        mState = data_020c0170.State02047a1c_1C;
        return;
    case 5:
        mState = data_020c0170.StateWirelessBattle_74;
        return;
    case 6:
        mState = data_020c0170.StateATM_6C;
        break;
    }
}

// The title screen's demo: the next battle from the list for how far the save has got.
void DinoMainLoop::StateGame() {
    DinoSaveSummary summary;
    gDinoBackup->GetSummary(gDinoBackup->GetSlot(), &summary);
    const DinoDemoBattles *demos;
    {
        MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
        const DinoExcavateDefs *data = defs.Get();
        demos = DinoOffsetPtr<DinoDemoBattles>(data, data->demoOffset);
    }
    u32 battle;
    if (summary.valid && summary.flag1a) {
        mUnkA0 = (mUnkA0 + 1) % demos->lateCount;
        battle = ((const u32 *)((const u8 *)demos + demos->lateOffset))[mUnkA0];
    } else if (summary.valid && summary.flag1b) {
        mUnkA0 = (mUnkA0 + 1) % demos->midCount;
        battle = ((const u32 *)((const u8 *)demos + demos->midOffset))[mUnkA0];
    } else {
        mUnkA0 = (mUnkA0 + 1) % demos->count;
        battle = ((const u32 *)((const u8 *)demos + demos->offset))[mUnkA0];
    }
    mSerial.AddTail(new DinoEventExecBattle(NULL, 0, battle, 0, mUnkA0));
}

void DinoMainLoop::StateVolume() {
    gDinoBackup->SetTargetVolume(8);
    gDinoBackup->SetVolume(8);
    switch (mUnk9C) {
    case 1:
        mTitleResult = 4;
        mState = data_020c0170.StateTitle_5C;
        break;
    case 0:
        mTitleResult = 0;
        mState = data_020c0170.StateTitle_54;
        break;
    }
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

// The second debug menu's choices.
void DinoMainLoop::State02048398() {
    switch (mMenuChoice) {
    case 17:
        mUnk84 = 0;
        mState = data_020bfff0.State02048a1c_64;
        break;
    case 18:
        mUnk84 = 1;
        mState = data_020c00f0.State02048a1c_54;
        break;
    case 19:
        mState = data_020c00f0.State020488a4_4C;
        break;
    case 20:
        mState = data_020c00f0.StateAutoBattle_44;
        break;
    }
}

// The debug menu's samples.
void DinoMainLoop::State020483fc() {
    switch (mMenuChoice) {
    case 7:
        mState = data_020c00f0.StateTopMenu_3C;
        break;
    case 8:
        mState = data_020c00f0.StateATMSample_34;
        break;
    case 9:
        mState = data_020c00f0.StateFieldMenuSample_2C;
        break;
    case 10:
        mState = data_020c00f0.StateSaveOption_24;
        break;
    case 11:
        mState = data_020c00f0.StateCleaningTest_1C;
        break;
    case 12:
        mState = data_020c00f0.StateMuseum_14;
        break;
    case 13:
        mState = data_020c00f0.StateMaskEquipTest_0C;
        break;
    case 14:
        mState = data_020c00f0.StateNetBattleSample_04;
        break;
    case 15:
        mState = data_020bfff0.StateMapSelect_2C;
        break;
    case 16:
        mState = data_020c0070.StateKeyboard_74;
        break;
    }
}

void DinoMainLoop::StateTopMenu() {
    MLoadOverlay(0, M_OVERLAY_ID(ov004));
    mSerial.AddTail(new DinoUiTopMenuLoop(NULL));
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
    mSerial.AddTail(new DinoUiIslandMap(&result, 0x291));
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

void DinoMainLoop::State020488a4() {
    mMultiPlay = new MMultiPlay(0x3FBEEC, 2, 1, 0x10000, -1, -1);
    mLinkIcon = new MMultiPlayLinkIcon(gLcd->GetTopDisplay(), TRUE);
    mLinkIcon->Show();
    mLinkIcon->SetPosition(data_020c003c);
    MMultiPlayLinkIcon *icon = mLinkIcon;
    MTaskSet &parallel = mParallel;
    parallel.AddTail(mMultiPlay);
    parallel.AddTail(icon);
    mSerial.AddTail(new DinoMultiPlayScanner(mMultiPlay, &mConnected));
    mState = data_020bfff0.State0204897c_3C;
}

void DinoMainLoop::State0204897c() {
    if (mConnected && !mMultiPlay->IsFailed()) {
        mParallel.Remove(mMultiPlay);
        mLinkIcon->Exit();
        mUnk84 = 2;
        mState = data_020bfff0.State02048a1c_34;
    } else {
        MTask *exitIcon = MTaskExitTransfer::Create(mLinkIcon);
        MTask *exitMultiPlay = MTaskExitTransfer::Create(mMultiPlay);
        MTask *finish = mMultiPlay->CreateFinishTask();
        MTaskSet &serial = mSerial;
        serial.AddTail(finish);
        serial.AddTail(exitMultiPlay);
        serial.AddTail(exitIcon);
        mState = data_020bff70.StateDebugMenu_44;
    }
}

// Starts the debug battle chosen (mUnk84): against the CPU, CPU against CPU, or wireless.
void DinoMainLoop::State02048a1c() {
    MLoadOverlay(0, M_OVERLAY_ID(ov000));
    switch (mUnk84) {
    case 0: {
        DinoBattleParams params;
        MI_CpuFill8(&params, 0, sizeof(params));
        params.unk00 = 0;
        params.unk08 = 0;
        params.unk0C = 0;
        params.unk10 = 0;
        params.unk14 = 1;
        params.unk18 = 0;
        params.unk1C = 1;
        mSerial.AddTail(new DinoBattleDebugSelect(params, NULL));
        break;
    }
    case 1: {
        DinoBattleParams params;
        MI_CpuFill8(&params, 0, sizeof(params));
        params.unk00 = 1;
        params.unk08 = 0;
        params.unk0C = 2;
        params.unk10 = 0;
        params.unk14 = 1;
        params.unk18 = 0;
        params.unk1C = 1;
        mSerial.AddTail(new DinoBattleDebugSelect(params, NULL));
        break;
    }
    case 2:
        mSerial.AddTail(new DinoMpGame(mMultiPlay));
        break;
    }
    mState = data_020bfff0.StateAutoBattleEnd_24;
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

// Adds an item showing its text.
static inline void AddTextItem(MSimpleMenu *menu, u32 text) {
    menu->AddItem(text, gDinoResource->GetText(text));
}

void DinoMainLoop::State02048b90() {
    mSerial.AddTail(new DinoHelpSample);
    mState = data_020bfff0.StateDebugMenu_0C;
    MSimpleMenu *menu = new MSimpleMenu(&mHelpChoice, gDinoResource->CreateFont12(0, 0, 0), NULL, 0);
    AddTextItem(menu, data_0208a464);
    AddTextItem(menu, data_0208a460);
    AddTextItem(menu, data_0208a45c);
    AddTextItem(menu, data_0208a458);
    AddTextItem(menu, data_0208a454);
    AddTextItem(menu, data_0208a450);
    AddTextItem(menu, data_0208a44c);
    AddTextItem(menu, data_0208a448);
    mSerial.AddTail(menu);
    mState = data_020bfff0.StateHelpSample_04;
}

void DinoMainLoop::StateHelpSample() {
    data_020cc048 = mHelpChoice - data_0208a464;
    mSerial.AddTail(new DinoHelpSample);
    mState = data_020bff70.StateDebugMenu_7C;
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoMainLoop>;

DinoMain::DinoMain() : MProcess(NULL) {
}

M_EXTERN_OVERLAY(ov001);

// Sets up the screens, measures the field's saved size (with the field's overlay loaded for it) and starts the main
// loop.
void DinoMain::Idle() {
    gDmaEnabled = FALSE;
    MSetMaxReadSize(0x1000);
    MEnableSoftReset();
    MEnableSleep();
    RegisterDinoMessages();
    gLcd->GetTopDisplay()->Setup(5, 4, 0, 0);
    gLcd->GetBottomDisplay()->Setup(5, 4, 0, 0);
    gLcd->GetTopDisplay()->mObj->Show();
    gLcd->GetBottomDisplay()->mObj->Show();
    gLcd->GetTopDisplay()->mBright->SetMode(1);
    gLcd->GetTopDisplay()->mBright->SetFactor(16);
    gLcd->GetBottomDisplay()->mBright->SetMode(1);
    gLcd->GetBottomDisplay()->mBright->SetFactor(16);
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetTopDisplay();
    MLoadOverlay(0, M_OVERLAY_ID(ov001));
    MScreen *screen = NULL;
    DinoHelpDriver *driver = new DinoHelpDriver(new MTimer(1));
    DinoField *field = new DinoField(&screen, 1);
    u32 size = field->GetSize();
    delete field;
    delete driver;
    MUnloadOverlay(0, M_OVERLAY_ID(ov001));
    mSerial.AddTail(new DinoHelpDriver(new DinoMainLoop(size)));
}

DinoMain::~DinoMain() {
}
