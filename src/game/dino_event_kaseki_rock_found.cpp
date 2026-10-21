// DinoEventKasekiRockFound.
// Only the destructors are decompiled so far.

#include "game/dino_event_kaseki_rock_found.h"
#include "game/dino_field.h"
#include "m/lcd.h"
#include "m/text_codec.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_message_box.h"
#include "nitro/os.h"
#include <string.h>

DinoEventKasekiRockFound::~DinoEventKasekiRockFound() {
    mEvent->GetField()->SetUnk258(FALSE);
}

void DinoEventKasekiRockFound::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    u8 unk00[0x68];
    void (DinoEventKasekiRockFound::*Start_68)();
} data_020bd380;

DinoEventKasekiRockFound::DinoEventKasekiRockFound(DinoEventLoop *event, u32 arg, MPoint pos)
    : MProcess(gLcd->GetBottomDisplay()), mEvent(event), mPos(pos), mArg(arg) {
    mMessages = MResource<DinoMessageData>("/msg/msg_dig");
    mState = data_020bd380.Start_68;
}

MTask *DinoEventKasekiRockFound::CreateMessageBox(u32 index, const std::string &arg) {
    const u8 *data = (const u8 *)mMessages.Get();
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[index];
    const char *format = (const char *)entry + ((const u32 *)entry)[1];
    char *text = new char[strlen(format) + arg.size() + 1];
    OS_SPrintf(text, format, arg.c_str());
    MTask *box = new DinoCommonWindowMessageBox(MPoint(128, 96), std::string(text), 4);
    delete[] text;
    return box;
}

extern const u32 kBackupEntryUnk2Text;
// What the found message calls the rock, by its kind (from 2).
extern "C" const u32 data_0208a348, data_0208a33c, data_0208a344, data_0208a340, data_0208a338;

// Says what was dug up.
MTask *DinoEventKasekiRockFound::CreateFoundMessage() {
    if (mEntry.unk10_0 == 1) {
        return CreateMessageBox(3);
    }
    DinoKasekiRockState rock;
    gDinoBackup->GetKasekiRock(mEntry.id, (u32 *)&rock);
    if (mEntry.type == 1) {
        return CreateMessageBox(0, std::string(gDinoResource->GetText(kBackupEntryUnk2Text)));
    }
    if (rock.unk10) {
        return CreateMessageBox(0, gDinoResource->GetKasekiRockName(mEntry.id));
    }
    if (mEntry.unk10_0 == 0) {
        return CreateMessageBox(2);
    }
    u32 texts[5] = {data_0208a348, data_0208a33c, data_0208a344, data_0208a340, data_0208a338};
    return CreateMessageBox(1, std::string(gDinoResource->GetText(texts[mEntry.unk10_0 - 2])));
}

// The message as a yes/no question: its last two lines are the answers.
// NONMATCHING: the game copies the size before scaling it for the answers' addresses (4 bytes more).
MTask *DinoEventKasekiRockFound::CreateYesNo(u32 message) {
    const u8 *data = (const u8 *)mMessages.Get();
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[message];
    std::string text((const char *)entry + ((const u32 *)entry)[1]);
    std::vector<std::string> lines = MSplitString(text, '\n');
    std::string yes(*(lines.end() - 2));
    std::string no(*(lines.end() - 1));
    lines.erase(lines.end() - 2, lines.end());
    text = MJoinStrings(lines, '\n');
    DinoCommonWindowYesNoDialog *dialog = new DinoCommonWindowYesNoDialog(text, &mAnswer);
    dialog->SetLabels(yes, no);
    return dialog;
}

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template class std::__vec_deleter<std::string, std::allocator<std::string> >;
template std::__vec_deleter<std::string, std::allocator<std::string> &>::~__vec_deleter();
