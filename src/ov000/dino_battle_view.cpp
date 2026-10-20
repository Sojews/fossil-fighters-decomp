// DinoBattleView, EnableRootSelectTask.

#include "types.h"
#include "m/resource.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/enable_root_select_task.h"
#include "game/dino_static_init.h"
#include "game/dino_battle.h"
#include "game/dino_battle_view_field.h"
#include "game/dino_battle_view_3d.h"
#include "game/dino_battle_view_kaseki_point.h"
#include "game/camera_wipe.h"
#include "game/dino_battle2.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "m/smart_oam.h"

#pragma auto_inline off

// The fonts (and the pooled constants) the view loads.
extern "C" const char data_ov000_0219158c[];
extern "C" const char data_ov000_021915a4[];
extern "C" const char data_ov000_021915bc[];
extern "C" const char data_ov000_021915d4[];
extern "C" struct {
    u32 unk00;
    const char *mDataPath;  // 0x04
} data_ov000_021914a0;

// A side's kaseki point counter.
static inline DinoBattleViewKasekiPoint *KasekiPoint(DinoBattleViewPlayer *player) {
    return player->GetKasekiPoint();
}

// Loads the fonts and the view's data and makes the screens' parts: the field, both players and (when the
// battle settings ask for it) the 3D view.
// NONMATCHING: the last loop's cursor and slot index swap r2/r4. (The report also docks the MResource
// constructor below it by 0.07%, though it matches on its own.)
DinoBattleView::DinoBattleView(const DinoBattleParams *params) : MProcess(NULL) {
    mUnk13C = 0;
    mUnk130 = params->unk2C;
    mUnk134 = params->unk154;
    mUnk118 = new MFontSet(std::string(data_ov000_0219158c), 0, 0, 0);
    mUnk11C = new MFontSet(std::string(data_ov000_021915a4), 0, 0, 0);
    mUnk120 = new MFontSet(std::string(data_ov000_021915bc), 0, 0, 0);
    mUnk124 = new MFontSet(std::string(data_ov000_021915d4), 0, 0, 0);
    mUnk118->AddRef();
    mUnk11C->AddRef();
    mUnk120->AddRef();
    mUnk124->AddRef();
    mUnk138 = params->unk00 == 2;
    mUnk140 = params->unk160;
    mData = MResource<DinoBattleViewData>(std::string(data_ov000_021914a0.mDataPath));
    mUnk10C = new MSmartOam(32, 17, FALSE);
    mUnk10C->SetUnk1C(1);
    mUnk104 = new DinoBattleViewActionInfoBar((u32)mUnk10C);
    mUnk108 = new DinoBattleViewFocusLCDFrame((u32)mUnk10C);
    mUnkE0 = (DinoBattleViewUnkE0 *)new DinoBattleViewField(this, mUnk10C, params->mTutorialId);
    mSubject.AddObserver(mUnkE0);
    for (u32 i = 0; i < 2; i++) {
        u8 flag = FALSE;
        switch ((&params->unk08)[i]) {
        case 2:
            if (params->unk164) {
                flag = TRUE;
            }
            break;
        case 0:
        case 1:
            if (gDinoBackup->GetSystemFlag(0x28)) {
                flag = TRUE;
            }
            break;
        }
        mPlayerOams[i] = new MSmartOam(42, 22, TRUE);
        mPlayerOams[i]->SetUnk1C(7);
        mUnkE4[i] = new DinoBattleViewPlayer(i, (&params->unk10)[i], (u32)mPlayerOams[i], this, flag);
        mSubject.AddObserver(mUnkE4[i]);
    }
    mUnk100 = new KasekiPointPalette(KasekiPoint(mUnkE4[1])->mUnk9C, KasekiPoint(mUnkE4[0])->mUnk9C);
    int i;
    for (i = 0; i < 2; i++) {
        KasekiPointPalette *palette = mUnk100;
        mUnkE4[i]->GetKasekiPoint()->mPalette = palette;
        mUnkE4[i]->GetKasekiPoint()->LoadPalette(0);
    }
    for (i = 0; i < 2; i++) {
        mUnkE0->mSlotViewsC0[(&params->unk10)[i]]->mPlayer = mUnkE4[i];
    }
    GetParallel()->AddTail(mUnkE0, mUnk104, mUnk108, mUnkE4[0], mUnkE4[1], mUnk10C, mPlayerOams[0], mPlayerOams[1]);
    mUnkEC = NULL;
    if (gDinoResource->GetBattleAdjustDefs()->unk170) {
        mUnkEC = new DinoBattleView3D(this);
        mSubject.AddObserver(mUnkEC);
        mParallel.AddTail(mUnkEC);
    }
}

