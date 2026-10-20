// DinoUiTeamSelect.
// NONMATCHING: DinoUiTeamSelect::Idle (an extra cmp on the new menu's null check).

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/bright.h"
#include "m/display.h"
#include "m/lcd.h"
#include "game/dino_ui_team_select.h"
#include "types.h"
#include "m/call_task.h"
#include "game/dino_backup.h"
#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "game/dino_static_init.h"

DinoUiTeamSelectMenu::~DinoUiTeamSelectMenu() {
}

DinoUiTeamSelectMenu::DinoUiTeamSelectMenu(int *result, u8 unk108, u8 unk109) : MMenu(NULL, 0) {
    mResult = result;
    mUnk108 = unk108;
    mUnk109 = unk109;
    mUnk10C = 0;
    mSelected = 1;
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

void DinoUiTeamSelectMenu::RefreshTeams() {
    for (int i = 0; i < 3; i++) {
        DinoTeam team;
        gDinoBackup->GetTeam(i + 1, &team);
        u32 j;
        int count = 0;
        for (j = 0; j < 5; j++) {
            if (team.slots[j] != 0) {
                count++;
            }
        }
        if (count == 0) {
            mNames[i]->SetText(std::wstring(L" "));
            mLabels[i]->SetText(std::string(" "));
            mUnkEC[i]->Show();
            mTeams[i]->Disable();
        } else {
            mNames[i]->SetText(std::wstring((const wchar_t *)team.name));
            mLabels[i]->SetText(std::wstring((const wchar_t *)team.name));
            mUnkEC[i]->Hide();
            mTeams[i]->Enable();
        }
        if (i + 1 == gDinoBackup->GetUnk34_26()) {
            mNames[i]->Show();
            mLabels[i]->Hide();
        } else {
            mLabels[i]->Show();
            mNames[i]->Hide();
        }
    }
}

// Where the teams are, and the menu's offset from them.
extern MPoint data_ov007_02192414[3];
extern struct {
    u8 unk00[0x74];
    MPoint menuOffset;  // 0x74
} data_ov007_02192318;
extern const u32 data_0208ab24;
extern const u32 data_0208ab28;

void DinoUiTeamSelectMenu::OpenMenu(int team) {
    mUnk10C = 0;
    mCursor->Hide();
    MPoint pos = data_ov007_02192414[team] + data_ov007_02192318.menuOffset;
    if (team >= 1) {
        pos.y -= 0x40;
    }
    DinoCommonWindowMenu *menu = new DinoCommonWindowMenu(&mUnk10C, pos, NULL, 0);
    menu->AddChoice(1, gDinoResource->GetText(data_0208ab28), gDinoResource->GetText(data_0208ab28));
    menu->AddChoice(-2, gDinoResource->GetText(data_0208ab24), gDinoResource->GetText(data_0208ab24));
    MTask *decided;
    MTask *show = mCursor->CreateShowTransfer();
    decided = MCreateCallTask(this, &DinoUiTeamSelectMenu::StateDecided);
    MTask *task = menu;
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(decided);
    serial->AddTail(show);
}

void DinoUiTeamSelectMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 3; i++) {
        if (item == mTeams[i]) {
            mSelected = i + 1;
            OpenMenu(i);
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

#pragma dont_inline on
void DinoUiTeamSelect::Idle() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 0, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiTeamSelectMenu((int *)mResult, mUnk78, mUnk79);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}
#pragma dont_inline reset

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

#pragma dont_inline on
void DinoUiTeamSelectMenu::StateDecided() {
    if (mUnk10C == 1) {
        *mResult = mSelected;
        Close();
    }
}
#pragma dont_inline reset

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov007_02192398[];
extern "C" u32 data_ov007_02192418[];
extern "C" u32 data_ov007_0219241c[];
extern "C" u32 data_ov007_02192424[];
static struct DinoUiTeamSelectStaticInit {
    DinoUiTeamSelectStaticInit() {
        DINO_POOL_WORD(data_ov007_02192318, 0x54) = 128;
        DINO_POOL_WORD(data_ov007_02192318, 0x58) = 176;
        DINO_POOL_WORD(data_ov007_02192318, 0x14) = 8;
        DINO_POOL_WORD(data_ov007_02192318, 0x18) = 150;
        DINO_POOL_WORD(data_ov007_02192318, 0x64) = 64;
        DINO_POOL_WORD(data_ov007_02192318, 0x68) = 112;
        DINO_POOL_WORD(data_ov007_02192318, 0x34) = 128;
        DINO_POOL_WORD(data_ov007_02192318, 0x38) = 96;
        DINO_POOL_WORD(data_ov007_02192318, 0x44) = 123;
        DINO_POOL_WORD(data_ov007_02192318, 0x48) = 120;
        DINO_POOL_WORD(data_ov007_02192318, 0x24) = 133;
        DINO_POOL_WORD(data_ov007_02192318, 0x28) = 120;
        DINO_POOL_WORD(data_ov007_02192318, 0x4c) = 246;
        DINO_POOL_WORD(data_ov007_02192318, 0x50) = 138;
        DINO_POOL_WORD(data_ov007_02192398, 0x7c) = 50;
        DINO_POOL_WORD(data_ov007_02192418, 0x0) = 46;
        volatile s32 *p = (volatile s32 *) &data_ov007_0219241c;
        p[0x0 / 4] = 50;
        p[0x4 / 4] = 85;
        DINO_POOL_WORD(data_ov007_02192424, 0x0) = 50;
        DINO_POOL_WORD(data_ov007_02192424, 0x4) = 124;
        DINO_POOL_WORD(data_ov007_02192318, 0x5c) = 78;
        DINO_POOL_WORD(data_ov007_02192318, 0x60) = 8;
        DINO_POOL_WORD(data_ov007_02192318, 0x74) = 78;
        DINO_POOL_WORD(data_ov007_02192318, 0x78) = -8;
        DINO_POOL_WORD(data_ov007_02192318, 0x6c) = -10;
        DINO_POOL_WORD(data_ov007_02192318, 0x70) = -4;
    }
} sDinoUiTeamSelectStaticInit;

// (Off at the end of the file, where template members are instantiated: the game calls them out of line.)
#pragma auto_inline off
