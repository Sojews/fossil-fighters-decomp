// DinoCleaningTool.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#define DINO_CLEANING_TOOL_OWNER
#include "game/dino_cleaning_tool.h"
#include "types.h"

DinoCleaningTool::DinoCleaningTool(DinoCleaningKasekiRock *rock) : MProcess(NULL) {
    mRock = rock;
}

DinoCleaningKasekiRock *DinoCleaningTool::GetRock() {
    return mRock;
}

void DinoCleaningTool::Unk18() {
}

void DinoCleaningTool::Unk1C() {
}

void DinoCleaningTool::Unk20() {
}

DinoCleaningTool::~DinoCleaningTool() {
}

void DinoCleaningTool::Idle() {
}
