// DinoUiMedalMenu.
// Only the destructor is decompiled so far.

#pragma ipa file

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/bright.h"
#include "m/obj.h"
#include "m/lcd.h"
#include <algorithm>
#include "game/dino_ui_medal_menu.h"
#include "m/bg.h"
#include "game/dino_ui_rv_status_menu_item_menu.h"
#include "game/dino_backup.h"
#include "game/dino_creature_sort.h"
#include "game/dino_ui_parts_reviver_name.h"
#include "game/dino_ui_parts_reviver_name_count_check_task.h"
#include "game/dino_ui_field_menu_cursor.h"
#include "m/indicator.h"
#include <printf.h>
#include "game/dino_ui_parts_reviver_medal_button.h"
#include "game/dino_ui_field_menu_fraction.h"
#include "m/display.h"
#include "m/blend.h"
#include "game/dino_sound.h"
#include "m/call_task.h"
#include "m/bg_new_task.h"
#include "m/picture.h"
#include "game/dino_static_init.h"
#include "game/dino_team.h"
#include "game/dino_ui_common.h"
#include "game/dino_guide_message.h"
#include "game/dino_ui_parts_team_tech.h"
#include "m/font.h"
#include "m/oam_canvas.h"
#include "m/input.h"
#include "m/smart_oam.h"

DinoUiMedalMenu::~DinoUiMedalMenu() {
    gCurrentDisplay->mBlend->Reset();
}

DinoUiSortMenuItem::~DinoUiSortMenuItem() {
}

DinoUiMedalList::~DinoUiMedalList() {
}

DinoUiMedalMenuItem::~DinoUiMedalMenuItem() {
}

DinoUiMedalBackButton::~DinoUiMedalBackButton() {
}

DinoUiMedalPageLButton::~DinoUiMedalPageLButton() {
}

DinoUiMedalPageRButton::~DinoUiMedalPageRButton() {
}

DinoUiMedalSlotCreateTask::~DinoUiMedalSlotCreateTask() {
}

void DinoUiSortMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiSortMenuItem::OnDecide() {
}

void DinoUiSortMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiMedalMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMedalMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiSortMenuItem::GetFrame() {
    return MRect(mPos, MPoint(mPos.x + 0x28, mPos.y + 0xE));
}

MDisplay *DinoUiSortMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiMedalMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiMedalBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

const char *DinoUiMedalMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiSortMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiMedalList::Idle() {
    (this->*mState)();
}

void DinoUiMedalBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided(FALSE);
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiMedalPageRButton::OnDecide() {
    mOwner->PageRight();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiMedalPageLButton::OnDecide() {
    mOwner->PageLeft();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiMedalMenu::OnIdle() {
    mUnk204 = TRUE;
}

MRect DinoUiSortMenuItem::GetRect() {
    int y = mPos.y;
    int x = mPos.x;
    return MRect(MPoint(x, y - 2), MPoint(x + 0x28, y + 0x10));
}

extern "C" const int data_02089a0c;

void DinoUiMedalSlotCreateTask::Idle() {
    if (mIndex <= 14) {
        mMenu->CreateSlot(mIndex);
        if (mIndex < mMenu->mSlotCount - 1) {
            mSerial.AddTail(new MTimer(data_02089a0c));
        }
    } else {
        Exit();
    }
    mIndex++;
}


#pragma auto_inline off

DinoUiMedalMenu::DinoUiMedalMenu(u32 *mode, u32 *index, u8 *unk)
    : MMenu(gLcd->mMain, NULL, 0), mPageL(NULL), mResult((int *)mode), mIndex(index), mUnk198(unk), mPage(0), mLastPage(0),
      mSort(*unk), mUnk204(0), mUnk21C(0xFF), mUnk21D(0) {
    mMotions.clear();
}

// Where the cursor and the name go relative to a medal, and the buttons' places.
extern struct {
    u8 unk00[0xC];
    MPoint sortPos;         // 0x0C: the first sort button
    MPoint sortStep;        // 0x14: to the next
    MPoint backPos;         // 0x1C
    MPoint pageLPos;        // 0x24
    MPoint pageRPos;        // 0x2C
    u8 unk34[0x4C - 0x34];
    MPoint nameOffset;      // 0x4C
    MPoint cursorOffset;    // 0x54
} data_ov006_021940d8;

// The TU's pooled paths, and the slots' numbers' layout.
extern "C" u32 data_ov006_02193f58[];
extern "C" u32 data_ov006_02193fd8[];
// The buttons' graphics, in the path pool.
static inline const char *DinoUiMedalPath(int i) {
    return ((const char **)data_ov006_02193fd8)[i];
}
struct DinoUiMedalLayout {
    u8 unk00[0x54];
    int slotX;          // 0x54: the first slot
    int slotY;          // 0x58
    int slotColumnX;    // 0x5C: to the next slot in a row
    int slotColumnY;    // 0x60
    int slotRowX;       // 0x64: to the next row
    int slotRowY;       // 0x68
    MPoint numberOffset;    // 0x6C: of a slot's number
};
extern DinoUiMedalLayout data_ov006_02194058;
extern "C" const MPoint data_ov006_0219410c;    // the page fraction's place
extern "C" const MPoint data_ov006_021940cc;    // the team name's place
extern "C" const MPoint data_ov006_0219427c;    // the team tech's place
extern "C" const char data_ov006_02196114[], data_ov006_02196128[];    // the sort cursor and the background
extern "C" const char data_ov006_02196140[];    // the numbers' font
// OnStart's function-local static: the sort buttons' help texts (from 0x10), set on the first call.
extern "C" u32 data_ov006_02194258[];
extern "C" u32 data_ov006_02194268[5];   // (the texts, as the loop reads them)
extern "C" u32 data_ov006_02199184;
extern const u32 data_0208a468, data_0208a46c, data_0208a470, data_0208a474, data_0208a478, data_0208a47c, data_0208a480,
    data_0208a484, data_0208a488, data_0208a48c, data_0208a490, data_0208a494;
extern const u32 data_0208a904;
extern const int data_02089abc, data_02089ac0, data_02089ac4;
extern const u32 data_0208aa34;

inline DinoUiMedalPageLButton::DinoUiMedalPageLButton(DinoUiMedalMenu *owner)
    : DinoUiMedalMenuItem(owner, DinoUiMedalPath(8), data_ov006_021940d8.pageLPos, data_0208aa34) {
    SetKeys(0x200);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiMedalPageRButton::DinoUiMedalPageRButton(DinoUiMedalMenu *owner)
    : DinoUiMedalMenuItem(owner, DinoUiMedalPath(14), data_ov006_021940d8.pageRPos, data_0208aa34) {
    SetKeys(0x100);
    SetNoFocus(TRUE);
    DinoHelpItem::Disable();
}

inline DinoUiMedalBackButton::DinoUiMedalBackButton(DinoUiMedalMenu *owner)
    : DinoUiMedalMenuItem(owner, ((const char **)data_ov006_02193f58)[9], data_ov006_021940d8.backPos, data_0208a468) {
    SetKeys(2);
    SetMode(4);
    mMotion->Play(2, 0);
}

// The excavation settings' section at offset7C (the input repeat is at 0x1C and 0x20).
static inline const int *GetExcavate7C(MResource<DinoExcavateDefs> &defs) {
    const DinoExcavateDefs *d = defs.Get();
    if (d->offset7C == 0) {
        return NULL;
    }
    return (const int *)((const u8 *)d + d->offset7C);
}

// &mParallel through a non-member inline: the numbers loop hoists it into the frame's lowest slot, as the game does
// (taken directly, it lands in the highest).
static inline MTaskSet *MedalParallel(DinoUiMedalMenu *menu) {
    return &menu->mParallel;
}
#pragma opt_common_subs off
// The page fraction, page and back buttons, the pictures, the help areas, the sort buttons, the team's name, the
// cursor, the slots' numbers and the name; then the sorted slots, focusing the creature the list was left on.
void DinoUiMedalMenu::OnStart() {
    u32 guard = data_ov006_02199184;
    if (!(guard & 1)) {
        DINO_POOL_WORD(data_ov006_02194258, 0x10) = data_0208a47c;
        DINO_POOL_WORD(data_ov006_02194258, 0x14) = data_0208a478;
        DINO_POOL_WORD(data_ov006_02194258, 0x18) = data_0208a474;
        DINO_POOL_WORD(data_ov006_02194258, 0x1c) = data_0208a470;
        DINO_POOL_WORD(data_ov006_02194258, 0x20) = data_0208a46c;
        DINO_POOL_WORD(data_ov006_02199184, 0) = guard | 1;
    }
    SetLinkFallback(1);
    {
        MResource<DinoExcavateDefs> delayDefs = gDinoResource->GetExcavateDefs();
        MResource<DinoExcavateDefs> intervalDefs = gDinoResource->GetExcavateDefs();
        GetInput()->SetRepeat(0xF0, GetExcavate7C(delayDefs)[7], GetExcavate7C(intervalDefs)[8]);
    }
    mPage = *mIndex / 15;
    gDinoBackup->GetTeam(gDinoBackup->GetUnk34_26(), &mTeam);
    mOam = new MSmartOam(0x31, 0, TRUE);
    mFraction = new DinoUiFieldMenuFraction(mOam, data_ov006_0219410c);
    mParallel.AddTail(mFraction);
    mFraction->SetNumerator(mPage + 1);
    mFraction->SetPriority(2);
    LoadCreatures();
    if ((int)mLastPage > 0) {
        mPageL = new DinoUiMedalPageLButton(this);
        AddItem(mPageL);
        mPageR = new DinoUiMedalPageRButton(this);
        AddItem(mPageR);
    }
    mParallel.AddTail(mSortCursor = new MTextPicture(std::string(data_ov006_02196114), TRUE, false));
    mSortCursor->Show();
    mSortCursor->SetPriority(2);
    MTextPicture *background = new MTextPicture(std::string(data_ov006_02196128), TRUE, false);
    background->Show();
    background->SetPriority(3);
    mParallel.AddTail(background);
    mUnk214 = new MParallelTaskSet();
    mParallel.AddTail(mUnk214);
    mUnk218 = new MParallelTaskSet();
    mParallel.AddTail(mUnk218);
    DinoHelpDummyItem *title, *list, *tabs, *info, *medals;
    mParallel.AddTail(title = new DinoHelpDummyItem(GetDisplay(), MRect(0x26, 3, 0x9C, 0x10),
                                                    std::string(gDinoResource->GetText(data_0208a494))));
    mParallel.AddTail(list = new DinoHelpDummyItem(GetDisplay(), MRect(0x1C, 0x16, 0x47, 0x3A),
                                                   std::string(gDinoResource->GetText(data_0208a490))));
    mParallel.AddTail(new DinoHelpDummyItem(GetDisplay(), MRect(0x4A, 0x16, 0x9B, 0x3A),
                                            std::string(gDinoResource->GetText(data_0208a48c))));
    mParallel.AddTail(tabs = new DinoHelpDummyItem(GetDisplay(), MRect(0xA4, 0x16, 0xEE, 0x3A),
                                                   std::string(gDinoResource->GetText(data_0208a488))));
    mParallel.AddTail(info = new DinoHelpDummyItem(GetDisplay(), MRect(0xF, 0x41, 0xEE, 0x9E),
                                                   std::string(gDinoResource->GetText(data_0208a484))));
    mParallel.AddTail(medals = new DinoHelpDummyItem(GetDisplay(), MRect(0xEF, 0x8B, 0xFD, 0x9D),
                                                     std::string(gDinoResource->GetText(data_0208a480))));
    mUnkB8 = new DinoUiMedalBackButton(this);
    AddItem(mUnkB8);
    for (int i = 0; i < 5; i++) {
        mSortButtons[i] = new DinoUiSortMenuItem(data_ov006_02194268[i], MPoint(data_ov006_021940d8.sortPos.x + data_ov006_021940d8.sortStep.x * i,
                                                                data_ov006_021940d8.sortPos.y + data_ov006_021940d8.sortStep.y * i));
        AddItem(mSortButtons[i]);
    }
    AddEntry((u32)title, 2, (u32)list);
    AddEntry((u32)title, 3, 0);
    AddEntry((u32)title, 1, 0);
    AddEntry((u32)list, 4, (u32)title);
    AddEntry((u32)tabs, 4, (u32)title);
    AddEntry((u32)list, 2, (u32)info);
    AddEntry((u32)tabs, 2, (u32)info);
    AddEntry((u32)medals, 4, (u32)info);
    AddEntry((u32)medals, 3, (u32)info);
    AddEntry((u32)(DinoHelpItem *)mSortButtons[0], 4, (u32)info);
    AddEntry((u32)(DinoHelpItem *)mSortButtons[4], 4, (u32)info);
    AddEntry((u32)(DinoHelpItem *)mSortButtons[0], 2, (u32)(DinoHelpItem *)mUnkB8);
    AddEntry((u32)(DinoHelpItem *)mSortButtons[1], 2, (u32)(DinoHelpItem *)mUnkB8);
    AddEntry((u32)(DinoHelpItem *)mSortButtons[2], 2, (u32)(DinoHelpItem *)mUnkB8);
    AddEntry((u32)(DinoHelpItem *)mSortButtons[3], 2, (u32)(DinoHelpItem *)mUnkB8);
    AddEntry((u32)(DinoHelpItem *)mSortButtons[4], 1, (u32)(DinoHelpItem *)mUnkB8);
    AddEntry((u32)(DinoHelpItem *)mUnkB8, 3, (u32)(DinoHelpItem *)mSortButtons[4]);
    MFont *font = gDinoResource->CreateFont12(0, 10, 11);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, MPoint(0xA0, 0x10), gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    MIndicator *teamName = new MIndicator(canvas, font, 0);
    mParallel.AddTail(teamName);
    teamName->SetPosition(data_ov006_021940cc);
    teamName->SetPriority(2);
    teamName->Show();
    teamName->SetText(std::wstring((const wchar_t *)mTeam.name));
    mCursorTasks = new MParallelTaskSet();
    mParallel.AddTail(mCursorTasks);
    mCursor = new DinoUiFieldMenuCursor(mOam);
    mParallel.AddTail(mCursor);
    mCursor->SetPriority(0);
    mCursor->mUnk10->GetUnit()->SetMode(1);
    mCursor->mUnk14->GetUnit()->SetMode(1);
    mCursor->Hide();
    MFontSet *numbers = new MFontSet(std::string(data_ov006_02196140), 0, 0, 0);
    for (int i = 0; i < 15; i++) {
        MTaskSet *tasks = MedalParallel(this);
        MObjCanvas *slotCanvas = new MObjCanvas(gCurrentDisplay, MPoint(0x20, 0x10), numbers->GetPalette(), TRUE);
        slotCanvas->GetOamUnit()->SetMode(1);
        mNumbers[i] = new MIndicator(slotCanvas, numbers, 0);
        tasks->AddTail(mNumbers[i]);
        int row = i / 5;
        int column = i % 5;
        mNumbers[i]->SetPosition(MPoint(data_ov006_02194058.slotX + data_ov006_02194058.slotColumnX * column +
                                            data_ov006_02194058.slotRowX * row,
                                        data_ov006_02194058.slotY + data_ov006_02194058.slotColumnY * column +
                                            data_ov006_02194058.slotRowY * row) +
                                 data_ov006_02194058.numberOffset);
        mNumbers[i]->SetPriority(2);
        mNumbers[i]->Show();
    }
    mParallel.AddTail(mOam);
    mName = new DinoUiPartsReviverName();
    mParallel.AddTail(mName);
    mName->Hide();
    mNameTasks = new MSerialTaskSet();
    mParallel.AddTail(mNameTasks);
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208a904))));
    mSortCursor->SetPosition(0, mSort * -0xC0);
    Sort(mSort);
    if (!gDinoBackup->GetSystemFlag(2)) {
        CreateUnkF0Items();
        if (DinoTeamHasTech(&mTeam)) {
            mParallel.AddTail(new DinoUiPartsTeamTech(&data_ov006_0219427c));
        }
    }
    CreateSlots();
    LinkItems();
    if (!gDinoBackup->GetSystemFlag(2) && *mResult == 1) {
        FocusItem(mUnkF0[*mIndex % 15]);
    } else {
        FocusItem(mSlots[*mIndex % 15]);
    }
    DinoDimScreen();
    mUnk220 = new MParallelTaskSet();
    mParallel.AddTail(mUnk220);
    MTask *fade = gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(data_02089ac4, data_02089ac0);
    MParallelTaskSet *fadeSet = new MParallelTaskSet(TRUE);
    fadeSet->AddTail(fade);
    mUnk220->AddTail(fadeSet);
    mSerial.AddTail(new MTimer(data_02089abc));
    mCursor->Hide();
    MTask *play;
    MTask *show;
    show = mCursor->CreateShowTransfer();
    play = mCursor->CreatePlayTask();
    MParallelTaskSet *cursorSet = new MParallelTaskSet(TRUE);
    cursorSet->AddTail(play);
    cursorSet->AddTail(show);
    mSerial.AddTail(cursorSet);
}
#pragma opt_common_subs reset

