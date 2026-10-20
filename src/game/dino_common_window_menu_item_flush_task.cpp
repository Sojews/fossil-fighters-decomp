// DinoCommonWindowMenuItemFlushTask.
// Only the destructors are decompiled so far.

#pragma ipa file

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "game/dino_common_window_menu_item_flush_task.h"
#include "game/help.h"
#include "game/dino_sound.h"
#include "game/dino_common_window.h"
#include "game/dino_common_window_menu_cursor.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/motion.h"
#include "m/input.h"
#include "game/dino_resource.h"
#include "m/font.h"
#include "m/display.h"
#include "m/oam_canvas.h"
#include "m/indicator.h"
#include "game/dino_static_init.h"

static const char *sCursorMotions[] = {NULL, "/motion/ui_cursor/shovel.bin"};

DinoCommonWindowMenuCursor::DinoCommonWindowMenuCursor(MSmartOam *oam) {
    mFrame.left = 0;
    mFrame.top = 0;
    mFrame.right = 0;
    mFrame.bottom = 0;
    mUnk10 = new MMotion(sCursorMotions[1], oam, 0, 1);
    mUnk10->Play(3, 0);
    mUnk10->GetUnit()->SetMode(1);
}

DinoCommonWindowMenu::DinoCommonWindowMenu(void *unkE4, MPoint pos, const std::vector<DinoWindowChoice> *choices,
                                           u32 unkFC)
    : MMenu(0), mUnkE4(unkE4), mPos(pos), mUnkFC(unkFC) {
    Disable();
    if (choices != NULL) {
        mChoices = *choices;
    }
}

DinoCommonWindowMenu::DinoCommonWindowMenu(MDisplay *display, void *unkE4, MPoint pos,
                                           const std::vector<DinoWindowChoice> *choices, u32 unkFC)
    : MMenu(display, NULL, 0), mUnkE4(unkE4), mPos(pos), mUnkFC(unkFC) {
    Disable();
    if (choices != NULL) {
        mChoices = *choices;
    }
}

void DinoCommonWindowMenu::AddChoice(u32 id, const std::string &a, const std::string &b) {
    DinoWindowChoice choice;
    choice.unk00 = id;
    choice.unk04 = a;
    choice.unk10 = b;
    mChoices.push_back(choice);
}

void DinoCommonWindowMenu::AddEntry(u32 id, MRect rect, const std::string &text) {
    DinoCommonWindowMenuEntry entry;
    entry.id = id;
    entry.rect = rect;
    entry.text = text;
    mEntries.push_back(entry);
}

// A choice: its text in an indicator (hidden until shown) at pos, and its frame the given width.
// NONMATCHING: ours loads gCurrentDisplay before the size argument's copy (into ip) and swaps this/canvas registers.
DinoCommonWindowMenuItem::DinoCommonWindowMenuItem(MFont *font, MPoint pos, std::string text, std::string name,
                                                   int width)
    : mText(name) {
    mUnk94 = new MIndicator(new MObjCanvas(gCurrentDisplay, MPoint(font->MeasureText(text, 0, 0).x, 16),
                                           gDinoResource->GetUnkTable(), TRUE),
                            font, 0);
    mParallel.AddTail(mUnk94);
    ((MIndicator *) mUnk94)->SetPosition(pos.x, pos.y + 1);
    mUnk94->SetPriority(0);
    mUnk94->Hide();
    ((MIndicator *) mUnk94)->SetText(std::string(text));
    mFrame = MRect(pos, MPoint(pos.x + width, pos.y + 14));
}

DinoCommonWindowMenuItemFlushTask::~DinoCommonWindowMenuItemFlushTask() {
}

// B chooses the cancel item: sets the result, hides the items and the cursor and closes the window.
// NONMATCHING: the game reloads the choice count each pass.
#pragma opt_strength_reduction off
void DinoCommonWindowMenu::OnIdle() {
    if (mCancelItem != NULL && (GetInput()->GetPressed() & 2)) {
        for (u32 i = 0; i < mChoices.size(); i++) {
            if (mUnkC0[i] == mCancelItem) {
                *(u32 *)mUnkE4 = mChoices[i].unk00;
                DinoSoundPlaySe(0x9A, 0);
                MParallelTaskSet *hides = new MParallelTaskSet(TRUE);
                for (u32 j = 0; j < mChoices.size(); j++) {
                    hides->AddTail(((DinoCommonWindowMenuItem *)mUnkC0[j])->mUnk94->CreateHideTransfer());
                }
                GetSerial()->AddTail(mCursor->CreateHideTransfer(), hides,
                                     mWindow->CreateCloseTask(MRect(mWidth * 4, 0, mWidth * 4, mUnkBC * 8)
                                                                  .Moved(MPoint(mPos.x - mUnk100.x,
                                                                                mPos.y - mUnk100.y))),
                                     CreateExitTransfer());
                Disable();
                return;
            }
        }
    }
}
#pragma opt_strength_reduction reset

// The TU's pooled data (the offset is set by the TU's static initializer).
extern "C" struct {
    u8 unk00[0x18];
    MPoint mCursorOffset;   // 0x18: (-9, 7), from an item to the cursor
} data_020bbd1c;

