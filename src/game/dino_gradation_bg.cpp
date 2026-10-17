// DinoGradationBg.
// Only the destructor is decompiled so far.

#include "game/dino_gradation_bg.h"
#include "m/display.h"

DinoGradationBg::~DinoGradationBg() {
    delete mRaster;
    mDisplay->SetBackdrop(0);
}

void DinoGradationBg::Execute() {
}
