// DinoUiMuseum.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include <algorithm>
#include "m/picture.h"
#include "nitro/fx.h"
#include "m/3d_scene.h"
#include "m/seed_generator.h"
#include "game/dino_ui_museum_information_menu.h"
#include "types.h"
#include "game/dino_ui_museum.h"
#include "game/dino_ui_parts_reviver_name_count_check_task.h"
#include "game/dino_ui_parts_reviver_name.h"
#include "game/dino_ui_field_menu_cursor.h"
#include "game/dino_ui_field_menu_fraction.h"
#include "m/lcd.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/bg.h"
#include "m/obj.h"
#include "game/dino_sound.h"
#include "game/dino_ui_museum_3d_model.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/anm_model.h"
#include "m/irq.h"
#include "m/console.h"
#include "m/indicator.h"
#include <stdio.h>
#include "game/dino_guide_message.h"
#include "game/dino_resource.h"
#include "game/dino_creature_sort.h"
#include "game/dino_backup.h"

DinoUiMuseumInformationMenu::~DinoUiMuseumInformationMenu() {
}

DinoUiMuseum::~DinoUiMuseum() {
}

DinoUiMuseumEffectLight::~DinoUiMuseumEffectLight() {
}

DinoUiMuseumMenuItem::~DinoUiMuseumMenuItem() {
}

DinoUiMuseumBackButton::~DinoUiMuseumBackButton() {
}

DinoUiMuseumPageLButton::~DinoUiMuseumPageLButton() {
}

DinoUiMuseumPageRButton::~DinoUiMuseumPageRButton() {
}

DinoUiMuseumSlotCreateTask::~DinoUiMuseumSlotCreateTask() {
}

DinoUiMuseumInformationMenuItem::~DinoUiMuseumInformationMenuItem() {
}

DinoUiMuseumInformationBackButton::~DinoUiMuseumInformationBackButton() {
}

DinoUiMuseumInformationPageLButton::~DinoUiMuseumInformationPageLButton() {
}

DinoUiMuseumInformationPageRButton::~DinoUiMuseumInformationPageRButton() {
}

DinoUiMuseumInformationVoiceButton::~DinoUiMuseumInformationVoiceButton() {
}

void DinoUiMuseumInformationMenu::OnIdle() {
}

void DinoUiMuseumInformationMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiMuseumInformationMenuItem::OnFocusChanged(BOOL value) {
}

void DinoUiMuseumMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiMuseumInformationMenuItem::GetRect() {
    return GetFrame();
}

MRect DinoUiMuseumMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMuseumInformationMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiMuseumMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiMuseumInformationMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiMuseumMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiMuseumInformationBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiMuseumBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

const char *DinoUiMuseumInformationMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiMuseumMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiMuseumInformationBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiMuseumInformationPageRButton::OnDecide() {
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

void DinoUiMuseumInformationPageLButton::OnDecide() {
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

DinoUiMuseum::DinoUiMuseum() : MProcess(gLcd->mSub) {
}

void DinoUiMuseum::Idle() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 0, 3, 1, 1, 1);
    gLcd->mMain->mBg->SetBg0Reserved(1);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
    GetSerial()->AddTail(new DinoUiMuseumMenu, MTaskExitTransfer::Create(this));
}

DinoUiMuseumReviverButton::~DinoUiMuseumReviverButton() {
}

MRect DinoUiMuseumReviverButton::GetFrame() {
    return mMotion->GetFrame();
}


void DinoUiMuseumBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiMuseumReviverButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(mBaseAnime + 3, 0);
    } else {
        mMotion->Play(mBaseAnime + 1, 0);
    }
    mSerial.Clear();
}


void DinoUiMuseumInformationVoiceButton::OnDecide() {
    mMotion->Play(2, 1);
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    mOwner->PlayVoice();
}


