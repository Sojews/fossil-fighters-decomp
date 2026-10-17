// CameraWipe.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/camera_wipe.h"
#include "types.h"

CameraWipe::~CameraWipe() {
}

M3DFadeTask::~M3DFadeTask() {
}

// NONMATCHING: the game calls std::vector<std::string>'s out-of-line destructor for mLines; ours inlines it into a
// __vec_deleter call. The bytes are identical, so only callcheck sees it.
DinoBattleViewActionInfoBar::~DinoBattleViewActionInfoBar() {
    mFont->Release();
}
