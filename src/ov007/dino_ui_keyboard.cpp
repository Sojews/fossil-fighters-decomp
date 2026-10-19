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

DinoUiKeyboardMenu::DinoUiKeyboardMenu(int mode, u32 arg, int flag)
    : MMenu(NULL, 0), mUnk13C(0), mUnk144(0), mMode(mode), mArg(arg), mUnk190(0x20), mFlag(flag) {
}

DinoUiKeyboardMenu::~DinoUiKeyboardMenu() {
    gLcd->GetBottomDisplay()->mWin->GetWindow0()->Hide();
    gLcd->GetBottomDisplay()->mBlend->Reset();
}

extern "C" void func_ov007_021832f0(void *, u32);

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

void PageChangeTask::Execute() {
    func_ov007_021832f0((void *)mUnk10, mUnk14);
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

extern "C" void func_ov007_02186c74(void *owner, u32 value, u32 arg);

void DinoUiSenryuSubMenu::Execute() {
    func_ov007_02186c74(mOwner, *mValue, mArg);
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

extern "C" void func_ov007_021841f4(u32 owner);
extern "C" void func_ov007_02186dd8(u32 owner);

void DinoKBBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    func_ov007_021841f4(unkB8);
    DinoSoundPlaySe(0x90, 0);
}

void DinoSenryuBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mSerial.AddTail(mMotion->CreateWaitStopTask());
    func_ov007_02186dd8(unkAC);
    DinoSoundPlaySe(0x90, 0);
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
