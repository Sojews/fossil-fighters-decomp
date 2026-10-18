#ifndef GAME_DINO_MAIN_LOOP_H
#define GAME_DINO_MAIN_LOOP_H

#include "m/task.h"

// The game's top level: the title screen, then the game or one of the debug menus' samples.
class DinoMainLoop : public MProcess {
public:
    DinoMainLoop(u32 slot);
    virtual ~DinoMainLoop();
    virtual void Idle();

    void StateStart();
    void LoadResources();
    void StateLoad();
    void StateTitle();
    void StateTitleEnd();
    void StateGame();
    void State02047a1c();
    void StateVolume();
    void StateWirelessBattle();
    void StateWirelessBattleEnd();
    void StateATM();
    void StateATMEnd();
    void StateDebugMenu();
    void State02048030();
    void State02048398();
    void State020483fc();
    void StateTopMenu();
    void StateTopMenuEnd();
    void StateATMSample();
    void StateATMSampleEnd();
    void StateFieldMenuSample();
    void StateFieldMenuSampleEnd();
    void StateSaveOption();
    void StateSaveOptionEnd();
    void StateCleaningTest();
    void StateCleaningTestEnd();
    void StateMuseum();
    void StateMuseumEnd();
    void StateMaskEquipTest();
    void StateMaskEquipTestEnd();
    void StateNetBattleSample();
    void StateNetBattleSampleEnd();
    void StateMapSelect();
    void StateMapSelectEnd();
    void StateKeyboard();
    void StateKeyboardEnd();
    void State020488a4();
    void State0204897c();
    void State02048a1c();
    void StateAutoBattle();
    void StateAutoBattleEnd();
    void State02048b90();
    void StateHelpSample();

    void (DinoMainLoop::*mState)(); // 0x74
    u32 mUnk7C;                     // 0x7C
    u32 mUnk80;                     // 0x80
    u32 mUnk84;                     // 0x84
    u32 mSlot;                      // 0x88: the save slot
    u32 mUnk8C;                     // 0x8C
    u32 mUnk90;                     // 0x90
    u32 mUnk94;                     // 0x94
    s32 mTitleResult;               // 0x98: what was chosen on the title screen
    u32 mUnk9C;                     // 0x9C
    s32 mUnkA0;                     // 0xA0
};

#endif
