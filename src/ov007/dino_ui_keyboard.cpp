// DinoUiKeyboard.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_static_init.h"
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
#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_guide_message.h"
#include "game/dino_resource.h"
#include "m/oam_canvas.h"
#include "m/smart_oam.h"
#include "m/text_codec.h"

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

extern "C" void func_ov007_02185d28(DinoUiSenryuMenu *menu, int cur);

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
    func_ov007_02185d28((DinoUiSenryuMenu *)mUnk10, mUnk14);
    Exit();
}

void KeyCheckTask::Execute() {
    ((DinoUiKeyboardMenu *)mOwner)->CheckKeys();
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

// Fades out and leaves.
void DinoUiSenryuMenu::OnBackDecided() {
    Deactivate();
    mUnk11C->Exit();
    MTask *fade, *exit;
    exit = MTaskExitTransfer::Create(&(MTask &)*this);
    fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_020899e8, data_020899d4);
    GetSerial()->AddTail(new MTimer(data_02089a1c), fade, exit);
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
void DinoUiKeyboardMenu::CheckKeys() {
    MSerialTaskSet *serial = &mSerial;
    if (!serial->IsEmpty()) {
        return;
    }
    if (GetInput()->GetPressed() & 2) {
        if (mCursorPos == 0) {
            BOOL blank = TRUE;
            for (int i = 0; i < mMaxLength; i++) {
                if (mUnk19C[i] != mUnk1B4[2][29]) {
                    blank = FALSE;
                }
            }
            if (blank && mFlag) {
                if (mBackButton != NULL) {
                    if (mBackButton->IsFocused()) {
                        mBackButton->Decide();
                    } else {
                        mSerial.AddTail(CreateSetFocusTask(mBackButton));
                    }
                }
                return;
            }
        }
        for (int i = mCursorPos; i < mMaxLength; i++) {
            if (i > 0) {
                mUnk19C[i - 1] = mUnk19C[i];
                mUnk1A8[i - 1] = mUnk1A8[i];
            }
        }
        mUnk19C[mMaxLength - 1] = mUnk1B4[2][29];
        mUnk1A8[mMaxLength - 1] = mUnk1B4[2][29];
        mCursorPos--;
        DinoSoundPlaySe(0x9A, 0);
        RefreshName();
    }
    if (GetInput()->GetPressed() & 0x800) {
        mCursorPos--;
        DinoSoundPlaySe(0x96, 0);
        RefreshName();
    }
    if (GetInput()->GetPressed() & 0x400) {
        mCursorPos++;
        DinoSoundPlaySe(0x96, 0);
        RefreshName();
    }
    if (GetInput()->GetPressed() & 8) {
        if (mOkButton->IsFocused()) {
            mOkButton->Decide();
        } else {
            mOkButton->Focus();
        }
    }
}

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

// (Instantiated ahead of OnDecide, whose size() call would otherwise change how mwcc names it.)
#pragma auto_inline off
template std::wstring &std::wstring::operator=(const wchar_t *s);
#pragma auto_inline reset

// A key: types its character (capitalizing a name's first letter), a blank or a delete, finishes the name
// (unless it's blank), turns the page or moves the cursor.
void DinoUiKeyboardMenu::OnDecide(MMenuItem *item) {
    KeyboardMenuItem *key = static_cast<KeyboardMenuItem *>(item);
    if (key == NULL) {
        return;
    }
    if (key->mIndex < 0xF0 && key->mIndex != 4 && key->mIndex != 5) {
        mUnk154->Clear();
        mFocusTasks->Clear();
        if (key->mIndex < 4) {
            mFocusFrame = new MMotion(std::string(data_ov007_02191120[key->mIndex]), mOam, 0, 1);
            mFocusFrame->Play(2, 0);
            mFocusFrame->SetPosition(MPoint(key->mPos.x, key->mPos.y));
            mFocusFrame->SetPriority(2);
            mFocusFrame->Show();
            mFocusTasks->AddTail(mFocusFrame);
            MTask *play = mFocusFrame->CreatePlayTransfer(1, 0);
            MTask *wait = mFocusFrame->CreateWaitStopTask();
            MTaskSet *tasks = mUnk154;
            tasks->AddTail(wait);
            tasks->AddTail(play);
            mFocusPos = MPoint(key->mPos.x, key->mPos.y);
        }
    }
    switch (key->mIndex) {
    case 0:
        if (mCursorPos < mMaxLength) {
            mUnk19C[mCursorPos] = mUnk1B4[mPage][key->mArg];
            if (mCursorPos == 0 && mMode == 0 && mUnk19C[mCursorPos] >= 'a' && mUnk19C[mCursorPos] <= 'z') {
                mUnk19C[mCursorPos] = mUnk19C[mCursorPos] - 0x20;
            }
            mCursorPos++;
            if (mMaxLength == mCursorPos) {
                mSerial.AddTail(CreateSetFocusTask(mOkButton));
            }
            DinoSoundPlaySe(0x98, 0);
        }
        break;
    case 1:
        if (mCursorPos < mMaxLength) {
            mUnk19C[mCursorPos] = mUnk1B4[2][29];
            mUnk1A8[mCursorPos] = mUnk1B4[2][29];
            mCursorPos++;
            if (mMaxLength == mCursorPos) {
                mSerial.AddTail(CreateSetFocusTask(mOkButton));
            }
            DinoSoundPlaySe(0x98, 0);
        }
        break;
    case 2:
        for (int i = mCursorPos; i < mMaxLength; i++) {
            if (i > 0) {
                mUnk19C[i - 1] = mUnk19C[i];
                mUnk1A8[i - 1] = mUnk1A8[i];
            }
        }
        mUnk19C[mMaxLength - 1] = mUnk1B4[2][29];
        mUnk1A8[mMaxLength - 1] = mUnk1B4[2][29];
        mCursorPos--;
        DinoSoundPlaySe(0x9A, 0);
        break;
    case 3:
        if (mUnk19C.size() != 0) {
            BOOL blank = TRUE;
            for (int i = 0; i < mMaxLength; i++) {
                if (mUnk19C[i] != mUnk1B4[2][29]) {
                    blank = FALSE;
                }
            }
            if (!blank) {
                DinoSoundPlaySe(0x99, 0);
                Finish();
            } else {
                DinoSoundPlaySe(0x90, 0);
            }
        }
        break;
    case 4:
        DinoSoundPlaySe(0x93, 0);
        mSerial.AddTail(CreatePageChangeTask(mPage - 1));
        break;
    case 5:
        DinoSoundPlaySe(0x93, 0);
        mSerial.AddTail(CreatePageChangeTask(mPage + 1));
        break;
    case 0xF0:
    case 0xF1:
        mCursorPos = key->mArg;
        break;
    case 0xF2:
        DinoSoundPlaySe(0x93, 0);
        mSerial.AddTail(CreatePageChangeTask(key->mArg));
        break;
    }
    RefreshName();
}

extern "C" struct {
    void (KasekiPictCache::*Load_00)();
} data_ov007_02190fe4;

inline KasekiPictCache::KasekiPictCache(const char *const *paths) : MProcess(NULL), mPaths(paths), mStop(FALSE) {
    mSerial.AddTail(MCreateBgCallTask(this, data_ov007_02190fe4.Load_00, 1));
}

extern "C" void func_ov007_02182020(DinoUiKeyboardMenu *menu);
extern const int data_020899bc, data_020899c8, data_020899e0;
extern const u32 data_0208aa78, data_0208ac28, data_0208ac2c, data_0208ac58, data_0208ac68, data_0208ac6c, data_0208ac7c,
    data_0208ac80, data_0208ac84, data_0208ac88;
// The motions' paths (the cursor's is [8]) and the name's caption.
extern "C" const char *const data_ov007_02190fa0[];
extern "C" const char data_ov007_02191854[];
extern "C" const MPoint data_ov007_021911f0[][18];

// Builds the keyboard for a player's name (mode 0) or a team's (mode 1): the name, padded with blanks to its
// length, a character each, the cursor and the help, then starts loading the keyboard pictures.
#pragma opt_common_subs off
void DinoUiKeyboardMenu::OnStart() {
    if (mMode == 0) {
        gLcd->GetTopDisplay()->mObj->Show();
        mParallel.AddTail(gLcd->GetTopDisplay()->mBright->CreateFadeInFromBlack(data_020899e0, data_020899c8));
    }
    gLcd->GetBottomDisplay()->mObj->Show();
    mParallel.AddTail(gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(data_020899e0, data_020899c8));
    mParallel.AddTail(mUnk150 = new MParallelTaskSet(), mFocusTasks = new MParallelTaskSet(),
                      mUnk154 = new MSerialTaskSet());
    mOam = new MSmartOam(0x55, 0, TRUE);
    mParallel.AddTail(mOam);
    mUnk1B4[0] = MDecodeUtf8(gDinoResource->GetText(data_0208ac88));
    mUnk1B4[1] = MDecodeUtf8(gDinoResource->GetText(data_0208ac84));
    mUnk1B4[2] = MDecodeUtf8(gDinoResource->GetText(data_0208ac80));
    mUnk1D8 = MDecodeUtf8(gDinoResource->GetText(data_0208ac7c));
    mUnk19C.clear();
    mUnk1A8.clear();
    DinoHelpDummyItem *help =
        new DinoHelpDummyItem(gCurrentDisplay, MRect(9, 0x54, 0xF6, 0xAA), gDinoResource->GetText(data_0208ac68));
    mKeysHelp = help;
    help->Enable();
    mParallel.AddTail(help);
    MFont *font;
    MPoint offset;
    switch (mMode) {
    case 0: {
        mParallel.AddTail(new DinoGuideMessage(gDinoResource->GetText(data_0208ac2c)));
        help = new DinoHelpDummyItem(gCurrentDisplay, MRect(9, 10, 0xF6, 0x35), gDinoResource->GetText(data_0208ac6c));
        mHelp = help;
        mMaxLength = 8;
        mCursor = new MMotion(data_ov007_02190fa0[8], mOam, 2, 1);
        font = gDinoResource->CreateFont24(0, 10, 1);
        offset = MPoint(6, 12);
        DinoPlayerData player;
        gDinoBackup->GetPlayer(&player, FALSE);
        if (mFlag) {
            mUnk19C = (const wchar_t *)player.name;
        } else {
            mUnk19C.clear();
        }
        MTextPicture *caption = new MTextPicture(gLcd->GetTopDisplay(), data_ov007_02191854, TRUE, false);
        caption->Show();
        caption->SetPriority(3);
        mParallel.AddTail(caption);
        break;
    }
    case 1: {
        mParallel.AddTail(new DinoGuideMessage(gDinoResource->GetText(data_0208ac28)));
        help = new DinoHelpDummyItem(gCurrentDisplay, MRect(9, 10, 0xF6, 0x35), gDinoResource->GetText(data_0208ac58));
        mHelp = help;
        mCursor = new MMotion(data_ov007_02190fa0[8], mOam, 2, 1);
        mMaxLength = 0x10;
        font = gDinoResource->CreateFont24(0, 10, 1);
        offset = MPoint(6, 12);
        DinoTeam team;
        gDinoBackup->GetTeam(mArg, &team);
        mUnk19C = (const wchar_t *)team.name;
        if (team.slots[0] == 0) {
            mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208aa78), 4));
        }
        break;
    }
    }
    help->Enable();
    mParallel.AddTail(help);
    // Drops the trailing NULs.
    for (int i = mUnk19C.size() - 1; i >= 0; i--) {
        if (mUnk19C[i] != 0) {
            break;
        }
        mUnk19C = mUnk19C.substr(0, i);
    }
    for (int i = mUnk1A8.size() - 1; i >= 0; i--) {
        if (mUnk1A8[i] != 0) {
            break;
        }
        mUnk1A8 = mUnk1A8.substr(0, i);
    }
    if (mUnk19C.size() > mMaxLength) {
        mUnk19C = mUnk19C.substr(0, mMaxLength);
    }
    mCursorPos = mUnk19C.size();
    while (mUnk19C.size() < mMaxLength) {
        mUnk19C += mUnk1B4[2].substr(29, 1);
    }
    while (mUnk1A8.size() < mMaxLength) {
        mUnk1A8 += mUnk1B4[2].substr(29, 1);
    }
    for (int i = 0; i < mMaxLength; i++) {
        MTaskSet *tasks = &mParallel;
        mChars[i] = new MIndicator(
            new MObjCanvas(gCurrentDisplay, MPoint(0x18, 0x18), gDinoResource->GetUnkTable(), TRUE), font, 0);
        tasks->AddTail(mChars[i]);
        const MPoint &pos = data_ov007_021911f0[mMode][i];
        mChars[i]->SetPosition(pos - offset);
        mChars[i]->SetPriority(3);
        mChars[i]->Show();
    }
    mCursor->Play(1, 0);
    mCursor->SetPriority(2);
    mCursor->Show();
    mUnk150->AddTail(mCursor);
    RefreshName();
    func_ov007_02182020(this);
    SetPage(2);
    KeyCheckTask *keys = new KeyCheckTask(this);
    keys->mState = 1;
    mParallel.AddTail(keys);
    GetInput()->SetRepeat(0xF0, 0x1E, 10);
    mSerial.AddTail(new MTimer(data_020899bc), MTaskRunTransfer::Create(keys));
    mUnk158 = keys;
    mPictCache = new KasekiPictCache(data_ov007_021911c0[mMode]);
    mParallel.AddTail(mPictCache);
}
#pragma opt_common_subs reset

