// DinoCommonWindowConsoleFilter.

#pragma ipa file


#include "game/dino_common_window_console_filter.h"
#include "game/dino_help_disabler_task.h"
#include "game/common_window_message.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "game/dino_common_window.h"
#include "m/input.h"
#include "m/view.h"
#include "m/text_codec.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_static_init.h"
#include "game/dino_backup.h"
#include "game/dino_common_font.h"
#include "m/console.h"
#include "m/canvas.h"
#include "m/oam_canvas.h"
#include "m/smart_oam.h"
#include "m/motion.h"
#include "game/dino_resource.h"
#include "m/seed_generator.h"
#include "nitro/os.h"
#include <string.h>

DinoCommonWindowMessage::~DinoCommonWindowMessage() {
    mSerial.Clear();
    mParallel.Clear();
}

DinoCommonWindowConsoleFilter::~DinoCommonWindowConsoleFilter() {
}

DinoHelpDisablerTask::~DinoHelpDisablerTask() {
}

DinoCommonWindowMessageBox::~DinoCommonWindowMessageBox() {
}

DinoCommonWindowMessageWait1::DinoCommonWindowMessageWait1(std::string text, MPoint pos, int arg, u8 flag)
    : MProcess(NULL), mText(text) {
    mUnk80 = 0;
    mPos = pos;
    mUnk8C = arg;
    mUnk90 = flag;
}

void DinoCommonWindowMessageWait1::Idle() {
    if (mUnk80 == 0) {
        mParallel.AddTail(new DinoHelpDisablerTask);
        mText = MJoinStrings(MSplitString(mText, '\n'), '\n') + "  ";
        DinoCommonWindowMessage *message = new DinoCommonWindowMessage(mPos, std::string(mText), mUnk8C, 2);
        message->SetUnkA4(mUnk90);
        mParallel.AddTail(message);
        mSerial.AddTail(MTaskSetWaitEmptyTask::Create(&message->mSerial));
        mUnk80++;
    }
}

DinoCommonWindowMessageWait2::DinoCommonWindowMessageWait2(std::string text, MPoint pos, int arg, u8 flag)
    : MProcess(NULL), mText(text) {
    mUnk80 = 0;
    mPos = pos;
    mUnk8C = arg;
    mUnk90 = flag;
}

void DinoCommonWindowMessageWait2::Idle() {
    if (mUnk80 == 0) {
        mParallel.AddTail(new DinoHelpDisablerTask);
        mText = MJoinStrings(MSplitString(mText, '\n'), '\n') + "\n \n ";
        DinoCommonWindowMessage *message = new DinoCommonWindowMessage(mPos, std::string(mText), mUnk8C, 3);
        message->SetUnkA4(mUnk90);
        mParallel.AddTail(message);
        mSerial.AddTail(MTaskSetWaitEmptyTask::Create(&message->mSerial));
        mUnk80++;
    }
}

// Reads a decimal number from the console (pushing back the character after it).
static inline int DinoWindowReadNumber(MConsole *console) {
    int n = 0;
    for (;;) {
        u32 c = console->PopChar();
        if (c >= '0' && c <= '9') {
            n *= 10;
            n += c - '0';
        } else {
            console->PushChar(c);
            break;
        }
    }
    return n;
}

