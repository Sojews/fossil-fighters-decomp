// DinoCommonCursor.
// Only the destructor is decompiled so far.

#include "game/dino_common_cursor.h"

DinoCommonCursor::~DinoCommonCursor() {
}

void DinoCommonCursor::Execute() {
    if (mTasks.mState == STATE_RUN) {
        MTask *tasks = &mTasks;
        tasks->Execute();
    }
}

void DinoCommonCursor::SetPosition(MPoint pos) {
    mFrame.MoveTo(pos);
    Layout();
}

MPoint DinoCommonCursor::GetPosition() {
    return mFrame.GetTopLeft();
}

void DinoCommonCursor::SetSize(MPoint size) {
    mFrame.right = mFrame.left + size.x;
    mFrame.bottom = mFrame.top + size.y;
    Layout();
}

MPoint DinoCommonCursor::GetSize() {
    return mFrame.GetSize();
}

void DinoCommonCursor::Layout() {
    mCorners[0]->SetPosition(MPoint(mFrame.left, mFrame.top));
    mCorners[1]->SetPosition(MPoint(mFrame.left, mFrame.bottom));
    mCorners[2]->SetPosition(MPoint(mFrame.right, mFrame.top));
    mCorners[3]->SetPosition(MPoint(mFrame.right, mFrame.bottom));
}

void DinoCommonCursor::SetPriority(int priority) {
    for (u32 i = 0; i < 4; i++) {
        mCorners[i]->SetPriority(priority);
    }
}

int DinoCommonCursor::GetPriority() {
    return mCorners[0]->GetPriority();
}

void DinoCommonCursor::Show() {
    for (u32 i = 0; i < 4; i++) {
        mCorners[i]->Show();
    }
}

void DinoCommonCursor::Hide() {
    for (u32 i = 0; i < 4; i++) {
        mCorners[i]->Hide();
    }
}

BOOL DinoCommonCursor::IsHidden() {
    return mCorners[0]->IsHidden();
}

u32 DinoCommonCursor::GetPlaneMask() {
    return 0x10;
}
