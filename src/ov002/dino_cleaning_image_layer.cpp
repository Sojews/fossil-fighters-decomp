// DinoCleaningImageLayer.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_image_layer.h"
#include "types.h"

DinoCleaningImageLayer::~DinoCleaningImageLayer() {
    if (!mShared) {
        delete[] mPixels;
    }
}
