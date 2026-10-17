// DinoUiExchange.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_exchange.h"
#include "types.h"
#include "m/blend.h"

DinoUiExchange::~DinoUiExchange() {
    GetDisplay()->mBlend->Reset();
}
