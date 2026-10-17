// DinoUiATMCursor.

#include "game/dino_ui_atm_cursor.h"
#include "m/motion.h"

extern const char *data_ov007_0218c9e0[];   // motion files

DinoUiATMCursor::DinoUiATMCursor(MSmartOam *oam) : mFrame(0, 0, 0, 0) {
    mUnk10 = new MMotion(std::string(data_ov007_0218c9e0[5]), oam, 0, 1);
    mUnk10->Play(1, 0);
    mUnk10->Hide();
}

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