void DinoUiMuseumPageLButton::OnDecide() {
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

void DinoUiMuseumPageRButton::OnDecide() {
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

extern "C" const int data_02089a0c;

#pragma auto_inline off

DinoUiMuseumInformationMenuItem::DinoUiMuseumInformationMenuItem(DinoUiMuseumInformationMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(0);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

DinoUiMuseumMenuItem::DinoUiMuseumMenuItem(DinoUiMuseumMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

DinoUiMuseumMenu::DinoUiMuseumMenu()
    : MMenu(gLcd->mSub, NULL, 0), mPicture(NULL), mPage(0), mPageCount(0), mTab(0), mSelected(0), mUnk1AC(FALSE) {
    mMotions.clear();
}

DinoUiMuseumMenu::~DinoUiMuseumMenu() {
    if (mPicture != NULL) {
        delete mPicture;
    }
}

static inline const DinoMuseumDef *GetMuseumDef(const DinoMuseumDefs *defs, u32 id) {
    return (const DinoMuseumDef *)((const u8 *)defs + defs->tableOffset) + id;
}

// OnStart's function-local static and its guard come first in the file's .bss (OnStart isn't decompiled yet).
static u32 sOnStartStatic[2];
// The museum's definitions, held in a static object (made and freed by the file's static initializer).
static MResource<DinoMuseumDefs> sMuseumDefs;

// The tabs' orders.
struct DinoMuseumByUnk0C {
    static u32 Key(u32 id) { return GetMuseumDef(sMuseumDefs.Get(), id)->unk0C; }
    bool operator()(const u32 &a, const u32 &b) const {
        u32 x = Key(a);
        return x > Key(b);
    }
};
struct DinoMuseumByUnk0A {
    static u32 Key(u32 id) { return GetMuseumDef(sMuseumDefs.Get(), id)->unk0A; }
    bool operator()(const u32 &a, const u32 &b) const {
        u32 x = Key(a);
        return x < Key(b);
    }
};
struct DinoMuseumByUnk08 {
    static u32 Key(u32 id) { return GetMuseumDef(sMuseumDefs.Get(), id)->unk08; }
    bool operator()(const u32 &a, const u32 &b) const {
        u32 x = Key(a);
        return x > Key(b);
    }
};

// The creature lists' sorts and copies are the ones the medal menu and the revive status menu emit.
namespace std {
template <>
void stable_sort(__vec_iterator<u32> first, __vec_iterator<u32> last, DinoCreatureByVoice comp);
template <>
void stable_sort(__vec_iterator<u32> first, __vec_iterator<u32> last, DinoCreatureByUnk88 comp);
template <>
template <>
void __vec_deleter<u32, allocator<u32> >::assign(u32 *first, u32 *last, Metrowerks::int2type<true>);
}  // namespace std

// NONMATCHING: clear()'s tag byte takes another stack slot; the medal menu's identical code matches under
// `#pragma ipa file`, which this file (likely two of the game's) can't take as a whole.
void DinoUiMuseumMenu::LoadCreatures() {
    mCreatures.clear();
    for (u32 id = 1; id <= gDinoResource->GetCreatureCount() - 1; id++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(id, &creature);
        if (creature.parts[0].level != 0) {
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
        mPageCount = 0;
    } else {
        mPageCount = (mCreatures.size() - 1) / 15;
    }
    mFraction->SetDenominator(mPageCount + 1);
}

// The tab's sort (the game emits the whole stable_sort machinery here).
// NONMATCHING: std::__inplace_merge (stack layout, and the game elides one argument copy), std::__stable_sort_move
// (comp and the buffer's owner swap registers), std::__insertion_sort_move (the first few instructions' order).
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoMuseumByUnk0C);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoMuseumByUnk0A);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoMuseumByUnk08);

// Lists the creatures in the tab's order: 0 and 1 are kept lists, the rest sort mByVoice.
void DinoUiMuseumMenu::LoadTab(u8 tab) {
    if (tab > 4) {
        return;
    }
    switch (tab) {
    case 0:
        mCreatures = mByVoice;
        break;
    case 1:
        mCreatures = mByUnk88;
        break;
    case 2:
        mCreatures = mByVoice;
        {
            DinoMuseumByUnk08 order;
            std::stable_sort(mCreatures.begin(), mCreatures.end(), order);
        }
        break;
    case 3:
        mCreatures = mByVoice;
        {
            DinoMuseumByUnk0A order;
            std::stable_sort(mCreatures.begin(), mCreatures.end(), order);
        }
        break;
    case 4:
        mCreatures = mByVoice;
        {
            DinoMuseumByUnk0C order;
            std::stable_sort(mCreatures.begin(), mCreatures.end(), order);
        }
        break;
    }
}


void DinoUiMuseumMenu::ClearSlots() {
    for (int i = 0; i < 15; i++) {
        RemoveItem(mSlots[i]);
        delete mSlots[i];
    }
    RemoveLink(mLinkFrom, 2);
}

// The top row goes up to the tabs, and the rows wrap around.
void DinoUiMuseumMenu::LinkSlots() {
    for (int i = 0; i < 5; i++) {
        AddLink(mSlots[i], 4, mLinkFrom);
    }
    AddLink(mSlots[0], 3, mSlots[4]);
    AddLink(mSlots[5], 3, mSlots[9]);
    AddLink(mSlots[10], 3, mSlots[14]);
    AddLink(mSlots[4], 1, mSlots[0]);
    AddLink(mSlots[9], 1, mSlots[5]);
    AddLink(mSlots[14], 1, mSlots[10]);
    AddLink(mLinkFrom, 2, mSlots[0]);
}

void DinoUiMuseumMenu::CreateSlots() {
    for (int i = 0; i < 15; i++) {
        CreateSlot(i);
    }
}

// Loads the motions of the page's slots.
void DinoUiMuseumMenu::LoadPageMotions() {
    int count;
    if (mPage == mPageCount) {
        count = mCreatures.size() - mPage * 15;
    } else {
        count = 15;
    }
    for (int i = 0; i < count; i++) {
        char path[0x100];
        OS_SPrintf(path, "/motion/ui_reviver_medal/reviver_%03d.bin", mCreatures[mPage * 15 + i]);
        mMotions.push_back(MMotionFile(std::string(path), 7));
    }
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoUiMuseum::*FinishPageRight_00)();
} data_ov006_02196234;
extern "C" struct {
    void (DinoUiMuseum::*LoadPageMotions_00)();
} data_ov006_02196274;
extern "C" struct {
    void (DinoUiMuseum::*LoadPageMotions_00)();
} data_ov006_02196254;
extern "C" struct {
    void (DinoUiMuseum::*FinishPageLeft_00)();
} data_ov006_0219627c;

// Turns to the next page (wrapping around), loading its motions in the background.
void DinoUiMuseumMenu::PageRight() {
    if (mPage == mPageCount) {
        mPage = 0;
    } else {
        mPage++;
    }
    mMotions.clear();
    mSerial.AddTail(MCreateBgCallTask((DinoUiMuseum *)this, data_ov006_02196274.LoadPageMotions_00, 1));
    mSerial.AddTail(MCreateCallTask((DinoUiMuseum *)this, data_ov006_02196234.FinishPageRight_00));
    Disable();
}

void DinoUiMuseumMenu::FinishPageRight() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(mCursor->GetPosition());
}

