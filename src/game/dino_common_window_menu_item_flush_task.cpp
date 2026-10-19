// DinoCommonWindowMenuItemFlushTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "game/dino_common_window_menu_item_flush_task.h"
#include "game/help.h"
#include "game/dino_common_window_menu_cursor.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/motion.h"

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

DinoCommonWindowMenuItemFlushTask::~DinoCommonWindowMenuItemFlushTask() {
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
