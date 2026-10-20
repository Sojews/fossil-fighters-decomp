// DinoBattleViewAbilityMenuB.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_ability_menu_bg.h"
#include "types.h"
#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/bg.h"
#include "m/text.h"
#include "m/pal.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_view_ui_ability_menu_item.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_battle_help.h"
#include "game/dino_sound.h"
#include "m/indicator.h"
#include <string.h>
#include "game/dino_static_init.h"
#include <string>
#include "m/picture.h"
#include "m/chr.h"
#include "m/lcd.h"
#include "nitro/mi.h"

extern "C" int data_ov000_021978dc;    // live ability menu BGs
extern "C" const u32 data_0208a19c;     // frames before the ability menu takes input

DinoBattleViewAbilityMenuBG::~DinoBattleViewAbilityMenuBG() {
    if (mUnk10D4) {
        mBg->DeleteText(mText);
        mBg->DeleteChr(mChr);
        mBg->mPal->Free(mPalBlock);
    }
    data_ov000_021978dc--;
}

void DinoBattleViewUiAbilityMenuItem::OnDecide() {
}

MRect DinoBattleViewUiAbilityMenuItem::GetFrame() {
    return mUnk98->GetFrame();
}

#pragma auto_inline off

AttackMenuBarMotion::AttackMenuBarMotion(void *owner, MSmartOam *oam, u32 flags) : MProcess(NULL) {
    mOwner = owner;
    mFlags = flags;
    bool flag1 = (flags & 0x10000) ? true : false;
    bool flag2 = (flags & 0x20000) ? true : false;
    Init(oam, (u16)flags, flag1, flag2, false, false);
}

AttackMenuBarMotion::AttackMenuBarMotion(void *owner, MSmartOam *oam, u32 attack, bool flag1, bool flag2, bool flag3, bool flag4)
    : MProcess(NULL) {
    mOwner = owner;
    mFlags = (u16)attack | (flag1 ? 0x10000 : 0) | (flag2 ? 0x20000 : 0);
    Init(oam, attack, flag1, flag2, flag3, flag4);
}

void AttackMenuBarMotion::SetPriority(int priority) {
    mMotion->SetPriority(priority);
    mUnk80->SetPriority(priority);
    mUnk7C->SetPriority(priority);
    if (mUnk78 != NULL) {
        mUnk78->SetPriority(priority);
    }
}

// Pooled file statics: the parts' offsets from the bar.
struct AttackMenuBarStatics {
    u8 unk00[8];
    const char *noCost;     // 0x08
    u8 unk0C[0x14 - 0x0C];
    const char *bgPath;     // 0x14: the ability menu's BG
    u8 unk18[0x20 - 0x18];
    void (DinoBattleViewAbilityMenuBG::*StateHidden_20)();
    u8 unk28[0x48 - 0x28];
    MPoint mUnk7COffset;    // 0x48
    u32 unk50[2];
    MPoint mUnk80Offset;    // 0x58
    int barX;               // 0x60: the bar's x, unfocused
    u8 unk64[0x70 - 0x64];
    void (DinoBattleViewAbilityMenuBG::*StateShow_70)();
};
struct AttackMenuBarStatics2 {
    int barFocusedFlagX;    // 0x00: focused, with mUnk94 set
    u8 unk04[0x18 - 0x04];
    int barFocusedX;        // 0x18: focused (or unfocused with mUnk94 set)
    u32 unk1C;
    MPoint mUnk78Offset;    // 0x20
};
extern "C" AttackMenuBarStatics data_ov000_021947ac;
extern "C" AttackMenuBarStatics2 data_ov000_0219482c;

void AttackMenuBarMotion::SetPosition(MPoint pos) {
    mMotion->SetPosition(pos);
    mUnk80->SetPosition(MPoint(pos.x + data_ov000_021947ac.mUnk80Offset.x, pos.y + data_ov000_021947ac.mUnk80Offset.y));
    mUnk7C->SetPosition(MPoint(pos.x + data_ov000_021947ac.mUnk7COffset.x, pos.y + data_ov000_021947ac.mUnk7COffset.y));
    if (mUnk78 != NULL) {
        mUnk78->SetPosition(MPoint(pos.x + data_ov000_0219482c.mUnk78Offset.x, pos.y + data_ov000_0219482c.mUnk78Offset.y));
    }
}

MPoint AttackMenuBarMotion::GetPosition() {
    return mMotion->GetPosition();
}

void AttackMenuBarMotion::Hide() {
    mMotion->Hide();
    mUnk80->Hide();
    mUnk7C->Hide();
    if (mUnk78 != NULL) {
        mUnk78->Hide();
    }
}