// The keyboard's layout: the motions' paths, then the places of the back button and the page key.
struct DinoUiKeyboardLayout {
    const char *mPaths[13];     // 0x00: [4]/[7] the page arrows, [6] the back button, [8] the cursor
    MPoint mBackPos;            // 0x34
    u8 unk3C[0x54 - 0x3C];
    MPoint mPageSize;           // 0x54: the page key's half size
    u8 unk5C[0x64 - 0x5C];
    MPoint mPagePos;            // 0x64
};
struct DinoUiKeyboardOkLayout {
    u32 unk00;
    MPoint mOkSize;             // 0x04: the OK key's half size
    u8 unk0C[0x2C - 0x0C];
    MPoint mOkPos;              // 0x2C
};
extern "C" const DinoUiKeyboardOkLayout data_ov007_02191020;
extern "C" const MPoint data_ov007_02190fd4;      // the back button's place
extern "C" const MPoint data_ov007_02191130[3];   // each group of character keys' first place
extern const u32 data_0208ac5c, data_0208ac50;    // the back button's help (player name, team name)
extern const u32 data_0208ac64;                   // the page key's help
extern const u32 data_0208ac60, data_0208ac54;    // the OK key's help (player name, team name)

inline DinoKBBackButton::DinoKBBackButton(DinoUiKeyboardMenu *menu, MSmartOam *oam, MPoint pos, u32 textId)
    : KeyboardMenuItem(6, 0, pos, menu) {
    unkB8 = (u32)menu;
    mTextId = textId;
    mMotion = new MMotion(std::string(((const DinoUiKeyboardLayout *)data_ov007_02190fa0)->mPaths[6]), oam, 2, 1);
    mMotion->Play(2, 0);
    mMotion->SetPosition(data_ov007_02190fd4);
    mMotion->SetPriority(3);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    DinoHelpItem::Enable();
}

