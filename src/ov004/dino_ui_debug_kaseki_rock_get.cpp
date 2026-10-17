// DinoUiDebugKasekiRockGet, DinoUiDebugKasekiRockGetMenuDecItem, DinoUiDebugKasekiRockGetMenuIncItem, DinoUiDebugKasekiRockGetKasekiTypeItem, DinoUiDebugKasekiRockGetChipsTypeItem, DinoUiDebugKasekiRockGetMenuStartItem.
// Only the destructors and empty overrides are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_resource.h"
#include <printf.h>
#include "m/font.h"
#include "game/dino_ui_debug_kaseki_rock_get_menu_menu.h"
#include "game/dino_ui_debug_kaseki_rock_get.h"
#include "game/dino_ui_debug_kaseki_rock_get_menu_dec_item.h"
#include "game/dino_ui_debug_kaseki_rock_get_menu_inc_item.h"
#include "game/dino_ui_debug_kaseki_rock_get_kaseki_type_item.h"
#include "game/dino_ui_debug_kaseki_rock_get_chips_type_item.h"
#include "game/dino_ui_debug_kaseki_rock_get_menu_start_item.h"

DinoUiDebugKasekiRockGetMenu::~DinoUiDebugKasekiRockGetMenu() {
    mFont->Release();
}

DinoUiDebugKasekiRockGetMenuKasekiRockIdItem::~DinoUiDebugKasekiRockGetMenuKasekiRockIdItem() {
    delete[] mBuffer;
}

void DinoUiDebugKasekiRockGetMenuKasekiRockIdItem::OnDecide() {
}

void DinoUiDebugKasekiRockGetMenuStartItem::OnDecide() {
}

void DinoUiDebugKasekiRockGetChipsTypeItem::OnDecide() {
}

void DinoUiDebugKasekiRockGetKasekiTypeItem::OnDecide() {
}

DinoUiDebugKasekiRockGet::~DinoUiDebugKasekiRockGet() {
}

DinoUiDebugKasekiRockGetMenuDecItem::~DinoUiDebugKasekiRockGetMenuDecItem() {
}

DinoUiDebugKasekiRockGetMenuIncItem::~DinoUiDebugKasekiRockGetMenuIncItem() {
}

DinoUiDebugKasekiRockGetKasekiTypeItem::~DinoUiDebugKasekiRockGetKasekiTypeItem() {
}

DinoUiDebugKasekiRockGetChipsTypeItem::~DinoUiDebugKasekiRockGetChipsTypeItem() {
}

DinoUiDebugKasekiRockGetMenuStartItem::~DinoUiDebugKasekiRockGetMenuStartItem() {
}

void DinoUiDebugKasekiRockGetChipsTypeItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mKeyItemA->SetKeys(0x20);
        mKeyItemB->SetKeys(0x10);
    } else {
        mKeyItemA->SetKeys(0);
        mKeyItemB->SetKeys(0);
    }
}

void DinoUiDebugKasekiRockGetKasekiTypeItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mKeyItemA->SetKeys(0x20);
        mKeyItemB->SetKeys(0x10);
    } else {
        mKeyItemA->SetKeys(0);
        mKeyItemB->SetKeys(0);
    }
}

void DinoUiDebugKasekiRockGetMenuKasekiRockIdItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mKeyItemA->SetKeys(0x20);
        mKeyItemB->SetKeys(0x10);
    } else {
        mKeyItemA->SetKeys(0);
        mKeyItemB->SetKeys(0);
    }
}

void DinoUiDebugKasekiRockGetMenuIncItem::OnDecide() {
    if (*mValue == mMax) {
        *mValue = mMin;
    } else {
        *mValue = *mValue + 1;
    }
}

void DinoUiDebugKasekiRockGetMenuDecItem::OnDecide() {
    if (*mValue == mMin) {
        *mValue = mMax;
    } else {
        *mValue = *mValue - 1;
    }
}

MRect DinoUiDebugKasekiRockGetMenuStartItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoUiDebugKasekiRockGetChipsTypeItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoUiDebugKasekiRockGetKasekiTypeItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoUiDebugKasekiRockGetMenuKasekiRockIdItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoUiDebugKasekiRockGetMenuIncItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoUiDebugKasekiRockGetMenuDecItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

extern const u32 data_0208a408; // the rock id's format
void DinoUiDebugKasekiRockGetMenuKasekiRockIdItem::Idle() {
    *mValue = mBuffer[mIndex];
    char text[0x100];
    sprintf(text, gDinoResource->GetText(data_0208a408), *mValue);
    mView->SetText(std::string(text));
}
