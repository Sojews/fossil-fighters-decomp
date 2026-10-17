// DinoUiTitle, DinoUiTitleSkip, DinoUiOpeningCloud, DinoUiOpeningChara, DinoUiTitleMenuGameItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

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
    mUnkC8 = 0;
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