#define DINO_KB_ARROW_MOTIONS()                                                                                  \
    if (mIndex >= 4) {                                                                                           \
        switch (mIndex) {                                                                                        \
        case 4:                                                                                                  \
            mMotion = new MMotion(std::string(((const DinoUiKeyboardLayout *)data_ov007_02190fa0)->mPaths[4]),   \
                                  ((DinoUiKeyboardMenu *)unk98)->mOam, 2, 1);                                    \
            mMotion->Play(1, 0);                                                                                 \
            mMotion->SetPosition(mPos);                                                                          \
            mMotion->SetPriority(2);                                                                             \
            mMotion->Show();                                                                                     \
            mFrame = mMotion->GetFrame();                                                                        \
            mParallel.AddTail(mMotion);                                                                          \
            mTasks = new MSerialTaskSet(FALSE);                                                                  \
            mParallel.AddTail(mTasks);                                                                           \
            break;                                                                                               \
        case 5:                                                                                                  \
            mMotion = new MMotion(std::string(((const DinoUiKeyboardLayout *)data_ov007_02190fa0)->mPaths[7]),   \
                                  ((DinoUiKeyboardMenu *)unk98)->mOam, 2, 1);                                    \
            mMotion->Play(1, 0);                                                                                 \
            mMotion->SetPosition(mPos);                                                                          \
            mMotion->SetPriority(2);                                                                             \
            mMotion->Show();                                                                                     \
            mFrame = mMotion->GetFrame();                                                                        \
            mParallel.AddTail(mMotion);                                                                          \
            mTasks = new MSerialTaskSet(FALSE);                                                                  \
            mParallel.AddTail(mTasks);                                                                           \
            break;                                                                                               \
        }                                                                                                        \
    }                                                                                                            \
    switch (mIndex) {                                                                                            \
    case 1:                                                                                                      \
    case 2:                                                                                                      \
    case 3:                                                                                                      \
    case 6:                                                                                                      \
        DinoHelpItem::Enable();                                                                                  \
        break;                                                                                                   \
    default:                                                                                                     \
        DinoHelpItem::Disable();                                                                                 \
        break;                                                                                                   \
    }

inline DinoUiKeyboardMenuItem::DinoUiKeyboardMenuItem(u32 index, u32 arg, MPoint pos, DinoUiKeyboardMenu *menu)
    : KeyboardMenuItem(index, arg, pos, menu) {
    mTextId = 0;
    mFrame.left = mPos.x - 7;
    mFrame.top = mPos.y - 7;
    mFrame.right = mPos.x + 6;
    mFrame.bottom = mPos.y + 6;
    mMotion = NULL;
    DINO_KB_ARROW_MOTIONS()
}

inline DinoUiKeyboardMenuItem::DinoUiKeyboardMenuItem(u32 index, u32 arg, MPoint pos, DinoUiKeyboardMenu *menu,
                                                      MPoint halfSize, u32 textId)
    : KeyboardMenuItem(index, arg, pos, menu) {
    mTextId = textId;
    mFrame.left = mPos.x - halfSize.x;
    mFrame.top = mPos.y - halfSize.y;
    mFrame.right = mPos.x + halfSize.x;
    mFrame.bottom = mPos.y + halfSize.y;
    mMotion = NULL;
    DINO_KB_ARROW_MOTIONS()
}

