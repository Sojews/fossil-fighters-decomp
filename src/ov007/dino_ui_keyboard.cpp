// DinoUiKeyboard.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/input.h"
#include "game/dino_sound.h"
#include "game/dino_ui_keyboard_menu.h"
#include "types.h"
#include "game/dino_ui_keyboard.h"
#include "game/dino_ui_senryu_menu.h"
#include "game/dino_ui_senryu.h"
#include "m/bg_new_task.h"
#include "m/blend.h"
#include "m/window.h"
#include "m/lcd.h"
#include "m/obj.h"
#include "m/display.h"
#include "m/indicator.h"
#include "m/motion.h"
#include "game/dino_backup.h"
#include "m/bright.h"
#include "m/picture.h"

DinoUiKeyboardMenu::DinoUiKeyboardMenu(int mode, u32 arg, int flag)
    : MMenu(NULL, 0), mPagePicture(NULL), mFocusFrame(0), mMode(mode), mArg(arg), mPage(0x20), mFlag(flag) {
}

DinoUiKeyboardMenu::~DinoUiKeyboardMenu() {
    gLcd->GetBottomDisplay()->mWin->GetWindow0()->Hide();
    gLcd->GetBottomDisplay()->mBlend->Reset();
}

// The keyboards' pictures: one row per mode, one column per page.
extern "C" const char *const data_ov007_021911c0[][4];

void DinoUiKeyboardMenu::SetPage(int page) {
    page %= 3;
    if (page < 0) {
        page += 3;
    }
    if (page == mPage) {
        return;
    }
    mPage = page;
    if (mPagePicture) {
        mParallel.Remove(mPagePicture);
        delete mPagePicture;
    }
    mPagePicture = new MTextPicture(std::string(data_ov007_021911c0[mMode][mPage]), 1, false);
    mPagePicture->Show();
    mPagePicture->SetPriority(3);
    mParallel.AddTail(mPagePicture);
    if (GetFocusedItem() != mUnkE8) {
        mUnk1F4 = TRUE;
    }
    mSerial.AddTail(CreateSetFocusTask(mUnkE8));
}

DinoUiKeyboard::~DinoUiKeyboard() {
}

KasekiPictCache::~KasekiPictCache() {
}

KeyCheckTask::~KeyCheckTask() {
}

KeyboardMenuItem::~KeyboardMenuItem() {
}

DinoUiKeyboardMenuItem::~DinoUiKeyboardMenuItem() {
}

DinoUiKeyboardHideMenuItem::~DinoUiKeyboardHideMenuItem() {
}

DinoKBBackButton::~DinoKBBackButton() {
}

SetFocusTask::~SetFocusTask() {
}

PageChangeTask::~PageChangeTask() {
}

void DinoKBBackButton::OnSelectChanged(BOOL value) {
}

void DinoUiKeyboardHideMenuItem::OnDecide() {
}

void DinoUiKeyboardHideMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiKeyboardHideMenuItem::OnFocusChanged(BOOL value) {
}

void DinoUiKeyboardMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiKeyboardMenuItem::OnFocusChanged(BOOL value) {
}

void KasekiPictCache::Load() {
    while (*mPaths != NULL) {
        if (mStop) {
            return;
        }
        mPictures.push_back(MPictureFile(std::string(*mPaths), 7));
        mPaths++;
    }
}

void KasekiPictCache::Idle() {
}

MRect DinoKBBackButton::GetRect() {
    return GetFrame();
}

MRect DinoUiKeyboardMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoKBBackButton::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiKeyboardMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoKBBackButton::GetFrame() {
    return mMotion->GetFrame();
}

void DinoKBBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 1);
    } else {
        mMotion->Play(2, 1);
    }
}

const char *DinoKBBackButton::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiKeyboardMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

DinoUiKeyboard::DinoUiKeyboard(int mode, int arg, u8 flag) : MProcess(NULL) {
    mMode = mode;
    mArg = arg;
    mFlag = flag;
}

DinoUiKeyboard::DinoUiKeyboard(int mode, int arg) : MProcess(NULL) {
    mMode = mode;
    mArg = arg;
    mFlag = TRUE;
}

DinoUiKeyboard::DinoUiKeyboard(int mode) : MProcess(NULL) {
    mMode = mode;
    mArg = 0;
    mFlag = TRUE;
}

