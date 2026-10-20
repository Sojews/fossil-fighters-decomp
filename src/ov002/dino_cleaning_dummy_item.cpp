// DinoCleaningDummyItem.
// Also the cleaning tool menu, its X-ray and tool items.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_resource.h"
#include "game/dino_cleaning_dummy_item_menu.h"
#include "game/dino_cleaning_dummy_item.h"
#include "game/dino_cleaning_xray_item.h"
#include "game/dino_cleaning_tool_menu_item.h"
#include "game/dino_sound.h"
#include "game/dino_common_live.h"
#include "game/dino_cleaning.h"
#include "game/dino_static_init.h"
#include "game/dino_backup.h"

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

void DinoCleaningXrayItem::OnDecide() {
    if (!mCleaning->mUnk128) {
        mMotion->Play(1, 1);
        mCleaning->GetKasekiRock()->StartXray();
    }
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

// Points the commentary at the tool, and lights it up.
void DinoCleaningToolMenuItem::OnFocusChanged(BOOL focused) {
    if (MMenuItem::IsEnabled()) {
        switch (mTool) {
        case 0:
        case 1:
            mCleaning->GetUnkE4()->SetUnkA0(12, focused);
            break;
        case 2:
        case 3:
            mCleaning->GetUnkE4()->SetUnkA0(14, focused);
            break;
        }
        switch (mTool) {
        case 0:
        case 2:
            mCleaning->GetUnkE4()->SetUnkA0(16, FALSE);
            break;
        case 1:
        case 3:
            mCleaning->GetUnkE4()->SetUnkA0(16, TRUE);
            break;
        }
        *mSelected = mTool;
        DinoSoundPlaySe(0x46, 0);
        if (focused) {
            mMotion->Play(2, 0);
            Select();
        } else {
            mMotion->Play(3, 0);
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

#pragma auto_inline off

// Focuses the first tool unless something has the focus.
void DinoCleaningToolMenu::FocusFirst() {
    if (GetFocusedItem() == NULL) {
        FocusItem((MMenuItem *)mTools.front());
    }
}

// Disables every tool and takes the focus away.
void DinoCleaningToolMenu::DisableAll() {
    for (std::vector<void *>::iterator it = mTools.begin(); it != mTools.end(); ++it) {
        ((MMenuItem *)*it)->Disable();
    }
    Deactivate();
    FocusItem(NULL);
}

void DinoCleaningToolMenu::EnableItem() {
    mXrayItem->Enable();
}

void DinoCleaningToolMenu::DisableItem() {
    mXrayItem->Disable();
}

#pragma auto_inline off

DinoCleaningToolMenu::DinoCleaningToolMenu(u32 arg0, u32 arg1, u32 arg2)
    : MMenu(0), mCleaning((DinoCleaning *)arg0), mOam((MSmartOam *)arg1), mSelected((u32 *)arg2) {
}

extern const u32 data_0208ad3c, data_0208ad40, data_0208ad44, data_0208ad48; // the tools' texts

// Makes the items: a dummy that takes the keys, the X-ray, and the tools the player has.
void DinoCleaningToolMenu::OnStart() {
    const char **motions = (const char **)data_ov002_02165418;
    AddItem(new DinoCleaningDummyItem());
    mXrayItem = new DinoCleaningXrayItem(mCleaning, mOam);
    AddItem(mXrayItem);
    MMenuItem *item;
    AddItem(item = new DinoCleaningToolMenuItem(mCleaning, mOam, motions[4], mSelected, data_0208ad48, 2));
    mTools.push_back(item);
    if (gDinoBackup->GetSystemFlag(0x22)) {
        AddItem(item = new DinoCleaningToolMenuItem(mCleaning, mOam, motions[1], mSelected, data_0208ad44, 3));
        mTools.push_back(item);
    }
    AddItem(item = new DinoCleaningToolMenuItem(mCleaning, mOam, motions[5], mSelected, data_0208ad40, 0));
    mTools.push_back(item);
    if (gDinoBackup->GetSystemFlag(0x21)) {
        AddItem(item = new DinoCleaningToolMenuItem(mCleaning, mOam, motions[3], mSelected, data_0208ad3c, 1));
        mTools.push_back(item);
    }
}

// The file's static initialization (see dino_static_init.h).
// NONMATCHING: scheduling. The game hoists the screen rect's zero half (past the pool's 0x80 anchor) above the
// screen size's stores; mwcc treats our pool accesses as one object and keeps them in order.
#pragma always_inline on
static struct DinoCleaningDummyItemStaticInit {
    DinoCleaningDummyItemStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_02165418, 0x18, 0x8c, 0x38, 0x30, 0x7c, 0x9c, 0xa4, 0xac);
    }
} sDinoCleaningDummyItemStaticInit;