void AttackMenuBarMotion::Show() {
    mMotion->Show();
    mUnk80->Show();
    mUnk7C->Show();
    if (mUnk78 != NULL) {
        mUnk78->Show();
    }
}

MTask *AttackMenuBarMotion::CreateShowTask() {
    return new ShowTask(this);
}

void AttackMenuBarMotion::Play(int anime) {
    mMotion->Play(anime, 0);
}

MRect AttackMenuBarMotion::GetFrame() {
    return mMotion->GetFrame();
}

// NONMATCHING: the game reads the menu's mUnk104 before calling GetUnkB4 (ours calls first), so flag3 gets spilled
DinoBattleViewUiAbilityMenuItem::DinoBattleViewUiAbilityMenuItem(DinoBattleViewUiAbilityMenu *menu, bool flag, bool flag3, u32 attack,
                                                                 u32 kind, u32 unk90) {
    mUnk88 = kind;
    mUnk8C = attack;
    mUnk90 = unk90;
    mUnk94 = flag;
    mMenu = menu;
    mUnk98 = new AttackMenuBarMotion(mMenu->mUnk104, mMenu->GetUnkB4(), mUnk8C, mUnk88 == 2, flag, flag3, false);
    mTasks = new MParallelTaskSet();
    MTask *bar = mUnk98;
    MTaskSet *parallel = &mParallel;
    parallel->AddTail(mTasks);
    parallel->AddTail(bar);
}

void DinoBattleViewUiAbilityMenuItem::SetBarPosition(MPoint pos) {
    mUnk98->SetPosition(pos);
}

MSmartOam *DinoBattleViewUiAbilityMenu::GetUnkB4() {
    return mUnkB4;
}

MPoint DinoBattleViewUiAbilityMenuItem::GetBarPosition() {
    return mUnk98->GetPosition();
}

u32 DinoBattleViewUiAbilityMenuItem::GetUnk8C() {
    return mUnk8C;
}

u8 DinoBattleViewUiAbilityMenuItem::GetUnk94() {
    return mUnk94;
}

u32 DinoBattleViewUiAbilityMenuItem::GetUnk90() {
    return mUnk90;
}

AttackMenuBarMotion *DinoBattleViewUiAbilityMenuItem::GetBar() {
    return mUnk98;
}

MRect DinoBattleViewUiAbilityMenuItem::GetRect() {
    return GetFrame();
}

// Whether the attack hits several creatures: its kind is 8 to 11, or its range is 2 or 9 to 15.
extern "C" BOOL func_ov000_02179aa0(u32 id) {
    const DinoAttackDef *def = gDinoResource->GetAttackDef(id);
    if ((1 << *DinoOffsetPtr<u8>(def, def->unk10)) & 0xF00) {
        return TRUE;
    }
    switch (*DinoOffsetPtr<u8>(def, def->unk14)) {
    case 2:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return TRUE;
    }
    return FALSE;
}

// Clears a side's status effect tasks.
extern "C" void func_ov000_0217e1ec(void *icons, int side);

// Points the attack helps at the item and shows its cost.
void DinoBattleViewUiAbilityMenu::OnFocus(MMenuItem *item) {
    for (u32 i = 0; i < 3; i++) {
        mHelps[i]->SetItem(item, ((DinoBattleViewUiAbilityMenuItem *)item)->GetUnk94());
    }
    const DinoAttackDef *def = gDinoResource->GetAttackDef(((DinoBattleViewUiAbilityMenuItem *)item)->GetUnk8C());
    u16 cost = DinoOffsetPtr<u16>(def, def->cost)[1];
    char text[64];
    char costText[8];
    if (cost != 0) {
        OS_SPrintf(costText, "%d", cost + ((DinoBattleView *)mUnk104)->FindCreatureByAC(mUnkC0)->GetUnkFC());
    } else {
        strcpy(costText, data_ov000_021947ac.noCost);
    }
    OS_SPrintf(text, gDinoResource->GetAttackText08(((DinoBattleViewUiAbilityMenuItem *)item)->GetUnk8C()), costText);
    mCostText->SetText(std::string(text));
}

