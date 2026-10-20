// DinoCleaningSample, DinoCleaningSampleMenuDecItem, DinoCleaningSampleMenuIncItem, DinoCleaningSampleKasekiTypeItem, DinoCleaningSampleChipsTypeItem, DinoCleaningSampleMenuStartItem, DinoCleaningSampleMenuRandomItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/bright.h"
#include "m/display.h"
#include "m/indicator.h"
#include "m/lcd.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "game/dino_static_init.h"
#include <printf.h>
#include "game/dino_cleaning_sample_menu.h"
#include "game/dino_cleaning_sample.h"
#include "game/dino_cleaning.h"
#include "m/overlay.h"
#include "nitro/fs.h"
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

// The type names' text ids. In the game they are function-local statics, filled in on first use (the guard words
// are data_0214d150 and data_0214d154); they live in data this unit can't place, so the guard is written out.
extern "C" u32 data_0214d150;
extern "C" u32 data_0214d154;
extern "C" u32 data_020bcd78[7];
extern "C" u32 data_020bcc54[2];
extern "C" const u32 data_0208a3d4, data_0208a3d8, data_0208a3dc, data_0208a3e0, data_0208a3e4, data_0208a3e8,
    data_0208a3ec, data_0208a3f0, data_0208a3f4;

void DinoCleaningSampleChipsTypeItem::Idle() {
    if (!(data_0214d150 & 1)) {
        data_020bcd78[0] = data_0208a3ec;
        data_020bcd78[1] = data_0208a3e8;
        data_020bcd78[2] = data_0208a3e4;
        data_020bcd78[3] = data_0208a3e0;
        data_020bcd78[4] = data_0208a3dc;
        data_020bcd78[5] = data_0208a3d8;
        data_020bcd78[6] = data_0208a3d4;
        data_0214d150 |= 1;
    }
    mView->SetText(gDinoResource->GetText(data_020bcd78[*mType]));
}

void DinoCleaningSampleKasekiTypeItem::Idle() {
    if (!(data_0214d154 & 1)) {
        data_020bcc54[0] = data_0208a3f4;
        data_020bcc54[1] = data_0208a3f0;
        data_0214d154 |= 1;
    }
    mView->SetText(gDinoResource->GetText(data_020bcc54[*mType]));
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

void DinoCleaningSampleMenu::OnFocus(MMenuItem *item) {
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(item->GetFrame());
    } else {
        mTasks->Clear();
        mTasks->AddTail(mCursor->CreateFrameAnime(mCursor->GetFrame(), item->GetFrame(), 5, -2));
    }
}

M_EXTERN_OVERLAY(ov002);

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x8];
    void (DinoCleaningSample::*Unk_08)();
    u8 unk10[0x28 - 0x10];
    void (DinoCleaningSample::*Start_28)();
    u8 unk30[0x40 - 0x30];
    void (DinoCleaningSample::*StateCleaning_40)();
} data_020bcc24;
extern "C" u8 data_0214d158[];

DinoCleaningSample::DinoCleaningSample() : MProcess(NULL) {
    mState = data_020bcc24.Start_28;
    mUnk7C = 0;
    mUnk80 = 0;
    mUnk84 = 0;
    FS_LoadOverlay(MI_PROCESSOR_ARM9, M_OVERLAY_ID(ov002));
}

void DinoCleaningSample::StateCleaning() {
    mSerial.AddTail(new DinoCleaning(mUnk7C, data_0214d158, mUnk80, mUnk84, TRUE, 0));
    mState = data_020bcc24.Unk_08;
}

void DinoCleaningSample::StateMenu() {
    gLcd->mMain->mBright->Reset();
    gLcd->mSub->mBright->Reset();
    gDinoBackup->SetSystemFlag(0x21, TRUE);
    gDinoBackup->SetSystemFlag(0x22, TRUE);
    mSerial.AddTail(new DinoCleaningSampleMenu(&mUnk7C, &mUnk80, &mUnk84));
    mState = data_020bcc24.StateCleaning_40;
}

// The file's static initialization (see dino_static_init.h): the common layout header.
// NONMATCHING: the game stores the screen rect's zero half before the screen size.
// (The menu links' tree destructor is instantiated first, so that always_inline doesn't reach it.)
typedef std::__red_black_tree<MMenuItem *, std::pair<MMenuItem *const, std::pair<int, MMenuItem *> >,
                              std::__select_first<std::pair<MMenuItem *const, std::pair<int, MMenuItem *> > >,
                              std::less<MMenuItem *>, std::allocator<std::pair<MMenuItem *const, std::pair<int, MMenuItem *> > > >
    DinoCleaningSampleLinkTree;
template DinoCleaningSampleLinkTree::~__red_black_tree();
template void DinoCleaningSampleLinkTree::destroy(DinoCleaningSampleLinkTree::node_base *);
#pragma always_inline on

static struct DinoCleaningSampleStaticInit {
    DinoCleaningSampleStaticInit() {
        DINO_POOL(MPoint, data_020bcc24, 0x10) = MPoint(256, 192);
        DINO_POOL(MRect, data_020bcc24, 0x134) = DinoSizeRect(DINO_POOL(MPoint, data_020bcc24, 0x10));
        DINO_POOL(MPoint, data_020bcc24, 0x20) = MPoint(2, 2);
        DINO_POOL(MPoint, data_020bcc24, 0x18) = MPoint(36, 28);
        DINO_POOL(MRect, data_020bcc24, 0x144) = DinoSizeRect(DINO_POOL(MPoint, data_020bcc24, 0x18));
        DINO_POOL(MPoint, data_020bcc24, 0x124) = MPoint(0, 24);
        DINO_POOL(MPoint, data_020bcc24, 0x12c) = MPoint(256, 192);
        DINO_POOL(MRect, data_020bcc24, 0x114) = MRect(0, 0, 256, 24);
    }
} sDinoCleaningSampleStaticInit;