// Moves the cursor to the item and flashes its row of the window.
void DinoCommonWindowMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetPosition(item->GetFrame().GetTopLeft() + data_020bbd1c.mCursorOffset);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateCoordAnime(
            mCursor->GetPosition(), item->GetFrame().GetTopLeft() + data_020bbd1c.mCursorOffset, 10, -5));
        DinoSoundPlaySe(0x96, 0);
    }
    for (int i = 0; i < mChoices.size(); i++) {
        if (item == mUnkC0[i]) {
            mWindow->Unk0201bb5c();
            mWindow->Flush(MRect(3, i / 4 + (i * 2 + 1), mWidth - 1, i / 4 + (i * 2 + 1) + (i % 4 < 2 ? 2 : 3)),
                           i % 4);
        }
    }
}

// Sets the result, hides the items, flashes the decided one and closes the window.
#pragma opt_strength_reduction off
void DinoCommonWindowMenu::OnDecide(MMenuItem *item) {
    for (u32 i = 0; i < mChoices.size(); i++) {
        if (item == mUnkC0[i]) {
            *(u32 *)mUnkE4 = mChoices[i].unk00;
            DinoSoundPlaySe(0x98, 0);
            MParallelTaskSet *hides = new MParallelTaskSet(TRUE);
            for (u32 j = 0; j < mChoices.size(); j++) {
                hides->AddTail(((DinoCommonWindowMenuItem *)mUnkC0[j])->mUnk94->CreateHideTransfer());
            }
            MPoint offset(mPos.x - mUnk100.x, mPos.y - mUnk100.y);
            MRect rect = MRect(mWidth * 4, 0, mWidth * 4, mUnkBC * 8).Moved(offset);
            GetSerial()->AddTail(mCursor->mUnk10->CreatePlayTransfer(2, 1),
                                 new DinoCommonWindowMenuItemFlushTask(this, i), hides,
                                 mWindow->CreateCloseTask(rect));
            mSerial.AddTail(CreateExitTransfer());
            Disable();
        }
    }
}
#pragma opt_strength_reduction reset

void DinoCommonWindowMenuItemFlushTask::Execute() {
    if (++mFrame >= 36) {
        Exit();
        return;
    }
    int phase = mFrame % 6;
    if (phase == 1) {
        mMenu->mWindow->Unk0201bb5c();
    } else if (phase == 4) {
        int row = mIndex % 4;
        int height = row < 2 ? 2 : 3;
        int top = mIndex / 4 + (mIndex * 2 + 1);
        mMenu->mWindow->Flush(MRect(3, top, mMenu->mWidth - 1, top + height), row);
    }
}

DinoCommonWindowMenu::~DinoCommonWindowMenu() {
}

DinoCommonWindowMenuItem::~DinoCommonWindowMenuItem() {
}

void DinoCommonWindowMenuItem::OnSelectChanged(BOOL value) {
}

void DinoCommonWindowMenuItem::OnFocusChanged(BOOL value) {
}

void DinoCommonWindowMenuItem::OnDecide() {
}

MRect DinoCommonWindowMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoCommonWindowMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoCommonWindowMenuItem::GetText() {
    return mText.c_str();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoCommonWindowMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoCommonWindowMenu *obj, void (DinoHelpBase::*func)());

MRect DinoCommonWindowMenuItem::GetFrame() {
    return mFrame;
}

DinoCommonWindowMenuCursor::~DinoCommonWindowMenuCursor() {
    delete mUnk10;
}

void DinoCommonWindowMenuCursor::Execute() {
    if (mUnk10->mState == STATE_RUN) {
        mUnk10->Execute();
    }
}

void DinoCommonWindowMenuCursor::SetPosition(MPoint pos) {
    mFrame.MoveTo(pos);
    mUnk10->SetPosition(mFrame.GetTopLeft());
}

MPoint DinoCommonWindowMenuCursor::GetPosition() {
    return mFrame.GetTopLeft();
}

void DinoCommonWindowMenuCursor::SetSize(MPoint size) {
    mFrame.right = mFrame.left + size.x;
    mFrame.bottom = mFrame.top + size.y;
    mUnk10->SetPosition(mFrame.GetTopLeft());
}

MPoint DinoCommonWindowMenuCursor::GetSize() {
    return mFrame.GetSize();
}

void DinoCommonWindowMenuCursor::SetPriority(int priority) {
    mUnk10->SetPriority(priority);
}

int DinoCommonWindowMenuCursor::GetPriority() {
    return mUnk10->GetPriority();
}

void DinoCommonWindowMenuCursor::Show() {
    mUnk10->Show();
}

void DinoCommonWindowMenuCursor::Hide() {
    mUnk10->Hide();
}

BOOL DinoCommonWindowMenuCursor::IsHidden() {
    return mUnk10->IsHidden();
}

u32 DinoCommonWindowMenuCursor::GetPlaneMask() {
    return 0x10;
}

// String members the game emits in this file.
template std::string std::string::substr(std::string::size_type pos, std::string::size_type n) const;
// (Calling find here instantiates it with the name the game's callers use.)
#pragma dont_inline on
std::string::size_type DinoFlushTaskFindAnchor(const std::string &s, char c) {
    return s.find(c, 0);
}
#pragma dont_inline reset

// The file's static initialization (see dino_static_init.h).
static struct DinoCommonWindowMenuItemFlushTaskStaticInit {
    DinoCommonWindowMenuItemFlushTaskStaticInit() {
        DINO_POOL_WORD(data_020bbd1c, 0x18) = -9;
        DINO_POOL_WORD(data_020bbd1c, 0x1c) = 7;
    }
} sDinoCommonWindowMenuItemFlushTaskStaticInit;

// (Off at the end of the file, where template members are instantiated: the game calls them out of line.)
#pragma auto_inline off