#pragma dont_inline on
void DinoUiKeyboard::Idle() {
    gLcd->mMain->ResetHooks();
    if (mMode == 0) {
        gLcd->mSub->ResetHooks();
        gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
        gLcd->mSub->mObj->Hide();
        gLcd->mSub->mBright->SetMode(2);
        gLcd->mSub->mBright->SetFactor(16);
    }
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->mObj->Hide();
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mBright->SetMode(2);
    gLcd->mMain->mBright->SetFactor(16);
    gLcd->VSyncNow();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiKeyboardMenu(mMode, mArg, mFlag);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}
#pragma dont_inline reset

void PageChangeTask::Execute() {
    ((DinoUiKeyboardMenu *)mUnk10)->SetPage(mUnk14);
    Exit();
}

DinoUiSenryuMenu::~DinoUiSenryuMenu() {
    mSerial.Clear();
    mFont120->Release();
    mFont124->Release();
    mFont128->Release();
}

extern "C" void func_ov007_02185d28(void *, u32);

SenryuMenuItem::~SenryuMenuItem() {
}

DinoSenryuBackButton::~DinoSenryuBackButton() {
    mSerial.Clear();
}

DinoUiSenryuMenuItem::~DinoUiSenryuMenuItem() {
    mSerial.Clear();
}

DinoUiSenryuList::~DinoUiSenryuList() {
}


DinoUiSenryuListHideMenuItem::~DinoUiSenryuListHideMenuItem() {
}

DinoUiSenryuSubMenu::~DinoUiSenryuSubMenu() {
}

SenryuKeyCheckTask::~SenryuKeyCheckTask() {
}

CurChangeTask::~CurChangeTask() {
}

void DinoUiSenryuMenu::OnIdle() {
}

void DinoSenryuBackButton::OnSelectChanged(BOOL value) {
}

void DinoUiSenryuListHideMenuItem::OnDecide() {
}

void DinoUiSenryuListHideMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiSenryuListHideMenuItem::OnFocusChanged(BOOL value) {
}

void DinoUiSenryuMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiSenryuMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiSenryuMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoSenryuBackButton::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiSenryuMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoSenryuBackButton::GetFrame() {
    return mMotion->GetFrame();
}

void DinoSenryuBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 1);
    } else {
        mMotion->Play(2, 1);
    }
}

const char *DinoSenryuBackButton::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiSenryuMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

DinoUiSenryuList::DinoUiSenryuList() : MProcess(NULL) {
}

void CurChangeTask::Execute() {
    func_ov007_02185d28((void *)mUnk10, mUnk14);
    Exit();
}

extern "C" void func_ov007_02183558(void *owner);

void KeyCheckTask::Execute() {
    func_ov007_02183558(mOwner);
}

void SenryuKeyCheckTask::Execute() {
    ((DinoUiSenryuMenu *)mOwner)->CheckKeys();
}

MRect DinoSenryuBackButton::GetRect() {
    return mMotion->GetFrame();
}

void SetFocusTask::Execute() {
    mMenu->FocusItem(mItem);
    Exit();
}


void DinoUiSenryuSubMenu::Execute() {
    ((DinoUiSenryuMenu *)mOwner)->OnSubMenuChoice(*mValue, mArg);
    Exit();
}

MRect SenryuMenuItem::GetFrame() {
    return mFrame;
}

MRect KeyboardMenuItem::GetFrame() {
    return mFrame;
}

void DinoUiSenryuMenuItem::OnDecide() {
    if (mMotion != NULL) {
        mSerial.AddTail(mMotion->CreatePlayTransfer(2, 0));
    }
}


void DinoKBBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    ((DinoUiKeyboardMenu *)unkB8)->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}


void DinoSenryuBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    ((DinoUiSenryuMenu *)unkAC)->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

extern const int data_020899d4, data_020899e8, data_02089a1c;

// Fades out and leaves.
void DinoUiSenryuMenu::OnBackDecided() {
    Deactivate();
    mUnk11C->Exit();
    MTask *fade, *exit;
    exit = MTaskExitTransfer::Create(&(MTask &)*this);
    fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_020899e8, data_020899d4);
    GetSerial()->AddTail(new MTimer(data_02089a1c), fade, exit);
}