// Turns to the previous page (wrapping around), loading its motions in the background.
void DinoUiMuseumMenu::PageLeft() {
    if (mPage == 0) {
        mPage = mPageCount;
    } else {
        mPage--;
    }
    mMotions.clear();
    mSerial.AddTail(MCreateBgCallTask((DinoUiMuseum *)this, data_ov006_02196254.LoadPageMotions_00, 1));
    mSerial.AddTail(MCreateCallTask((DinoUiMuseum *)this, data_ov006_0219627c.FinishPageLeft_00));
    Disable();
}

void DinoUiMuseumMenu::FinishPageLeft() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(mCursor->GetPosition());
}

DinoUiMuseumSlotCreateTask::DinoUiMuseumSlotCreateTask(DinoUiMuseumMenu *menu) : MProcess(NULL) {
    mMenu = menu;
    mIndex = 0;
}

#pragma auto_inline on

void DinoUiMuseumSlotCreateTask::Idle() {
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

// Shows the page's creature: its number, element, name, description and model.
void DinoUiMuseumInformationMenu::Refresh() {
    u32 id = mInfo->mCreatures[*mPage];
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(id);
    char number[4];
    sprintf(number, "%03d", def->voice);
    mNumber->SetText(std::string(number));
    for (int i = 0; i < 6; i++) {
        mTypes[i]->Hide();
    }
    switch (def->unk0D) {
    case 1:
        mTypes[0]->Show();
        break;
    case 2:
        mTypes[1]->Show();
        break;
    case 3:
        mTypes[2]->Show();
        break;
    case 4:
        mTypes[3]->Show();
        break;
    case 5:
        mTypes[4]->Show();
        break;
    case 0:
    default:
        mTypes[5]->Show();
        break;
    }
    mName->SetText(std::string(gDinoResource->GetCreatureText08(id)));
    mDescription->Clear();
    mDescription->Print(std::string(gDinoResource->GetText(GetMuseumDef(gDinoResource->LoadMuseumDefs().Get(), id)->text)));
    mModel->SetModel(id);
}

void DinoUiMuseumInformationMenu::PlayVoice() {
    DinoSoundPlaySe(gDinoResource->GetCreatureDef(mInfo->mCreatures[*mPage])->voice + 0x475, 0);
}

void DinoUiMuseumInformationMenu::OnBackDecided() {
    MTask *fade;
    MTask *exit;
    MTask &self = *this;
    exit = MTaskExitTransfer::Create(&self);
    fade = gLcd->mSub->mBright->CreateFadeOutToBlack(5, 0);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(fade);
    serial->AddTail(exit);
}

void DinoUiMuseumInformationMenu::PageRight() {
    *mPage = (*mPage + 1) % mInfo->mPageCount;
    Refresh();
}

void DinoUiMuseumInformationMenu::PageLeft() {
    *mPage = (*mPage + mInfo->mPageCount - 1) % mInfo->mPageCount;
    Refresh();
}

void DinoUiMuseumMenu::OnIdle() {
    mUnk1AC = TRUE;
}

// DinoUiMuseum3DModel::Idle and DinoUiMuseum's page turns (not decompiled yet) start background loads.
#pragma auto_inline off
template MTask *MCreateBgCallTask(DinoUiMuseum3DModel *obj, void (DinoUiMuseum3DModel::*func)(), int thread);
#pragma auto_inline reset
template MTask *MCreateBgCallTask(DinoUiMuseum *obj, void (DinoUiMuseum::*func)(), int thread);
// DinoUiMuseumMenu::OnFocus (not decompiled yet) re-enables the help through a call task.
// The menu's layout (filled in at start).
struct DinoUiMuseumLayout {
    u8 unk00[0x34];
    MPoint column;      // 0x34: from a slot to the next in its row
    u8 unk3C[0x44 - 0x3C];
    MPoint row;         // 0x44: to the next row
    u8 unk4C[0x64 - 0x4C];
    MPoint first;       // 0x64: the first slot
    u8 unk6C[0x7C - 0x6C];
    MPoint nameOffset;  // 0x7C: of the creature's name from its slot
    u8 unk84[0xB4 - 0x84];
    MPoint focusOffset; // 0xB4: of the cursor on a slot
};
extern DinoUiMuseumLayout data_ov006_02196208;
extern MPoint data_020b5858[3];

// Moves the cursor to the item; a slot also shows its creature's name (and model) and restarts the help.
void DinoUiMuseumMenu::OnFocus(MMenuItem *item) {
    mUnk1AC = FALSE;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    int i;
    for (i = 0; i < 5; i++) {
        if (item == mTabs[i]) {
            SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
        }
    }
    if (item == mLinkFrom) {
        SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    }
    MRect frame = item->GetFrame();
    for (i = 0; i < 15; i++) {
        if (item == mSlots[i]) {
            frame.Offset(data_ov006_02196208.focusOffset);
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(frame);
    } else {
        mCursor->SetMode(1);
        mCursor->Show();
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    for (i = 0; i < mSlotCount; i++) {
        if (item == mSlots[i]) {
            mName->SetCreature(mCreatures[mPage * 15 + i]);
            mName->SetPosition(MPoint(data_ov006_02196208.first.x + data_ov006_02196208.column.x * (i % 5) +
                                          data_ov006_02196208.row.x * (i / 5),
                                      data_ov006_02196208.first.y + data_ov006_02196208.column.y * (i % 5) +
                                          data_ov006_02196208.row.y * (i / 5)) +
                               data_ov006_02196208.nameOffset);
            Disable();
            mHelpTasks->Clear();
            mHelpTasks->AddTail(new DinoUiPartsReviverNameCountCheckTask(mName),
                                MCreateCallTask(this, &DinoHelpBase::Enable));
            mModel->SetModel(mCreatures[mPage * 15 + i]);
            return;
        }
    }
    mName->Hide();
    mHelpTasks->Clear();
    Enable();
    mModel->SetModel(0);
}

#pragma auto_inline off
template MTask *MCreateCallTask(DinoUiMuseumMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiMuseum *obj, void (DinoUiMuseum::*func)());
#pragma auto_inline reset

void DinoUiMuseum3DModel::LoadModel() {
    char path[0x100];
    OS_SPrintf(path, "/model/battle/din%03d_07", mCurrent);
    mModel = new MAnmModel(mScene);
    mModel->SetFrames(0, 0x40000000);
    mModel->Load(path, 0);
    mModel->Run();
    mModel->mUnkF4 = 1;
    mModel->mFollowParent = TRUE;
    MIrqLock lock;
    mTasks->AddTail(mModel);
}

void DinoUiMuseum3DModel::SetModel(u32 id) {
    if (mRequested != id) {
        mRequested = id;
    }
}

extern "C" struct {
    void (DinoUiMuseum3DModel::*LoadModel_00)();
} data_ov006_02196264;

#pragma auto_inline off
// Loads the model asked for (placing the camera for its size), then circles the camera around it and slides the
// background in.
void DinoUiMuseum3DModel::Idle() {
    if (mCurrent != mRequested) {
        mCurrent = mRequested;
        mSerial.AddTail(MTaskSetMakeEmptyTask::Create(mTasks));
        if (mCurrent == 0) {
            return;
        }
        mTimer = 0;
        u32 angle = 0xFFFFF000;
        mAngle = angle;
        mHeight = MFx32(5);
        mDistance = MFx32(11);
        mAngle = angle * 2;
        switch (mCurrent) {
        case 0x29:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x69:
            mDistance = MFx32(15);
            mHeight = MFx32(7);
            break;
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x54:
            mDistance = MFx32(18);
            mHeight = MFx32(7);
            break;
        }
        mSerial.AddTail(MCreateBgCallTask(this, data_ov006_02196264.LoadModel_00, 1));
    }
    VecFx32 v;
    v.x = 0;
    v.y = mHeight.v;
    v.z = 0;
    mScene->SetCameraTarget(&v);
    v.x = (mDistance * MFx32::FromRaw(FX_SinIdx((u16)mAngle))).v;
    v.y = 0x800;
    v.z = (mDistance * MFx32::FromRaw(FX_CosIdx((u16)mAngle))).v;
    mScene->SetCameraPos(&v);
    mAngle += 0x6D;
    if (mTimer < 60) {
        mTimer++;
    }
    int x = gDinoResource->GetRandom()->Next(mBackground->GetWidth() - 256);
    mBackground->SetPosition(-x, -(mBackground->GetHeight() - 192) * mTimer / 60);
}
#pragma auto_inline reset

// Rises into place, bobbing up and down.
void DinoUiMuseumEffectLight::Idle() {
    if (mTimer != 0) {
        mTimer--;
        mHeight = 0x60 * (60 - mTimer) / 60;
    }
    mAngle += 0x190;
    mPicture->SetPosition(0, MFx32ToInt(MFx32::FromRaw(FX_SinIdx(mAngle)) * MFx32(10)) - mHeight);
}

// A tab shows its fossils; a slot shows that fossil's information.
void DinoUiMuseumMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 5; i++) {
        if (item == mTabs[i]) {
            SelectTab(i);
        }
    }
    for (int i = 0; i < (int)mSlotCount; i++) {
        if (item == mSlots[i]) {
            if (!mUnk1AC) {
                return;
            }
            mSelected = i + mPage * 15;
            DinoSoundPlaySe(0x98, 0);
            ShowInformation();
            return;
        }
    }
}

#pragma auto_inline off

DinoUiMuseumInformationMenu::DinoUiMuseumInformationMenu(Info *info, u32 *page, DinoUiMuseum3DModel *model)
    : MMenu(gLcd->mSub, NULL, 0), mInfo(info), mPage(page), mStartPage(*page), mModel(model) {
}

// The TU's pooled pointer-to-member constants (typed as the call tasks the game makes of them).
extern "C" struct {
    void (DinoUiMuseum::*CloseInformation_00)();
} data_ov006_021961dc;
extern "C" struct {
    void (DinoUiMuseum::*OpenInformation_00)();
} data_ov006_0219621c;

// Fades both screens out to white, then exits.
void DinoUiMuseumMenu::OnBackDecided() {
    MTask *main, *sub, *wait, *exit;
    exit = MTaskExitTransfer::Create(&(MTask &)*this);
    wait = MTaskSetWaitEmptyTask::Create(&mModel->mSerial);
    sub = gLcd->mSub->mBright->CreateFadeOutToWhite(30, 0);
    main = gLcd->mMain->mBright->CreateFadeOutToWhite(30, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(main);
    fades->AddTail(sub);
    GetSerial()->AddTail(new MTimer(30), fades, wait, exit);
}

// Fades out, shows the fossil's information, and fades back in.
void DinoUiMuseumMenu::ShowInformation() {
    GetSerial()->AddTail(gLcd->mSub->mBright->CreateFadeOutToBlack(5, 0),
                         MCreateCallTask((DinoUiMuseum *)this, data_ov006_021961dc.CloseInformation_00),
                         new DinoUiMuseumInformationMenu((DinoUiMuseumInformationMenu::Info *)&mCreatures,
                                                         (u32 *)&mSelected, mModel),
                         MCreateCallTask((DinoUiMuseum *)this, data_ov006_0219621c.OpenInformation_00),
                         gLcd->mSub->mBright->CreateFadeInFromBlack(5, 0));
}

extern "C" const u32 data_0208a914;

// Shows the information picture, and turns to the page of the fossil whose information was shown.
void DinoUiMuseumMenu::OpenInformation() {
    mInformation = new MTextPicture(*mPicture, 1, false);
    mParallel.AddTail(mInformation);
    mInformation->Show();
    mInformation->SetPriority(3);
    u32 page = mSelected / 15;
    if (mPage != page) {
        mPage = page;
        ClearSlots();
        mFraction->SetNumerator(mPage + 1);
        CreateSlots();
    }
    FocusItem(mSlots[mSelected % 15]);
    mUnk1C0->AddTail(new DinoGuideMessage(gDinoResource->GetText(data_0208a914)));
}

void DinoUiMuseumMenu::CloseInformation() {
    mParallel.Remove(mInformation);
    delete mInformation;
    mUnk1C0->Clear();
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoHelpBase::*Enable_00)();
} data_ov006_021962a4;

void DinoUiMuseumMenu::SelectTab(u8 tab) {
    if (tab >= 5 || mTab == tab) {
        return;
    }
    mTab = tab;
    DinoSoundPlaySe(0x98, 0);
    mTabPicture->SetPosition(0, tab * -192);
    ClearSlots();
    LoadTab(tab);
    Disable();
    GetSerial()->AddTail(new DinoUiMuseumSlotCreateTask(this), MCreateCallTask(this, data_ov006_021962a4.Enable_00));
}

