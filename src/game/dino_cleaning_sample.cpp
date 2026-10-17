// DinoCleaningSample, DinoCleaningSampleMenuDecItem, DinoCleaningSampleMenuIncItem, DinoCleaningSampleKasekiTypeItem, DinoCleaningSampleChipsTypeItem, DinoCleaningSampleMenuStartItem, DinoCleaningSampleMenuRandomItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

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
