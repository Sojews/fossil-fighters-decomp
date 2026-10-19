// DinoEventParkKasekiRockFound.

#include "game/dino_event_park_kaseki_rock_found.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/polygon_char.h"
#include "game/dino_field.h"
#include "game/dino_event_kaseki_rock_found.h"
#include "game/dino_field_hole.h"
#include "game/dino_event_exec.h"
#include "m/fx32.h"
#include "game/dino_sound.h"
#include "m/display.h"
#include "game/dino_resource.h"
#include "game/dino_event_talk_window.h"
#include "nitro/mi.h"

DinoEventParkKasekiRockFound::~DinoEventParkKasekiRockFound() {
}

void DinoEventParkKasekiRockFound::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x18];
    void (DinoEventKasekiRockFound::*StateBattleEnd_18)();
    u8 unk20[0x30 - 0x20];
    void (DinoEventKasekiRockFound::*StateBattle_30)();
    u8 unk38[0x40 - 0x38];
    void (DinoEventKasekiRockFound::*StateGet_40)();
    u8 unk48[0x50 - 0x48];
    void (DinoEventKasekiRockFound::*StateFound_50)();
    void (DinoEventKasekiRockFound::*StateAnswered_58)();
    void (DinoEventKasekiRockFound::*StateLeave_60)();
    u8 unk68[0x70 - 0x68];
    void (DinoEventParkKasekiRockFound::*StateEnd_70)();
    void (DinoEventKasekiRockFound::*StateFoundEnd_78)();
} data_020bd380;
extern "C" struct {
    u8 unk00[0x28];
    void (DinoEventKasekiRockFound::*Unk02036bd4_28)();
    void (DinoEventParkKasekiRockFound::*StateFound_30)();
    void (DinoEventParkKasekiRockFound::*StateStart_38)();
} data_020bd400;

// The rock-found event's strings (in dino_event_kaseki_rock_found.cpp's data).
extern "C" const char data_020bd5f4[];  // "/msg/msg_dig"
extern "C" const char data_020bd604[];  // ""

void DinoEventKasekiRockFound::StateFound() {
    mSerial.AddTail(CreateFoundMessage());
    mState = data_020bd380.StateFoundEnd_78;
}