// Rebuilds the keyboard's menu items: the back button (for the mode), the character keys (three groups of 6 rows
// of 5), the page and OK keys, the name's places, then the help's and the items' links.
#pragma inline_max_size(2000)
// NONMATCHING
extern "C" void func_ov007_02182020(DinoUiKeyboardMenu *menu) {
    u32 okText = 0;
    MMenuItem *back = NULL;
    menu->ClearItems();
    menu->mBackButton = NULL;
    menu->mUnkE8 = NULL;
    const DinoUiKeyboardLayout *layout = (const DinoUiKeyboardLayout *)data_ov007_02190fa0;
    switch (menu->mMode) {
    case 0:
        if (menu->mFlag == 1) {
            menu->mBackButton = new DinoKBBackButton(menu, menu->mOam, layout->mBackPos, data_0208ac5c);
            back = menu->mBackButton;
            menu->AddItem(back);
        }
        break;
    case 1:
        menu->mBackButton = new DinoKBBackButton(menu, menu->mOam, layout->mBackPos, data_0208ac50);
        back = menu->mBackButton;
        menu->AddItem(back);
        break;
    }
    menu->mBackHelp = static_cast<DinoKBBackButton *>(menu->mBackButton);
    MMenuItem *keys[6][15];
    for (int group = 0; group < 3; group++) {
        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < 5; col++) {
                keys[row][group * 5 + col] = new DinoUiKeyboardMenuItem(0, col + row * 5 + group * 30,
                                                  MPoint(data_ov007_02191130[group].x + col * 14,
                                                         data_ov007_02191130[group].y + row * 14),
                                                  menu);
                if (menu->mUnkE8 == NULL) {
                    menu->mUnkE8 = keys[row][group * 5 + col];
                }
                menu->AddItem(keys[row][group * 5 + col]);
            }
        }
    }
    DinoUiKeyboardMenuItem *page =
        new DinoUiKeyboardMenuItem(2, 0, layout->mPagePos, menu, layout->mPageSize, data_0208ac64);
    menu->AddItem(page);
    menu->mPageHelp = page;
    switch (menu->mMode) {
    case 0:
        okText = data_0208ac60;
        break;
    case 1:
        okText = data_0208ac54;
        break;
    }
    menu->mOkButton = new DinoUiKeyboardMenuItem(3, 0, data_ov007_02191020.mOkPos, menu, data_ov007_02191020.mOkSize,
                                                 okText);
    menu->AddItem(menu->mOkButton);
    menu->mOkHelp = static_cast<DinoUiKeyboardMenuItem *>(menu->mOkButton);
    for (int i = 0; i < menu->mMaxLength; i++) {
        DinoUiKeyboardHideMenuItem *place = new DinoUiKeyboardHideMenuItem(
            menu->mMode == 2 ? 0xF0 : 0xF1, i, data_ov007_021911f0[menu->mMode][i], menu);
        place->SetNoFocus(TRUE);
        menu->AddItem(place);
    }
    menu->AddEntry((u32)menu->mHelp, 4, 0);
    menu->AddEntry((u32)menu->mHelp, 2, (u32)menu->mKeysHelp);
    menu->AddEntry((u32)menu->mHelp, 3, 0);
    menu->AddEntry((u32)menu->mHelp, 1, 0);
    menu->AddEntry((u32)menu->mKeysHelp, 4, (u32)menu->mHelp);
    menu->AddEntry((u32)menu->mKeysHelp, 2, (u32)menu->mOkHelp);
    menu->AddEntry((u32)menu->mKeysHelp, 3, 0);
    menu->AddEntry((u32)menu->mKeysHelp, 1, 0);
    menu->AddEntry((u32)menu->mPageHelp, 4, (u32)menu->mKeysHelp);
    menu->AddEntry((u32)menu->mPageHelp, 2, 0);
    menu->AddEntry((u32)menu->mPageHelp, 3, 0);
    menu->AddEntry((u32)menu->mPageHelp, 1, (u32)menu->mOkHelp);
    menu->AddEntry((u32)menu->mOkHelp, 4, (u32)menu->mKeysHelp);
    menu->AddEntry((u32)menu->mOkHelp, 2, 0);
    menu->AddEntry((u32)menu->mOkHelp, 3, (u32)menu->mPageHelp);
    if (menu->mBackHelp != NULL) {
        menu->AddEntry((u32)menu->mOkHelp, 1, (u32)menu->mBackHelp);
    }
    if (menu->mBackHelp != NULL) {
        menu->AddEntry((u32)menu->mBackHelp, 4, (u32)menu->mKeysHelp);
        menu->AddEntry((u32)menu->mBackHelp, 2, 0);
        menu->AddEntry((u32)menu->mBackHelp, 3, (u32)menu->mOkHelp);
        menu->AddEntry((u32)menu->mBackHelp, 1, 0);
    }
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 14; col++) {
            menu->AddLink(keys[row][col], 1, keys[row][col + 1]);
            menu->AddLink(keys[row][col], 2, keys[row + 1][col]);
            menu->AddLink(keys[row][col + 1], 3, keys[row][col]);
            menu->AddLink(keys[row + 1][col], 4, keys[row][col]);
            menu->AddLink(keys[row + 1][col], 1, keys[row + 1][col + 1]);
            menu->AddLink(keys[row + 1][col + 1], 3, keys[row + 1][col]);
            menu->AddLink(keys[row][col + 1], 2, keys[row + 1][col + 1]);
            menu->AddLink(keys[row + 1][col + 1], 4, keys[row][col + 1]);
        }
    }
    for (int row = 0; row < 6; row++) {
        menu->AddLink(keys[row][0], 3, keys[row][14]);
        menu->AddLink(keys[row][14], 1, keys[row][0]);
    }
    menu->AddLink(keys[5][0], 2, page);
    menu->AddLink(keys[5][1], 2, page);
    menu->AddLink(keys[5][2], 2, page);
    menu->AddLink(keys[5][3], 2, page);
    menu->AddLink(page, 2, keys[0][0]);
    for (int col = 4; col < 10; col++) {
        menu->AddLink(keys[0][col], 4, menu->mOkButton);
        menu->AddLink(keys[5][col], 2, menu->mOkButton);
    }
    menu->AddLink(menu->mOkButton, 2, keys[0][7]);
    menu->AddLink(menu->mOkButton, 4, keys[5][7]);
    menu->AddLink(keys[0][0], 4, page);
    menu->AddLink(keys[0][1], 4, page);
    menu->AddLink(keys[0][2], 4, page);
    menu->AddLink(keys[0][3], 4, page);
    menu->AddLink(page, 4, keys[5][0]);
    menu->AddLink(page, 1, menu->mOkButton);
    menu->AddLink(menu->mOkButton, 3, page);
    if (back != NULL) {
        for (int col = 10; col < 15; col++) {
            menu->AddLink(keys[0][col], 4, back);
            menu->AddLink(keys[5][col], 2, back);
        }
        menu->AddLink(menu->mOkButton, 1, back);
        menu->AddLink(page, 3, back);
        menu->AddLink(back, 1, page);
        menu->AddLink(back, 3, menu->mOkButton);
        menu->AddLink(back, 4, keys[5][14]);
        menu->AddLink(back, 2, keys[0][14]);
    } else {
        for (int col = 10; col < 15; col++) {
            menu->AddLink(keys[0][col], 4, menu->mOkButton);
            menu->AddLink(keys[5][col], 2, menu->mOkButton);
        }
        menu->AddLink(menu->mOkButton, 1, page);
        menu->AddLink(page, 3, menu->mOkButton);
    }
}
#pragma inline_max_size reset