void DinoUiKeyboardMenuItem::OnDecide() {
    if (mMotion != NULL) {
        mTasks->Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MTask *loop = mMotion->CreatePlayTransfer(2, 0);
        MSerialTaskSet *tasks = mTasks;
        tasks->AddTail(loop);
        tasks->AddTail(wait);
        tasks->AddTail(play);
    }
}


void DinoUiSenryuMenu::OnFocus(MMenuItem *item) {
    if (item != NULL) {
        SenryuMenuItem *senryu = static_cast<SenryuMenuItem *>(item);
        if (senryu->mIndex < 4) {
            if (!mSilentFocus) {
                DinoSoundPlaySe(0x96, 0);
            }
            mSilentFocus = FALSE;
        }
        switch (senryu->mIndex) {
        case 0:
            mSerial.AddTail(CreateCurChangeTask(mPage * 3));
            mCursor->Show();
            return;
        case 1:
            mSerial.AddTail(CreateCurChangeTask(mPage * 3 + 1));
            mCursor->Show();
            return;
        case 2:
            mSerial.AddTail(CreateCurChangeTask(mPage * 3 + 2));
            mCursor->Show();
            return;
        default:
            mCursor->Hide();
            break;
        }
    }
}

// The function keys' focus frames.
extern "C" const char *const data_ov007_02191120[4];

// Frames a focused function key.
void DinoUiKeyboardMenu::OnFocus(MMenuItem *item) {
    KeyboardMenuItem *key = static_cast<KeyboardMenuItem *>(item);
    if (key != NULL && key->mIndex < 0xF0) {
        mUnk154->Clear();
        mFocusTasks->Clear();
        if (key->mIndex < 4) {
            mFocusFrame = new MMotion(std::string(data_ov007_02191120[key->mIndex]), mOam, 0, 1);
            mFocusFrame->Play(1, 0);
            mFocusFrame->SetPosition(MPoint(key->mPos.x, key->mPos.y));
            mFocusFrame->SetPriority(2);
            mFocusFrame->Show();
            mFocusTasks->AddTail(mFocusFrame);
            mFocusPos = MPoint(key->mPos.x, key->mPos.y);
            if (!mUnk1F4) {
                DinoSoundPlaySe(0x96, 0);
            }
            mUnk1F4 = FALSE;
        } else if (key->mIndex == 6) {
            if (!mUnk1F4) {
                DinoSoundPlaySe(0x96, 0);
            }
            mUnk1F4 = FALSE;
        }
    }
}

// DinoUiKeyboardMenu::OnStart (not decompiled yet) starts a background load.
template MTask *MCreateBgCallTask(DinoUiKeyboardMenu *obj, void (DinoUiKeyboardMenu::*func)(), int thread);

MRect DinoUiKeyboardHideMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiKeyboardHideMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiSenryuListHideMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiSenryuListHideMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MTask *DinoUiKeyboardMenu::CreateSetFocusTask(MMenuItem *item) {
    return new SetFocusTask(this, item);
}

MTask *DinoUiKeyboardMenu::CreatePageChangeTask(u32 page) {
    return new PageChangeTask(this, page);
}

void DinoUiSenryuMenu::CheckKeys() {
    MSerialTaskSet *serial = &mSerial;
    if (serial->IsEmpty()) {
        if (GetInput()->GetPressed() & 2) {
            if (mBackButton) {
                if (mBackButton->IsFocused()) {
                    mBackButton->Decide();
                } else {
                    mBackButton->Focus();
                }
            }
        }
    }
}

void DinoUiSenryuMenu::OnSubMenuChoice(u32 choice, u32 index) {
    s32 entry = mEntries[index];
    u32 id = entry >> 8;
    switch (choice) {
    case 1: {
        mEntries[index] = entry | 2;
        DinoBackupRecord record;
        gDinoBackup->GetRecord(id, &record);
        mUnk13C = TRUE;
        record.unk79 = TRUE;
        gDinoBackup->SetRecord(id, &record);
        GetSerial()->AddTail(gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(15, 0), CreateCurChangeTask(index),
                             gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(15, 0));
        break;
    }
    case 2: {
        mEntries[index] = entry & ~2;
        DinoBackupRecord record;
        gDinoBackup->GetRecord(id, &record);
        record.unk79 = FALSE;
        mUnk13C = TRUE;
        gDinoBackup->SetRecord(id, &record);
        GetSerial()->AddTail(gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(15, 0), CreateCurChangeTask(index),
                             gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(15, 0));
        break;
    }
    case 3:
    case (u32)-2:
        break;
    }
}

