// DinoMultiPlayIcon.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_multi_play_icon_menu.h"
#include "game/dino_multi_play_icon.h"
#include "m/lcd.h"

DinoMultiPlayIcon::DinoMultiPlayIcon() : MProcess(NULL) {
    GetParallel()->AddTail(new DinoMultiPlayIconHook(gLcd->GetTopDisplay(), MPoint(236, 4)),
                           new DinoMultiPlayIconHook(gLcd->GetBottomDisplay(), MPoint(236, 4)));
}

DinoMultiPlayIcon::~DinoMultiPlayIcon() {
}

DinoMultiPlayIconHook::~DinoMultiPlayIconHook() {
}

void DinoMultiPlayIconHook::Unk08() {
}

void DinoMultiPlayIcon::Idle() {
}

void DinoMultiPlayIconHook::Idle() {
    if (mUnk7C == NULL && GetDisplay() == gLcd->GetTopDisplay() && GetDisplay()->mObj != NULL) {
        mUnk7C = new MMultiPlayLinkIcon(GetDisplay(), FALSE);
        mUnk7C->SetPosition(mPos);
        mUnk7C->Show();
        mParallel.AddTail(mUnk7C);
    }
    if (mUnk7C != NULL && GetDisplay() != gLcd->GetTopDisplay()) {
        mUnk7C->Exit();
        mUnk7C = NULL;
    }
}

void DinoMultiPlayIconHook::Unk0C() {
    mParallel.Clear();
    mUnk7C = 0;
}