DinoBattleViewUiAbilityMenu::DinoBattleViewUiAbilityMenu(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4,
                                                         DinoBattleViewUiAbilityMenuArgs args, u8 arg6, u32 arg7,
                                                         DinoBattleView *view)
    : MMenu(gDinoResource->GetBattleAdjustDefs()->abilityMenuWrap ? 2 : 0), mUnkB8(arg0), mUnkBC(arg1),
      mUnkC0(arg2), mUnkC4(arg3), mUnkC8(arg4), mUnkCC(0), mUnkD4(arg6), mUnkD8(arg7), mArgs(args), mUnk104(view),
      mUnk11C(0) {
    func_ov000_0217e1ec(view->mUnkE0->mUnk140, 0);
    func_ov000_0217e1ec(view->mUnkE0->mUnk140, 1);
    SetDeferStart(TRUE);
    mSerial.AddTail(new MTimer(data_0208a19c));
    Disable();
}

// (Unreferenced.)
extern "C" void func_ov000_0217b838() {
}


// Only the first two menus at a time load the BG; the others share it.
// NONMATCHING (99%): the scheduling of the Transfer call's point arguments.
DinoBattleViewAbilityMenuBG::DinoBattleViewAbilityMenuBG() : MProcess(NULL) {
    if (data_ov000_021978dc >= 2) {
        mUnk10D4 = FALSE;
        mOffset.x = 0;
        mOffset.y = 0;
        mState = data_ov000_021947ac.StateHidden_20;
    } else {
        mUnk10D4 = TRUE;
        MI_CpuFill8(mScreen, 0, sizeof(mScreen));
        unk78 = (u32)gLcd->mSub;
        mPicture = MPictureFile(std::string(data_ov000_021947ac.bgPath), 0);
        mBg = gCurrentDisplay->mBg;
        mText = mBg->CreateText(2, TRUE);
        mText->mTextCntBits.colors256 = mPicture.IsUnk04();
        MPal *pal = mBg->mPal;
        mPalBlock = pal->Alloc(mPicture.GetPaletteSize(), NULL, TRUE);
        mPalBlock->Load(mPicture.GetPalette(), 0, -1);
        mChr = mBg->CreateChr(mPicture.IsUnk04(), mPicture.GetChrSize(), TRUE);
        mChrBlock = mChr->Alloc(mPicture.GetChrSize(), TRUE);
        mChrBlock->Load(mPicture.GetChr(), 0, 0x7FFF, mPalBlock->index);
        mText->SetChr(mChr);
        mText->Transfer(&mScreen[0][0], MPoint(0x20, 0x40), MPoint(0, 0), mChrBlock->index, mPalBlock->index, false,
                        false);
        mState = data_ov000_021947ac.StateShow_70;
    }
    data_ov000_021978dc++;
}

// Moves the bar (and its parts with it).
extern "C" MTask *func_ov000_0217a080(AttackMenuBarMotion *bar, MPoint from, MPoint to, int frames, int ease) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(bar->mMotion->CreateCoordAnime(from, to, frames, ease));
    set->AddTail(bar->mUnk80->CreateCoordAnime(
        MPoint(from.x + data_ov000_021947ac.mUnk80Offset.x, from.y + data_ov000_021947ac.mUnk80Offset.y),
        MPoint(to.x + data_ov000_021947ac.mUnk80Offset.x, to.y + data_ov000_021947ac.mUnk80Offset.y), frames, ease));
    set->AddTail(bar->mUnk7C->CreateCoordAnime(
        MPoint(from.x + data_ov000_021947ac.mUnk7COffset.x, from.y + data_ov000_021947ac.mUnk7COffset.y),
        MPoint(to.x + data_ov000_021947ac.mUnk7COffset.x, to.y + data_ov000_021947ac.mUnk7COffset.y), frames, ease));
    if (bar->mUnk78 != NULL) {
        set->AddTail(bar->mUnk78->CreateCoordAnime(
            MPoint(from.x + data_ov000_0219482c.mUnk78Offset.x, from.y + data_ov000_0219482c.mUnk78Offset.y),
            MPoint(to.x + data_ov000_0219482c.mUnk78Offset.x, to.y + data_ov000_0219482c.mUnk78Offset.y), frames,
            ease));
    }
    return set;
}

extern "C" MTask *func_ov000_0217a45c(DinoBattleViewUiAbilityMenuItem *item, MPoint from, MPoint to, int frames,
                                      int ease) {
    return func_ov000_0217a080(item->mUnk98, from, to, frames, ease);
}

