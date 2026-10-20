// DinoTalkConsoleFilter, DinoTalkWindowWaitStartTask.
// Only the destructors are decompiled so far.

#define MSL_STRING_DEFINITIONS
#define MSL_STRING_BASICS
#include "game/dino_talk_console_filter.h"
#include "game/dino_talk_window_wait_start_task.h"
#include "game/dino_talk_window.h"
#include "m/console.h"
#include "game/dino_resource.h"
#include "game/dino_sound.h"
#include "m/seed_generator.h"
#include "m/view.h"
#include "game/dino_static_init.h"

// The text settings in the resource table.
struct DinoTalkSettings {
    u8 unk00[0x10];
    MFx32 mRate;    // 0x10: characters per frame
};

DinoTalkConsoleFilter::~DinoTalkConsoleFilter() {
}

DinoTalkWindowWaitStartTask::~DinoTalkWindowWaitStartTask() {
}

void DinoTalkWindow::Idle() {
    (this->*mState)();
}

DinoTalkWindow::~DinoTalkWindow() {
}

DinoTalkWindowNamePanel::~DinoTalkWindowNamePanel() {
}

void DinoTalkWindowNamePanel::Idle() {
}

void DinoTalkWindowWaitStartTask::Execute() {
    if (*mStarted) {
        Exit();
    }
}

#pragma auto_inline off

MTask *DinoTalkWindow::CreateWaitStartTask() {
    return new DinoTalkWindowWaitStartTask(&mStarted);
}

void DinoTalkWindow::SetUnkF4() {
    mUnkF4 = TRUE;
}

void DinoTalkWindow::ClearUnkF4() {
    mUnkF4 = 0;
}

void DinoTalkWindow::SetRate(MFx32 rate) {
    mRate = rate;
    mConsole->SetRate(rate);
}

void DinoTalkWindow::Print(const std::string &text) {
    mConsole->Print(std::string(text));
}

void DinoTalkWindow::ResetRate() {
    MFx32 rate = ((const DinoTalkSettings *) gDinoResource->GetExcavate7C())->mRate;
    mConsole->SetRate(rate);
}

BOOL DinoTalkWindow::IsEmpty() {
    return mConsole->IsEmpty();
}

void DinoTalkWindow::SetUnkAC(u32 value) {
    mUnkAC = value;
}

void DinoTalkWindow::SetUnkB0(u8 value) {
    mUnkB0 = value;
}

u8 DinoTalkWindow::GetUnkB0() {
    return mUnkB0;
}

// While the text is still typing, stops it there and carries on after the frames.
void DinoTalkWindow::Pause(int frames) {
    if (mConsole->GetSpeed() != MFx32(0)) {
        mConsole->PushChar(0xC);
        MTask *run = MTaskRunTransfer::Create(mConsole);
        MTask *timer = new MTimer(frames);
        MTaskSet *anims = mAnims;
        anims->AddTail(timer);
        anims->AddTail(run);
    }
}

// The page is all out (the console waits) and the window is still.
BOOL DinoTalkWindow::IsPageDone() {
    BOOL done = mConsole->mState == MTask::STATE_SUSPEND;
    if (done) {
        done = mAnims->IsEmpty();
    }
    return done;
}

// Clears the window for the next page, back at the normal rate.
void DinoTalkWindow::NextPage() {
    DinoSoundPlaySe(0xB0, 0);
    mConsole->SetRate(mRate);
    mConsole->Clear();
    mConsole->Run();
}

// Shows the rest of the page at once.
void DinoTalkWindow::Skip() {
    DinoSoundPlaySe(0xAF, 0);
    mConsole->SetSpeed(MFx32(0));
    if (!mAnims->IsEmpty()) {
        mConsole->Run();
        mAnims->Clear();
    }
}

// The TU's pooled data.
extern "C" struct {
    u8 unk00[0x10];
    s32 unk10;
    u8 unk14[4];
    MPoint unk18;   // centered in 256x64 under unk40
    u8 unk20[8];
    MPoint unk28;
    u8 unk30[8];
    MPoint mFlipOffset;     // 0x38: the name panel's, on the other side
    MPoint unk40;   // y derived from unk48's (mwcc reuses the stored value)
    MPoint unk48;
} data_020b62f0;

// NONMATCHING: register allocation (the game keeps the flip offset in registers and spills the shake).
void DinoTalkWindow::StateShow() {
    if (mUnkF4 && IsPageDone() && !mUnkB0) {
        mNextMark->Show();
    } else {
        mNextMark->Hide();
    }
    if (mUnkAC == 0) {
        return;
    }
    mUnkAC--;
    u32 n = mUnkAC;
    int y = gDinoResource->GetRandom()->Next(n * 2 + 1) - n;
    int x = gDinoResource->GetRandom()->Next(n * 2 + 1) - mUnkAC;
    DinoTalkWindowNamePanel *panel = mNamePanel;
    if (panel != NULL) {
        panel->mShake = MPoint(x, y);
        MPoint flip(0, 0);
        if (panel->mFlip) {
            flip = data_020b62f0.mFlipOffset;
        }
        panel->mPanel->SetPosition(
            MPoint(panel->mPos.x + panel->mShake.x + flip.x, panel->mPos.y + panel->mShake.y + flip.y));
        panel->mName->SetPosition(MPoint(panel->mPos.x + panel->mShake.x + panel->mNameOffset.x + flip.x,
                                         panel->mPos.y + panel->mShake.y + panel->mNameOffset.y + flip.y));
    }
    mFrame->SetPosition(MPoint(mPos.x + x, mPos.y + y));
    mConsole->SetPosition(MPoint(mConsolePos.x + x, mConsolePos.y + y));
}

BOOL DinoTalkWindow::IsAnimating() {
    return !mAnims->IsEmpty();
}

// String members the game emits in this file (made explicitly until the filter code using them is decompiled).
template std::wstring::const_iterator std::wstring::erase(std::wstring::const_iterator p);
template void std::wstring::push_back(wchar_t c);
template void std::string::push_back(char c);

// String members the game emits in this file.
template std::wstring::basic_string(const std::wstring &other);
template std::wstring &std::wstring::operator=(const std::wstring &str);
// (Calling these here instantiates them with the names the game's callers use.)
#pragma dont_inline on
int DinoTalkFilterCompareAnchor(const std::string &s, const char *text) {
    return s.compare(text);
}
int DinoTalkFilterCompare4Anchor(const std::string &s, const char *text) {
    return s.compare(0, 0, text, 0);
}
std::wstring &DinoTalkFilterReplaceAnchor(std::wstring &s, const std::wstring &text) {
    return s.replace(0, 0, text);
}
#pragma dont_inline reset

// The file's static initialization (see dino_static_init.h).
static struct DinoTalkConsoleFilterStaticInit {
    DinoTalkConsoleFilterStaticInit() {
        data_020b62f0.unk10 = 3 << 14;
        data_020b62f0.unk48.x = 0;
        data_020b62f0.unk48.y = 4;
        data_020b62f0.unk40.x = 216;
        data_020b62f0.unk40.y = data_020b62f0.unk48.y * 2 + 36;
        data_020b62f0.unk18.x = (256 - data_020b62f0.unk40.x) / 2;
        data_020b62f0.unk18.y = (64 - data_020b62f0.unk40.y) / 2 + 134;
        data_020b62f0.unk28.x = 96;
        data_020b62f0.unk28.y = 12;
        data_020b62f0.mFlipOffset.x = 0;
        data_020b62f0.mFlipOffset.y = -64;
    }
} sDinoTalkConsoleFilterStaticInit;
