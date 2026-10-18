// DinoCommonWindowConsoleFilter.
// Only the destructors are decompiled so far.

#include "game/dino_common_window_console_filter.h"
#include "game/dino_help_disabler_task.h"
#include "game/common_window_message.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
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

DinoCommonWindowMessageWait2::DinoCommonWindowMessageWait2(std::string text, MPoint pos, int arg, u8 flag)
    : MProcess(NULL), mText(text) {
    mUnk80 = 0;
    mPos = pos;
    mUnk8C = arg;
    mUnk90 = flag;
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