// Where the mUnk1EC creatures' medals go.
extern "C" const MPoint data_ov006_021942f4[5];
void DinoUiMedalMenu::CreateUnkF0Items() {
    for (int i = 0; i < 5; i++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(mTeam.slots[i], &creature);
        u8 maxed;
        if (GetCreatureLevel(creature) == 12) {
            maxed = TRUE;
        } else {
            maxed = FALSE;
        }
        mUnkF0[i] = new DinoUiPartsReviverMedalButton(mOam, data_ov006_021942f4[i], mTeam.slots[i], maxed, 0);
        AddItem(mUnkF0[i]);
    }
}

void DinoUiMedalMenu::LinkItems() {
    if (!gDinoBackup->GetSystemFlag(2)) {
        AddLink(mUnkB8, 2, mUnkF0[0]);
        for (int i = 0; i < 5; i++) {
            AddLink(mUnkF0[i], 4, mUnkB8);
        }
        AddLink(mUnkF0[0], 3, mUnkF0[4]);
        AddLink(mUnkF0[4], 1, mUnkF0[0]);
    }
    AddLink(mSortButtons[0], 3, mSortButtons[4]);
    AddLink(mSortButtons[4], 1, mSortButtons[0]);
    for (int i = 0; i < 5; i++) {
        AddLink(mSortButtons[i], 2, mUnkB8);
    }
    AddLink(mUnkB8, 4, mSortButtons[4]);
    AddLink(mUnkB8, 3, NULL);
}

// Loads the motions of the page's slots.
void DinoUiMedalMenu::LoadPage() {
    int count;
    if (mPage == mLastPage) {
        count = mCreatures.size() - mPage * 15;
    } else {
        count = 15;
    }
    mMotions.clear();
    for (int i = 0; i < count; i++) {
        char path[0x100];
        OS_SPrintf(path, "/motion/ui_reviver_medal/reviver_%03d.bin", mCreatures[mPage * 15 + i]);
        mMotions.push_back(MMotionFile(std::string(path), 7));
    }
}

void DinoUiMedalMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 5; i++) {
        if (item == mSortButtons[i]) {
            SetSort(i);
        }
    }
    for (int i = 0; i < 5; i++) {
        if (item == mUnkF0[i]) {
            if (mTeam.slots[i] && mUnk204) {
                *mIndex = mPage * 15 + i;
                *mUnk198 = mSort;
                OnBackDecided(1);
                DinoSoundPlaySe(0x98, 0);
            }
            return;
        }
    }
    for (int i = 0; i < (int)mSlotCount; i++) {
        if (item == mSlots[i]) {
            if (mUnk204) {
                *mIndex = mPage * 15 + i;
                *mUnk198 = mSort;
                OnBackDecided(2);
                DinoSoundPlaySe(0x98, 0);
            }
            return;
        }
    }
}

DinoUiMedalMenuItem::DinoUiMedalMenuItem(DinoUiMedalMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mMotion->GetUnit()->SetMode(1);
}

// The call-task makers, instantiated ahead of their users so none directly follows a function that calls it
// (DinoUiMedalMenu::OnFocus, not decompiled yet, uses the DinoHelpBase one).
template MTask *MCreateCallTask(DinoUiMedalMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiMedalMenu *obj, void (DinoUiMedalMenu::*func)());
template MTask *MCreateBgCallTask(DinoUiMedalMenu *obj, void (DinoUiMedalMenu::*func)(), int thread);

void DinoUiMedalMenu::PageRight() {
    if (mPage == mLastPage) {
        mPage = 0;
    } else {
        mPage++;
    }
    mSerial.AddTail(MCreateBgCallTask(this, &DinoUiMedalMenu::LoadPage, 1));
    mSerial.AddTail(MCreateCallTask(this, &DinoUiMedalMenu::FinishPageRight));
    Disable();
}

void DinoUiMedalMenu::PageLeft() {
    if (mPage == 0) {
        mPage = mLastPage;
    } else {
        mPage--;
    }
    mSerial.AddTail(MCreateBgCallTask(this, &DinoUiMedalMenu::LoadPage, 1));
    mSerial.AddTail(MCreateCallTask(this, &DinoUiMedalMenu::FinishPageLeft));
    Disable();
}

DinoUiMedalSlotCreateTask::DinoUiMedalSlotCreateTask(DinoUiMedalMenu *menu) : MProcess(NULL) {
    mMenu = menu;
    mIndex = 0;
}

BOOL DinoUiMedalMenu::HasUnk1EC(u32 id) {
    if (id == mTeam.slots[0] || id == mTeam.slots[1] || id == mTeam.slots[2] || id == mTeam.slots[3] || id == mTeam.slots[4]) {
        return TRUE;
    }
    return FALSE;
}

void DinoUiMedalMenu::ClearSlots() {
    for (int i = 0; i < 15; i++) {
        RemoveItem(mSlots[i]);
        delete mSlots[i];
    }
}

