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
#include "m/oam_canvas.h"
#include "m/font.h"
#include "m/smart_oam.h"
#include <string.h>
#include "game/dino_static_init.h"
#include <string>
#include "m/picture.h"
#include "m/chr.h"
#include "m/lcd.h"
#include "nitro/mi.h"
#include "game/dino_battle_view_ability_text_window.h"
#include "game/dino_message.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view.h"
#include "m/input.h"

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

// Pooled file statics: the parts' offsets from the bar.
struct AttackMenuBarStatics {
    u32 unk00;
    const char *barPath;    // 0x04: the bar
    const char *noCost;     // 0x08
    const char *hitsPath;   // 0x0C: the hit count
    const char *creatureMotionPath; // 0x10: the attacking creature's motion
    const char *bgPath;     // 0x14: the ability menu's BG
    const char *barPath1;   // 0x18: the bar, for attacks on several creatures
    const char *barPath2;   // 0x1C: the bar, with flag1
    void (DinoBattleViewAbilityMenuBG::*StateHidden_20)();
    u8 unk28[0x30 - 0x28];
    MPoint rowStep;         // 0x30: between the item bars
    u8 unk38[0x48 - 0x38];
    MPoint mUnk7COffset;    // 0x48
    u32 unk50[2];
    MPoint mUnk80Offset;    // 0x58
    int barX;               // 0x60: the bar's x, unfocused
    int barY;               // 0x64: the first bar's y
    u8 unk68[0x70 - 0x68];
    void (DinoBattleViewAbilityMenuBG::*StateShow_70)();
};
struct AttackMenuBarStatics2 {
    int barFocusedFlagX;    // 0x00: focused, with mUnk94 set
    u32 unk04;
    MPoint chosenBarPos;    // 0x08: where the chosen attack's bar goes
    u8 unk10[0x18 - 0x10];
    int barFocusedX;        // 0x18: focused (or unfocused with mUnk94 set)
    u32 unk1C;
    MPoint mUnk78Offset;    // 0x20
};
extern "C" AttackMenuBarStatics data_ov000_021947ac;
extern "C" AttackMenuBarStatics2 data_ov000_0219482c;


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

extern "C" BOOL func_ov000_02179aa0(u32 id);

// Builds the bar: its motion (by kind), the attack's name and cost, and the hit count when it has several.
void AttackMenuBarMotion::Init(MSmartOam *oam, u32 attack, bool flag1, bool flag2, bool flag3, bool flag4) {
    const DinoAttackDef *def = gDinoResource->GetAttackDef(attack);
    const u8 *range = DinoOffsetPtr<u8>(def, def->unk10);
    int kind = flag1 ? 2 : 0;
    if (kind == 0 && func_ov000_02179aa0(attack)) {
        kind = 1;
    }
    int anchor = flag4 ? 3 : 0;
    switch (kind) {
    case 1:
        mMotion = new MMotion(std::string(data_ov000_021947ac.barPath1), oam, anchor, 1);
        break;
    case 2:
        mMotion = new MMotion(std::string(data_ov000_021947ac.barPath2), oam, flag4, 1);
        break;
    case 0:
    default:
        mMotion = new MMotion(std::string(data_ov000_021947ac.barPath), oam, flag4, 1);
        break;
    }
    mMotion->Show();
    mMotion->Play(flag2 ? (flag3 ? 4 : 3) : 1, 0);
    mMotion->SetAnchor(1);
    mMotion->GetUnit()->SetMode(1);
    MFontSet *font = gDinoResource->CreateFont12(0, flag2 ? 12 : 10, 1);
    MSmartObjCanvas *canvas = new MSmartObjCanvas(
        oam, MPoint(font->MeasureText(std::string(gDinoResource->GetAttackName(attack)), 0, 0).x, flag4 ? 16 : 32),
        gDinoResource->GetUnkTable(), TRUE);
    mUnk80 = new MIndicator(canvas, font, 0);
    canvas->GetOamUnit()->SetMode(1);
    mUnk80->SetText(std::string(gDinoResource->GetAttackName(attack)));
    mUnk80->Show();
    char cost[16];
    OS_SPrintf(cost, "%3d", *DinoOffsetPtr<u16>(def, def->cost));
    if (flag2) {
        for (char *c = cost; *c != 0; c++) {
            *c += 0x20;
        }
    }
    MFont *costFont = ((DinoBattleView *)mOwner)->GetUnk120();
    canvas = new MSmartObjCanvas(oam, MPoint(24, 16), ((DinoBattleView *)mOwner)->GetUnk11CPalette(), TRUE);
    mUnk7C = new MIndicator(canvas, costFont, 0);
    canvas->GetOamUnit()->SetMode(1);
    mUnk7C->SetText(std::string(cost));
    mUnk7C->Show();
    GetParallel()->AddTail(mMotion, mUnk80, mUnk7C);
    if (range[3] > 1) {
        mUnk78 = new MMotion(std::string(data_ov000_021947ac.hitsPath), oam, flag4, 1);
        mUnk78->Show();
        mUnk78->Play(range[3] - 1, 0);
        mUnk78->SetAnchor(0);
        mUnk78->GetUnit()->SetMode(1);
        mParallel.AddTail(mUnk78);
    } else {
        mUnk78 = NULL;
    }
    SetPosition(MPoint(0, 0));
    SetPriority(0);
}


