// DinoEventKasekiRockFound.
// Only the destructors are decompiled so far.

#include "game/dino_event_kaseki_rock_found.h"
#include "game/dino_field.h"
#include "m/lcd.h"
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

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template class std::__vec_deleter<std::string, std::allocator<std::string> >;