MTask *DinoUiSenryuMenu::CreateCurChangeTask(u32 cur) {
    return new CurChangeTask(this, cur);
}

#pragma auto_inline off

DinoUiSenryuMenu::DinoUiSenryuMenu() : MMenu(NULL, 0), mCursor(NULL), mPage(99), mUnk134(99) {
    mUnk108 = 0;
    mUnk10C = 0;
    mBackButton = NULL;
}

#pragma auto_inline off
// Vector members the game emits in this file (made explicitly until the code that uses them is decompiled).
template void std::__vec_deleter<MPictureFile, std::allocator<MPictureFile> >::sub_push_back(const MPictureFile &x, Metrowerks::int2type<false>);
template std::wstring std::wstring::substr(std::wstring::size_type pos, std::wstring::size_type n) const;
template std::wstring &std::wstring::operator=(const wchar_t *s);

extern const int data_02089ac8, data_02089acc, data_02089ad0;

// NONMATCHING: the two fade constants swap registers.
void DinoUiKeyboardMenu::OnBackDecided() {
    if (mMode == 0) {
        DinoPlayerData player;
        gDinoBackup->GetPlayer(&player, 0);
        std::wstring name((const wchar_t *)player.name);
        if (name.size() == 0) {
            player.name[0] = mUnk1B4[0][0x1D];
            player.name[1] = mUnk1B4[0][0x29];
            player.name[2] = mUnk1B4[0][0xC];
            player.name[3] = mUnk1B4[0][8];
        }
        mUnk19C = (const wchar_t *)player.name;
        RefreshName();
        gDinoBackup->SetPlayer(&player);
    }
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    MBright *bright = gLcd->GetBottomDisplay()->mBright;
    fades->AddTail(bright->CreateFadeOutToBlack(data_02089acc, data_02089ac8));
    if (mMode == 0) {
        fades->AddTail(gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(data_02089acc, data_02089ac8));
    }
    mUnk158->Exit();
    MTask *exit = MTaskExitTransfer::Create(&(MTask &)*this);
    KasekiPictCache *cache = mPictCache;
    cache->mStop = TRUE;
    MTask *waitTask = MTaskSetWaitEmptyTask::Create(&cache->mSerial);
    GetSerial()->AddTail(new MTimer(data_02089ad0), fades, waitTask, exit);
}

// Where the cursor goes for each place in the name, per mode.
extern "C" const MPoint data_ov007_021911f0[][18];

void DinoUiKeyboardMenu::RefreshName() {
    for (int i = mUnk19C.size(); i < mMaxLength; i++) {
        mChars[i]->SetText(std::string(""));
    }
    for (u32 i = 0; i < mUnk19C.size(); i++) {
        wchar_t text[3];
        text[0] = mUnk19C.substr(i, 1)[0];
        text[1] = mUnk1A8.substr(i, 1)[0];
        text[2] = 0;
        mChars[i]->SetText(std::wstring(text));
    }
    if (mCursorPos < 0) {
        mCursorPos = 0;
    }
    if (mCursorPos > mMaxLength) {
        mCursorPos = mMaxLength;
    }
    mCursor->SetPosition(data_ov007_021911f0[mMode][mCursorPos]);
    if (mCursorPos == mMaxLength) {
        mSerial.AddTail(mCursor->CreatePlayTransfer(2, 0));
    } else {
        mSerial.AddTail(mCursor->CreatePlayTransfer(1, 0));
    }
}

extern const int data_020899f8, data_02089a84;

void DinoUiSenryuList::Idle() {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->mMain;
    gLcd->mMain->mBright->SetMode(2);
    gLcd->mMain->mBright->SetFactor(16);
    gLcd->VSyncNow();
    mParallel.AddTail(gLcd->mMain->mBright->CreateFadeInFromBlack(data_020899f8, data_02089a84));
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *menu = new DinoUiSenryuMenu();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(menu);
    serial->AddTail(exit);
}
