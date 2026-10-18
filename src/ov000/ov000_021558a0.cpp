// Ov000021558a0.
// Only the destructors are decompiled so far.

#include "types.h"
#include "m/resource.h"

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
