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
