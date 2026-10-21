// DinoUiDebugKasekiRockGet, DinoUiDebugKasekiRockGetMenuDecItem, DinoUiDebugKasekiRockGetMenuIncItem, DinoUiDebugKasekiRockGetKasekiTypeItem, DinoUiDebugKasekiRockGetChipsTypeItem, DinoUiDebugKasekiRockGetMenuStartItem.

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
#include "game/dino_common_label.h"
#include "game/dino_common_cursor.h"
#include "m/smart_oam.h"
#include "m/input.h"
#include "m/oam_canvas.h"

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

extern "C" const u32 data_0208a400; // "-"
extern "C" const u32 data_0208a404; // "+"
extern "C" const u32 data_0208acbc; // "back"
extern "C" const u32 data_0208acd4; // "start"

inline DinoUiDebugKasekiRockGetMenuDecItem::DinoUiDebugKasekiRockGetMenuDecItem(int *value, int min, int max, MSmartOam *oam, MFont *font, MPoint center) {
    mValue = value;
    mMin = min;
    mMax = max;
    SetNoFocus(TRUE);
    mView = new DinoCommonLabel(oam, font, gDinoResource->GetUnkTable(), data_0208a400, MPoint(0, 0));
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(center.x - half.x, center.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
}

inline DinoUiDebugKasekiRockGetMenuIncItem::DinoUiDebugKasekiRockGetMenuIncItem(int *value, int min, int max, MSmartOam *oam, MFont *font, MPoint center) {
    mValue = value;
    mMin = min;
    mMax = max;
    SetNoFocus(TRUE);
    mView = new DinoCommonLabel(oam, font, gDinoResource->GetUnkTable(), data_0208a404, MPoint(0, 0));
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(center.x - half.x, center.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
}

inline DinoUiDebugKasekiRockGetMenuKasekiRockIdItem::DinoUiDebugKasekiRockGetMenuKasekiRockIdItem(MMenu *menu, u32 *value, MSmartOam *oam, MFont *font, MPoint center)
    : mValue(value), mDefs(std::string(data_ov004_02173580.mDefsPath)) {
    mView = new MIndicator(new MSmartObjCanvas(oam, MPoint(0x50, 0x10), gDinoResource->GetUnkTable(), TRUE), font, 0);
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(center.x - half.x, center.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
    mBuffer = new u32[mDefs.Get()->count];
    mCount = 0;
    mIndex = 0;
    for (u32 i = 0; i < mDefs.Get()->count; i++) {
        if (*(const u8 *)mDefs.Get()->GetDef(i)) {
            if (*mValue == i) {
                mIndex = mCount;
            }
            mBuffer[mCount++] = i;
        }
    }
    mKeyItemA = new DinoUiDebugKasekiRockGetMenuDecItem((int *)&mIndex, 0, mCount - 1, oam, font, MPoint(center.x - 0x40, center.y));
    menu->AddItem(mKeyItemA);
    mKeyItemB = new DinoUiDebugKasekiRockGetMenuIncItem((int *)&mIndex, 0, mCount - 1, oam, font, MPoint(center.x + 0x40, center.y));
    menu->AddItem(mKeyItemB);
}

inline DinoUiDebugKasekiRockGetKasekiTypeItem::DinoUiDebugKasekiRockGetKasekiTypeItem(MMenu *menu, u32 *type, MSmartOam *oam, MFont *font, MPoint center) {
    mType = type;
    mView = new MIndicator(new MSmartObjCanvas(oam, MPoint(0x50, 0x10), gDinoResource->GetUnkTable(), TRUE), font, 0);
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(center.x - half.x, center.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
    mKeyItemA = new DinoUiDebugKasekiRockGetMenuDecItem((int *)mType, 0, 1, oam, font, MPoint(center.x - 0x40, center.y));
    menu->AddItem(mKeyItemA);
    mKeyItemB = new DinoUiDebugKasekiRockGetMenuIncItem((int *)mType, 0, 1, oam, font, MPoint(center.x + 0x40, center.y));
    menu->AddItem(mKeyItemB);
}

inline DinoUiDebugKasekiRockGetChipsTypeItem::DinoUiDebugKasekiRockGetChipsTypeItem(MMenu *menu, u32 *type, MSmartOam *oam, MFont *font, MPoint center) {
    mType = type;
    mView = new MIndicator(new MSmartObjCanvas(oam, MPoint(0x50, 0x10), gDinoResource->GetUnkTable(), TRUE), font, 0);
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(center.x - half.x, center.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
    mKeyItemA = new DinoUiDebugKasekiRockGetMenuDecItem((int *)mType, 0, 6, oam, font, MPoint(center.x - 0x40, center.y));
    menu->AddItem(mKeyItemA);
    mKeyItemB = new DinoUiDebugKasekiRockGetMenuIncItem((int *)mType, 0, 6, oam, font, MPoint(center.x + 0x40, center.y));
    menu->AddItem(mKeyItemB);
}

inline DinoUiDebugKasekiRockGetMenuStartItem::DinoUiDebugKasekiRockGetMenuStartItem(MSmartOam *oam, MFont *font, u32 textId, MPoint center) {
    SetKeys(8);
    mView = new DinoCommonLabel(oam, font, gDinoResource->GetUnkTable(), textId, MPoint(0, 0));
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(center.x - half.x, center.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
}

// The rock id (only the ids of fossils that exist), fossil type and chips type, each with - and + items, then start
// and back.
// NONMATCHING: register and stack slot choices in the item ctors (the centring halves, the type items' oam).
#pragma inline_max_size(2000)
void DinoUiDebugKasekiRockGetMenu::OnStart() {
    mKaseki = 1;
    mUnkAC = 0;
    mUnkB0 = 0;
    mOam = new MSmartOam(0x50, 0, TRUE);
    GetInput()->SetRepeat(0x1030, 0x14, 2);
    mFont = gDinoResource->CreateFont16(0, 10, 0);
    mFont->AddRef();
    AddItem(new DinoUiDebugKasekiRockGetMenuKasekiRockIdItem(this, &mKaseki, mOam, mFont, MPoint(0x80, 0x20)));
    AddItem(new DinoUiDebugKasekiRockGetKasekiTypeItem(this, &mUnkAC, mOam, mFont, MPoint(0x80, 0x40)));
    AddItem(new DinoUiDebugKasekiRockGetChipsTypeItem(this, &mUnkB0, mOam, mFont, MPoint(0x80, 0x60)));
    mStartItem = new DinoUiDebugKasekiRockGetMenuStartItem(mOam, mFont, data_0208acd4, MPoint(0x80, 0x80));
    AddItem(mStartItem);
    mBackItem = new DinoUiDebugKasekiRockGetMenuStartItem(mOam, mFont, data_0208acbc, MPoint(0x80, 0xA0));
    mBackItem->SetKeys(2);
    AddItem(mBackItem);
    mTasks = new MParallelTaskSet;
    mCursor = new DinoCommonCursor(mOam);
    mParallel.AddTail(mTasks, mCursor, mOam);
    FocusNearestItem(MPoint(0, 0));
}
#pragma inline_max_size reset

void DinoUiDebugKasekiRockGet::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiDebugKasekiRockGetMenu();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}

DinoUiDebugKasekiRockGet::DinoUiDebugKasekiRockGet() : MProcess(NULL) {
}

// (Off at the end of the file, where template members are instantiated: the game calls them out of line.)
#pragma auto_inline off