template MTask *MCreateBgCallTask(KasekiPictCache *obj, void (KasekiPictCache::*func)(), int thread);

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

extern const int data_02089ac8, data_02089acc, data_02089ad0;

// Keeps the name, without its trailing blanks, as the player's or the team's (a team's first name also fades the top
// screen out), and fades out.
void DinoUiKeyboardMenu::Finish() {
    BOOL fadeTop = FALSE;
    for (int i = mUnk19C.size() - 1; i >= 0; i--) {
        if (mUnk19C[i] != mUnk1B4[2][29]) {
            break;
        }
        mUnk19C = mUnk19C.substr(0, i);
    }
    if (mUnk19C.size() != 0) {
        switch (mMode) {
        case 0: {
            DinoPlayerData player;
            gDinoBackup->GetPlayer(&player, 0);
            for (u32 i = 0; i <= mUnk19C.size(); i++) {
                player.name[i] = mUnk19C[i];
            }
            gDinoBackup->SetPlayer(&player);
            break;
        }
        case 1: {
            DinoTeam team;
            gDinoBackup->GetTeam(mArg, &team);
            u32 i;
            int members = 0;
            for (i = 0; i < 5; i++) {
                if (team.slots[i]) {
                    members++;
                }
            }
            if (members == 0) {
                fadeTop = TRUE;
            }
            for (u32 i = 0; i <= mUnk19C.size(); i++) {
                team.name[i] = mUnk19C[i];
            }
            gDinoBackup->SetTeam(mArg, &team);
            break;
        }
        }
    }
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    MDisplay *display = gLcd->GetBottomDisplay();
    MBright *bright = display->mBright;
    fades->AddTail(bright->CreateFadeOutToBlack(data_02089acc, data_02089ac8));
    if (mMode == 0) {
        fades->AddTail(gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(data_02089acc, data_02089ac8));
    } else if (fadeTop) {
        fades->AddTail(gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(data_02089acc, data_02089ac8));
    }
    mUnk158->Exit();
    MTask *exit = MTaskExitTransfer::Create(&(MTask &)*this);
    KasekiPictCache *cache = mPictCache;
    cache->mStop = TRUE;
    MTask *waitTask = MTaskSetWaitEmptyTask::Create(&cache->mSerial);
    GetSerial()->AddTail(new MTimer(data_02089ad0), fades, waitTask, exit);
}

// The poem list's pooled layout (anchor data_ov007_02191880).
extern "C" struct {
    u8 unk00[8];
    const char *mFramePath;     // 0x08
    const char *mMarkPath;      // 0x0C
    const char *mPrevPath;      // 0x10: the page arrows
    const char *mCursorPath;    // 0x14
    u8 unk18[0x20 - 0x18];
    const char *mNextPath;      // 0x20
    const char *mBackPath;      // 0x24
    u8 unk28[0x3C - 0x28];
    MPoint mMarkOffset;         // 0x3C
    u8 unk44[0x64 - 0x44];
    MPoint mNameOffset;         // 0x64
} data_ov007_02191880;
extern "C" const MPoint data_ov007_021919ac[3];   // each poem's place
extern "C" const MPoint data_ov007_021919c4[3];   // each line's place in a poem
extern "C" const MRect data_ov007_02191a3c[3];    // each poem's frame
extern "C" const MPoint data_ov007_021919b4;      // the cursor's place
extern "C" const char data_ov007_02191e3c[];      // the title picture
extern "C" const char data_ov007_02191e50[];      // the page number's font
extern const u32 data_0208ac24, data_0208ac40, data_0208ac44;
extern const int data_02089a18;

