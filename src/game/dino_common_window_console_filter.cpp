// DinoCommonWindowConsoleFilter.

#include "game/dino_common_window_console_filter.h"
#include "game/dino_help_disabler_task.h"
#include "game/common_window_message.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/input.h"
#include "m/view.h"
#include "m/text_codec.h"
#include "game/dino_common_window_message_box.h"

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
            mUnk78->Show();
            mUnk74->Show();
        }
        if (mUnk94 == 1) {
            mUnk78->Hide();
            mUnk74->Hide();
        }
    } else if (mUnk94 > 0) {
        mUnk94 = 0;
        mUnk78->Show();
        mUnk74->Show();
    }
}
