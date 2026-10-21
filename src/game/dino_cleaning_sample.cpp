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
#include "game/dino_weighted_random.h"
#include "m/seed_generator.h"
#include "m/menu.h"
#include "m/input.h"
#include "m/smart_oam.h"
#include "m/oam_canvas.h"
#include "game/dino_common_cursor.h"
#include "game/dino_common_label.h"

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

// A field of the excavation settings (not named in DinoExcavateDefs yet).
static inline u32 DinoExcavateWord(MResource<DinoExcavateDefs> &defs, u32 offset) {
    return *(const u32 *)((const u8 *)defs.Get() + offset);
}

// One of the excavation settings' tables (count at offset, the table's offset after it).
static inline u32 DinoExcavateTableEntry(MResource<DinoExcavateDefs> &defs, u32 offset, u32 i) {
    const u8 *data = (const u8 *)defs.Get();
    return ((const u32 *)(data + *(const u32 *)(data + offset + 4)))[i];
}

// Picks a fossil (and how it is cleaned) at random.
void DinoCleaningSampleMenuRandomItem::OnDecide() {
    MResource<DinoKasekiDefs> kaseki = gDinoResource->LoadKasekiDefs();
    MResource<DinoExcavateDefs> excavate = gDinoResource->GetExcavateDefs();
    MRandom *random = gDinoResource->GetRandom();
    std::vector<void *> ids;   // (u32 ids)
    for (u32 id = 0; id < kaseki.Get()->count; id++) {
        if (*(const u8 *)kaseki.Get()->GetDef(id) != 0 && id >= 1 && id <= 400) {
            ids.push_back(*(void **)&id);
        }
    }
    if ((int)random->Next(100) <= (int)DinoExcavateWord(excavate, 0x20)) {
        DinoWeightedRandom weights(random);
        const u8 *data = (const u8 *)excavate.Get();
        weights.Add(((const u32 *)(data + *(const u32 *)(data + 0x30)))[2]);
        data = (const u8 *)excavate.Get();
        weights.Add(((const u32 *)(data + *(const u32 *)(data + 0x30)))[0]);
        data = (const u8 *)excavate.Get();
        weights.Add(((const u32 *)(data + *(const u32 *)(data + 0x30)))[1]);
        *mUnk94 = 1;
        switch (weights.Pick()) {
        case 0:
            *mUnk90 = 1;
            *mKaseki = (u32)ids[random->Next(ids.size())];
            break;
        case 1:
            *mUnk90 = 0;
            *mKaseki = DinoExcavateTableEntry(excavate, 0x40, random->Next(DinoExcavateWord(excavate, 0x40)));
            break;
        case 2:
            *mUnk90 = 0;
            *mKaseki = DinoExcavateTableEntry(excavate, 0x48, random->Next(DinoExcavateWord(excavate, 0x48)));
            break;
        }
    } else if ((int)random->Next(100) >= 10) {
        *mKaseki = (u32)ids[random->Next(ids.size())];
        *mUnk94 = gDinoResource->GetCreatureUnk0D((*mKaseki - 1) / 4 + 1);
        if (random->Next() <= DinoExcavateWord(excavate, 0x24)) {
            *mUnk90 = 1;
        } else {
            *mUnk90 = 0;
        }
    } else {
        *mUnk90 = 0;
        *mKaseki = DinoExcavateTableEntry(excavate, 0x40, random->Next(DinoExcavateWord(excavate, 0x40)));
        *mUnk94 = 0;
    }
    GetMenu()->mParallel.Suspend();
    GetMenu()->Exit();
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
    u8 unk00[0x4];
    const char *mKasekiDefsPath_04;     // 0x04
    void (DinoCleaningSample::*Unk_08)();
    u8 unk10[0x28 - 0x10];
    void (DinoCleaningSample::*Start_28)();
    u8 unk30[0x40 - 0x30];
    void (DinoCleaningSample::*StateCleaning_40)();
} data_020bcc24;
extern "C" u8 data_0214d158[];

// The menu's labels' text ids.
extern "C" const u32 data_0208a400;    // "<"
extern "C" const u32 data_0208a404;    // ">"
extern "C" const u32 data_0208a3f8;    // random
extern "C" const u32 data_0208a3fc;    // start