// The poems saved in the records (three lines each, with the author and its flags), their labels, frames and
// marks, the page number, the cursor and the help.
// NONMATCHING: the game reads gCurrentDisplay for the canvases after copying their size (as elsewhere), and its
// spill slots differ.
#pragma opt_common_subs off
void DinoUiSenryuMenu::OnStart() {
    mParallel.AddTail(mUnk114 = new MParallelTaskSet, mUnk110 = new MParallelTaskSet, mUnk118 = new MSerialTaskSet);
    mOam = new MSmartOam(0x28, 0, TRUE);
    mParallel.AddTail(mOam);
    MTextPicture *title = new MTextPicture(std::string(data_ov007_02191e3c), TRUE, false);
    title->Show();
    title->SetPriority(3);
    mParallel.AddTail(title);
    mParallel.AddTail(new DinoGuideMessage(std::string(gDinoResource->GetText(data_0208ac24))));
    DinoHelpDummyItem *help = new DinoHelpDummyItem(gCurrentDisplay, MRect(0x15, 5, 0xF9, 0xA9),
                                                    std::string(gDinoResource->GetText(data_0208ac44)));
    help->Enable();
    mParallel.AddTail(help);
    help = new DinoHelpDummyItem(gCurrentDisplay, MRect(0x73, 0xB4, 0x8D, 0xBF),
                                 std::string(gDinoResource->GetText(data_0208ac40)));
    help->Enable();
    mParallel.AddTail(help);
    mFont120 = gDinoResource->CreateFont12(0, 1, 0);
    mFont124 = gDinoResource->CreateFont12(0, 1, 0);
    mFont128 = gDinoResource->CreateFont12(0, 1, 0);
    mFont120->AddRef();
    mFont124->AddRef();
    mFont128->AddRef();
    mCount = 0;
    for (int j = 0; j < 42; j++) {
        mEntries[j] = 0;
    }
    s32 index = 0;
    for (int i = 0; i < 42; index += 0x100, i++) {
        DinoBackupRecord record;
        gDinoBackup->GetRecord(i, &record);
        if (record.unk78) {
            std::wstring text = MDecodeUtf8(std::string((const char *)record.data));
            int line = 0;
            for (int c = 0; c < text.size(); c++) {
                if (text[c] == L'\n') {
                    line++;
                } else {
                    mLineTexts[mCount][line].push_back(text[c]);
                }
                if (line == 3) {
                    break;
                }
            }
            mUnk728[mCount] = (const wchar_t *)record.name;
            mEntries[mCount] = index | ((record.unk79 ? 2 : 0) | record.unk7a | 0x80);
            mCount++;
        }
    }
    if (mCount == 0) {
        mCount = 1;
    }
    for (int o = 0; o < 3; o++) {
        MTaskSet *outer = &mParallel;
        for (int j = 0; j < 3; j++) {
            MTaskSet *parallel = &mParallel;
            mLines[o][j] = new MIndicator(
                new MObjCanvas(gCurrentDisplay, MPoint(0xCC, 0xC),
                               gDinoResource->GetUnkTable(), TRUE),
                (&mFont120)[o], 0);
            parallel->AddTail(mLines[o][j]);
            mLines[o][j]->SetPosition(MPoint(data_ov007_021919c4[j].x - 6 + data_ov007_021919ac[o].x,
                                                  data_ov007_021919c4[j].y - 6 + data_ov007_021919ac[o].y));
            mLines[o][j]->SetPriority(3);
            mLines[o][j]->Show();
        }
        mNames[o] = new MIndicator(new MObjCanvas(gCurrentDisplay, MPoint(0x30, 0xC),
                                                  gDinoResource->GetUnkTable(), TRUE),
                                   (&mFont120)[o], 0);
        outer->AddTail(mNames[o]);
        mNames[o]->SetPosition(MPoint(data_ov007_02191880.mNameOffset.x + data_ov007_021919ac[o].x,
                                      data_ov007_02191880.mNameOffset.y + data_ov007_021919ac[o].y));
        mNames[o]->SetPriority(3);
        mNames[o]->Hide();
        mMarks[o] = new MMotion(std::string(data_ov007_02191880.mMarkPath), mOam, 0, 1);
        mMarks[o]->Play(1, 0);
        mMarks[o]->SetPosition(MPoint(data_ov007_02191880.mMarkOffset.x + data_ov007_021919ac[o].x,
                                      data_ov007_02191880.mMarkOffset.y + data_ov007_021919ac[o].y));
        mMarks[o]->SetPriority(2);
        mMarks[o]->Hide();
        mUnk110->AddTail(mMarks[o]);
        mFrames[o] = new MMotion(std::string(data_ov007_02191880.mFramePath), mOam, 0, 1);
        mFrames[o]->Play(1, 0);
        mFrames[o]->SetPosition(MPoint((data_ov007_02191a3c[o].right + data_ov007_02191a3c[o].left) / 2 - 6,
                                       (data_ov007_02191a3c[o].bottom + data_ov007_02191a3c[o].top) / 2 + 4));
        mFrames[o]->SetPriority(2);
        mFrames[o]->Hide();
        mUnk110->AddTail(mFrames[o]);
    }
    MFontSet *font = new MFontSet(std::string(data_ov007_02191e50), 0, 0, 0);
    font->AddRef();
    mPageText = new MIndicator(
        new MObjCanvas(gCurrentDisplay, MPoint(0x20, 8), font->GetPalette(), TRUE), font,
        0);
    mParallel.AddTail(mPageText);
    mPageText->SetPosition(MPoint(0x73, 0xB6));
    mPageText->SetPriority(3);
    mPageText->Show();
    font->Release();
    mCursor = new MMotion(std::string(data_ov007_02191880.mCursorPath), mOam, 0, 1);
    ((MMotion *)mCursor)->Play(1, 0);
    mCursor->SetPosition(data_ov007_021919b4);
    mCursor->SetPriority(2);
    mCursor->Show();
    mUnk110->AddTail(mCursor);
    func_ov007_02185d28(this, 0);
    GetInput()->SetRepeat(0xF0, 0x1E, 10);
    mSerial.AddTail(new MTimer(data_02089a18));
    SenryuKeyCheckTask *keys = new SenryuKeyCheckTask(this);
    keys->Suspend();
    mParallel.AddTail(keys);
    mSerial.AddTail(MTaskRunTransfer::Create(keys));
    mUnk11C = keys;
}
#pragma opt_common_subs reset

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

extern "C" const MPoint data_ov007_021918cc;      // the back button's place
extern "C" const char data_ov007_02191e68[];      // ""
extern "C" const char data_ov007_02191e6c[];      // the page number's format
extern const u32 data_0208ac3c;                   // the back button's help

inline DinoSenryuBackButton::DinoSenryuBackButton(DinoUiSenryuMenu *menu, MSmartOam *oam) : SenryuMenuItem(3, menu) {
    unkAC = (u32)menu;
    mTextId = data_0208ac3c;
    mMotion = new MMotion(std::string(data_ov007_02191880.mBackPath), oam, 2, 1);
    mMotion->Play(2, 0);
    mMotion->SetPosition(data_ov007_021918cc);
    mMotion->SetPriority(3);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    DinoHelpItem::Enable();
    mFrame = mMotion->GetFrame();
}

// The menu's OAM (the menu item's owner).
static inline MSmartOam *DinoSenryuOam(SenryuMenuItem *item) {
    return item->GetMenu()->mOam;
}

inline DinoUiSenryuMenuItem::DinoUiSenryuMenuItem(u32 index, DinoUiSenryuMenu *menu, MPoint pos)
    : SenryuMenuItem(index, menu) {
    mPos = pos;
    mTextId = 0;
    mFrame.left = pos.x - 7;
    mFrame.top = pos.y - 7;
    mFrame.right = pos.x + 6;
    mFrame.bottom = pos.y + 6;
    mMotion = NULL;
    // (Only the page arrows, 4 and 5, have motions; below 4 are the function keys.)
    if (mIndex >= 4) {
        switch (mIndex) {
        case 4:
            mMotion = new MMotion(std::string(data_ov007_02191880.mPrevPath), DinoSenryuOam(this), 2, 1);
            mMotion->Play(1, 0);
            mMotion->SetPosition(mPos);
            mMotion->SetPriority(2);
            mMotion->SetAnchor(2);
            mMotion->Show();
            mFrame = mMotion->GetFrame();
            mParallel.AddTail(mMotion);
            DinoHelpItem::Disable();
            break;
        case 5:
            mMotion = new MMotion(std::string(data_ov007_02191880.mNextPath), DinoSenryuOam(this), 2, 1);
            mMotion->Play(1, 0);
            mMotion->SetPosition(mPos);
            mMotion->SetPriority(2);
            mMotion->SetAnchor(2);
            mMotion->Show();
            mFrame = mMotion->GetFrame();
            mParallel.AddTail(mMotion);
            DinoHelpItem::Disable();
            break;
        }
    }
}