// (The game keeps each "%d" separately.)
#pragma dont_reuse_strings on
BOOL DinoCommonWindowConsoleFilter::Filter(MConsole *console, int c) {
    if (c != '$') {
        return FALSE;
    }
    if (console->IsEmpty()) {
        return TRUE;
    }
    switch ((u32)console->PopChar()) {
    case 'p': {
        DinoPlayerData player;
        gDinoBackup->GetPlayer(&player, 0);
        std::wstring name((const wchar_t *)player.name);
        for (std::wstring::const_reverse_iterator it = name.rbegin(); it != name.rend(); ++it) {
            console->PushChar(*it);
        }
        break;
    }
    case 'N': {
        DinoPlayerUnk58 rock;
        gDinoBackup->GetPlayerUnk58(&rock);
        std::wstring name = MDecodeUtf8(gDinoBackup->GetEntryName((const DinoBackupEntry *)&rock));
        for (std::wstring::const_reverse_iterator it = name.rbegin(); it != name.rend(); ++it) {
            console->PushChar(*it);
        }
        break;
    }
    case 'f':
        console->PushChar(12);
        break;
    case 'n':
        if (mFont != NULL) {
            mFont->Select(DinoWindowReadNumber(console));
        }
        break;
    case 'v': {
        char buf[0x100];
        OS_SPrintf(buf, "%d", gDinoBackup->GetLocalVar(DinoWindowReadNumber(console)));
        for (int i = strlen(buf) - 1; i >= 0; i--) {
            console->PushChar(buf[i]);
        }
        break;
    }
    case 'V': {
        char buf[0x100];
        OS_SPrintf(buf, "%d", gDinoBackup->GetSystemVar(DinoWindowReadNumber(console)));
        for (int i = strlen(buf) - 1; i >= 0; i--) {
            console->PushChar(buf[i]);
        }
        break;
    }
    case 'c':
        if (!console->IsEmpty()) {
            u32 color = console->PopChar();
            if (color >= '0' && color <= '9') {
                color -= '0';
            } else if (color >= 'a' && color <= 'f') {
                color -= 'a' - 10;
            } else if (color >= 'A' && color <= 'F') {
                color -= 'A' - 5;
            } else {
                color = 0;
            }
            if (mFont != NULL) {
                mFont->SetShadowColor(color + 1);
            }
        }
        break;
    }
    return TRUE;
}
#pragma dont_reuse_strings reset

DinoCommonWindowMessageWait1::~DinoCommonWindowMessageWait1() {
}

DinoCommonWindowMessageWait2::~DinoCommonWindowMessageWait2() {
}

void DinoHelpDisablerTask::Execute() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoCommonWindowMessage>;
template MTask *MCreateBgCallTask(DinoCommonWindowMessage *obj, void (DinoCommonWindowMessage::*func)(), int thread);
template class MCallTask0<DinoCommonWindowMessage, DinoCommonWindowMessage>;
template MTask *MCreateCallTask(DinoCommonWindowMessage *obj, void (DinoCommonWindowMessage::*func)());

DinoCommonWindowMessageBox::DinoCommonWindowMessageBox(MPoint pos, const std::string &text, int frames)
    : MProcess(NULL), mUnk78(0), mPos(pos), mText(text) {
    mFrames = frames;
    mUnk94 = 0;
    mUnk98 = TRUE;
}

DinoCommonWindowMessageBox::DinoCommonWindowMessageBox(MPoint pos, const std::string &text, u32 unk78, int frames)
    : MProcess(NULL), mUnk78(unk78), mPos(pos), mText(text) {
    mFrames = frames;
    mUnk94 = 0;
    mUnk98 = TRUE;
}

DinoCommonWindowMessageBox::DinoCommonWindowMessageBox(MDisplay *display, MPoint pos, const std::string &text,
                                                       int frames)
    : MProcess(display), mUnk78(0), mPos(pos), mText(text) {
    mFrames = frames;
    mUnk94 = 0;
    mUnk98 = TRUE;
}

// Shows the message until A, B or a touch (or until the frames run out).
void DinoCommonWindowMessageBox::Idle() {
    switch (mUnk94) {
    case 0: {
        mParallel.AddTail(new DinoHelpDisablerTask);
        (mInput = new MSingleInputHub(1))->EnableTouch();
        mParallel.AddTail(mInput);
        mText = MJoinStrings(MSplitString(mText, '\n'), '\n') + "   ";
        DinoCommonWindowMessage *message = new DinoCommonWindowMessage(mPos, std::string(mText), mFrames, 1);
        message->SetUnkA4(TRUE);
        if (message->mState == MTask::STATE_RUN) {
            message->Execute();
        }
        if (mUnk98) {
            message->SetUnkA8(0x38C);
        }
        mParallel.AddTail(message);
        mSerial.AddTail(MTaskSetWaitEmptyTask::Create(&message->mSerial));
        mUnk94++;
        break;
    }
    case 1:
        if (mInput->GetInput(0)->GetPressed() & 0x1003) {
            Exit();
        }
        if (mUnk78 != 0 && --mUnk78 == 0) {
            Exit();
        }
        break;
    }
}

