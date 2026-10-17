// DinoCleaningReport.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_report.h"
#include "types.h"

DinoCleaningReport::~DinoCleaningReport() {
}

void DinoCleaningReport::Idle() {
    (this->*mState)();
}
