// DinoEventTalkWindow: the window event text is shown in.
// Only the background-call template it instantiates is here so far.

#pragma ipa file

#define MSL_STRING_BASICS
#define MSL_VECTOR_COMMON_DEFINITION
#include "m/input.h"
#include "m/lcd.h"
#include "m/text_codec.h"
#include "game/dino_resource.h"
#include "game/dino_talk_window.h"
#include "game/dino_event_talk_window.h"
#include "m/bg_new_task.h"
#include "game/dino_window_choice.h"
#include "game/dino_common_window_menu_item_flush_task_menu.h"

DinoEventTalkWindow::~DinoEventTalkWindow() {
}

void DinoEventTalkWindow::SetYesNo(u32 arg, MPoint pos, u32 unkB4, const std::string &a, const std::string &b,
                                   const std::string &c, const std::string &d) {
    mUnk98 = arg;
    mPos = pos;
    mUnkB4 = unkB4;
    mUnkBC = a;
    mUnkC8 = b;
    mUnkD4 = c;
    mUnkE0 = d;
}

void DinoEventTalkWindow::SetChoices(u32 arg, MPoint pos, const std::vector<DinoWindowChoice> &choices, u32 unkB4) {
    mUnk9C = arg;
    mPos = pos;
    mChoices = choices;
    mUnkB4 = unkB4;
}

void DinoEventTalkWindow::AddEntry(u32 id, MRect rect, const std::string &text) {
    DinoCommonWindowMenuEntry entry;
    entry.id = id;
    entry.rect = rect;
    entry.text = text;
    mEntries.push_back(entry);
}

void DinoEventTalkWindow::Idle() {
    (this->*mState)();
}

// Emitted in this file in the game (the window starts background loads with MCreateBgCallTask); until the code
// that makes them is decompiled, the instances are made explicitly.
template class MBgCallTask0<DinoEventTalkWindow>;
template void std::__vector_common<true>::throw_length_error();
template MTask *MCreateBgCallTask(DinoEventTalkWindow *obj, void (DinoEventTalkWindow::*func)(), int thread);
template class std::__vec_deleter<DinoWindowChoice, std::allocator<DinoWindowChoice> >;
template class std::__vec_deleter<std::string, std::allocator<std::string> >;
template class std::__vec_deleter<DinoCommonWindowMenuEntry, std::allocator<DinoCommonWindowMenuEntry> >;

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoEventTalkWindow::*Load_04)();
    void (DinoEventTalkWindow::*StateEnd_0C)();
    void (DinoEventTalkWindow::*StateWait_14)();
    void (DinoEventTalkWindow::*StateStart_1C)();
    void (DinoEventTalkWindow::*StateEnd_24)();
    void (DinoEventTalkWindow::*StateYesNo_2C)();
    void (DinoEventTalkWindow::*StateChoose_34)();
    void (DinoEventTalkWindow::*StateEnd_3C)();
} data_020b6144;

// Waits for the window to show the page, then asks, chooses or ends.
// Makes the window (in the background): the text's first line names the speaker, the rest is printed.
void DinoEventTalkWindow::Load() {
    std::vector<std::string> lines = MSplitString(mText, '\n');
    std::string speaker(lines[0]);
    lines.erase(lines.begin());
    mText = MJoinStrings(lines, '\n');
    mWindow = new DinoTalkWindow(gLcd->GetBottomDisplay(), std::string(speaker), mArg,
                                 (mUnk98 != 0 || mUnk9C != 0) ? TRUE : FALSE, mFlag);
    mWindow->SetUnkF4();
    mWindow->Print(std::string(mText));
    mParallel.AddTail(mWindow);
    MTask *wait = mWindow->CreateWaitStartTask();
    GetSerial()->AddTail(new MTimer(1), wait);
    mUnk90 = 0;
}

void DinoEventTalkWindow::StatePage() {
    if (mWindow->IsEmpty()) {
        if (mUnk98 != 0) {
            mState = data_020b6144.StateYesNo_2C;
        } else if (mUnk9C != 0) {
            mState = data_020b6144.StateChoose_34;
        } else {
            mState = data_020b6144.StateEnd_0C;
        }
    } else {
        mWindow->NextPage();
        mUnk90 = 0;
    }
}

