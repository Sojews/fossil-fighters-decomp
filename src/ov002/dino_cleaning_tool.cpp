// DinoCleaningTool.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#define DINO_CLEANING_TOOL_OWNER
#include "game/dino_cleaning_tool.h"
#include "types.h"
#include "game/dino_static_init.h"

DinoCleaningTool::DinoCleaningTool(DinoCleaning *cleaning) : MProcess(NULL) {
    mCleaning = cleaning;
}

DinoCleaning *DinoCleaningTool::GetCleaning() {
    return mCleaning;
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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov002_02164e4c[];
#pragma always_inline on
static struct DinoCleaningToolStaticInit {
    DinoCleaningToolStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_02164e4c, 0x0, 0x38, 0x18, 0x10, 0x48, 0x58, 0x60, 0x68);
    }
} sDinoCleaningToolStaticInit;
