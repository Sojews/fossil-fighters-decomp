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
#include "game/dino_common_font.h"
#include "game/dino_backup.h"
#include "m/picture.h"
#include "m/canvas.h"
#include "m/text_codec.h"
#include "m/pal.h"
#include "m/oam_canvas.h"
#include "m/font.h"
#include "m/motion.h"
#include "m/smart_oam.h"

// A speaker's entry in the text settings (offsets from the entry).
struct DinoTalkSpeaker {
    u32 mName;      // 0x00
    u32 mColor;     // 0x04: the name panel's colors (r, g, b)
    u32 mColor2;    // 0x08
    u32 mSe;        // 0x0C: the typing sound

    const char *GetName() const { return (const char *) this + mName; }
    const u8 *GetColor() const { return mColor == 0 ? NULL : (const u8 *) this + mColor; }
    const u8 *GetColor2() const { return mColor2 == 0 ? NULL : (const u8 *) this + mColor2; }
};

// The text settings in the resource table (offsets from the table).
struct DinoTalkSettings {
    u32 mSpeakerCount;  // 0x00
    u32 mSpeakers;      // 0x04: the speaker entries' offsets
    u32 unk08;
    u32 mRates;         // 0x0C: the text speeds, by the option
    MFx32 mRate;        // 0x10: characters per frame

    const DinoTalkSpeaker *GetSpeaker(u32 i) const {
        return (const DinoTalkSpeaker *) ((const u8 *) this + ((const u32 *) ((const u8 *) this + mSpeakers))[i]);
    }
    MFx32 GetRate(u32 i) const { return ((const MFx32 *) ((const u8 *) this + mRates))[i]; }
};

// The TU's pooled data.
extern "C" struct {
    u8 unk00[0x08];
    const char *mPanelPath; // 0x08: the name panel's motion
    const char *mNextMarkPath;  // 0x0C
    s32 unk10;
    const char *mFramePath; // 0x14
    MPoint unk18;   // the console's position: centered in 256x64 under unk40
    u8 unk20[8];
    MPoint unk28;
    u8 unk30[8];
    MPoint mFlipOffset;     // 0x38: the name panel's, on the other side
    MPoint unk40;   // the console's size; y derived from unk48's (mwcc reuses the stored value)
    MPoint unk48;   // the console's letter and line spacing
    void (DinoTalkWindow::*mFirstState)();  // 0x50
} data_020b62f0;

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


// Moves the name panel by shake (on the other side if flipped).
static inline void DinoTalkShakePanel(DinoTalkWindowNamePanel *panel, MPoint shake) {
    panel->mShake = shake;
    MPoint flip(0, 0);
    if (panel->mFlip) {
        flip = data_020b62f0.mFlipOffset;
    }
    panel->mPanel->SetPosition(panel->mPos + panel->mShake + flip);
    panel->mName->SetPosition(panel->mPos + panel->mShake + panel->mNameOffset + flip);
}

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
        DinoTalkShakePanel(panel, MPoint(x, y));
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

// An opaque 15-bit color from 8-bit components.
#define DINO_TALK_RGB(r, g, b) (0x8000 | ((u32) (r) >> 3) | (((u32) (g) >> 3) << 5) | (((u32) (b) >> 3) << 10))

