// DinoCleaningDummyItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_resource.h"
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

MRect DinoCleaningToolMenuItem::GetRect() {
    return GetFrame();
}

MRect DinoCleaningXrayItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoCleaningToolMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoCleaningXrayItem::GetDisplay() {
    return MProcess::GetDisplay();
}

void DinoCleaningXrayItem::OnEnableChanged(BOOL enabled) {
    mMotion->Play(2, 0);
}

void DinoCleaningToolMenuItem::OnEnableChanged(BOOL enabled) {
    if (!enabled) {
        if (IsFocused()) {
            mMotion->Play(5, 0);
        } else {
            mMotion->Play(4, 0);
        }
    }
}

void DinoCleaningToolMenuItem::OnDecide() {
    Focus();
}

MRect DinoCleaningDummyItem::GetFrame() {
    return MRect(256, 192, 256, 192);
}

extern const u32 data_0208ad38; // a text number
const char *DinoCleaningXrayItem::GetText() {
    return gDinoResource->GetText(data_0208ad38);
}

MRect DinoCleaningToolMenuItem::GetFrame() {
    return mMotion->GetFrame().Intersection(MRect(0, 0, 256, 192));
}

MRect DinoCleaningXrayItem::GetFrame() {
    return mMotion->GetFrame().Intersection(MRect(0, 0, 256, 192));
}