void DinoUiMedalMenu::SetSort(u8 sort) {
    if (sort < 5 && mSort != sort) {
        mSort = sort;
        DinoSoundPlaySe(0x98, 0);
        mSortCursor->SetPosition(0, sort * -0xC0);
        ClearSlots();
        Sort(sort);
        Disable();
        MTask *enable = MCreateCallTask(this, &DinoHelpBase::Enable);
        MTask *create = new DinoUiMedalSlotCreateTask(this);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(create);
        serial->AddTail(enable);
    }
}

// The creature lists' sorts and their push_back are the ones ov006_02178b80.cpp emits.
namespace std {
template <>
void stable_sort(__vec_iterator<u32> first, __vec_iterator<u32> last, DinoCreatureByVoice comp);
template <>
void stable_sort(__vec_iterator<u32> first, __vec_iterator<u32> last, DinoCreatureByUnk88 comp);
template <>
void stable_sort(__vec_iterator<u32> first, __vec_iterator<u32> last, DinoCreatureByLevel comp);
template <>
void stable_sort(__vec_iterator<u32> first, __vec_iterator<u32> last, DinoCreatureByUnk0D comp);
template <>
void stable_sort(__vec_iterator<u32> first, __vec_iterator<u32> last, DinoCreatureByUnk02031104 comp);
template <>
void __vec_deleter<u32, allocator<u32> >::sub_push_back(const u32 &x, Metrowerks::int2type<true>);
}  // namespace std

void DinoUiMedalMenu::LoadCreatures() {
    mCreatures.clear();
    for (u32 id = 1; id <= gDinoResource->GetCreatureCount() - 1; id++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(id, &creature);
        if (creature.parts[0].level != 0 ||
            creature.parts[1].level + creature.parts[2].level + creature.parts[3].level != 0) {
            u32 index = creature.index;
            mCreatures.push_back(index);
        }
    }
    DinoCreatureByVoice byVoice;
    std::stable_sort(mCreatures.begin(), mCreatures.end(), byVoice);
    mByVoice = mCreatures;
    mByUnk88 = mCreatures;
    DinoCreatureByUnk88 byUnk88;
    std::stable_sort(mByUnk88.begin(), mByUnk88.end(), byUnk88);
    if (mCreatures.size() == 0) {
        mLastPage = 0;
    } else {
        mLastPage = (mCreatures.size() - 1) / 15;
    }
    mFraction->SetDenominator(mLastPage + 1);
}

// Lists the creatures in the sort's order: 0 and 1 are kept lists, the rest sort mByVoice.
void DinoUiMedalMenu::Sort(u8 sort) {
    if (sort > 4) {
        return;
    }
    switch (sort) {
    case 0:
        mCreatures = mByVoice;
        break;
    case 1:
        mCreatures = mByUnk88;
        break;
    case 2:
        mCreatures = mByVoice;
        {
            DinoCreatureByLevel order;
            std::stable_sort(mCreatures.begin(), mCreatures.end(), order);
        }
        break;
    case 3:
        mCreatures = mByVoice;
        {
            DinoCreatureByUnk02031104 order;
            std::stable_sort(mCreatures.begin(), mCreatures.end(), order);
        }
        break;
    case 4:
        mCreatures = mByVoice;
        {
            DinoCreatureByUnk0D order;
            std::stable_sort(mCreatures.begin(), mCreatures.end(), order);
        }
        break;
    }
}

// Where the page's slots go.

// Makes the page's slot (the first clears the numbers and counts the slots; the last links them).
void DinoUiMedalMenu::CreateSlot(int slot) {
    if (slot == 0) {
        if (mPage == mLastPage) {
            mSlotCount = mCreatures.size() - mPage * 15;
        } else {
            mSlotCount = 15;
        }
        for (int i = 0; i < 15; i++) {
            mNumbers[i]->SetText(std::string("   "));
        }
    }
    if (slot < mSlotCount) {
        u32 id = mCreatures[mPage * 15 + slot];
        DinoCreatureState creature;
        gDinoBackup->GetCreature(id, &creature);
        u8 maxed;
        if (GetCreatureLevel(creature) == 12) {
            maxed = TRUE;
        } else {
            maxed = FALSE;
        }
        mSlots[slot] = new DinoUiPartsReviverMedalButton(
            mOam,
            MPoint(data_ov006_02194058.slotX + data_ov006_02194058.slotColumnX * (slot % 5) +
                       data_ov006_02194058.slotRowX * (slot / 5),
                   data_ov006_02194058.slotY + data_ov006_02194058.slotColumnY * (slot % 5) +
                       data_ov006_02194058.slotRowY * (slot / 5)),
            id, maxed, creature.parts[0].level == 0);
        AddItem(mSlots[slot]);
        if (HasUnk1EC(id)) {
            mSlots[slot]->mMotion->GetUnit()->SetMode(0);
        }
        char number[4];
        sprintf(number, "%03d", gDinoResource->GetCreatureDef(id)->voice);
        mNumbers[slot]->SetText(std::string(number));
    } else {
        mSlots[slot] = new DinoUiPartsReviverMedalButton(
            mOam,
            MPoint(data_ov006_02194058.slotX + data_ov006_02194058.slotColumnX * (slot % 5) +
                       data_ov006_02194058.slotRowX * (slot / 5),
                   data_ov006_02194058.slotY + data_ov006_02194058.slotColumnY * (slot % 5) +
                       data_ov006_02194058.slotRowY * (slot / 5)),
            0xFFFF, TRUE, FALSE);
        AddItem(mSlots[slot]);
    }
    if (slot == 14) {
        AddLink(mSlots[0], 3, mSlots[4]);
        AddLink(mSlots[5], 3, mSlots[9]);
        AddLink(mSlots[10], 3, mSlots[14]);
        AddLink(mSlots[4], 1, mSlots[0]);
        AddLink(mSlots[9], 1, mSlots[5]);
        AddLink(mSlots[14], 1, mSlots[10]);
        if (gDinoBackup->GetSystemFlag(2)) {
            for (int i = 0; i < 5; i++) {
                AddLink(mSlots[i], 4, mUnkB8);
            }
            AddLink(mUnkB8, 2, mSlots[0]);
        }
    }
}

void DinoUiMedalMenu::CreateSlots() {
    for (int i = 0; i < 15; i++) {
        CreateSlot(i);
    }
}

void DinoUiMedalMenu::FinishPageRight() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(GetCursor());
}

void DinoUiMedalMenu::FinishPageLeft() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(GetCursor());
}

#pragma auto_inline on


#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template void std::__vec_deleter<u32, std::allocator<u32> >::reallocate_clear(std::__vec_deleter<u32, std::allocator<u32> >::size_type, std::__vec_deleter<u32, std::allocator<u32> >::size_type);
template void std::__vec_deleter<MMotionFile, std::allocator<MMotionFile> >::sub_push_back(const MMotionFile &x, Metrowerks::int2type<false>);
template void std::__vec_deleter<u32, std::allocator<u32> >::assign<u32 *>(u32 *first, u32 *last, Metrowerks::int2type<true>);

// The list's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x1C];
    void (DinoUiMedalList::*Start_1C)();
    void (DinoUiMedalList::*StateResult_24)();
    void (DinoUiMedalList::*StateMenu_2C)();
    void (DinoUiMedalList::*StateMenu_34)();
} data_ov006_02194158;

DinoUiMedalList::DinoUiMedalList() : MProcess(NULL), mUnk7C(1), mUnk80(0), mUnk84(0), mUnk85(0) {
    mState = data_ov006_02194158.Start_1C;
}