// Slides the bar out when focused, back when not.
void DinoBattleViewUiAbilityMenuItem::OnFocusChanged(BOOL focused) {
    MPoint pos = mUnk98->GetPosition();
    MPoint to;
    if (focused) {
        to = MPoint(mUnk94 ? data_ov000_0219482c.barFocusedFlagX : data_ov000_0219482c.barFocusedX, pos.y);
        DinoSoundPlaySe(0x5E, 0);
    } else {
        to = MPoint(focused ? data_ov000_0219482c.barFocusedX : data_ov000_021947ac.barX, pos.y);
    }
    mTasks->Clear();
    mTasks->AddTail(func_ov000_0217a45c(this, pos, to, 12, -1));
    if (!mUnk94) {
        mUnk98->Play(focused ? 2 : 1);
    }
}

#pragma auto_inline on

extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, BOOL *proceed);

void DinoBattleViewUiAbilityMenu::OnIdle() {
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnk104)->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_0208a36c[];
extern "C" u32 data_0208a364[];
extern "C" u32 data_ov000_0219492c[];
extern "C" u32 data_ov000_021949ac[];
extern "C" u32 data_0208a368[];
static struct DinoBattleViewAbilityMenuBgStaticInit {
    DinoBattleViewAbilityMenuBgStaticInit() {
        DINO_POOL_WORD(data_ov000_0219482c, 0x8) = 128;
        DINO_POOL_WORD(data_ov000_0219482c, 0xc) = 176;
        DINO_POOL_WORD(data_ov000_021947ac, 0x40) = 8;
        DINO_POOL_WORD(data_ov000_021947ac, 0x44) = 150;
        DINO_POOL_WORD(data_ov000_021947ac, 0x28) = 64;
        DINO_POOL_WORD(data_ov000_021947ac, 0x2c) = 112;
        DINO_POOL_WORD(data_ov000_021947ac, 0x60) = 128;
        DINO_POOL_WORD(data_ov000_021947ac, 0x64) = 32;
        DINO_POOL_WORD(data_ov000_021947ac, 0x30) = 0;
        DINO_POOL_WORD(data_ov000_021947ac, 0x34) = 24;
        DINO_POOL_WORD(data_ov000_0219482c, 0x18) = 104;
        DINO_POOL_WORD(data_ov000_0219482c, 0x1c) = 32;
        DINO_POOL_WORD(data_ov000_0219482c, 0x0) = 116;
        DINO_POOL_WORD(data_ov000_0219482c, 0x4) = 32;
        DINO_POOL_WORD(data_ov000_0219482c, 0x20) = 39;
        DINO_POOL_WORD(data_ov000_0219482c, 0x24) = 0;
        DINO_POOL_WORD(data_ov000_021947ac, 0x58) = -91;
        DINO_POOL_WORD(data_ov000_021947ac, 0x5c) = -6;
        DINO_POOL_WORD(data_ov000_021947ac, 0x48) = 49;
        DINO_POOL_WORD(data_ov000_021947ac, 0x4c) = -8;
        DINO_POOL_WORD(data_ov000_0219492c, 0x74) = DINO_POOL_WORD(data_0208a36c, 0x0);
        DINO_POOL_WORD(data_ov000_0219492c, 0x78) = 0;
        DINO_POOL_WORD(data_ov000_0219492c, 0x7c) = 0;
        DINO_POOL_WORD(data_ov000_021949ac, 0x0) = 127;
        DINO_POOL_WORD(data_ov000_021949ac, 0x4) = 22;
        DINO_POOL_WORD(data_ov000_021949ac, 0x8) = DINO_POOL_WORD(data_0208a368, 0x0);
        DINO_POOL_WORD(data_ov000_021949ac, 0xc) = 123;
        DINO_POOL_WORD(data_ov000_021949ac, 0x10) = 0;
        DINO_POOL_WORD(data_ov000_021949ac, 0x14) = 145;
        DINO_POOL_WORD(data_ov000_021949ac, 0x18) = 22;
        DINO_POOL_WORD(data_ov000_021949ac, 0x1c) = DINO_POOL_WORD(data_0208a364, 0x0);
        DINO_POOL_WORD(data_ov000_021949ac, 0x20) = 141;
        DINO_POOL_WORD(data_ov000_021949ac, 0x24) = 0;
        DINO_POOL_WORD(data_ov000_021949ac, 0x28) = 189;
        DINO_POOL_WORD(data_ov000_021949ac, 0x2c) = 22;
        DINO_POOL_WORD(data_ov000_0219492c, 0x24) = 0;
        DINO_POOL_WORD(data_ov000_0219492c, 0x28) = 145;
        DINO_POOL_WORD(data_ov000_0219492c, 0x2c) = 255;
        DINO_POOL_WORD(data_ov000_0219492c, 0x30) = 164;
    }
} sDinoBattleViewAbilityMenuBgStaticInit;
