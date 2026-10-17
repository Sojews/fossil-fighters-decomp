// DinoCleaningSample, DinoCleaningSampleMenuDecItem, DinoCleaningSampleMenuIncItem, DinoCleaningSampleKasekiTypeItem, DinoCleaningSampleChipsTypeItem, DinoCleaningSampleMenuStartItem, DinoCleaningSampleMenuRandomItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_resource.h"
#include <printf.h>
#include "game/dino_cleaning_sample_menu.h"
#include "game/dino_cleaning_sample.h"
#include "game/dino_cleaning_sample_menu_dec_item.h"
#include "game/dino_cleaning_sample_menu_inc_item.h"
#include "game/dino_cleaning_sample_kaseki_type_item.h"
#include "game/dino_cleaning_sample_chips_type_item.h"
#include "game/dino_cleaning_sample_menu_start_item.h"
#include "game/dino_cleaning_sample_menu_random_item.h"

DinoCleaningSample::~DinoCleaningSample() {
}

DinoCleaningSampleMenuDecItem::~DinoCleaningSampleMenuDecItem() {
}

DinoCleaningSampleMenuIncItem::~DinoCleaningSampleMenuIncItem() {
}

DinoCleaningSampleKasekiTypeItem::~DinoCleaningSampleKasekiTypeItem() {
}

DinoCleaningSampleChipsTypeItem::~DinoCleaningSampleChipsTypeItem() {
}

DinoCleaningSampleMenuStartItem::~DinoCleaningSampleMenuStartItem() {
}

DinoCleaningSampleMenuRandomItem::~DinoCleaningSampleMenuRandomItem() {
}

DinoCleaningSampleMenu::~DinoCleaningSampleMenu() {
    mFont->Release();
}

DinoCleaningSampleMenuKasekiRockIdItem::~DinoCleaningSampleMenuKasekiRockIdItem() {
    delete[] mBuffer;
}

void DinoCleaningSampleMenuKasekiRockIdItem::OnDecide() {
}

void DinoCleaningSampleChipsTypeItem::OnDecide() {
}

void DinoCleaningSampleKasekiTypeItem::OnDecide() {
}

void DinoCleaningSample::Idle() {
    (this->*mState)();
}

void DinoCleaningSampleChipsTypeItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mKeyItemA->SetKeys(0x20);
        mKeyItemB->SetKeys(0x10);
    } else {
        mKeyItemA->SetKeys(0);
        mKeyItemB->SetKeys(0);
    }
}

void DinoCleaningSampleKasekiTypeItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mKeyItemA->SetKeys(0x20);
        mKeyItemB->SetKeys(0x10);
    } else {
        mKeyItemA->SetKeys(0);
        mKeyItemB->SetKeys(0);
    }
}

void DinoCleaningSampleMenuKasekiRockIdItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mKeyItemA->SetKeys(0x20);
        mKeyItemB->SetKeys(0x10);
    } else {
        mKeyItemA->SetKeys(0);
        mKeyItemB->SetKeys(0);
    }
}

void DinoCleaningSampleMenuIncItem::OnDecide() {
    if (*mValue == mMax) {
        *mValue = mMin;
    } else {
        *mValue = *mValue + 1;
    }
}

void DinoCleaningSampleMenuDecItem::OnDecide() {
    if (*mValue == mMin) {
        *mValue = mMax;
    } else {
        *mValue = *mValue - 1;
    }
}

void DinoCleaningSampleMenuStartItem::OnDecide() {
    GetMenu()->Exit();
}

MRect DinoCleaningSampleMenuIncItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoCleaningSampleMenuRandomItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoCleaningSampleMenuStartItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoCleaningSampleChipsTypeItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoCleaningSampleKasekiTypeItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoCleaningSampleMenuKasekiRockIdItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

MRect DinoCleaningSampleMenuDecItem::GetFrame() {
    return mView->GetFrame().Inflate(8);
}

extern const u32 data_0208a408; // the rock id's format
void DinoCleaningSampleMenuKasekiRockIdItem::Idle() {
    *mValue = mBuffer[mIndex];
    char text[0x100];
    sprintf(text, gDinoResource->GetText(data_0208a408), *mValue);
    mView->SetText(std::string(text));
}