MFont *DinoBattleView::GetUnk11C() {
    return mUnk11C;
}

MFont *DinoBattleView::GetUnk118() {
    return mUnk118;
}

MFont *DinoBattleView::GetUnk120() {
    return mUnk120;
}

MFont *DinoBattleView::GetUnk124() {
    return mUnk124;
}

DinoBattleView::~DinoBattleView() {
    mUnk118->Release();
    mUnk11C->Release();
    mUnk120->Release();
    mUnk124->Release();
    delete mUnk100;
}

const void *DinoBattleView::GetUnk11CPalette() {
    return mUnk11C->GetPalette();
}

DinoBattleViewPlayer *DinoBattleView::GetPlayerByAC(u32 id) {
    if (id == mUnkE4[0]->GetUnkAC()) {
        return mUnkE4[0];
    }
    return mUnkE4[1];
}

int DinoBattleView::GetUnk140() {
    if (GetUnk138()) {
        return -1;
    }
    return mUnk140;
}

DinoBattleViewPlayer *DinoBattleView::GetUnkE4(int index) {
    return mUnkE4[index];
}

u8 DinoBattleView::GetUnk138() {
    return mUnk138;
}

void DinoBattleView::SetUnk128(u32 value) {
    mUnk128 = value;
}

void DinoBattleView::SetUnk12C(u32 value) {
    mUnk12C = value;
}

u32 DinoBattleView::GetUnk12C() {
    return mUnk12C;
}

u32 DinoBattleView::GetUnk130() {
    return mUnk130;
}

u32 DinoBattleView::GetUnk134() {
    return mUnk134;
}

u32 DinoBattleView::GetUnkEC() {
    return (u32)mUnkEC;
}

void DinoBattleView::Idle() {
    Dispatch();
}

void DinoBattleView::OnMessage(MMessage *msg) {
    mSubject.Notify(msg);
}

DinoBattleViewCreature *DinoBattleView::FindCreatureByAC(u32 id) {
    for (int i = 0; i < 2; i++) {
        DinoBattleViewCreature *creature = mUnkE4[i]->FindCreatureByAC(id);
        if (creature != NULL) {
            return creature;
        }
    }
    return NULL;
}

u32 DinoBattleView::GetUnk104() {
    return (u32)mUnk104;
}

u32 DinoBattleView::GetUnk108() {
    return (u32)mUnk108;
}

void DinoBattleView::IncUnk13C() {
    mUnk13C++;
}

void DinoBattleView::DecUnk13C() {
    mUnk13C--;
}

int DinoBattleView::GetUnk13C() {
    return mUnk13C;
}

MTask *DinoBattleView::CreateEnableRootSelectTask() {
    return new EnableRootSelectTask(this);
}

EnableRootSelectTask::~EnableRootSelectTask() {
}

void EnableRootSelectTask::Execute() {
    ((DinoBattleView *)mUnk10)->DecUnk13C();
    Exit();
}

// Resources the file loads (their data types are unknown); until the code that constructs them is decompiled,
// these keep out-of-line copies of the constructors.
struct DinoBattleViewData;
#pragma dont_inline on
MResource<DinoBattleViewData> DinoBattleViewDataLoad(std::string path) {
    return MResource<DinoBattleViewData>(path);
}
#pragma dont_inline reset

// The file's static initialization (see dino_static_init.h).
static struct DinoBattleViewStaticInit {
    DinoBattleViewStaticInit() {
        DINO_POOL_WORD(data_ov000_021914a0, 0x8) = 128;
        DINO_POOL_WORD(data_ov000_021914a0, 0xc) = 176;
        DINO_POOL_WORD(data_ov000_021914a0, 0x20) = 8;
        DINO_POOL_WORD(data_ov000_021914a0, 0x24) = 150;
        DINO_POOL_WORD(data_ov000_021914a0, 0x10) = 64;
        DINO_POOL_WORD(data_ov000_021914a0, 0x14) = 112;
    }
} sDinoBattleViewStaticInit;