// NONMATCHING: the game reloads `colored` after its first store, and allocates the color pointers differently.
DinoTalkWindow::DinoTalkWindow(MDisplay *display, std::string speaker, int oam, int priority, u8 flip)
    : MProcess(display), mText(MDecodeUtf8(speaker)), mUnkB0(0), mOam(oam), mFlip(flip) {
    u32 option = gDinoBackup->GetUnk34_16();
    mRate = ((const DinoTalkSettings *) gDinoResource->GetExcavate7C())->GetRate(option);
    mUnkF4 = FALSE;
    if (mFlip) {
        mOffset.x = data_020b62f0.mFlipOffset.x;
        mOffset.y = data_020b62f0.mFlipOffset.y;
    } else {
        mOffset.x = 0;
        mOffset.y = 0;
    }
    BOOL colored;
    mStarted = colored = FALSE;
    u32 se = mText.size() == 0 ? 0xAE : 0xAA;
    const DinoTalkSettings *settings = (const DinoTalkSettings *) gDinoResource->GetExcavate7C();
    for (u32 i = 0; i < settings->mSpeakerCount; i++) {
        const DinoTalkSpeaker *entry = settings->GetSpeaker(i);
        if (speaker == entry->GetName()) {
            mColors[1] = DINO_TALK_RGB(entry->GetColor()[0], entry->GetColor()[1], entry->GetColor()[2]);
            mColors[2] = DINO_TALK_RGB(entry->GetColor2()[0], entry->GetColor2()[1], entry->GetColor2()[2]);
            se = entry->mSe;
            colored = TRUE;
            break;
        }
    }
    mAnims = new MSerialTaskSet;
    GetParallel()->AddTail(mAnims);
    if (mText.size() == 0) {
        mNamePanel = NULL;
    } else {
        mNamePanel = new DinoTalkWindowNamePanel(this, mText, priority, mFlip, colored ? mColors : NULL);
        GetParallel()->AddTail(mNamePanel);
    }
    DinoCommonFont *font = new DinoCommonFont(0, 1, 0);
    MCanvas *canvas;
    if (mOam != 0) {
        canvas = new MSmartObjCanvas((MSmartOam *) mOam, data_020b62f0.unk40, gDinoResource->GetUnkTable(), TRUE);
    } else {
        canvas = new MTextCanvas(data_020b62f0.unk40, gDinoResource->GetUnkTable(), 0, TRUE);
    }
    canvas->Clear();
    if (canvas->mState == 0) {
        canvas->Execute();
    }
    mConsole = new MConsole(canvas, font, data_020b62f0.unk48, MFx32(0));
    mConsole->SetRate(mRate);
    mConsole->SetPriority(priority);
    mConsole->SetPosition(MPoint(data_020b62f0.unk18.x + mOffset.x, data_020b62f0.unk18.y + mOffset.y));
    mConsole->SetUnk44(12);
    mConsole->SetFilter(new DinoTalkConsoleFilter(font, this, se));
    mConsoleTasks = new MSerialTaskSet;
    GetParallel()->AddTail(mConsoleTasks);
    mConsoleTasks->AddTail(mConsole);
    mConsoleTasks->mState = 1;
    mFrame = new MTextPicture(data_020b62f0.mFramePath, TRUE, TRUE);
    mFrame->SetPriority(priority);
    if (colored) {
        ((MTextPicture *) mFrame)->GetPalBlock()->LoadColors(mColors, 0, 3);
    }
    GetParallel()->AddTail(mFrame);
    mUnkAC = 0;
    mSerial.AddTail(DinoSoundCreatePlaySeTask(0xA8, 0));
    if (!mFlip) {
        GetSerial()->AddTail(new MTimer(1), mFrame->CreateShowTransfer(), mConsole->CreateShowTransfer(),
                             mFrame->CreateCoordTransfer(0, 0x60), new MTimer(5), mFrame->CreateCoordTransfer(-0x100, 0x60),
                             new MTimer(5), mFrame->CreateCoordTransfer(-0x200, 0x60), new MTimer(5),
                             mFrame->CreateCoordTransfer(-0x300, 0x60));
    } else {
        GetSerial()->AddTail(new MTimer(1), mFrame->CreateShowTransfer(), mConsole->CreateShowTransfer(),
                             mFrame->CreateCoordTransfer(0, mOffset.y + 0x70), new MTimer(5),
                             mFrame->CreateCoordTransfer(-0x100, mOffset.y + 0x70), new MTimer(5),
                             mFrame->CreateCoordTransfer(-0x200, mOffset.y + 0x70), new MTimer(5),
                             mFrame->CreateCoordTransfer(-0x300, mOffset.y + 0x70));
    }
    mUnk8C = 0;
    MSmartOam *markOam = new MSmartOam(1, 0, TRUE);
    mNextMark = new MMotion(data_020b62f0.mNextMarkPath, markOam, 0, 1);
    ((MMotion *) mNextMark)->SetPosition(0x80, mOffset.y + 0x60);
    ((MMotion *) mNextMark)->Play(1, 0);
    GetParallel()->AddTail(mNextMark, markOam);
    mState = data_020b62f0.mFirstState;
}

