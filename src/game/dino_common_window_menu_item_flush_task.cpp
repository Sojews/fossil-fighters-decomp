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

extern "C" struct {
    void (DinoHelpBase::*Enable)();
} data_020bbd44;

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

// Sizes the window to the widest choice and the count, then a menu item per choice ("*" marks the cancel choice;
// "text:name" gives the help a name), help areas for the entries, the cursor, and opens the window. With no choices,
// just exits.
// NONMATCHING: stack layout (the game's frame is 0x1C smaller: fewer string/point temporaries) and the width/height
// stores' order.
#pragma opt_strength_reduction off
#pragma auto_inline off
template MTask *MCreateCallTask(DinoCommonWindowMenu *obj, void (DinoHelpBase::*func)());
void DinoCommonWindowMenu::OnStart() {
    if (mChoices.size() == 0) {
        Exit();
        return;
    }
    SetLinkFallback(1);
    MFont *font = gDinoResource->CreateFont12(0, 1, 0);
    int width = 0x18;
    for (u32 i = 0; i < mChoices.size(); i++) {
        std::string text = mChoices[i].unk04;
        if (text.size() != 0 && text[0] == '*') {
            text = text.substr(1);
        }
        u32 colon = text.find(':', 0);
        if (colon != std::string::npos) {
            text = text.substr(0, colon);
        }
        MPoint size = font->MeasureText(text, 0, 0);
        if (size.x > width) {
            width = size.x;
        }
    }
    mUnkBC = (mChoices.size() + 1) / 4 + mChoices.size() * 2 + 2;
    mWidth = (width + 7) / 8 + 4;
    switch (mUnkFC) {
    case 0:
        mUnk100 = MPoint(0, 0);
        break;
    case 1:
        mUnk100 = MPoint(mWidth * 8, 0);
        break;
    case 2:
        mUnk100 = MPoint(0, mUnkBC * 8);
        break;
    case 3:
        mUnk100 = MPoint(mWidth * 8, mUnkBC * 8);
        break;
    case 4:
        mUnk100 = MPoint(mWidth * 4, mUnkBC * 4);
        break;
    case 5:
        mUnk100 = MPoint(mWidth * 4, 0);
        break;
    case 6:
        mUnk100 = MPoint(mWidth * 4, mUnkBC * 8);
        break;
    }
    mWindow = new DinoCommonWindow(0, false, false);
    mWindow->SetFrame(MRect(MPoint(0, 0), MPoint(mWidth, mUnkBC)));
    mWindow->SetPriority(0);
    mWindow->SetPosition(mPos - mUnk100);
    mParallel.AddTail(mWindow);
    MParallelTaskSet *shows = new MParallelTaskSet(TRUE);
    MMenuItem *focus = NULL;
    mCancelItem = NULL;
    int y = 8;
    for (u32 i = 0; i < mChoices.size(); i++) {
        std::string text;
        std::string name;
        BOOL cancel = FALSE;
        if (mChoices[i].unk00 == (u32)-2) {
            cancel = TRUE;
        }
        text = mChoices[i].unk04;
        name = mChoices[i].unk10;
        if (text.size() != 0 && text[0] == '*') {
            cancel = TRUE;
            text = text.substr(1);
        }
        u32 colon = text.find(':', 0);
        if (colon != std::string::npos) {
            name = text.substr(colon + 1);
            text = text.substr(0, colon);
        }
        mUnkC0.push_back(new DinoCommonWindowMenuItem(font, (mPos - mUnk100).Offset(0x18, y), text, name, width));
        AddItem((MMenuItem *)mUnkC0[i]);
        DinoHelpBase::AddEntry((u32)(DinoHelpItem *)(DinoCommonWindowMenuItem *)mUnkC0[i], 1, 0);
        DinoHelpBase::AddEntry((u32)(DinoHelpItem *)(DinoCommonWindowMenuItem *)mUnkC0[i], 3, 0);
        shows->AddTail(((DinoCommonWindowMenuItem *)mUnkC0[i])->mUnk94->CreateShowTransfer());
        if (*(u32 *)mUnkE4 == mChoices[i].unk00) {
            focus = (MMenuItem *)mUnkC0[i];
        }
        if (cancel) {
            mCancelItem = (MMenuItem *)mUnkC0[i];
        }
        y += 0x12;
    }
    AddLink((MMenuItem *)mUnkC0[0], 4, (MMenuItem *)mUnkC0[mUnkC0.size() - 1]);
    AddLink((MMenuItem *)mUnkC0[mUnkC0.size() - 1], 2, (MMenuItem *)mUnkC0[0]);
    DinoHelpDummyItem *prev = NULL;
    for (u32 i = 0; i < mEntries.size(); i++) {
        DinoCommonWindowMenuEntry *entry = &mEntries[i];
        DinoHelpDummyItem *help = new DinoHelpDummyItem((MDisplay *)entry->id, entry->rect, std::string(entry->text));
        mParallel.AddTail(help);
        if (prev != NULL) {
            DinoHelpBase::AddEntry((u32)prev, 2, (u32)help);
        }
        DinoHelpBase::AddEntry((u32)help, 4, (u32)prev);
        DinoHelpBase::AddEntry((u32)help, 1, 0);
        DinoHelpBase::AddEntry((u32)help, 3, 0);
        prev = help;
    }
    if (prev != NULL) {
        DinoHelpBase::AddEntry((u32)prev, 2, (u32)(DinoHelpItem *)(DinoCommonWindowMenuItem *)mUnkC0[0]);
        DinoHelpBase::AddEntry((u32)(DinoHelpItem *)(DinoCommonWindowMenuItem *)mUnkC0[0], 4, (u32)prev);
    }
    MSmartOam *oam = new MSmartOam(1, 0, TRUE);
    mCursor = new DinoCommonWindowMenuCursor(oam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(0);
    mCursorTasks = new MParallelTaskSet();
    mParallel.AddTail(mCursorTasks);
    mParallel.AddTail(oam);
    if (focus == NULL) {
        focus = (MMenuItem *)mUnkC0[0];
    }
    FocusItem(focus);
    mCursor->Hide();
    MTask *play = mCursor->mUnk10->CreatePlayTransfer(1, 1);
    shows->AddTail(mCursor->CreateShowTransfer());
    shows->AddTail(play);
    DinoSoundPlaySe(0x95, 0);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    MTask *enable = MCreateCallTask(this, data_020bbd44.Enable);
    MTask *open = mWindow->CreateOpenTask(MRect(mPos, mPos));
    MTask *show = mWindow->CreateShowTransfer();
    mSerial.AddTail(new MTimer(1), show, open, shows, enable);
}
#pragma auto_inline reset
#pragma opt_strength_reduction reset

// A choice: its text in an indicator (hidden until shown) at pos, and its frame the given width.
// (No common subexpressions: the game reads gCurrentDisplay after copying the size argument.)
#pragma opt_common_subs off
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
#pragma opt_common_subs reset

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

// The entry list's growth, emitted ahead of the push it serves.
template size_t std::__vec_deleter<DinoCommonWindowMenuEntry, std::allocator<DinoCommonWindowMenuEntry> >::grow_by(size_t n);
template void std::__vec_deleter<DinoCommonWindowMenuEntry, std::allocator<DinoCommonWindowMenuEntry> >::append_realloc(size_t n, const DinoCommonWindowMenuEntry &x);
template void std::__vec_deleter<DinoCommonWindowMenuEntry, std::allocator<DinoCommonWindowMenuEntry> >::sub_push_back(const DinoCommonWindowMenuEntry &x, Metrowerks::int2type<false>);
