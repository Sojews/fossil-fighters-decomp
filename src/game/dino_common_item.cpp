// DinoCommonItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_common_item_menu.h"
#include "game/dino_common_item.h"

DinoCommonItem::~DinoCommonItem() {
}

MRect DinoCommonItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoCommonItem::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoCommonItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoCommonItem::OnDecide() {
}