DinoCommonWindowMessage::DinoCommonWindowMessage(MPoint pos, const std::string &text, int arg3, int arg4)
    : MProcess(NULL), mPos(pos), mText(text), mUnk90(arg3), mUnk94(-1), mUnk98(FALSE), mUnk9C(arg4), mUnkA4(FALSE),
      mUnkA8(0) {
}

DinoCommonWindowMessage::DinoCommonWindowMessage(MDisplay *display, MPoint pos, const std::string &text, int arg3,
                                                 int arg4)
    : MProcess(display), mPos(pos), mText(text), mUnk90(arg3), mUnk94(-1), mUnk98(FALSE), mUnk9C(arg4), mUnkA4(FALSE),
      mUnkA8(0) {
}

#pragma auto_inline off

void DinoCommonWindowMessage::SetUnkA8(u32 value) {
    mUnkA8 = value;
}

void DinoCommonWindowMessage::SetUnkA4(u8 value) {
    mUnkA4 = value;
}

// The TU's pooled data.
extern "C" struct {
    u8 unk00[0x08];
    const char *mSandglassPath; // 0x08
    const char *mCursorPath;    // 0x0C
    const char *mGaogaoPath;    // 0x10
    MPoint unk14;   // (0, 4): the console's spacing
    u8 unk1C[0x28];
    MPoint unk44;   // (216, 44): derived from unk14 (mwcc reuses the stored 4)
} data_020bc024;

// NONMATCHING: the game keeps the measuring canvas in ip (ours spills it) and loads gCurrentDisplay after copying
// the size.
// Measures the text, then makes its console, the window around it (anchored by mUnk90) and the mark of kind mUnk9C.
void DinoCommonWindowMessage::Load() {
    DinoCommonFont *font = new DinoCommonFont(0, 1, 0);
    MConsole *measure = new MConsole(new MDummyCanvas(MPoint(0xD8, 0xC0)), font, data_020bc024.unk14, MFx32(0));
    measure->SetFilter(new DinoCommonWindowConsoleFilter(font));
    measure->Print(std::string(mText));
    if (measure->mState == MTask::STATE_RUN) {
        measure->Execute();
    }
    MPoint size = measure->GetCursor();
    delete measure;
    DinoCommonFont *font2 = new DinoCommonFont(0, 1, 0);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, size, gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mUnk74 = new MConsole(canvas, font2, data_020bc024.unk14, MFx32(0));
    mUnk74->SetPriority(0);
    ((MConsole *)mUnk74)->SetFilter(new DinoCommonWindowConsoleFilter(font2));
    mUnk74->Hide();
    ((MConsole *)mUnk74)->Print(std::string(mText));
    if (mUnk74->mState == MTask::STATE_RUN) {
        mUnk74->Execute();
    }
    mParallel.AddTail(mUnk74);
    MPoint tiles((size.x + 7) / 8 + 2, (size.y + 7) / 8 + 2);
    int dx, dy;
    switch (mUnk90) {
    case 0:
        dx = 0;
        dy = 0;
        break;
    case 1:
        dx = tiles.x * 8;
        dy = 0;
        break;
    case 2:
        dx = 0;
        dy = tiles.y * 8;
        break;
    case 3:
        dx = tiles.x * 8;
        dy = tiles.y * 8;
        break;
    case 4:
        dx = tiles.x * 4;
        dy = tiles.y * 4;
        break;
    case 5:
        dx = tiles.x * 4;
        dy = 0;
        break;
    case 6:
        dx = tiles.x * 4;
        dy = tiles.y * 8;
        break;
    }
    mUnk74->SetPosition(mPos - MPoint(dx, dy) + MPoint(8, 8));
    mWindow = new DinoCommonWindow(0, false, false);
    mWindow->Hide();
    mWindow->SetFrame(MRect(MPoint(0, 0), tiles));
    mWindow->SetPriority(0);
    mWindow->SetPosition(MPoint(mPos.x - dx, mPos.y - dy));
    mParallel.AddTail(mWindow);
    MSmartOam *oam = NULL;
    if (mUnk9C != 0) {
        switch (mUnk9C) {
        case 1:
            oam = new MSmartOam(1, 0, 1);
            mUnkA0 = new MMotion(std::string(data_020bc024.mCursorPath), oam, 0, 1);
            mUnkA0->SetPosition(mPos - MPoint(0x68, 0x56) - MPoint(dx, dy) + MPoint(tiles.x * 8, tiles.y * 8) -
                                MPoint(0x10, 0x10));
            ((MMotion *)mUnkA0)->Play(1, 0);
            break;
        case 2:
            oam = new MSmartOam(1, 0, 1);
            mUnkA0 = new MMotion(std::string(data_020bc024.mSandglassPath), oam, 0, 1);
            mUnkA0->SetPosition(mPos - MPoint(dx, dy) + MPoint(tiles.x * 8, tiles.y * 8) - MPoint(0x10, 0x10));
            ((MMotion *)mUnkA0)->Play(1, 0);
            break;
        case 3:
            oam = new MSmartOam(10, 0, 1);
            mUnkA0 = new MMotion(std::string(data_020bc024.mGaogaoPath), oam, 0, 1);
            mUnkA0->SetPosition(mPos - MPoint(dx, dy) + MPoint(tiles.x * 8 / 2, tiles.y * 8) - MPoint(0, 0x10));
            ((MMotion *)mUnkA0)->Play(gDinoResource->GetRandom()->Next(10) + 1, 0);
            break;
        }
        if (mUnkA4) {
            ((MMotion *)mUnkA0)->GetUnit()->SetMode(1);
        }
        GetParallel()->AddTail(mUnkA0, oam);
    } else {
        mUnkA0 = (MView *)oam;
    }
}

