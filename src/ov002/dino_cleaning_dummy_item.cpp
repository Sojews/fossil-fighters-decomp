// DinoCleaningDummyItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_dummy_item_menu.h"
#include "game/dino_cleaning_dummy_item.h"
#include "game/dino_cleaning_xray_item.h"
#include "game/dino_cleaning_tool_menu_item.h"

DinoCleaningDummyItem::~DinoCleaningDummyItem() {
}

DinoCleaningXrayItem::~DinoCleaningXrayItem() {
}

DinoCleaningToolMenuItem::~DinoCleaningToolMenuItem() {
}

DinoCleaningToolMenu::~DinoCleaningToolMenu() {
}

void DinoCleaningDummyItem::OnDecide() {
}

const char *DinoCleaningToolMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}