void DinoEventKasekiRockFound::StateFoundEnd() {
    if (mBattle != 0) {
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

void DinoEventKasekiRockFound::StateBattle() {
    mHole = NULL;
    mMessages = MResource<DinoMessageData>();
    mBattleData = MResource<DinoBattleData>();
    mSerial.AddTail(new DinoEventExecBattle(mEvent, 0, mBattle, 0, -1));
    mState = data_020bd380.StateBattleEnd_18;
}


void DinoEventKasekiRockFound::StateBattleEnd() {
    mMessages = MResource<DinoMessageData>(data_020bd5f4);
    mBattleData = gDinoResource->LoadBattle(mBattle);
    if (gDinoBackup->GetFlag(0)) {
        mBattle = 0;
        if (((const u32 *)mBattleData.Get())[8] != 0) {
            mSerial.AddTail(new DinoEventTalkWindow(gCurrentDisplay,
                                                    mEvent->GetTalkText(((const u32 *)mBattleData.Get())[8]), 0, 0));
        }
        mSerial.AddTail(CreateMessageBox(5, data_020bd604));
        mState = data_020bd380.StateFound_50;
        return;
    }
    if (((const u32 *)mBattleData.Get())[7] != 0) {
        mSerial.AddTail(new DinoEventTalkWindow(gCurrentDisplay,
                                                mEvent->GetTalkText(((const u32 *)mBattleData.Get())[7]), 0, 0));
    }
    mSerial.AddTail(CreateMessageBox(6, gDinoBackup->GetEntryName(&mEntry)));
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

// NONMATCHING: the three speed temporaries get their stack slots in another order.
void DinoEventKasekiRockFound::State020374d4() {
    std::string text(data_020bd604);
    GetSerial()->AddTail(mChara->CreateSetMotionTask(0x26, 1, MFx32(1)), CreateMessageBox(0x11, text),
                         mChara->CreateSetMotionTask(0x28, 1, MFx32(1)), mChara->CreateAngleToTask(mAngle, 20, 12),
                         new MTimer(30), DinoSoundCreatePlaySeTask(0x3B8, 0), mChara->CreateWaitStopTask(),
                         mChara->CreateSetMotionTask(1, 1, MFx32(1)),
                         MCreateCallTask(mHole, &DinoFieldHole::StartChangeToHole), MTaskExitTransfer::Create(this));
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
        mState = data_020bd380.StateBattle_30;
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
    mState = data_020bd400.StateFound_30;
}

extern "C" const u32 data_0208b2d8[]; // per rock color: the chara's motion

// NONMATCHING: the task locals' stack slots (the game spills them in a different order).
void DinoEventParkKasekiRockFound::StateFound() {
    MTask *camera;
    MTask *motion;
    MTask *removeRock;
    MTask *wait10;
    MTask *se2;
    MTask *showManpu;
    MTask *wait30;
    MTask *turn;
    MTask *hideManpu;
    MTask *wait5;
    MTask *wait15;
    MTask *get = new DinoEventKasekiRockGet(mEvent, 3, (const DinoBackupEntry *) &mEntry.unk14, &mTaken);
    MTask *se = DinoSoundCreatePlaySeTask(0x69, 0);
    MParallelTaskSet *getting = new MParallelTaskSet(TRUE);
    getting->AddTail(se);
    getting->AddTail(get);
    wait15 = new MTimer(15);
    wait5 = new MTimer(5);
    hideManpu = mChara->CreateHideManpuTask();
    turn = mChara->CreateAngleToTask(0x4000, 0x14, 0xC);
    wait30 = new MTimer(30);
    showManpu = mChara->CreateShowManpuTask(2, MPoint(0, 0));
    se2 = DinoSoundCreatePlaySeTask(0xD0, 0);
    wait10 = new MTimer(10);
    removeRock = MTaskExitTransfer::Create((MTask *) mEvent->GetField()->GetKasekiRock(mIndex));
    motion = mChara->CreateSetMotionTask(data_0208b2d8[mEntry.color], 0, MFx32(1));
    camera = new DinoEventExecCamera(mEvent, 0x400, 0x1B53, 0, 0x2B000, 0x3C, 0xC);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(camera);
    serial->AddTail(motion);
    serial->AddTail(removeRock);
    serial->AddTail(wait10);
    serial->AddTail(se2);
    serial->AddTail(showManpu);
    serial->AddTail(wait30);
    serial->AddTail(turn);
    serial->AddTail(hideManpu);
    serial->AddTail(wait5);
    serial->AddTail(wait15);
    serial->AddTail(getting);
    mState = data_020bd380.StateEnd_70;
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoFieldHole, DinoFieldHole>;
template MTask *MCreateCallTask(DinoFieldHole *obj, void (DinoFieldHole::*func)());
template class MCallTask1<DinoEventLoop, DinoEventLoop, bool>;
template MTask *MCreateCallTask(DinoEventLoop *obj, void (DinoEventLoop::*func)(bool), bool a0);
template class MCallTask2<MPolygonChar, MPolygonChar, const char *, const char *>;
template MTask *MCreateCallTask(MPolygonChar *obj, void (MPolygonChar::*func)(const char *, const char *), const char * a0, const char * a1);

void DinoEventParkKasekiRockFound::StateEnd() {
    if (!mTaken) {
        gDinoBackup->SetUnk138(mIndex, &mEntry);
        mEvent->GetField()->CreateKasekiRock(mIndex);
    }
    mEvent->GetField()->SetUnk258(FALSE);
    Exit();
}