// Opens the window out of its position and shows the text.
void DinoCommonWindowMessage::Start() {
    if (mUnkA8 != 0) {
        DinoSoundPlaySe(mUnkA8, 0);
    }
    GetSerial()->AddTail(new MTimer(1), mWindow->CreateShowTransfer(), mWindow->CreateOpenTask(MRect(mPos, mPos)),
                         mUnk74->CreateShowTransfer());
    if (mUnkA0 != NULL) {
        mSerial.AddTail(mUnkA0->CreateShowTransfer());
    }
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoCommonWindowMessage::*Start_00)();
} data_020bc040;
extern "C" struct {
    void (DinoCommonWindowMessage::*Load_00)();
} data_020bc058;

// The first time, loads the window and shows it; after that, blinks it.
void DinoCommonWindowMessage::Idle() {
    if (mUnk94 == -1) {
        mUnk94++;
        MTaskSet *s = &mSerial;
        s->SetUseMutex(TRUE);
        MParallelTaskSet *parallel = GetParallel();
        parallel->SetUseMutex(TRUE);
        parallel->Suspend();
        MTask *start = MCreateCallTask(this, data_020bc040.Start_00);
        MTask *run = MTaskRunTransfer::Create(&mParallel);
        MTask *load = MCreateBgCallTask(this, data_020bc058.Load_00, 0);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(load);
        serial->AddTail(run);
        serial->AddTail(start);
        return;
    }
    if (mUnk98) {
        mUnk94 = (mUnk94 + 1) % (mUnk98 * 2);
        if (mUnk94 == mUnk98 + 1) {
            mWindow->Show();
            mUnk74->Show();
        }
        if (mUnk94 == 1) {
            mWindow->Hide();
            mUnk74->Hide();
        }
    } else if (mUnk94 > 0) {
        mUnk94 = 0;
        mWindow->Show();
        mUnk74->Show();
    }
}

// The file's static initialization (see dino_static_init.h).
static struct DinoCommonWindowConsoleFilterStaticInit {
    DinoCommonWindowConsoleFilterStaticInit() {
        data_020bc024.unk14.x = 0;
        data_020bc024.unk14.y = 4;
        data_020bc024.unk44.x = 216;
        data_020bc024.unk44.y = data_020bc024.unk14.y * 2 + 36;
    }
} sDinoCommonWindowConsoleFilterStaticInit;
