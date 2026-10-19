// DinoUiDebugKasekiRockGet, DinoUiDebugKasekiRockGetMenuDecItem, DinoUiDebugKasekiRockGetMenuIncItem, DinoUiDebugKasekiRockGetKasekiTypeItem, DinoUiDebugKasekiRockGetChipsTypeItem, DinoUiDebugKasekiRockGetMenuStartItem.
// Only the destructors and empty overrides are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_resource.h"
#include "game/dino_backup.h"
#include <printf.h>
#include "m/font.h"
#include "m/indicator.h"
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

// The type names' text ids: function-local statics filled in on first use, as in DinoCleaningSample (see there).
extern "C" u32 data_ov004_02177288;
extern "C" u32 data_ov004_0217728c;
extern "C" u32 data_ov004_02173588[2];
extern "C" u32 data_ov004_02173658[7];
extern "C" const u32 data_0208a3d4, data_0208a3d8, data_0208a3dc, data_0208a3e0, data_0208a3e4, data_0208a3e8,
    data_0208a3ec, data_0208a3f0, data_0208a3f4;

void DinoUiDebugKasekiRockGetChipsTypeItem::Idle() {
    if (!(data_ov004_0217728c & 1)) {
        data_ov004_02173658[0] = data_0208a3ec;
        data_ov004_02173658[1] = data_0208a3e8;
        data_ov004_02173658[2] = data_0208a3e4;
        data_ov004_02173658[3] = data_0208a3e0;
        data_ov004_02173658[4] = data_0208a3dc;
        data_ov004_02173658[5] = data_0208a3d8;
        data_ov004_02173658[6] = data_0208a3d4;
        data_ov004_0217728c |= 1;
    }
    mView->SetText(gDinoResource->GetText(data_ov004_02173658[*mType]));
}

void DinoUiDebugKasekiRockGetKasekiTypeItem::Idle() {
    if (!(data_ov004_02177288 & 1)) {
        data_ov004_02173588[0] = data_0208a3f4;
        data_ov004_02173588[1] = data_0208a3f0;
        data_ov004_02177288 |= 1;
    }
    mView->SetText(gDinoResource->GetText(data_ov004_02173588[*mType]));
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

extern "C" struct {
    u8 unk00[4];
    const char *mDefsPath;  // 0x04: "/etc/kaseki_defs"
} data_ov004_02173580;

// Start adds the chosen fossil rock (if there's room); back leaves.
void DinoUiDebugKasekiRockGetMenu::OnDecide(MMenuItem *item) {
    if (item == mBackItem) {
        Exit();
        return;
    }
    if (item == mStartItem) {
        if (!gDinoBackup->IsUnk98Full()) {
            MResource<DinoKasekiDefs> defs(std::string(data_ov004_02173580.mDefsPath));
            DinoBackupEntry entry;
            MI_CpuFill8(&entry, 0, sizeof(entry));
            entry.id = mKaseki;
            entry.type = defs.Get()->GetDef(mKaseki)->unk24;
            entry.unkC_12 = mUnkAC;
            entry.unk10_0 = mUnkB0;
            if (defs.Get()->GetDef(mKaseki)->IsUncleanable()) {
                entry.unk10_3 = TRUE;
            }
            gDinoBackup->SetUnk98(gDinoBackup->FindFreeUnk98(), &entry);
            gDinoBackup->CompactUnk98();
        }
        Exit();
    }
}

void DinoUiDebugKasekiRockGetMenu::OnFocus(MMenuItem *item) {
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(item->GetFrame());
    } else {
        mTasks->Clear();
        mTasks->AddTail(mCursor->CreateFrameAnime(mCursor->GetFrame(), item->GetFrame(), 5, -2));
    }
}

void DinoUiDebugKasekiRockGet::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiDebugKasekiRockGetMenu();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

DinoUiDebugKasekiRockGet::DinoUiDebugKasekiRockGet() : MProcess(NULL) {
}
