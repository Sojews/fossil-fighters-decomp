// DinoUiTitle, DinoUiTitleSkip, DinoUiOpeningCloud, DinoUiOpeningChara, DinoUiTitleMenuGameItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/menu.h"
#include "game/dino_ui_top_menu_loop.h"
#include "game/dino_ui_save_slot_select.h"
#include "game/dino_sound.h"
#include "game/dino_ui_title_menu.h"
#include "game/dino_ui_title.h"
#include "game/dino_ui_title_skip.h"
#include "game/dino_ui_opening_cloud.h"
#include "game/dino_ui_opening_chara.h"
#include "game/dino_ui_title_menu_game_item.h"
#include "m/bg.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiTitle::~DinoUiTitle() {
}

DinoUiTitleSkip::~DinoUiTitleSkip() {
}

DinoUiOpeningCloud::~DinoUiOpeningCloud() {
}

DinoUiOpeningChara::~DinoUiOpeningChara() {
}

DinoUiTitleMenuGameItem::~DinoUiTitleMenuGameItem() {
}

DinoUiTitleMenu::~DinoUiTitleMenu() {
}

void DinoUiTitle::Idle() {
    (this->*mState)();
}

DinoUiOpening::~DinoUiOpening() {
}

void DinoUiOpening::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoUiTitle>;
template MTask *MCreateBgCallTask(DinoUiTitle *obj, void (DinoUiTitle::*func)(), int thread);
template class MCallTask1<MBg, MBg, bg_mode>;
template MTask *MCreateCallTask(MBg *obj, void (MBg::*func)(bg_mode), bg_mode a0);
template class MBgCallTask0<DinoUiOpening>;
template MTask *MCreateBgCallTask(DinoUiOpening *obj, void (DinoUiOpening::*func)(), int thread);

void DinoUiTitleMenu::OnFocus(MMenuItem *item) {
    mIdleTime = 0;
    if (mSilentFocus) {
        mSilentFocus = FALSE;
    } else {
        DinoSoundPlaySe(0x38E, 0);
    }
}

void DinoUiTitleMenu::OnDecide(MMenuItem *item) {
    if (item == mStartItem) {
        DinoSoundPlaySe(0x390, 0);
        *mResult = 3;
        mMotion->Play(2, 0);
        mSerial.AddTail(mMotion->CreateWaitStopTask());
    }
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

MRect DinoUiTitleMenuGameItem::GetFrame() {
    return MRect(0, 0, 256, 192);
}

void DinoUiTitleMenu::OnIdle() {
    if (mOam->mOffset.y == 0) {
        mMotion->Play(1, 0);
        if (mIdleTime++ >= 1200) {
            *mResult = 8;
            Exit();
        }
    }
}

void DinoUiOpening::Execute() {
    if (mInput->GetPressed() & 0x1C0B) { // A, B, Start, X, Y or a touch
        mSkipped = TRUE;
    }
    MProcess::Execute();
}

void DinoUiOpeningCloud::Idle() {
    mSerial.AddTail(mCloud->CreateCoordAnime(MPoint(-512, 0), MPoint(0, 0), 600, 0));
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
// The TU's pooled pointer-to-member constants (see menu.cpp). The game's pool is one block; dsd splits it every 0x80
// bytes (data_ov004_02172ff8, data_ov004_02173078), and tools/anon_names.py maps the offsets back.
extern "C" struct {
    u8 unk00[0x3C];
    void (DinoUiTitle::*Unk0215ee60_3C)();
    void (DinoUiTitle::*Unk0215f4ec_44)();
    u8 unk4C[0x7C - 0x4C];
    void (DinoUiTitle::*StateSaveSlot_7C)();
    void (DinoUiTitle::*StateTopMenuEnd_84)();
    u8 unk8C[0x9C - 0x8C];
    void (DinoUiTitle::*Unk0215f1e0_9C)();
    u8 unkA4[0xAC - 0xA4];
    void (DinoUiTitle::*Unk0215e03c_AC)();
    void (DinoUiTitle::*Start_B4)();
    u8 unkBC[0xF4 - 0xBC];
    void (DinoUiTitle::*Unk0215f5ac_F4)();
    u8 unkFC[0x114 - 0xFC];
    void (DinoUiOpening::*OpeningStart_114)();
    u8 unk11C[0x12C - 0x11C];
    void (DinoUiTitle::*Unk0215f420_12C)();
    u8 unk134[0x144 - 0x134];
    void (DinoUiTitle::*Unk0215e9e4_144)();
    u8 unk14C[0x164 - 0x14C];
    void (DinoUiTitle::*StateSaveSlotEnd_164)();
} data_ov004_02172f78;

inline DinoUiOpening::DinoUiOpening(MInput *input) : MProcess(NULL), mInput(input) {
    mState = data_ov004_02172f78.OpeningStart_114;
}

DinoUiTitle::DinoUiTitle(s32 *result) : MProcess(NULL), mResult(result) {
    mState = data_ov004_02172f78.Start_B4;
}

void DinoUiTitle::StateOpening() {
    mSerial.AddTail(new DinoUiOpening(mInput));
    mState = data_ov004_02172f78.Unk0215e03c_AC;
}

void DinoUiTitle::StateMenu() {
    mParallel.Remove(mMenu);
    mMenu->Activate();
    mSerial.AddTail(mMenu);
    mState = data_ov004_02172f78.Unk0215e9e4_144;
}

void DinoUiTitle::StateSaveSlot() {
    *mResult = 4;
    mSerial.AddTail(new DinoUiSaveSlotSelect(mInput, &mSlotChoice));
    mState = data_ov004_02172f78.StateSaveSlotEnd_164;
}

void DinoUiTitle::StateSaveSlotEnd() {
    switch (mSlotChoice) {
    case 1:
        mState = data_ov004_02172f78.Unk0215ee60_3C;
        break;
    case 0:
        mState = data_ov004_02172f78.Unk0215f1e0_9C;
        break;
    }
}

void DinoUiTitle::StateTopMenu() {
    mSerial.AddTail(new DinoUiTopMenuLoop(&mTopResult));
    mState = data_ov004_02172f78.StateTopMenuEnd_84;
}

void DinoUiTitle::StateTopMenuEnd() {
    switch (mTopResult) {
    case 1:
        mState = data_ov004_02172f78.Unk0215f420_12C;
        break;
    case 0:
        mState = data_ov004_02172f78.StateSaveSlot_7C;
        break;
    case 2:
        mState = data_ov004_02172f78.Unk0215f4ec_44;
        break;
    case 3:
        mState = data_ov004_02172f78.Unk0215f5ac_F4;
        break;
    }
}