DinoTalkWindowNamePanel::DinoTalkWindowNamePanel(DinoTalkWindow *owner, std::wstring text, int priority, u8 flip,
                                                 const void *palette)
    : MProcess(NULL), mOwner(owner), mText(text), mPos(0, 0), mFlip(flip) {
    mOam = new MSmartOam(0x14, 0, TRUE);
    mOam->SetUnk1C(7);
    mFont = gDinoResource->CreateFont12(0, 1, 0);
    mWidth = mFont->MeasureText(std::wstring(mText), 0, 0).x;
    mPanelWidth = (mWidth + 7) & ~7;
    mNameOffset.x = ((mPanelWidth - mWidth) >> 1) + 8;
    mNameOffset.y = 6;
    mName = new MConsole(new MSmartObjCanvas(mOam, MPoint(mWidth, 12), gDinoResource->GetUnkTable(), TRUE), mFont,
                         MPoint(0, 0), MFx32(0));
    mName->SetPriority(priority);
    mName->SetFilter(new DinoTalkConsoleFilter(mOwner));
    mPanel = new MMotion(data_020b62f0.mPanelPath, mOam, 0, 1);
    mPanel->SetPriority(priority);
    mPanel->SetAnchor(1);
    mPanel->Play(1, 0);
    mPanel->Hide();
    if (palette != NULL) {
        mPanel->GetPalette(0)->LoadColors(palette, 0, 3);
    }
    mTiles = mPanelWidth >> 3;
    mUnkAC = 0;
    mShake.x = 0;
    mShake.y = 0;
    GetParallel()->AddTail(mName, mPanel, mOam);
    GetSerial()->AddTail(new MTimer(1), mName->CreateShowTransfer());
}

// NONMATCHING: this and the zero swap registers (r5/r6), and the position temporaries' stack slots.
MTask *DinoTalkWindow::CreateCloseTask() {
    mUnkF4 = FALSE;
    mUnkAC = 0;
    mConsole->SetPosition(mConsolePos);
    mFrame->SetPosition(mPos);
    if (mNamePanel != NULL) {
        DinoTalkShakePanel(mNamePanel, MPoint(0, 0));
    }
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(DinoSoundCreatePlaySeTask(0xA9, 0));
    if (!mFlip) {
        set->AddTail(mFrame->CreateCoordAnime(mFrame->GetPosition(), mFrame->GetPosition().Offset(0, 0x60), 0x14, 2),
                     mConsole->CreateCoordAnime(mConsole->GetPosition(), mConsole->GetPosition().Offset(0, 0x60), 0x14, 2));
        DinoTalkWindowNamePanel *panel = mNamePanel;
        if (panel != NULL) {
            MPoint name = panel->mPos + panel->mNameOffset;
            MTask *nameAnime = panel->mName->CreateCoordAnime(name, name.Offset(0, 0x60), 0x14, 2);
            MTask *panelAnime = panel->mPanel->CreateCoordAnime(panel->mPos, panel->mPos.Offset(0, 0x60), 0x14, 2);
            MParallelTaskSet *anims = new MParallelTaskSet(TRUE);
            anims->AddTail(panelAnime, nameAnime);
            set->AddTail(anims);
        }
    } else {
        mConsole->Hide();
        if (mNamePanel != NULL) {
            set->AddTail(MTaskExitTransfer::Create(mNamePanel));
        }
        MSerialTaskSet *slide = new MSerialTaskSet(TRUE);
        slide->AddTail(mFrame->CreateCoordTransfer(-0x300, mOffset.y + 0x70), new MTimer(3),
                       mFrame->CreateCoordTransfer(-0x200, mOffset.y + 0x70), new MTimer(3),
                       mFrame->CreateCoordTransfer(-0x100, mOffset.y + 0x70), new MTimer(3),
                       mFrame->CreateCoordTransfer(0, mOffset.y + 0x70), new MTimer(3));
        set->AddTail(slide);
    }
    return set;
}

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
