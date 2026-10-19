// DinoCommonCursor.
// NONMATCHING: the constructor (the game keeps &mCorners[i] in a stack slot across the path string's destructor).

#include "game/dino_common_cursor.h"
#include "m/motion.h"

// The TU's pooled constants.
extern "C" struct {
    u8 unk00[4];
    const char *mPath;  // 0x04
} data_020bb288;
// The corners' animations.
extern "C" const int data_0208ae20[4];

DinoCommonCursor::DinoCommonCursor(MSmartOam *oam) {
    mFrame = MRect(0, 0, 0, 0);
    for (u32 i = 0; i < 4; i++) {
        mCorners[i] = new MMotion(std::string(data_020bb288.mPath), oam, 0, 1);
        mCorners[i]->Play(data_0208ae20[i], 0);
        mCorners[i]->SetAnchor(2);
        mTasks.AddTail(mCorners[i]);
    }
}

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
