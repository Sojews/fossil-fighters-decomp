// DinoEventKasekiRockFound.

#include "game/dino_event_kaseki_rock_found.h"
#include "game/dino_field.h"
#include "m/lcd.h"
#include "m/text_codec.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_message_box.h"
#include "nitro/os.h"
#include <string.h>
#include "game/dino_field_hole.h"
#include "game/dino_event_exec.h"
#include "game/dino_sound.h"
#include "m/polygon_char.h"
#include "m/anm_model.h"
#include "m/call_task.h"

DinoEventKasekiRockFound::~DinoEventKasekiRockFound() {
    mEvent->GetField()->SetUnk258(FALSE);
}

void DinoEventKasekiRockFound::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    u8 unk00[0x10];
    void (DinoEventKasekiRockFound::*StateFound_10)();
    u8 unk18[0x38 - 0x18];
    void (DinoEventKasekiRockFound::*State020374d4_38)();
    u8 unk40[0x48 - 0x40];
    void (MPolygonChar::*Load_48)(const char *, const char *);
    u8 unk50[0x68 - 0x50];
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
MTask *DinoEventKasekiRockFound::CreateYesNo(u32 message) {
    const u8 *data = (const u8 *)mMessages.Get();
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[message];
    std::string text((const char *)entry + ((const u32 *)entry)[1]);
    std::vector<std::string> lines = MSplitString(text, '\n');
    std::string yes(*(lines.begin() + (u32)lines.size() - 2));
    std::string no(*(lines.begin() + (u32)lines.size() - 1));
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

// The finder's digging motion for each kind of rock.
extern const u32 data_0208b2f4[8];
extern const u32 data_0208a1fc;
extern const u32 data_0208a200;

// NONMATCHING: the game keeps the effect's z and height in r6 and r4 the other way round.
void DinoEventKasekiRockFound::StateDig() {
    u32 focus = mEvent->GetField()->GetCamera()->GetFocus();
    mChara = mEvent->GetField()->GetChara(focus);
    u32 motion = data_0208b2f4[mEntry.unk10_0];
    if (mUnkDC) {
        motion = 0x1C;
    }
    mHole = mEvent->GetField()->CreateHole(*(MFx32Point *)&mPos);
    mAngle = mChara->GetModel()->angle;
    mEffect = new MPolygonChar(mEvent->GetField()->mScene, 0x7FFF, NULL, NULL);
    MPolygonChar *e;
    (e = mEffect)->SetPosition(mHole->GetPos().x.v, mHole->mUnk90, mHole->GetPos().y.v);
    for (MAnmModelBase *anm = (MAnmModelBase *)e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *)anm->mNext) {
        anm->mPosRef = &e->mPos;
        anm->mUnk11B = TRUE;
    }
    int angle = (u16)(0x4000 - mChara->GetModel()->angle) * 360 / 16;
    e = mEffect;
    e->mRot.x = 0;
    e->mRot.y = angle;
    e->mRot.z = 0;
    for (MAnmModelBase *anm = (MAnmModelBase *)e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *)anm->mNext) {
        anm->mRotRef = &e->mRot;
        anm->mUnk11B = TRUE;
    }
    for (MAnmModelBase *anm = (MAnmModelBase *)mEffect->mParallel.mHead; anm != NULL;
         anm = (MAnmModelBase *)anm->mNext) {
        anm->mActive = TRUE;
    }
    e = mEffect;
    e->mUnkAC = 1;
    for (MAnmModelBase *anm = (MAnmModelBase *)e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *)anm->mNext) {
        anm->mFollowParent = TRUE;
    }
    mParallel.AddTail(mEffect);
    mChara->ClearUnk2E0();
    mChara->SetMotion(4, 0, MFx32(1));
    GetSerial()->AddTail(new MTimer(0x1E),
                    MCreateCallTask(mEffect, data_020bd380.Load_48, "/model/fieldchar/effect03.csv", (const char *)NULL),
                    DinoSoundCreatePlaySeTask(0xCA, 0),
                    new DinoEventExecCamera(mEvent, 0x400, 0x1B53, 0, 0x2B000, 0x3C, 0xC),
                    DinoSoundCreatePlaySeTask(!mUnkDC ? data_0208a200 : data_0208a1fc, 0),
                    mChara->CreateSetMotionTask(motion, 0, MFx32(1)), new MTimer(10),
                    mChara->CreateShowManpuTask(2, MPoint(0, 0)), new MTimer(0x1E),
                    mChara->CreateAngleToTask(0x4000, 0x14, 0xC), mChara->CreateHideManpuTask(), new MTimer(0x14),
                    MTaskExitTransfer::Create(mEffect));
    if (!mUnkDC) {
        mState = data_020bd380.StateFound_10;
    } else {
        mState = data_020bd380.State020374d4_38;
    }
}