void AttackMenuBarMotion::SetPriority(int priority) {
    mMotion->SetPriority(priority);
    mUnk80->SetPriority(priority);
    mUnk7C->SetPriority(priority);
    if (mUnk78 != NULL) {
        mUnk78->SetPriority(priority);
    }
}

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

// The menu's battle view (through an inline, so it's read before GetUnkB4 is called).
static inline void *DinoAbilityMenuView(DinoBattleViewUiAbilityMenu *menu) {
    return menu->mUnk104;
}

DinoBattleViewUiAbilityMenuItem::DinoBattleViewUiAbilityMenuItem(DinoBattleViewUiAbilityMenu *menu, bool flag, bool flag3, u32 attack,
                                                                 u32 kind, u32 unk90) {
    mUnk88 = kind;
    mUnk8C = attack;
    mUnk90 = unk90;
    mUnk94 = flag;
    mMenu = menu;
    mUnk98 = new AttackMenuBarMotion(DinoAbilityMenuView(mMenu), mMenu->GetUnkB4(), mUnk8C, mUnk88 == 2, flag, flag3, false);
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
DinoBattleViewAbilityMenuBG::DinoBattleViewAbilityMenuBG() : MProcess(NULL) {
    MPal *pal;
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
        int colors256 = mPicture.IsUnk04();
        mText->mTextCntBits.colors256 = colors256;
        pal = mBg->mPal;
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

// The attack helps' rects and texts.
struct DinoBattleViewAbilityHelpDef {
    u32 text;
    MRect rect;
};
extern "C" DinoBattleViewAbilityHelpDef data_ov000_021949a0[3];
extern "C" MRect data_ov000_02194950;     // the cancel help's rect
extern "C" MPoint data_ov000_021947ec;    // the cost text's position
extern "C" const u32 data_0208a190;       // the open slide's length
extern const u32 gKasekiRockSizeText[];
extern "C" const u32 data_0208a360;   // (gKasekiRockSizeText[5], which the game reaches by its own address)
extern "C" DinoBattleViewAbilityTextWindow *func_ov000_0217c0b8(DinoBattleViewAbilityMenuBG *bg, BOOL rows);
extern "C" MPoint func_ov000_02175b90(DinoBattleViewCreature *creature);
extern "C" MTask *func_ov000_0218ec8c(u32 arg0, int arg1, BOOL *proceed);

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *DinoAbilityMenuRepeat(const DinoExcavateDefs *d) {
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *) ((const u8 *) d + d->offset7C);
}

// Builds the menu: the helps, the creature's motion, an item bar per attack (sliding in one after another, linked
// round in a loop), the cancel item, the cost text, the BG and its text windows; then opens it (after the tutorial,
// if one shows).
// NONMATCHING: the help loop's &mParallel takes the top spill slot (the game's lowest), and gKasekiRockSizeText[5]
// folds the offset into the load.
// (A non-member inline: the loop's hoisted &mParallel takes the game's slot.)
static inline MTaskSet *DinoAbilityParallel(DinoBattleViewUiAbilityMenu *menu) {
    return &menu->mParallel;
}

void DinoBattleViewUiAbilityMenu::OnStart() {
    const DinoBattleViewAbilityHelpDef *def = data_ov000_021949a0;
    for (u32 i = 0; i < 3; def++, i++) {
        MTaskSet *parallel = DinoAbilityParallel(this);
        mHelps[i] = new DinAttackItemHelp(def->rect, def->text, gLcd->GetBottomDisplay());
        parallel->AddTail(mHelps[i]);
    }
    DinoBattleHelpBase *cancel =
        new DinoBattleHelpBase(gLcd->GetBottomDisplay(), data_ov000_02194950, data_0208a360);
    mParallel.AddTail(cancel);
    SetSelectedItem(mHelps[0]);
    for (u32 i = 0; i < 3; i++) {
        AddEntry((u32) (DinoHelpItem *) mHelps[i], 2, (u32) (DinoHelpItem *) cancel);
        if (i < 2) {
            AddEntry((u32) (DinoHelpItem *) mHelps[i], 1, (u32) (DinoHelpItem *) mHelps[i + 1]);
        }
        if (i != 0) {
            AddEntry((u32) (DinoHelpItem *) mHelps[i], 3, (u32) (DinoHelpItem *) mHelps[i - 1]);
        }
    }
    AddEntry((u32) (DinoHelpItem *) cancel, 4, (u32) (DinoHelpItem *) mHelps[0]);
    AddEntry((u32) (DinoHelpItem *) cancel, 1, 0);
    AddEntry((u32) (DinoHelpItem *) cancel, 3, 0);

    DinoBattleViewCreature *creature = ((DinoBattleView *) mUnk104)->FindCreatureByAC(mUnkC0);
    ((DinoBattleView *) mUnk104)->GetUnkE4(((DinoBattleViewPlayer *) creature->GetUnkDC())->mUnkA8)->GetOpponent();
    MSmartOam *oam = new MSmartOam(1, 0, 1);
    MMotion *motion = new MMotion(std::string(data_ov000_021947ac.creatureMotionPath), oam, 0, 1);
    motion->SetPosition(func_ov000_02175b90(creature));
    motion->Play(1, 0);
    motion->SetPriority(1);
    motion->SetAnchor(0);
    motion->Show();
    ((DinoBattleViewPlayer *) creature->GetUnkDC())->SetMotions(motion, (MTask *) oam);

    mUnkB4 = new MSmartOam(0x22, 0, 1);
    mUnkB4->mHidden = TRUE;
    mParallel.AddTail(mUnkB4->CreateShowTransfer());
    GetInput()->SetRepeat(0xC0, DinoAbilityMenuRepeat(gDinoResource->GetExcavateDefs().Get())[7],
                          DinoAbilityMenuRepeat(gDinoResource->GetExcavateDefs().Get())[8]);
    gDinoResource->GetCreatureDef(mUnkBC);
    MParallelTaskSet *slides = new MParallelTaskSet(TRUE);
    mSerial.AddTail(slides);
    int count = creature->CountAttacks();
    int rows = count;
    if (mUnkD4 && creature->GetSpecialAttack() != 0) {
        rows = count + 1;
    }
    unkD0 = 5 - rows;
    std::vector<DinoBattleViewUiAbilityMenuItem *> items;
    for (int i = 0; i <= count; i++) {
        u32 attack;
        int kind = 0;
        if ((u32) i < creature->CountAttacks()) {
            attack = creature->GetAttack(i);
        } else if (mUnkD4) {
            attack = creature->GetSpecialAttack();
            kind = 2;
        } else {
            continue;
        }
        if (attack == 0) {
            continue;
        }
        mUnkCC++;
        mItems[i] = new DinoBattleViewUiAbilityMenuItem(this, !(mUnkC4 & (1 << i)), (mUnkC8 & (1 << i)) != 0, attack,
                                                        kind, i);
        AddItem(mItems[i]);
        int row = i + unkD0;
        int x = data_ov000_021947ac.barX + data_ov000_021947ac.rowStep.x * row;
        int y = data_ov000_021947ac.barY + data_ov000_021947ac.rowStep.y * row;
        mItems[i]->GetBar();
        MPoint start(x, y - 160);
        MPoint to(x, y);
        mItems[i]->SetBarPosition(start);
        MSerialTaskSet *seq = new MSerialTaskSet(TRUE);
        MTask *slide = func_ov000_0217a45c(mItems[i], start, to, 20, -1);
        seq->AddTail(new MTimer((count - i) * (data_0208a190 / 7)));
        seq->AddTail(slide);
        slides->AddTail(seq);
        items.push_back(mItems[i]);
    }
    u32 size = items.size();
    if (size > 1) {
        for (u32 j = 0; j < size; j++) {
            AddLink(items[j], 2, items[(j + 1) % size]);
            AddLink(items[j], 4, items[(j - 1 + size) % size]);
        }
    }
    unk120 = (u32) new DinoBattleViewCancelMenuItem();
    AddItem((MMenuItem *) unk120);
    MFont *font = gDinoResource->CreateFont12(0, 10, 1);
    mCostText = new MIndicator(new MTextCanvas(MPoint(0xF0, 0x20), gDinoResource->GetUnkTable(), 0, TRUE), font, 0);
    mCostText->SetPriority(0);
    mCostText->SetPosition(data_ov000_021947ec);
    mParallel.AddTail(mCostText);
    mBG = new DinoBattleViewAbilityMenuBG();
    mParallel.AddTail(mBG);
    unkF8 = (u32) func_ov000_0217c0b8(mBG, FALSE);
    ((DinoBattleViewAbilityTextWindow *) unkF8)
        ->StartAnime(MRect(16, 16, 16, 16), MRect(3, unkD0 * 3 + 2, 28, 17), 12, -1);
    ((DinoBattleViewAbilityTextWindow *) unkF8)->Show();
    mTextWindow = func_ov000_0217c0b8(mBG, TRUE);
    mBG->SetOffset(MPoint(0, -192));
    mParallel.AddTail(mBG->CreateOffsetAnime(MPoint(0, -192), MPoint(0, 0), data_0208a190, -1));
    MTask *focus = CreateFocusItemTransfer(mItems[mUnkD8]);
    MTask *show = mCostText->CreateShowTransfer();
    MTask *showWindow = mTextWindow->CreateShowTask();
    MTaskSet &serial = mSerial;
    serial.AddTail(showWindow);
    serial.AddTail(show);
    serial.AddTail(focus);
    MTask *tutorial = func_ov000_0218ec8c(((DinoBattleView *) mUnk104)->GetUnk12C(), 11, NULL);
    if (tutorial != NULL) {
        MTask *enable = new MenuEnableTask(this, TRUE);
        MTask *disable = new MenuEnableTask(this, FALSE);
        MTaskSet &serial2 = mSerial;
        serial2.AddTail(disable);
        serial2.AddTail(tutorial);
        serial2.AddTail(enable);
    }
    if (!((u8 *) ((DinoBattleView *) mUnk104)->GetUnk12C())[0xD]) {
        mSerial.AddTail(DinoHelpBase::CreateEnableTask());
    }
    mParallel.AddTail(mUnkB4);
}

extern "C" const u32 data_0208a1b4;     // the chosen bar's slide length
extern "C" const MPoint data_ov000_02194834;
extern "C" void func_ov000_021615b4(DinoBattleViewUnkE0 *field, AttackMenuBarMotion *bar,
                                    DinoBattleViewAbilityMenuBG *bg, int y);

// The background's vertical offset.
static inline int DinoBgOffsetY(DinoBattleViewAbilityMenuBG *bg) {
    return bg->GetOffset().y;
}

// Cancel closes the menu; an attack (when usable) slides the other bars off, moves the chosen one down with the BG,
// hands a copy of its bar to the field and reports the choice.
#pragma opt_common_subs off
void DinoBattleViewUiAbilityMenu::OnDecide(MMenuItem *menuItem) {
    DinoBattleViewUiAbilityMenuItem *item = (DinoBattleViewUiAbilityMenuItem *) menuItem;
    if (menuItem == (MMenuItem *) unk120) {
        BOOL proceed = TRUE;
        MTask *tutorial = func_ov000_0218ec8c(((DinoBattleView *) mUnk104)->GetUnk12C(), 14, &proceed);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        if (proceed) {
            DinoSoundPlaySe(0x63, 0);
            Close();
        }
    } else if (!item->GetUnk94()) {
        BOOL proceed = TRUE;
        MTask *tutorial =
            func_ov000_0218ec8c(((DinoBattleView *) mUnk104)->GetUnk12C(), item->GetUnk90() + 15, &proceed);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        if (proceed) {
            DinoHelpBase::Disable();
            MParallelTaskSet *slides = new MParallelTaskSet(TRUE);
            for (int i = 0; i < (int) mUnkCC; i++) {
                if (mItems[i] != item) {
                    MPoint pos = mItems[i]->GetBarPosition();
                    MPoint dest(368, pos.y);
                    slides->AddTail(func_ov000_0217a45c(mItems[i], pos, dest, 20, 1));
                }
            }
            slides->AddTail(DinoSoundCreatePlaySeTask(0x61, 0));
            u32 row = unkD0 * 3 + item->GetUnk90() * 3;
            ((DinoBattleViewAbilityTextWindow *) unkF8)
                ->StartAnime(((DinoBattleViewAbilityTextWindow *) unkF8)->GetRect(), MRect(-1, row + 2, 32, row + 5),
                             4, 0);
            mTextWindow->Hide();
            mCostText->Hide();
            mSerial.AddTail(slides);

            MParallelTaskSet *move = new MParallelTaskSet(TRUE);
            MPoint from;
            MPoint to(0, DinoBgOffsetY(mBG) + item->GetBarPosition().y - data_ov000_0219482c.chosenBarPos.y);
            from = mBG->GetOffset();
            MTask *bgSlide = mBG->CreateOffsetAnime(from, to, data_0208a1b4, -1);
            MTask *barSlide = func_ov000_0217a45c(item, item->GetBarPosition(), data_ov000_0219482c.chosenBarPos,
                                                  data_0208a1b4, -1);
            move->AddTail(bgSlide);
            move->AddTail(barSlide);

            item->GetBar();
            AttackMenuBarMotion *bar = new AttackMenuBarMotion(
                (DinoBattleView *) mUnk104, ((DinoBattleView *) mUnk104)->mUnk10C, item->GetBar()->mFlags);
            bar->Play(2);
            bar->Hide();
            bar->SetPosition(data_ov000_02194834);
            mParallel.Remove(mBG);
            DinoBattleViewUnkE0 *field = ((DinoBattleView *) mUnk104)->mUnkE0;
            func_ov000_021615b4(field, bar, mBG, item->GetBarPosition().y);
            MTask *post, *exit;
            exit = MTaskExitTransfer::Create(this);
            post = ((MMessageObserver *) mUnkB8)
                       ->CreatePostTask(new DinoBattleViewUiAbilityMenuResult(mUnkC0, item->GetUnk90(), FALSE));
            mSerial.AddTail(move, bar->CreateShowTask(), mBG->CreateSetPriorityTask(1),
                            new AttackMenuBarMotion::SetPriorityTask(bar, 1), post, exit);
        }
    } else {
        DinoSoundPlaySe(0x62, 0);
    }
}
#pragma opt_common_subs reset

#pragma auto_inline on



void DinoBattleViewUiAbilityMenu::OnIdle() {
    MTask *task = func_ov000_0218ec8c(((DinoBattleView *)mUnk104)->GetUnk12C(), -1, 0);
    if (task != NULL) {
        mSerial.AddTail(task);
    }
}

extern "C" const u32 data_0208a194;    // the close slide's length

// (The anime re-reads the length: one use fewer for frames, which takes the game's spill slot.)
void DinoBattleViewUiAbilityMenu::Close() {
    Disable();
    MParallelTaskSet *slides = NULL;
    u32 frames = data_0208a194;
    if (frames != 0) {
        slides = new MParallelTaskSet(TRUE);
        // (The count check before the loop is the game's: its loop entry tests twice.)
        int i = 0;
        if ((int)mUnkCC > 0) {
            for (; i < (int)mUnkCC; i++) {
                MPoint pos = mItems[i]->GetBarPosition();
                MSerialTaskSet *seq = new MSerialTaskSet(TRUE);
                MPoint dest(pos.x, pos.y - 192);
                MTask *slide = func_ov000_0217a45c(mItems[i], pos, dest, 20, -1);
                seq->AddTail(new MTimer(frames / 7 * i), slide);
                slides->AddTail(seq);
            }
        }
    }
    mTextWindow->Hide();
    mCostText->Hide();
    MPoint from;
    MPoint to(0, -192);
    from = mBG->mUnk10D4 ? mBG->mText->GetOffset() : mBG->mOffset;
    mParallel.AddTail(mBG->mUnk10D4 ? mBG->mText->CreateOffsetAnime(from, to, data_0208a194, 1) : new MTimer(1));
    if (slides != NULL) {
        mSerial.AddTail(slides);
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    GetSerial()->AddTail(((MMessageObserver *)mUnkB8)->CreatePostTask(new DinoBattleViewUiAbilityMenuResult(mUnkC0, 0, TRUE)),
                         exit);
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_0208a36c[];
extern "C" u32 data_0208a364[];
extern "C" u32 data_ov000_021949ac[];
extern "C" u32 data_ov000_0219492c[];
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
        DINO_POOL_WORD(data_ov000_0219492c, 0x74) = (s32)data_0208a36c[0];
        DINO_POOL_WORD(data_ov000_0219492c, 0x78) = 0;
        DINO_POOL_WORD(data_ov000_0219492c, 0x7c) = 0;
        DINO_POOL_WORD(data_ov000_021949ac, 0x0) = 127;
        DINO_POOL_WORD(data_ov000_021949ac, 0x4) = 22;
        DINO_POOL_WORD(data_ov000_021949ac, 0x8) = (s32)data_0208a368[0];
        DINO_POOL_WORD(data_ov000_021949ac, 0xc) = 123;
        DINO_POOL_WORD(data_ov000_021949ac, 0x10) = 0;
        DINO_POOL_WORD(data_ov000_021949ac, 0x14) = 145;
        DINO_POOL_WORD(data_ov000_021949ac, 0x18) = 22;
        DINO_POOL_WORD(data_ov000_021949ac, 0x1c) = (s32)data_0208a364[0];
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