// Moves the cursor to the entry cur (wrapping around), and when that changes the page (or a rebuild is asked
// for), fills the page's three poems and rebuilds the menu items and their links.
#pragma inline_max_size(2000)
// NONMATCHING: 99.96%, the inlined menu items' string and frame temporaries are laid out in another order.
extern "C" void func_ov007_02185d28(DinoUiSenryuMenu *menu, int cur) {
    u32 count = menu->mCount;
    cur = (cur + count) % count;
    if (cur < 0) {
        cur += count;
    }
    int pos = cur;
    int page = pos / 3;
    if (pos != menu->mUnk134) {
        menu->mUnk134 = pos;
        u32 row = menu->mUnk134 % 3;
        MPoint cursor((data_ov007_02191a3c[row].right + data_ov007_02191a3c[row].left) / 2 - 6,
                      (data_ov007_02191a3c[row].bottom + data_ov007_02191a3c[row].top) / 2 + 4);
        menu->mCursor->SetPosition(cursor);
    }
    if (page != menu->mPage || menu->mUnk13C) {
        menu->mPage = page;
        menu->mUnk13C = FALSE;
        menu->mSilentFocus = TRUE;
        for (int i = 0; i < 3; i++) {
            int entry = menu->mPage * 3 + i;
            if (menu->mEntries[entry] & 0x80) {
                if (menu->mEntries[entry] & 1) {
                    (&menu->mFont120)[i]->SetShadowColor(4);
                } else {
                    (&menu->mFont120)[i]->SetShadowColor(1);
                }
                if (menu->mEntries[entry] & 2) {
                    menu->mMarks[i]->Show();
                } else {
                    menu->mMarks[i]->Hide();
                }
                menu->mFrames[i]->Hide();
                std::wstring *lines = menu->mLineTexts[entry];
                for (int j = 0; j < 3; j++) {
                    std::wstring line(lines[j]);
                    menu->mLines[i][j]->SetText(std::string(data_ov007_02191e68));
                    if (menu->mLines[i][j]->mState == MTask::STATE_RUN) {
                        menu->mLines[i][j]->Execute();
                    }
                    menu->mLines[i][j]->SetText(std::wstring(line));
                }
                MPoint size = menu->mFont120->MeasureText(std::wstring(menu->mUnk728[entry]), 0, 0);
                menu->mNames[i]->SetPosition(0xE5 - size.x, data_ov007_02191880.mNameOffset.y + data_ov007_021919ac[i].y);
            } else {
                for (int j = 0; j < 3; j++) {
                    menu->mLines[i][j]->SetText(std::string(data_ov007_02191e68));
                }
                menu->mNames[i]->SetText(std::string(data_ov007_02191e68));
                menu->mFrames[i]->Show();
            }
        }
        char text[16];
        OS_SPrintf(text, data_ov007_02191e6c, menu->mPage + 1, (menu->mCount + 2) / 3);
        menu->mPageText->SetText(std::string(text));
        menu->ClearItems();
        MMenuItem *items[3];
        items[0] = new DinoUiSenryuListHideMenuItem(0, menu, data_ov007_02191a3c[0]);
        menu->AddItem(items[0]);
        items[1] = new DinoUiSenryuListHideMenuItem(1, menu, data_ov007_02191a3c[1]);
        menu->AddItem(items[1]);
        items[2] = new DinoUiSenryuListHideMenuItem(2, menu, data_ov007_02191a3c[2]);
        menu->AddItem(items[2]);
        int last = -1;
        for (int i = 0; i < 3; i++) {
            if (menu->mEntries[page * 3 + i] & 0x80) {
                items[i]->Enable();
                last = i;
            } else {
                items[i]->Disable();
            }
        }
        DinoSenryuBackButton *back = new DinoSenryuBackButton(menu, menu->mOam);
        menu->AddItem(back);
        menu->mBackButton = back;
        if (menu->mCount > 3) {
            menu->mPrevArrow = new DinoUiSenryuMenuItem(4, menu, MPoint(0xD0, 0xD0));
            MMenuItem *arrow = menu->mPrevArrow;
            menu->AddItem(arrow);
            arrow->SetKeys(0x200);
            arrow->SetNoFocus(TRUE);
            menu->mNextArrow = new DinoUiSenryuMenuItem(5, menu, MPoint(0x30, 0xD0));
            arrow = menu->mNextArrow;
            menu->AddItem(arrow);
            arrow->SetKeys(0x100);
            arrow->SetNoFocus(TRUE);
        }
        menu->AddLink(items[0], 4, back);
        if (last >= 1) {
            menu->AddLink(items[0], 2, items[1]);
            menu->AddLink(items[1], 4, items[0]);
        }
        if (last >= 2) {
            menu->AddLink(items[1], 2, items[2]);
            menu->AddLink(items[2], 4, items[1]);
        }
        menu->AddLink(back, 2, items[0]);
        if (last >= 0) {
            menu->AddLink(items[last], 2, back);
            menu->AddLink(back, 4, items[last]);
        }
        menu->FocusItem(items[menu->mUnk134 % 3]);
    }
    if (menu->mPrevArrow != NULL && menu->mPrevArrow->mMotion != NULL) {
        menu->mPrevArrow->mMotion->Play(1, 0);
    }
    if (menu->mNextArrow != NULL && menu->mNextArrow->mMotion != NULL) {
        menu->mNextArrow->mMotion->Play(1, 0);
    }
}
#pragma inline_max_size reset

extern "C" const char data_ov007_02191e74[];
extern const u32 data_0208ac74, data_0208ac34, data_0208ac78, data_0208ac38, data_0208ac70, data_0208ac30;

