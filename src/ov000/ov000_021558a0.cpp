// Ov000021558a0.

#include "types.h"
#include "m/resource.h"
#include "game/dino_battle_cpu_controller.h"

// Resources the file loads (their data types are unknown); until the code that constructs them is decompiled,
// these keep out-of-line copies of the constructors.
struct DinoOv000DataA;
struct DinoOv000DataB;
#pragma dont_inline on
MResource<DinoOv000DataA> DinoOv000DataALoad(std::string path) {
    return MResource<DinoOv000DataA>(path);
}
#pragma dont_inline reset
// (mwcc compiles a pending template instance when it reaches the next function, with that function's pragmas: this
// one makes the constructor above get compiled with inlining on.)
void DinoOv000DataAFlush() {
}
#pragma dont_inline on
MResource<DinoOv000DataB> DinoOv000DataBLoad(std::string path) {
    return MResource<DinoOv000DataB>(path);
}
#pragma dont_inline reset

void DinoOv000DataBFlush() {
}

// (Destroying a group here emits the file structs' implicit destructors, which the game's code in this file calls.)
#pragma dont_inline on
void DinoOv000FileGroupAnchor(DinoBattleFileGroup *group) {
    group->~DinoBattleFileGroup();
}
#pragma dont_inline reset

// The TU's pooled constants.
extern "C" struct {
    u8 unk00[8];
    const char *aiPath;     // 0x08: "/btl_ai/btl_ai_%03d"
} data_ov000_02190fa0;

// The CPU's AI data for the id (none for 0).
extern "C" MResource<DinoOv000DataA> func_ov000_021558a0(int id) {
    if (id == 0) {
        return MResource<DinoOv000DataA>();
    }
    char path[256];
    OS_SPrintf(path, data_ov000_02190fa0.aiPath, id);
    return MResource<DinoOv000DataA>(std::string(path));
}

// (Constructing a group here emits the file structs' implicit constructors.)
#pragma dont_inline on
void DinoOv000FileGroupCtorAnchor(void *p) {
    new (p) DinoBattleFileGroup;
}
#pragma dont_inline reset
