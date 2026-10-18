// DinoUiTeamSelect.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/bright.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_ui_team_select.h"
#include "types.h"
#include "m/call_task.h"

DinoUiTeamSelectMenu::~DinoUiTeamSelectMenu() {
}

DinoUiTeamSelect::~DinoUiTeamSelect() {
}

DinoUiTeamSelectTeamButton::~DinoUiTeamSelectTeamButton() {
}

void DinoUiTeamSelectMenu::OnIdle() {
}

void DinoUiTeamSelectTeamButton::OnDecide() {
}

MRect DinoUiTeamSelectTeamButton::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiTeamSelectTeamButton::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoUiTeamSelectTeamButton::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiTeamSelectTeamButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mView->Show();
    } else {
        mView->Hide();
    }
}

extern "C" void func_ov007_0218b81c(DinoUiTeamSelectMenu *menu, int index);

void DinoUiTeamSelectMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 3; i++) {
        if (item == mTeams[i]) {
            mSelected = i + 1;
            func_ov007_0218b81c(this, i);
        }
    }
}

MRect DinoUiTeamSelectTeamButton::GetFrame() {
    return MRect(mPos.Offset(-100, -12), mPos.Offset(100, 12));
}

void DinoUiTeamSelectMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(item->GetFrame());
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), item->GetFrame(), 10, -5));
    }
    for (int i = 0; i < 3; i++) {
        if (item == mTeams[i]) {
            mTeamInfo->SetTeam(i + 1);
        }
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiTeamSelectMenu, DinoUiTeamSelectMenu>;
template MTask *MCreateCallTask(DinoUiTeamSelectMenu *obj, void (DinoUiTeamSelectMenu::*func)());

DinoUiTeamSelect::DinoUiTeamSelect(u32 *result, u8 unk78, u8 unk79)
    : MProcess(NULL), mResult(result), mUnk78(unk78), mUnk79(unk79) {
}

// Fades both screens out to white and leaves.
void DinoUiTeamSelectMenu::Close() {
    MTask *fadeMain;
    MTask *fadeSub;
    MTask *exit;
    exit = CreateExitTransfer();
    fadeSub = gLcd->mSub->mBright->CreateFadeOutToWhite(30, 0);
    fadeMain = gLcd->mMain->mBright->CreateFadeOutToWhite(30, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeMain);
    fades->AddTail(fadeSub);
    MTask *wait = new MTimer(30);
    MTaskSet &serial = mSerial;
    serial.AddTail(wait);
    serial.AddTail(fades);
    serial.AddTail(exit);
}