// A poem: asks whether to mark it (or unmark it) or read it; the arrows turn the page.
void DinoUiSenryuMenu::OnDecide(MMenuItem *item) {
    if (item == NULL) {
        return;
    }
    u32 id = ((SenryuMenuItem *)item)->mIndex;
    if (id < 0xF0 && id != 4 && id != 5) {
        mUnk118->Clear();
    }
    switch (((SenryuMenuItem *)item)->mIndex) {
    case 0:
    case 1:
    case 2: {
        u32 base = mPage * 3;
        u32 index = base + ((SenryuMenuItem *)item)->mIndex;
        mSerial.AddTail(CreateCurChangeTask(index));
        if (!(mEntries[index] & 0x80)) {
            break;
        }
        if (!(mEntries[index] & 2)) {
            u32 marked = 0;
            for (int i = 0; i < 42; i++) {
                if ((mEntries[i] & 0x82) == 0x82) {
                    marked++;
                }
            }
            if (marked >= 41) {
                break;
            }
        }
        mChoice = 1;
        DinoCommonWindowMenu *menu = new DinoCommonWindowMenu(gLcd->GetBottomDisplay(), &mChoice,
                                                              MPoint(0x90, (((SenryuMenuItem *)item)->mIndex - 1) * 0x3A + 0x40), NULL, 0);
        if (mEntries[index] & 2) {
            menu->AddChoice(2, std::string(gDinoResource->GetText(data_0208ac74)), std::string(gDinoResource->GetText(data_0208ac34)));
        } else {
            menu->AddChoice(1, std::string(gDinoResource->GetText(data_0208ac78)), std::string(gDinoResource->GetText(data_0208ac38)));
        }
        menu->AddChoice(3, std::string(data_ov007_02191e74) + gDinoResource->GetText(data_0208ac70),
                        std::string(gDinoResource->GetText(data_0208ac30)));
        GetSerial()->AddTail(menu, new DinoUiSenryuSubMenu(this, &mChoice, index));
        break;
    }
    case 4: {
        DinoSoundPlaySe(0x93, 0);
        u32 cur;
        if (mPage != 0) {
            cur = (mPage - 1) * 3;
        } else {
            cur = ((mCount - 1) / 3) * 3;
        }
        GetSerial()->AddTail(gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(15, 0), CreateCurChangeTask(cur),
                             gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(15, 0));
        break;
    }
    case 5: {
        DinoSoundPlaySe(0x93, 0);
        u32 cur = mPage;
        if (cur == (mCount - 1) / 3) {
            cur = 0;
        } else {
            cur = (cur + 1) * 3;
        }
        GetSerial()->AddTail(gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(15, 0), CreateCurChangeTask(cur),
                             gLcd->GetBottomDisplay()->mBright->CreateFadeInFromBlack(15, 0));
        break;
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
    mPrevArrow = NULL;
    mNextArrow = NULL;
    mBackButton = NULL;
}

#pragma auto_inline off
// Vector members the game emits in this file (made explicitly until the code that uses them is decompiled).
template void std::__vec_deleter<MPictureFile, std::allocator<MPictureFile> >::sub_push_back(const MPictureFile &x, Metrowerks::int2type<false>);
template std::wstring std::wstring::substr(std::wstring::size_type pos, std::wstring::size_type n) const;

extern const int data_02089ac8, data_02089acc, data_02089ad0;

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
    MDisplay *display = gLcd->GetBottomDisplay();
    MBright *bright = display->mBright;
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

// Dims the keyboard behind a window while the name is blank; lifts it once a letter is typed.
void DinoUiKeyboardMenu::OnIdle() {
    BOOL changed = FALSE;
    for (int i = 0; i < mMaxLength && i < mUnk19C.size(); i++) {
        if (mUnk19C[i] != mUnk1B4[2][29]) {
            changed = TRUE;
            break;
        }
    }
    if (changed) {
        gLcd->GetBottomDisplay()->mBlend->Reset();
        gLcd->GetBottomDisplay()->mWin->GetWindow0()->Hide();
        return;
    }
    gLcd->GetBottomDisplay()->mBlend->SetTarget1(0xF);
    gLcd->GetBottomDisplay()->mBlend->SetTarget2(0xF);
    gLcd->GetBottomDisplay()->mBlend->SetMode(3);
    gLcd->GetBottomDisplay()->mBlend->SetBrightness(8);
    gLcd->GetBottomDisplay()->mWin->GetWindow0()->SetShape(0x4F, 0xAE, 0xB0, 0xBA);
    gLcd->GetBottomDisplay()->mWin->GetWindow0()->SetBlendEnabled(1);
    gLcd->GetBottomDisplay()->mWin->GetWindow0()->SetTarget(0x1F);
    gLcd->GetBottomDisplay()->mWin->GetWindow0()->Show();
    gLcd->GetBottomDisplay()->mWin->GetObjWindow()->SetBlendEnabled(0);
    gLcd->GetBottomDisplay()->mWin->GetObjWindow()->SetTarget(0x1F);
    gLcd->GetBottomDisplay()->mWin->GetObjWindow()->Show();
}

// The file's static initialization (see dino_static_init.h).
// NONMATCHING: scheduling (the game folds none of the origin subtractions and stores the origin later).
extern "C" u32 data_ov007_02191980[];
extern "C" u32 data_ov007_021919bc[];
extern "C" u32 data_ov007_02191a00[];
extern "C" u32 data_ov007_02191a4c[];
extern "C" u32 data_ov007_02191a5c[];
#pragma always_inline on
static struct DinoUiKeyboardStaticInit {
    DinoUiKeyboardStaticInit() {
        DINO_POOL_NV(MPoint, data_ov007_02191880, 0x4c) = MPoint(128, 96);
        DINO_POOL_NV(MPoint, data_ov007_02191880, 0x44) = MPoint(123, 120);
        DINO_POOL_NV(MPoint, data_ov007_02191880, 0x54) = MPoint(133, 120);
        DINO_POOL_NV(MPoint, data_ov007_02191880, 0x64) = MPoint(188, 43);
        DINO_POOL_NV(MPoint, data_ov007_02191880, 0x3c) = MPoint(242, 16);
        DINO_POOL_NV(MPoint, data_ov007_02191880, 0x34) = MPoint(0, 0);
        DINO_POOL_NV(MPoint, data_ov007_02191980, 0x2c) =
            DINO_POOL_NV(MPoint, data_ov007_02191880, 0x34) - MPoint(0, 2);
        *(MPoint *) &data_ov007_021919b4 = MPoint(0, 56);
        DINO_POOL_NV(MPoint, data_ov007_021919bc, 0) = MPoint(0, 114);
        DINO_POOL_NV(MPoint, data_ov007_02191980, 0x44) = MPoint(34, 17) - DINO_POOL_NV(MPoint, data_ov007_02191880, 0x34);
        DINO_POOL_NV(MPoint, data_ov007_02191980, 0x4c) = MPoint(34, 31) - DINO_POOL_NV(MPoint, data_ov007_02191880, 0x34);
        DINO_POOL_NV(MPoint, data_ov007_02191980, 0x54) = MPoint(34, 45) - DINO_POOL_NV(MPoint, data_ov007_02191880, 0x34);
        DINO_POOL_NV(MRect, data_ov007_02191a00, 0x3c) = MRect(21, 6, 249, 62);
        DINO_POOL_NV(MRect, data_ov007_02191a4c, 0) = MRect(21, 64, 249, 120);
        DINO_POOL_NV(MRect, data_ov007_02191a5c, 0) = MRect(21, 122, 249, 178);
        DINO_POOL_NV(u32, data_ov007_02191980, 0x1c) = DINO_POOL_NV(u32, data_ov007_02191880, 0x1c);
        DINO_POOL_NV(u32, data_ov007_02191980, 0x20) = DINO_POOL_NV(u32, data_ov007_02191880, 0x28);
        DINO_POOL_NV(u32, data_ov007_02191980, 0x24) = DINO_POOL_NV(u32, data_ov007_02191880, 0x28);
        DINO_POOL_NV(u32, data_ov007_02191980, 0x28) = DINO_POOL_NV(u32, data_ov007_02191880, 0x18);
    }
} sDinoUiKeyboardStaticInit;