inline DinoCleaningSampleMenuDecItem::DinoCleaningSampleMenuDecItem(int *value, int min, int max, MSmartOam *oam,
                                                                    MFont *font, MPoint pos) {
    mValue = value;
    mMin = min;
    mMax = max;
    SetNoFocus(TRUE);
    mView = new DinoCommonLabel(oam, font, gDinoResource->GetUnkTable(), data_0208a400, MPoint(0, 0));
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(pos.x - half.x, pos.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
}

inline DinoCleaningSampleMenuIncItem::DinoCleaningSampleMenuIncItem(int *value, int min, int max, MSmartOam *oam,
                                                                    MFont *font, MPoint pos) {
    mValue = value;
    mMin = min;
    mMax = max;
    SetNoFocus(TRUE);
    mView = new DinoCommonLabel(oam, font, gDinoResource->GetUnkTable(), data_0208a404, MPoint(0, 0));
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(pos.x - half.x, pos.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
}

inline DinoCleaningSampleMenuKasekiRockIdItem::DinoCleaningSampleMenuKasekiRockIdItem(MMenu *menu, MSmartOam *oam,
                                                                                      MFont *font, u32 *value,
                                                                                      MPoint pos)
    : mValue(value), mDefs(std::string(data_020bcc24.mKasekiDefsPath_04)) {
    mView = new MIndicator(new MSmartObjCanvas(oam, MPoint(0x50, 0x10), gDinoResource->GetUnkTable(), TRUE), font, 0);
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(pos.x - half.x, pos.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
    mBuffer = new u32[mDefs.Get()->count];
    mCount = 0;
    mIndex = 0;
    for (u32 i = 0; i < mDefs.Get()->count; i++) {
        if (*(const u8 *)mDefs.Get()->GetDef(i) != 0) {
            if (*mValue == i) {
                mIndex = mCount;
            }
            mBuffer[mCount++] = i;
        }
    }
    mKeyItemA = new DinoCleaningSampleMenuDecItem((int *)&mIndex, 0, mCount - 1, oam, font, MPoint(pos.x - 0x40, pos.y));
    menu->AddItem(mKeyItemA);
    mKeyItemB = new DinoCleaningSampleMenuIncItem((int *)&mIndex, 0, mCount - 1, oam, font, MPoint(pos.x + 0x40, pos.y));
    menu->AddItem(mKeyItemB);
}

inline DinoCleaningSampleKasekiTypeItem::DinoCleaningSampleKasekiTypeItem(MMenu *menu, MSmartOam *oam, MFont *font,
                                                                          u32 *type, MPoint pos) {
    mType = type;
    mView = new MIndicator(new MSmartObjCanvas(oam, MPoint(0x50, 0x10), gDinoResource->GetUnkTable(), TRUE), font, 0);
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(pos.x - half.x, pos.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
    mKeyItemA = new DinoCleaningSampleMenuDecItem((int *)mType, 0, 1, oam, font, MPoint(pos.x - 0x40, pos.y));
    menu->AddItem(mKeyItemA);
    mKeyItemB = new DinoCleaningSampleMenuIncItem((int *)mType, 0, 1, oam, font, MPoint(pos.x + 0x40, pos.y));
    menu->AddItem(mKeyItemB);
}

inline DinoCleaningSampleChipsTypeItem::DinoCleaningSampleChipsTypeItem(MMenu *menu, MSmartOam *oam, MFont *font,
                                                                        u32 *type, MPoint pos) {
    mType = type;
    mView = new MIndicator(new MSmartObjCanvas(oam, MPoint(0x50, 0x10), gDinoResource->GetUnkTable(), TRUE), font, 0);
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(pos.x - half.x, pos.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
    mKeyItemA = new DinoCleaningSampleMenuDecItem((int *)mType, 0, 6, oam, font, MPoint(pos.x - 0x40, pos.y));
    menu->AddItem(mKeyItemA);
    mKeyItemB = new DinoCleaningSampleMenuIncItem((int *)mType, 0, 6, oam, font, MPoint(pos.x + 0x40, pos.y));
    menu->AddItem(mKeyItemB);
}

inline DinoCleaningSampleMenuRandomItem::DinoCleaningSampleMenuRandomItem(MSmartOam *oam, MFont *font, u32 *kaseki,
                                                                          u32 *unk90, u32 *unk94, MPoint pos) {
    mKaseki = kaseki;
    mUnk90 = unk90;
    mUnk94 = unk94;
    SetKeys(0x100);
    mView = new DinoCommonLabel(oam, font, gDinoResource->GetUnkTable(), data_0208a3f8, MPoint(0, 0));
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(pos.x - half.x, pos.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
}

inline DinoCleaningSampleMenuStartItem::DinoCleaningSampleMenuStartItem(MSmartOam *oam, MFont *font, MPoint pos) {
    SetKeys(8);
    mView = new DinoCommonLabel(oam, font, gDinoResource->GetUnkTable(), data_0208a3fc, MPoint(0, 0));
    MPoint half = mView->GetSize() / 2;
    mView->SetPosition(MPoint(pos.x - half.x, pos.y - half.y));
    mView->Show();
    mParallel.AddTail(mView);
}

// The debug cleaning menu: the rock (by id), its type and the chips' type, a random pick, and start.
// NONMATCHING: the game inlines the items' constructors with their own inline calls (MPoint, the resource, the
// key items' constructors), which needs `#pragma ipa file` (this file then breaks its destructors). Under ipa only each
// centering's two MPoint temporaries swap stack slots.
void DinoCleaningSampleMenu::OnStart() {
    mOam = new MSmartOam(0x50, 0, 1);
    GetInput()->SetRepeat(0x1030, 0x14, 2);
    mFont = gDinoResource->CreateFont16(0, 10, 0);
    mFont->AddRef();
    AddItem(new DinoCleaningSampleMenuKasekiRockIdItem(this, mOam, mFont, mRock, MPoint(0x80, 0x40)));
    AddItem(new DinoCleaningSampleKasekiTypeItem(this, mOam, mFont, mUnkAC, MPoint(0x80, 0x58)));
    AddItem(new DinoCleaningSampleChipsTypeItem(this, mOam, mFont, mUnkB0, MPoint(0x80, 0x70)));
    AddItem(new DinoCleaningSampleMenuRandomItem(mOam, mFont, mRock, mUnkAC, mUnkB0, MPoint(0x80, 0x88)));
    AddItem(new DinoCleaningSampleMenuStartItem(mOam, mFont, MPoint(0x80, 0xa0)));
    mTasks = new MParallelTaskSet();
    mCursor = new DinoCommonCursor(mOam);
    GetParallel()->AddTail(mTasks, mCursor, mOam);
    FocusNearestItem(MPoint(0, 0));
}


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
// (So are the random item's vector members, which the game calls out of line.)
#pragma auto_inline off
template class std::__vec_deleter<void *, std::allocator<void *> &>;
template class std::__vec_deleter<void *, std::allocator<void *> >;
#pragma auto_inline reset
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