void DinoUiMedalList::StateMenu() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 1, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    if (mUnk7C == 1) {
        mUnk80 += mUnk85 * 15;
    }
    mSerial.AddTail(new DinoUiMedalMenu(&mUnk7C, &mUnk80, &mUnk84));
    mState = data_ov006_02194158.StateResult_24;
}

void DinoUiMedalList::StateResult() {
    if (mUnk7C == 0) {
        gLcd->GetTopDisplay()->mBright->Reset();
        gLcd->GetBottomDisplay()->mBright->Reset();
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 0, 4, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    if (mUnk7C == 1) {
        mUnk85 = mUnk80 / 15;
        mUnk80 = mUnk80 % 15;
        mSerial.AddTail(new DinoUiRvStatusMenu((int *)&mUnk80));
        mState = data_ov006_02194158.StateMenu_2C;
    } else if (mUnk7C == 2) {
        mSerial.AddTail(new DinoUiRvStatusMenu((int *)&mUnk80, mUnk84));
        mState = data_ov006_02194158.StateMenu_34;
    } else {
        Exit();
    }
}

extern const int data_02089aa4, data_02089aa8, data_02089aac, data_02089ab0, data_02089ab4, data_02089ab8;

void DinoUiMedalMenu::OnBackDecided(BOOL result) {
    *mResult = result;
    if (result == 0) {
        MTask *exit = MTaskExitTransfer::Create(&(MTask &)*this);
        MTask *fade = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(data_02089ab4, data_02089ab0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fade);
        GetSerial()->AddTail(new MTimer(data_02089ab8), MTaskSetWaitEmptyTask::Create(mUnk220), fades, exit);
    } else {
        MTask *exit = MTaskExitTransfer::Create(&(MTask &)*this);
        MTask *fade = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(data_02089aa8, data_02089aa4);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fade);
        GetSerial()->AddTail(new MTimer(data_02089aac), MTaskSetWaitEmptyTask::Create(mUnk220), fades, exit);
    }
}

