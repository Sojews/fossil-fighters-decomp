// DinoEventParkKasekiRockFound.
// Only the destructors are decompiled so far.

#include "game/dino_event_park_kaseki_rock_found.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/polygon_char.h"
#include "game/dino_field.h"
#include "game/dino_event_kaseki_rock_found.h"
#include "game/dino_field_hole.h"
#include "game/dino_event_exec.h"
#include "nitro/mi.h"

DinoEventParkKasekiRockFound::~DinoEventParkKasekiRockFound() {
}

void DinoEventParkKasekiRockFound::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x30];
    void (DinoEventKasekiRockFound::*Unk02037064_30)();
    u8 unk38[0x40 - 0x38];
    void (DinoEventKasekiRockFound::*StateGet_40)();
    u8 unk48[0x58 - 0x48];
    void (DinoEventKasekiRockFound::*StateAnswered_58)();
    void (DinoEventKasekiRockFound::*StateLeave_60)();
    u8 unk68[0x78 - 0x68];
    void (DinoEventKasekiRockFound::*StateFoundEnd_78)();
} data_020bd380;
extern "C" struct {
    u8 unk00[0x28];
    void (DinoEventKasekiRockFound::*Unk02036bd4_28)();
    void (DinoEventParkKasekiRockFound::*Unk02037704_30)();
    void (DinoEventParkKasekiRockFound::*StateStart_38)();
} data_020bd400;

void DinoEventKasekiRockFound::StateFound() {
    mSerial.AddTail(CreateFoundMessage());
    mState = data_020bd380.StateFoundEnd_78;
}

void DinoEventKasekiRockFound::StateFoundEnd() {
    if (mUnk8C != 0) {
        mState = data_020bd400.Unk02036bd4_28;
    } else {
        mState = data_020bd380.StateGet_40;
    }
}

void DinoEventKasekiRockFound::StateGet() {
    if (mHole != NULL) {
        mHole->StartChangeToHole();
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    GetSerial()->AddTail(new DinoEventKasekiRockGet(mEvent, 0, &mEntry, NULL), exit);
}

void DinoEventKasekiRockFound::StateLeave() {
    std::string name = gDinoBackup->GetEntryName(&mEntry);
    GetSerial()->AddTail(CreateMessageBox(0x10, name), new DinoEventExecFade(mEvent, 5, 60, TRUE),
                         MCreateCallTask(mEvent, &DinoEventLoop::CloseField, false),
                         MCreateCallTask(mEvent, &DinoEventLoop::OpenField, false),
                         new DinoEventExecFade(mEvent, 0, 30, TRUE), MTaskExitTransfer::Create(this));
}

void DinoEventKasekiRockFound::StateAsk() {
    mAnswer = 1;
    mSerial.AddTail(CreateYesNo(0xF));
    mState = data_020bd380.StateAnswered_58;
}

void DinoEventKasekiRockFound::StateAnswered() {
    if (mAnswer == 0) {
        mState = data_020bd380.StateLeave_60;
    } else {
        mState = data_020bd380.Unk02037064_30;
    }
}

DinoEventParkKasekiRockFound::DinoEventParkKasekiRockFound(DinoEventLoop *event, u32 index)
    : MProcess(NULL), mEvent(event), mIndex(index) {
    mState = data_020bd400.StateStart_38;
}

void DinoEventParkKasekiRockFound::StateStart() {
    mEvent->GetField()->SetUnk258(TRUE);
    gDinoBackup->GetUnk138(mIndex, &mEntry);
    DinoBackupEntry28 empty;
    MI_CpuFill8(&empty, 0, sizeof(empty));
    gDinoBackup->SetUnk138(mIndex, &empty);
    u32 focus = mEvent->GetField()->GetCamera()->GetFocus();
    mChara = mEvent->GetField()->GetChara(focus);
    mState = data_020bd400.Unk02037704_30;
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoFieldHole, DinoFieldHole>;
template MTask *MCreateCallTask(DinoFieldHole *obj, void (DinoFieldHole::*func)());
template class MCallTask1<DinoEventLoop, DinoEventLoop, bool>;
template MTask *MCreateCallTask(DinoEventLoop *obj, void (DinoEventLoop::*func)(bool), bool a0);
template class MCallTask2<MPolygonChar, MPolygonChar, const char *, const char *>;
template MTask *MCreateCallTask(MPolygonChar *obj, void (MPolygonChar::*func)(const char *, const char *), const char * a0, const char * a1);
