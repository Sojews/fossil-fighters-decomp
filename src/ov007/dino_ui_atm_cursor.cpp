// DinoUiATMCursor.
// Only the destructor is decompiled so far.

#include "game/dino_ui_atm_cursor.h"

DinoUiATMCursor::~DinoUiATMCursor() {
    delete mUnk10;
}

void DinoUiATMCursor::Execute() {
    if (mUnk10->mState == STATE_RUN) {
        mUnk10->Execute();
    }
}

void DinoUiATMCursor::SetPosition(MPoint pos) {
    mFrame.MoveTo(pos);
    mUnk10->SetPosition(mFrame.GetTopLeft());
}

MPoint DinoUiATMCursor::GetPosition() {
    return mFrame.GetTopLeft();
}

void DinoUiATMCursor::SetSize(MPoint size) {
    mFrame.right = mFrame.left + size.x;
    mFrame.bottom = mFrame.top + size.y;
    mUnk10->SetPosition(mFrame.GetTopLeft());
}

MPoint DinoUiATMCursor::GetSize() {
    return mFrame.GetSize();
}

void DinoUiATMCursor::SetPriority(int priority) {
    mUnk10->SetPriority(priority);
}

int DinoUiATMCursor::GetPriority() {
    return mUnk10->GetPriority();
}

void DinoUiATMCursor::Show() {
    mUnk10->Show();
}

void DinoUiATMCursor::Hide() {
    mUnk10->Hide();
}

BOOL DinoUiATMCursor::IsHidden() {
    return mUnk10->IsHidden();
}

u32 DinoUiATMCursor::GetPlaneMask() {
    return 0x10;
}