// Moves the cursor to the item (offset for the medals), and shows the medal's creature's name by it.
void DinoUiMedalMenu::OnFocus(MMenuItem *item) {
    mUnk204 = FALSE;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    int j;
    for (j = 0; j < 5; j++) {
        if (item == mUnkF0[j]) {
            frame.Offset(data_ov006_021940d8.cursorOffset);
        }
    }
    for (j = 0; j < 15; j++) {
        if (item == mSlots[j]) {
            frame.Offset(data_ov006_021940d8.cursorOffset);
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
    int i;
    for (i = 0; i < 5; i++) {
        if (item == mUnkF0[i] && mTeam.slots[i] != 0) {
            mName->SetCreature(mTeam.slots[i]);
            mName->SetPosition(data_ov006_021942f4[i] + data_ov006_021940d8.nameOffset);
            Disable();
            mNameTasks->Clear();
            mNameTasks->AddTail(new DinoUiPartsReviverNameCountCheckTask(mName),
                                MCreateCallTask(this, &DinoHelpBase::Enable));
            break;
        }
    }
    if (i == 5) {
        for (i = 0; i < mSlotCount; i++) {
            if (item == mSlots[i]) {
                mName->SetCreature(mCreatures[mPage * 15 + i]);
                mName->SetPosition(MPoint(data_ov006_02194058.slotX + data_ov006_02194058.slotColumnX * (i % 5) +
                                              data_ov006_02194058.slotRowX * (i / 5),
                                          data_ov006_02194058.slotY + data_ov006_02194058.slotColumnY * (i % 5) +
                                              data_ov006_02194058.slotRowY * (i / 5)) +
                                   data_ov006_021940d8.nameOffset);
                Disable();
                mNameTasks->Clear();
                mNameTasks->AddTail(new DinoUiPartsReviverNameCountCheckTask(mName),
                                    MCreateCallTask(this, &DinoHelpBase::Enable));
                break;
            }
        }
        if (i == mSlotCount) {
            mName->Hide();
            mNameTasks->Clear();
            Enable();
        }
    }
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov006_02193f58[];
extern "C" u32 data_ov006_02194358[];
extern "C" u32 data_ov006_02193fd8[];
extern "C" u32 data_ov006_02193e58[];
extern "C" u32 data_ov006_02193ed8[];
extern "C" u32 data_ov006_021943d8[];
extern "C" u32 data_ov006_02194458[];
extern "C" u32 data_ov006_021944d8[];
extern "C" u32 data_ov006_021942d8[];
extern "C" u32 data_ov006_021942fc[];
extern "C" u32 data_ov006_02194304[];
extern "C" u32 data_ov006_0219430c[];
extern "C" u32 data_ov006_02194314[];
extern "C" u32 data_ov006_02194258[];
extern "C" u32 data_ov006_02194284[];
extern "C" u32 data_ov006_0219428c[];
static struct DinoUiMedalMenuStaticInit {
    DinoUiMedalMenuStaticInit() {
        DINO_POOL_WORD(data_ov006_02194058, 0x3c) = 128;
        DINO_POOL_WORD(data_ov006_02194058, 0x40) = 176;
        DINO_POOL_WORD(data_ov006_02194058, 0x44) = 8;
        DINO_POOL_WORD(data_ov006_02194058, 0x48) = 150;
        DINO_POOL_WORD(data_ov006_02194058, 0x4c) = 64;
        DINO_POOL_WORD(data_ov006_02194058, 0x50) = 112;
        DINO_POOL_WORD(data_ov006_02194358, 0x3c) = ((s32 *) &data_ov006_02193f58)[0x34 / 4];
        DINO_POOL_WORD(data_ov006_02194358, 0x40) = ((s32 *) &data_ov006_02193f58)[0x34 / 4];
        DINO_POOL_WORD(data_ov006_02194358, 0x44) = DINO_POOL_WORD(data_ov006_02193fd8, 0xc);
        DINO_POOL_WORD(data_ov006_02194358, 0x48) = DINO_POOL_WORD(data_ov006_02193e58, 0x64);
        DINO_POOL_WORD(data_ov006_02194358, 0x4c) = DINO_POOL_WORD(data_ov006_02193e58, 0x30);
        DINO_POOL_WORD(data_ov006_02194358, 0x50) = DINO_POOL_WORD(data_ov006_02193f58, 0x0);
        DINO_POOL_WORD(data_ov006_02194358, 0x54) = DINO_POOL_WORD(data_ov006_02193e58, 0x40);
        DINO_POOL_WORD(data_ov006_02194358, 0x58) = DINO_POOL_WORD(data_ov006_02193e58, 0x1c);
        DINO_POOL_WORD(data_ov006_02194358, 0x5c) = DINO_POOL_WORD(data_ov006_02193e58, 0x54);
        DINO_POOL_WORD(data_ov006_02194358, 0x60) = DINO_POOL_WORD(data_ov006_02193ed8, 0xc);
        DINO_POOL_WORD(data_ov006_02194358, 0x64) = DINO_POOL_WORD(data_ov006_02193e58, 0x28);
        DINO_POOL_WORD(data_ov006_02194358, 0x68) = DINO_POOL_WORD(data_ov006_02193ed8, 0x2c);
        DINO_POOL_WORD(data_ov006_02194358, 0x6c) = DINO_POOL_WORD(data_ov006_02193e58, 0x48);
        DINO_POOL_WORD(data_ov006_02194358, 0x70) = DINO_POOL_WORD(data_ov006_02193ed8, 0x78);
        DINO_POOL_WORD(data_ov006_02194358, 0x74) = DINO_POOL_WORD(data_ov006_02193fd8, 0x6c);
        DINO_POOL_WORD(data_ov006_02194358, 0x78) = DINO_POOL_WORD(data_ov006_02193ed8, 0x58);
        DINO_POOL_WORD(data_ov006_02194358, 0x7c) = DINO_POOL_WORD(data_ov006_02193fd8, 0x4);
        DINO_POOL_WORD(data_ov006_021943d8, 0x0) = DINO_POOL_WORD(data_ov006_02193e58, 0x38);
        DINO_POOL_WORD(data_ov006_021943d8, 0x4) = DINO_POOL_WORD(data_ov006_02193ed8, 0x74);
        DINO_POOL_WORD(data_ov006_021943d8, 0x8) = DINO_POOL_WORD(data_ov006_02193ed8, 0x40);
        DINO_POOL_WORD(data_ov006_021943d8, 0xc) = DINO_POOL_WORD(data_ov006_02194058, 0x0);
        DINO_POOL_WORD(data_ov006_021943d8, 0x10) = DINO_POOL_WORD(data_ov006_02193f58, 0xc);
        DINO_POOL_WORD(data_ov006_021943d8, 0x14) = DINO_POOL_WORD(data_ov006_02194058, 0x18);
        DINO_POOL_WORD(data_ov006_021943d8, 0x18) = DINO_POOL_WORD(data_ov006_02193f58, 0x8);
        DINO_POOL_WORD(data_ov006_021943d8, 0x1c) = DINO_POOL_WORD(data_ov006_02194058, 0x28);
        DINO_POOL_WORD(data_ov006_021943d8, 0x20) = DINO_POOL_WORD(data_ov006_02194058, 0x2c);
        DINO_POOL_WORD(data_ov006_021943d8, 0x24) = DINO_POOL_WORD(data_ov006_02193e58, 0x10);
        DINO_POOL_WORD(data_ov006_021943d8, 0x28) = DINO_POOL_WORD(data_ov006_02194058, 0x20);
        DINO_POOL_WORD(data_ov006_021943d8, 0x2c) = DINO_POOL_WORD(data_ov006_02193f58, 0x38);
        DINO_POOL_WORD(data_ov006_021943d8, 0x30) = DINO_POOL_WORD(data_ov006_02193e58, 0x2c);
        DINO_POOL_WORD(data_ov006_021943d8, 0x34) = DINO_POOL_WORD(data_ov006_02193e58, 0x20);
        DINO_POOL_WORD(data_ov006_021943d8, 0x38) = DINO_POOL_WORD(data_ov006_02193e58, 0x58);
        DINO_POOL_WORD(data_ov006_021943d8, 0x3c) = DINO_POOL_WORD(data_ov006_02193e58, 0x14);
        DINO_POOL_WORD(data_ov006_021943d8, 0x40) = DINO_POOL_WORD(data_ov006_02193f58, 0x2c);
        DINO_POOL_WORD(data_ov006_021943d8, 0x44) = DINO_POOL_WORD(data_ov006_02193e58, 0x50);
        DINO_POOL_WORD(data_ov006_021943d8, 0x48) = DINO_POOL_WORD(data_ov006_02193f58, 0x30);
        DINO_POOL_WORD(data_ov006_021943d8, 0x4c) = DINO_POOL_WORD(data_ov006_02193f58, 0x5c);
        DINO_POOL_WORD(data_ov006_021943d8, 0x50) = DINO_POOL_WORD(data_ov006_02193ed8, 0x28);
        DINO_POOL_WORD(data_ov006_021943d8, 0x54) = DINO_POOL_WORD(data_ov006_02193e58, 0x44);
        DINO_POOL_WORD(data_ov006_021943d8, 0x58) = DINO_POOL_WORD(data_ov006_02193ed8, 0x1c);
        DINO_POOL_WORD(data_ov006_021943d8, 0x5c) = DINO_POOL_WORD(data_ov006_02193e58, 0x3c);
        DINO_POOL_WORD(data_ov006_021943d8, 0x60) = DINO_POOL_WORD(data_ov006_02193ed8, 0x8);
        DINO_POOL_WORD(data_ov006_021943d8, 0x64) = DINO_POOL_WORD(data_ov006_02193f58, 0x48);
        DINO_POOL_WORD(data_ov006_021943d8, 0x68) = DINO_POOL_WORD(data_ov006_02193f58, 0x40);
        DINO_POOL_WORD(data_ov006_021943d8, 0x6c) = DINO_POOL_WORD(data_ov006_02193f58, 0x1c);
        DINO_POOL_WORD(data_ov006_021943d8, 0x70) = DINO_POOL_WORD(data_ov006_02193e58, 0x70);
        DINO_POOL_WORD(data_ov006_021943d8, 0x74) = DINO_POOL_WORD(data_ov006_02193ed8, 0x34);
        DINO_POOL_WORD(data_ov006_021943d8, 0x78) = DINO_POOL_WORD(data_ov006_02193ed8, 0x24);
        DINO_POOL_WORD(data_ov006_021943d8, 0x7c) = DINO_POOL_WORD(data_ov006_02193f58, 0x7c);
        DINO_POOL_WORD(data_ov006_02194458, 0x0) = DINO_POOL_WORD(data_ov006_02193fd8, 0x1c);
        DINO_POOL_WORD(data_ov006_02194458, 0x4) = DINO_POOL_WORD(data_ov006_02193ed8, 0x64);
        DINO_POOL_WORD(data_ov006_02194458, 0x8) = DINO_POOL_WORD(data_ov006_02193e58, 0x6c);
        DINO_POOL_WORD(data_ov006_02194458, 0xc) = DINO_POOL_WORD(data_ov006_02193f58, 0x68);
        DINO_POOL_WORD(data_ov006_02194458, 0x10) = DINO_POOL_WORD(data_ov006_02193f58, 0x60);
        DINO_POOL_WORD(data_ov006_02194458, 0x14) = DINO_POOL_WORD(data_ov006_02193ed8, 0x14);
        DINO_POOL_WORD(data_ov006_02194458, 0x18) = DINO_POOL_WORD(data_ov006_02193e58, 0x78);
        DINO_POOL_WORD(data_ov006_02194458, 0x1c) = DINO_POOL_WORD(data_ov006_02193fd8, 0x8);
        DINO_POOL_WORD(data_ov006_02194458, 0x20) = DINO_POOL_WORD(data_ov006_02193ed8, 0x50);
        DINO_POOL_WORD(data_ov006_02194458, 0x24) = DINO_POOL_WORD(data_ov006_02193fd8, 0x18);
        DINO_POOL_WORD(data_ov006_02194458, 0x28) = DINO_POOL_WORD(data_ov006_02193fd8, 0x78);
        DINO_POOL_WORD(data_ov006_02194458, 0x2c) = DINO_POOL_WORD(data_ov006_02193ed8, 0x48);
        DINO_POOL_WORD(data_ov006_02194458, 0x30) = DINO_POOL_WORD(data_ov006_02193f58, 0x78);
        DINO_POOL_WORD(data_ov006_02194458, 0x34) = DINO_POOL_WORD(data_ov006_02193e58, 0x4c);
        DINO_POOL_WORD(data_ov006_02194458, 0x38) = DINO_POOL_WORD(data_ov006_02193fd8, 0x44);
        DINO_POOL_WORD(data_ov006_02194458, 0x3c) = DINO_POOL_WORD(data_ov006_02193fd8, 0x4c);
        DINO_POOL_WORD(data_ov006_02194458, 0x40) = DINO_POOL_WORD(data_ov006_02193fd8, 0x30);
        DINO_POOL_WORD(data_ov006_02194458, 0x44) = DINO_POOL_WORD(data_ov006_02193fd8, 0x5c);
        DINO_POOL_WORD(data_ov006_02194458, 0x48) = DINO_POOL_WORD(data_ov006_02193fd8, 0x64);
        DINO_POOL_WORD(data_ov006_02194458, 0x4c) = DINO_POOL_WORD(data_ov006_02193ed8, 0x6c);
        DINO_POOL_WORD(data_ov006_02194458, 0x50) = DINO_POOL_WORD(data_ov006_02193fd8, 0x50);
        DINO_POOL_WORD(data_ov006_02194458, 0x54) = DINO_POOL_WORD(data_ov006_02193fd8, 0x58);
        DINO_POOL_WORD(data_ov006_02194458, 0x58) = DINO_POOL_WORD(data_ov006_02193f58, 0x64);
        DINO_POOL_WORD(data_ov006_02194458, 0x5c) = DINO_POOL_WORD(data_ov006_02193f58, 0x70);
        DINO_POOL_WORD(data_ov006_02194458, 0x60) = DINO_POOL_WORD(data_ov006_02193ed8, 0x7c);
        DINO_POOL_WORD(data_ov006_02194458, 0x64) = DINO_POOL_WORD(data_ov006_02193ed8, 0x10);
        DINO_POOL_WORD(data_ov006_02194458, 0x68) = DINO_POOL_WORD(data_ov006_02193ed8, 0x54);
        DINO_POOL_WORD(data_ov006_02194458, 0x6c) = DINO_POOL_WORD(data_ov006_02193e58, 0x74);
        DINO_POOL_WORD(data_ov006_02194458, 0x70) = DINO_POOL_WORD(data_ov006_02193f58, 0x74);
        DINO_POOL_WORD(data_ov006_02194458, 0x74) = DINO_POOL_WORD(data_ov006_02193fd8, 0x2c);
        DINO_POOL_WORD(data_ov006_02194458, 0x78) = DINO_POOL_WORD(data_ov006_02193f58, 0x18);
        DINO_POOL_WORD(data_ov006_02194458, 0x7c) = DINO_POOL_WORD(data_ov006_02193f58, 0x20);
        DINO_POOL_WORD(data_ov006_021944d8, 0x0) = DINO_POOL_WORD(data_ov006_02193ed8, 0x68);
        DINO_POOL_WORD(data_ov006_021944d8, 0x4) = DINO_POOL_WORD(data_ov006_02193ed8, 0x38);
        DINO_POOL_WORD(data_ov006_021944d8, 0x8) = DINO_POOL_WORD(data_ov006_02193f58, 0x54);
        DINO_POOL_WORD(data_ov006_021944d8, 0xc) = DINO_POOL_WORD(data_ov006_02193e58, 0xc);
        DINO_POOL_WORD(data_ov006_021944d8, 0x10) = DINO_POOL_WORD(data_ov006_02193fd8, 0x10);
        DINO_POOL_WORD(data_ov006_021944d8, 0x14) = DINO_POOL_WORD(data_ov006_02193f58, 0x50);
        DINO_POOL_WORD(data_ov006_021944d8, 0x18) = DINO_POOL_WORD(data_ov006_02193f58, 0x58);
        DINO_POOL_WORD(data_ov006_021944d8, 0x1c) = DINO_POOL_WORD(data_ov006_02193f58, 0x14);
        DINO_POOL_WORD(data_ov006_021944d8, 0x20) = DINO_POOL_WORD(data_ov006_02193fd8, 0x0);
        DINO_POOL_WORD(data_ov006_021944d8, 0x24) = DINO_POOL_WORD(data_ov006_02193ed8, 0x18);
        DINO_POOL_WORD(data_ov006_021944d8, 0x28) = DINO_POOL_WORD(data_ov006_02193ed8, 0x30);
        DINO_POOL_WORD(data_ov006_021944d8, 0x2c) = DINO_POOL_WORD(data_ov006_02193fd8, 0x14);
        DINO_POOL_WORD(data_ov006_021944d8, 0x30) = DINO_POOL_WORD(data_ov006_02193ed8, 0x5c);
        DINO_POOL_WORD(data_ov006_021944d8, 0x34) = DINO_POOL_WORD(data_ov006_02193e58, 0x5c);
        DINO_POOL_WORD(data_ov006_021944d8, 0x38) = DINO_POOL_WORD(data_ov006_02193fd8, 0x28);
        DINO_POOL_WORD(data_ov006_021944d8, 0x3c) = DINO_POOL_WORD(data_ov006_02193fd8, 0x74);
        DINO_POOL_WORD(data_ov006_021944d8, 0x40) = DINO_POOL_WORD(data_ov006_02194058, 0x10);
        DINO_POOL_WORD(data_ov006_021944d8, 0x44) = DINO_POOL_WORD(data_ov006_02193ed8, 0x0);
        DINO_POOL_WORD(data_ov006_021944d8, 0x48) = DINO_POOL_WORD(data_ov006_02193e58, 0x24);
        DINO_POOL_WORD(data_ov006_021944d8, 0x4c) = DINO_POOL_WORD(data_ov006_02193f58, 0x6c);
        DINO_POOL_WORD(data_ov006_021944d8, 0x50) = DINO_POOL_WORD(data_ov006_02193fd8, 0x48);
        DINO_POOL_WORD(data_ov006_021944d8, 0x54) = DINO_POOL_WORD(data_ov006_02193fd8, 0x68);
        DINO_POOL_WORD(data_ov006_021944d8, 0x58) = DINO_POOL_WORD(data_ov006_02193fd8, 0x34);
        DINO_POOL_WORD(data_ov006_021944d8, 0x5c) = DINO_POOL_WORD(data_ov006_02194058, 0x4);
        DINO_POOL_WORD(data_ov006_021944d8, 0x60) = DINO_POOL_WORD(data_ov006_02194058, 0xc);
        DINO_POOL_WORD(data_ov006_021944d8, 0x64) = DINO_POOL_WORD(data_ov006_02194058, 0x1c);
        DINO_POOL_WORD(data_ov006_021942d8, 0x44) = ((s32 *) &data_ov006_02194058)[0x24 / 4];
        DINO_POOL_WORD(data_ov006_021942d8, 0x48) = ((s32 *) &data_ov006_02194058)[0x24 / 4];
        DINO_POOL_WORD(data_ov006_021942d8, 0x4c) = DINO_POOL_WORD(data_ov006_02194058, 0x30);
        DINO_POOL_WORD(data_ov006_021942d8, 0x50) = DINO_POOL_WORD(data_ov006_02193e58, 0x8);
        DINO_POOL_WORD(data_ov006_021942d8, 0x54) = DINO_POOL_WORD(data_ov006_02193ed8, 0x44);
        DINO_POOL_WORD(data_ov006_021942d8, 0x58) = DINO_POOL_WORD(data_ov006_02193e58, 0x18);
        DINO_POOL_WORD(data_ov006_021942d8, 0x5c) = DINO_POOL_WORD(data_ov006_02194058, 0x14);
        DINO_POOL_WORD(data_ov006_021942d8, 0x60) = DINO_POOL_WORD(data_ov006_02193f58, 0x3c);
        DINO_POOL_WORD(data_ov006_021942d8, 0x64) = DINO_POOL_WORD(data_ov006_02193fd8, 0x70);
        DINO_POOL_WORD(data_ov006_021942d8, 0x68) = DINO_POOL_WORD(data_ov006_02193e58, 0x34);
        DINO_POOL_WORD(data_ov006_021942d8, 0x6c) = DINO_POOL_WORD(data_ov006_02193f58, 0x4c);
        DINO_POOL_WORD(data_ov006_021942d8, 0x70) = DINO_POOL_WORD(data_ov006_02193ed8, 0x4);
        DINO_POOL_WORD(data_ov006_021942d8, 0x74) = DINO_POOL_WORD(data_ov006_02193e58, 0x7c);
        DINO_POOL_WORD(data_ov006_021942d8, 0x78) = DINO_POOL_WORD(data_ov006_02193e58, 0x68);
        DINO_POOL_WORD(data_ov006_021942d8, 0x7c) = DINO_POOL_WORD(data_ov006_02193e58, 0x60);
        DINO_POOL_WORD(data_ov006_02194358, 0x0) = DINO_POOL_WORD(data_ov006_02193f58, 0x44);
        DINO_POOL_WORD(data_ov006_02194358, 0x4) = DINO_POOL_WORD(data_ov006_02193f58, 0x4);
        DINO_POOL_WORD(data_ov006_02194358, 0x8) = DINO_POOL_WORD(data_ov006_02193ed8, 0x70);
        DINO_POOL_WORD(data_ov006_02194358, 0xc) = DINO_POOL_WORD(data_ov006_02193fd8, 0x24);
        DINO_POOL_WORD(data_ov006_02194358, 0x10) = DINO_POOL_WORD(data_ov006_02193ed8, 0x4c);
        DINO_POOL_WORD(data_ov006_02194358, 0x14) = DINO_POOL_WORD(data_ov006_02193fd8, 0x3c);
        DINO_POOL_WORD(data_ov006_02194358, 0x18) = DINO_POOL_WORD(data_ov006_02193fd8, 0x40);
        DINO_POOL_WORD(data_ov006_02194358, 0x1c) = DINO_POOL_WORD(data_ov006_02193fd8, 0x54);
        DINO_POOL_WORD(data_ov006_02194358, 0x20) = DINO_POOL_WORD(data_ov006_02193fd8, 0x60);
        DINO_POOL_WORD(data_ov006_02194358, 0x24) = DINO_POOL_WORD(data_ov006_02193ed8, 0x20);
        DINO_POOL_WORD(data_ov006_02194358, 0x28) = DINO_POOL_WORD(data_ov006_02193f58, 0x10);
        DINO_POOL_WORD(data_ov006_02194358, 0x2c) = DINO_POOL_WORD(data_ov006_02193f58, 0x28);
        DINO_POOL_WORD(data_ov006_02194358, 0x30) = DINO_POOL_WORD(data_ov006_02193fd8, 0x7c);
        DINO_POOL_WORD(data_ov006_02194358, 0x34) = DINO_POOL_WORD(data_ov006_02193ed8, 0x3c);
        DINO_POOL_WORD(data_ov006_02194358, 0x38) = DINO_POOL_WORD(data_ov006_02193ed8, 0x60);
        DINO_POOL_WORD(data_ov006_02194058, 0x54) = 43;
        DINO_POOL_WORD(data_ov006_02194058, 0x58) = 80;
        DINO_POOL_WORD(data_ov006_02194058, 0x5c) = 45;
        DINO_POOL_WORD(data_ov006_02194058, 0x60) = 0;
        DINO_POOL_WORD(data_ov006_02194058, 0x64) = 0;
        DINO_POOL_WORD(data_ov006_02194058, 0x68) = 32;
        DINO_POOL_WORD(data_ov006_02194058, 0x6c) = -27;
        DINO_POOL_WORD(data_ov006_02194058, 0x70) = -14;
        DINO_POOL_WORD(data_ov006_02194058, 0x74) = 40;
        DINO_POOL_WORD(data_ov006_02194058, 0x78) = 4;
        DINO_POOL_WORD(data_ov006_02194058, 0x7c) = 19;
        DINO_POOL_WORD(data_ov006_021940d8, 0x0) = 179;
        DINO_POOL_WORD(data_ov006_021940d8, 0x4) = 27;
        DINO_POOL_WORD(data_ov006_021940d8, 0x8) = 0;
        DINO_POOL_WORD(data_ov006_021940d8, 0xc) = 19;
        DINO_POOL_WORD(data_ov006_021940d8, 0x10) = 158;
        DINO_POOL_WORD(data_ov006_021940d8, 0x14) = 45;
        DINO_POOL_WORD(data_ov006_021940d8, 0x18) = 0;
        DINO_POOL_WORD(data_ov006_021940d8, 0x1c) = 128;
        DINO_POOL_WORD(data_ov006_021940d8, 0x20) = 96;
        DINO_POOL_WORD(data_ov006_021940d8, 0x24) = 121;
        DINO_POOL_WORD(data_ov006_021940d8, 0x28) = 116;
        DINO_POOL_WORD(data_ov006_021940d8, 0x2c) = 135;
        DINO_POOL_WORD(data_ov006_021940d8, 0x30) = 116;
        DINO_POOL_WORD(data_ov006_021940d8, 0x34) = 247;
        DINO_POOL_WORD(data_ov006_021940d8, 0x38) = 151;
        DINO_POOL_WORD(data_ov006_021940d8, 0x3c) = 176;
        DINO_POOL_WORD(data_ov006_021940d8, 0x40) = 180;
        DINO_POOL_WORD(data_ov006_021942d8, 0x1c) = 51;
        DINO_POOL_WORD(data_ov006_021942d8, 0x20) = 41;
        DINO_POOL_WORD(data_ov006_021942fc, 0x0) = 96;
        DINO_POOL_WORD(data_ov006_021942fc, 0x4) = 41;
        DINO_POOL_WORD(data_ov006_02194304, 0x0) = 133;
        DINO_POOL_WORD(data_ov006_02194304, 0x4) = 41;
        volatile s32 *p = (volatile s32 *) &data_ov006_0219430c;
        p[0x0 / 4] = 183;
        p[0x4 / 4] = 41;
        p = (volatile s32 *) &data_ov006_02194314;
        p[0x0 / 4] = 219;
        p[0x4 / 4] = 41;
        DINO_POOL_WORD(data_ov006_021940d8, 0x44) = -40;
        DINO_POOL_WORD(data_ov006_021940d8, 0x48) = 28;
        DINO_POOL_WORD(data_ov006_021940d8, 0x4c) = -32;
        DINO_POOL_WORD(data_ov006_021940d8, 0x50) = 8;
        DINO_POOL_WORD(data_ov006_021940d8, 0x54) = -6;
        DINO_POOL_WORD(data_ov006_021940d8, 0x58) = 12;
        DINO_POOL_WORD(data_ov006_02194258, 0x24) = 51;
        DINO_POOL_WORD(data_ov006_02194258, 0x28) = 41;
        DINO_POOL_WORD(data_ov006_02194284, 0x0) = 96;
        DINO_POOL_WORD(data_ov006_02194284, 0x4) = 41;
        DINO_POOL_WORD(data_ov006_0219428c, 0x0) = 133;
        DINO_POOL_WORD(data_ov006_0219428c, 0x4) = 41;
    }
} sDinoUiMedalMenuStaticInit;