DinoEventTalkWindow::DinoEventTalkWindow(MDisplay *display, std::string text, int arg, u8 flag)
    : MProcess(display), mText(text), mArg(arg), mUnk98(0), mUnk9C(0), mFlag(flag) {
    mState = data_020b6144.StateStart_1C;
}

extern "C" const u32 data_0208accc, data_0208acc8;   // "yes", "no"

void DinoEventTalkWindow::StateYesNo() {
    mWindow->ClearUnkF4();
    mChoices.push_back(DinoWindowChoice(0, std::string(mUnkBC.size() == 0 ? std::string(gDinoResource->GetText(data_0208accc)) : mUnkBC),
                                        std::string(mUnkD4.size() == 0 ? std::string(gDinoResource->GetText(data_0208accc)) : mUnkD4)));
    mChoices.push_back(DinoWindowChoice(1, std::string(mUnkC8.size() == 0 ? std::string(gDinoResource->GetText(data_0208acc8)) : mUnkC8),
                                        std::string(mUnkE0.size() == 0 ? std::string(gDinoResource->GetText(data_0208acc8)) : mUnkE0)));
    mSerial.AddTail(new DinoCommonWindowMenu((void *)mUnk98, mPos, &mChoices, mUnkB4));
    mState = data_020b6144.StateEnd_24;
}

#pragma dont_inline on
DinoWindowChoice::DinoWindowChoice(u32 id, const std::string &text, const std::string &text2)
    : unk00(id), unk04(text), unk10(text2) {
}

void DinoEventTalkWindow::StateStart() {
    mInputHub = new MSingleInputHub(1);
    mInputHub->EnableTouch();
    mParallel.AddTail(mInputHub);
    MTaskSet *parallel = &mParallel;
    parallel->mUseMutex = TRUE;
    MTaskSet *serial = &mSerial;
    serial->mUseMutex = TRUE;
    mSerial.AddTail(MCreateBgCallTask(this, data_020b6144.Load_04, 1));
    mState = data_020b6144.StateWait_14;
}
#pragma dont_inline reset

// Shows the choices.
void DinoEventTalkWindow::StateChoose() {
    mWindow->ClearUnkF4();
    DinoCommonWindowMenu *menu = new DinoCommonWindowMenu((void *)mUnk9C, mPos, &mChoices, mUnkB4);
    for (std::vector<DinoCommonWindowMenuEntry>::iterator entry = mEntries.begin(); entry != mEntries.end(); entry++) {
        menu->AddEntry(entry->id, entry->rect, std::string(entry->text));
    }
    mSerial.AddTail(menu);
    mState = data_020b6144.StateEnd_3C;
}

// Pages on with A (or after holding the buttons a while); B makes the text fast, A skips it.
void DinoEventTalkWindow::StateWait() {
    MInput *input = mInputHub->GetInput(0);
    if (mWindow->GetUnkB0()) {
        if (!mWindow->IsAnimating() && mWindow->IsPageDone()) {
            StatePage();
        }
        return;
    }
    if (mWindow->IsPageDone()) {
        if (input->GetPressed() & 0x110B) {
            StatePage();
            return;
        }
        if (input->mButtons & 0x10A) {
            mUnk90++;
            if (mUnk90 >= 18) {
                StatePage();
            }
            return;
        }
        if (input->mButtons & 0x8) {
            mUnk90 = 18;
        } else {
            mUnk90 = 0;
        }
        return;
    }
    if (input->GetPressed() & 0x1) {
        mWindow->Skip();
        return;
    }
    if (input->mButtons & 0x110A) {
        mWindow->ResetRate();
    }
}

// Closes the window and exits.
void DinoEventTalkWindow::StateEnd() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *timer = new MTimer(1);
    MTask *windowExit = MTaskExitTransfer::Create(mWindow);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(mWindow->CreateCloseTask());
    serial->AddTail(windowExit);
    serial->AddTail(timer);
    serial->AddTail(exit);
}

// String members the game emits in this file.
template std::string &std::string::operator=(const std::string &str);
