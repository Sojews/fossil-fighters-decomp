// DinoEventLoop: runs the event scripts.
// Only a few small methods are decompiled so far.

#include "game/dino_static_init.h"
#include "game/dino_field.h"
#include "m/font.h"
#include "game/dino_common_font.h"
#include "m/file.h"
#include "m/lcd.h"
#include "m/bg.h"
#include "m/bg_new_task.h"
#include "m/overlay.h"
#include "game/dino_event_save_option.h"
#include "game/dino_ui_save_option.h"
#include "game/dino_backup.h"
#include "game/dino_event_exec.h"
#include "game/dino_event_field_menu.h"
#include "game/dino_event_medal_list.h"
#include "game/dino_event_debug_menu.h"
#include "game/dino_event_kaseki_rock_found.h"
#include "game/dino_event_park_kaseki_rock_found.h"

// The player's control on the field (in the field overlay).
extern "C" MProcess *func_ov001_021580a0(DinoField *field, u32 request, DinoFieldPlayerControlResult *result,
                                         u32 regions);

void DinoEventLoop::Idle() {
    (this->*mState)();
}

void DinoEventLoop::SetResult(u32 result) {
    if (mResult) {
        *mResult = result;
    }
}

DinoField *DinoEventLoop::GetField() {
    return mField;
}

void DinoEventLoop::SetEpisodesChanged() {
    mEpisodesChanged = TRUE;
}

void DinoEventLoop::ResetEpisode(u32 id) {
    mEpisodeFiles.erase(id);
}

void DinoEventLoop::RemoveEpisode(u32 id) {
    ProcessEpisode(mEpisodeFiles.find(id));
}

void DinoEventLoop::MarkCameraUsed() {
    mCameraUsed = TRUE;
}

void DinoEventLoop::Fade(u32 a, u32 b) {
    mField->Fade(a, b);
}

MTask *DinoEventLoop::CreateFadeWaitTask() {
    return mField->CreateFadeWaitTask();
}

MTask *DinoEventLoop::CreateTalkTask(u32 id) {
    return MCreateBgCallTask(this, &DinoEventLoop::Talk, id, 1);
}

#pragma auto_inline off

M_EXTERN_OVERLAY(ov001);
M_EXTERN_OVERLAY(ov004);

// Closes the field and opens the save option screen, starting it right away.
void DinoEventSaveOption::OpenUi() {
    mEvent->CloseField(FALSE);
    MLoadOverlay(0, M_OVERLAY_ID(ov004));
    mUi = new DinoUiSaveOption((u32)&mResult);
    if (mUi->mState == MTask::STATE_RUN) {
        mUi->Execute();
    }
    if (mUi->mState == MTask::STATE_RUN) {
        mUi->Execute();
    }
}

// Makes the field (without reading its state).
void DinoEventLoop::CreateField() {
    MLoadOverlay(0, M_OVERLAY_ID(ov001));
    mField = new DinoField(&mScreen, 0);
    mParallel.AddTail(mField);
}

// Adds every episode the player has seen (from 1).
void DinoEventLoop::AddSeenEpisodes() {
    for (u32 id = 1; id < gDinoResource->GetEpisodeCount(); id++) {
        if (gDinoBackup->GetEpisodeSeen(id)) {
            AddEpisode(id);
        }
    }
}

// Loads the field overlay and makes the field from the saved state.
void DinoEventLoop::OpenField(bool arg) {
    if (arg) {
        AddSeenEpisodes();
    }
    MLoadOverlay(0, M_OVERLAY_ID(ov001));
    mField = new DinoField(&mScreen, 0);
    mParallel.AddTail(mField);
    mField->Read(gDinoBackup->GetUnk140());
}

// A block of an episode file: the file holds the blocks' offsets (from its start) in a table at the offset in the
// given word.
static inline const u8 *DinoEventBlock(const u8 *data, int table, u32 i) {
    return data + ((const u32 *)(data + ((const u32 *)data)[table]))[i];
}

// Runs the first entry of the episode that isn't cleared yet and whose conditions hold. Entries run once unless
// they say they repeat.
BOOL DinoEventLoop::ProcessEpisode(const std::map<u32, DinoEventEpisode>::iterator &it) {
    for (u32 i = 0; i < ((const u32 *)it->second.Get())[1]; i++) {
        const u8 *data = (const u8 *)it->second.Get();
        const u8 *entry = DinoEventBlock(data, 2, i);
        if (!gDinoBackup->GetEventClear(*(const u32 *)entry) && CheckConditions(entry)) {
            mCommands = gDinoResource->LoadEpisode(it->first);
            const u8 *commands = (const u8 *)mCommands.Get();
            RunCommands(DinoEventBlock(commands, 2, i));
            if (!entry[8]) {
                gDinoBackup->SetEventClear(*(const u32 *)entry, TRUE);
                mEventCleared = TRUE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL DinoEventLoop::CheckConditions(const u8 *entry) {
    int count = ((const u32 *)entry)[3];
    if (count == 0) {
        return TRUE;
    }
    switch (((const u32 *)entry)[1]) {
    case 0:
        for (int i = 0; i < count; i++) {
            if (!CheckCondition(DinoEventBlock(entry, 4, i))) {
                return FALSE;
            }
        }
        return TRUE;
    case 1:
        for (int i = 0; i < count; i++) {
            if (CheckCondition(DinoEventBlock(entry, 4, i))) {
                return TRUE;
            }
        }
        return FALSE;
    }
    return FALSE;
}

void DinoEventLoop::RunCommands(const u8 *block) {
    mUnk108 = 0;
    mUnk10C = 0;
    for (int i = 0; i < ((const u32 *)block)[0]; i++) {
        RunCommand(DinoEventBlock(block, 1, i));
    }
    mField->SetUnk258(TRUE);
    mSerial.AddTail(new DinoEventExecFinish(this));
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    u32 unk00;
    const char *EpisodePath_04;     // "/episode/%04d"
    u8 unk08[0x20 - 0x08];
    void (DinoEventLoop::*StateRun_20)();
    u8 unk28[0x50 - 0x28];
    void (DinoEventLoop::*Start_50)();
} data_020bd9d0;

DinoEventLoop::DinoEventLoop(u32 *result)
    : MProcess(gLcd->mMain), mResult(result), mRegions(gDinoResource->GetRegionCount()), mEventCleared(TRUE),
      mHasPicture(FALSE) {
    mScreen = NULL;
    mState = data_020bd9d0.Start_50;
}

DinoEventLoop::~DinoEventLoop() {
    if (mScreen != NULL) {
        gLcd->mSub->mBg->DeleteDirectBitmap((MDirectBitmap *) mScreen);
    }
    if (mHasPicture) {
        mHasPicture = FALSE;
        mFont->Release();
        mPicture = MPictureFile();
    }
    delete mRequest;
    delete mControlResult;
    delete[] mEpisodes;
}

void DinoEventLoop::Start() {
    MTaskSet *parallel = &mParallel;
    parallel->mUseMutex = TRUE;
    MLoadOverlay(0, M_OVERLAY_ID(ov001));
    mEpisodes = new FSFileID[gDinoResource->GetEpisodeCount()];
    u32 size = gDinoResource->GetEpisodeCount() * sizeof(FSFileID);
    MI_CpuFill8(mEpisodes, 0, size);
    mEpisodesChanged = FALSE;
    mRequest = new DinoEventLoopRequest;
    mRequest->type = 0;
    mControlResult = new DinoFieldPlayerControlResult;
    mControlResult->unk08 = 0;
    mCameraUsed = FALSE;
    mUnkBD = 0;
    mUnkBE = 0;
    AddSeenEpisodes();
    mField = new DinoField(&mScreen, 0);
    mParallel.AddTail(mField);
    if (gDinoBackup->IsLoaded()) {
        mField->Read(gDinoBackup->GetUnk140());
        Fade(2, 30);
        mSerial.AddTail(CreateFadeWaitTask());
    }
    mState = data_020bd9d0.StateRun_20;
}

// NONMATCHING: ours reloads the iterator for the copy it passes ProcessEpisode (the game reuses the one it compared).
void DinoEventLoop::StateRun() {
    u32 type = mRequest->type;
    if (type == 3) {
        mSerial.AddTail(new DinoEventFieldMenu(this, mRequest->arg));
        mRequest->type = 0;
        mControlResult->unk08 = 0;
        return;
    }
    if (type == 4) {
        mSerial.AddTail(new DinoEventMedalList(this));
        mRequest->type = 0;
        mControlResult->unk08 = 0;
        return;
    }
    if (type == 5) {
        mSerial.AddTail(new DinoEventSaveOption(this));
        mRequest->type = 0;
        mControlResult->unk08 = 0;
        return;
    }
    if (type == 7) {
        mSerial.AddTail(new DinoEventDebugMenu(this));
        mRequest->type = 0;
        mControlResult->unk08 = 0;
        return;
    }
    if (type == 6) {
        MPoint pos(mRequest->pos.x, mRequest->pos.y);
        GetSerial()->AddTail(new DinoEventKasekiRockFound(this, mRequest->arg, pos), new DinoEventExecFinish(this));
        mRequest->type = 0;
        mControlResult->unk08 = 0;
        return;
    }
    if (type == 9) {
        GetSerial()->AddTail(new DinoEventParkKasekiRockFound(this, mRequest->arg), new DinoEventExecFinish(this));
        mRequest->type = 0;
        mControlResult->unk08 = 0;
        return;
    }
    for (std::map<u32, DinoEventEpisode>::iterator it = mEpisodeFiles.begin(); it != mEpisodeFiles.end(); ++it) {
        if (ProcessEpisode(std::map<u32, DinoEventEpisode>::iterator(it))) {
            mRequest->type = 0;
            mControlResult->unk08 = 0;
            return;
        }
    }
    if (mField != NULL && mRequest->type == 2 && mRequest->arg != 0) {
        u32 link = gDinoBackup->GetRegionLink(mRequest->arg);
        if (link != 0 && gDinoResource->GetRegionMapMatch(link)) {
            mSerial.AddTail(new DinoEventExecAutoWarp(this, mRequest->pos.x, mRequest->pos.y, link));
            mControlResult->unk08 = 0;
        }
    }
    mRequest->type = 0;
    UpdateRegions();
    mSerial.AddTail(func_ov001_021580a0(mField, (u32)mRequest, mControlResult, (u32)&mRegions));
}

void DinoEventLoop::UpdateRegions() {
    if (mEventCleared) {
        mEventCleared = FALSE;
        MI_CpuFill8(mRegions.mBytes, 0, (mRegions.mBits + 7) / 8);
        gDinoBackup->GetLinkedRegions(&mRegions);
        std::map<u32, DinoEventEpisode>::iterator it = mEpisodeFiles.begin();
        std::map<u32, DinoEventEpisode>::iterator end = mEpisodeFiles.end();
        for (; it != end; ++it) {
            for (u32 i = 0; i < ((const u32 *)it->second.Get())[1]; i++) {
                const u8 *data = (const u8 *)it->second.Get();
                const u8 *entry = DinoEventBlock(data, 2, i);
                if (!gDinoBackup->GetEventClear(*(const u32 *)entry)) {
                    for (u32 j = 0; j < ((const u32 *)entry)[3]; j++) {
                        const u8 *condition = DinoEventBlock(entry, 4, j);
                        if (*(const u16 *)condition == 3) {
                            mRegions.Set(*(const u32 *)(condition + ((const u32 *)condition)[2]));
                            break;
                        }
                    }
                }
            }
        }
    }
}

// An event condition: its type, and its arguments (count words at the offset).
struct DinoEventCondition {
    const u32 *GetArgs() const { return (const u32 *)((const u8 *)this + argOffset); }

    u16 type;           // 0x00
    u16 unk02;
    u32 count;          // 0x04
    u32 argOffset;      // 0x08
};

static inline u32 DinoEventArg(const u8 *data, int i) {
    return ((const u32 *)(data + ((const u32 *)data)[2]))[i];
}

// A variable packed in a word: its type in the top byte, its index below.
static inline DinoEventVar DinoEventPackedVar(u32 packed) {
    DinoEventVar var;
    var.type = packed >> 24;
    var.index = packed & 0xffffff;
    return var;
}

// Whether an event condition holds: the field's request, an episode cleared, variables against each other or
// values, characters met, creatures revived, flags and clears, a full container, a creature in the team.
// NONMATCHING: in the team case the game loads the argument inside the loop (ours hoists it).
BOOL DinoEventLoop::CheckCondition(const u8 *data) {
    const DinoEventCondition *condition = (const DinoEventCondition *)data;
    switch (condition->type) {
    case 1:
        return TRUE;
    case 2:
        if (mField != NULL && mRequest->type == 1 && mRequest->arg == condition->GetArgs()[0]) {
            return TRUE;
        }
        return FALSE;
    case 5:
        if (mField != NULL && mRequest->type == 8 && mRequest->arg == condition->GetArgs()[0]) {
            return TRUE;
        }
        return FALSE;
    case 6:
        if (mField != NULL && mRequest->type == 10 && mRequest->arg == condition->GetArgs()[0]) {
            return TRUE;
        }
        return FALSE;
    case 3:
        if (mField != NULL && mRequest->type == 2 && mRequest->arg == condition->GetArgs()[0]) {
            return TRUE;
        }
        return FALSE;
    case 4:
        return gDinoBackup->GetEpisodeClear(condition->GetArgs()[0]);
    case 7:
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) ==
            gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[1]))) {
            return TRUE;
        }
        return FALSE;
    case 8:
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) !=
            gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[1]))) {
            return TRUE;
        }
        return FALSE;
    case 9:
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) <
            gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[1]))) {
            return TRUE;
        }
        return FALSE;
    case 10:
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) <=
            gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[1]))) {
            return TRUE;
        }
        return FALSE;
    case 11:
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) >
            gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[1]))) {
            return TRUE;
        }
        return FALSE;
    case 12:
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) >=
            gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[1]))) {
            return TRUE;
        }
        return FALSE;
    case 13:
        {
        s32 value = condition->GetArgs()[1];
        s32 var = gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0]));
        if (var == value) {
            return TRUE;
        }
        return FALSE;
    }
    case 14:
        {
        s32 value = condition->GetArgs()[1];
        s32 var = gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0]));
        if (var != value) {
            return TRUE;
        }
        return FALSE;
    }
    case 15:
        {
        s32 value = condition->GetArgs()[1];
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) < value) {
            return TRUE;
        }
        return FALSE;
    }
    case 16:
        {
        s32 value = condition->GetArgs()[1];
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) <= value) {
            return TRUE;
        }
        return FALSE;
    }
    case 17:
        {
        s32 value = condition->GetArgs()[1];
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) > value) {
            return TRUE;
        }
        return FALSE;
    }
    case 18:
        {
        s32 value = condition->GetArgs()[1];
        if (gDinoBackup->GetVar(DinoEventPackedVar(condition->GetArgs()[0])) >= value) {
            return TRUE;
        }
        return FALSE;
    }
    case 23: {
        DinoCharaState chara;
        for (u32 i = 0; i < condition->count; i++) {
            gDinoBackup->GetChara(DinoEventArg(data, i), &chara);
            if (!*(const u8 *)&chara) {
                return FALSE;
            }
        }
        return TRUE;
    }
    case 42: {
        DinoCreatureState creature;
        for (u32 i = 0; i < condition->count; i++) {
            gDinoBackup->GetCreature(DinoEventArg(data, i), &creature);
            if (creature.parts[0].level == 0) {
                return FALSE;
            }
        }
        return TRUE;
    }
    case 36:
        for (u32 i = 0; i < condition->count; i++) {
            if (!gDinoBackup->GetVarClear(DinoEventPackedVar(DinoEventArg(data, i)))) {
                return FALSE;
            }
        }
        return TRUE;
    case 37:
        for (u32 i = 0; i < condition->count; i++) {
            if (gDinoBackup->GetVarClear(DinoEventPackedVar(DinoEventArg(data, i)))) {
                return TRUE;
            }
        }
        return FALSE;
    case 38:
        for (u32 i = 0; i < condition->count; i++) {
            if (gDinoBackup->GetVarClear(DinoEventPackedVar(DinoEventArg(data, i)))) {
                return FALSE;
            }
        }
        return TRUE;
    case 39:
        for (u32 i = 0; i < condition->count; i++) {
            if (!gDinoBackup->GetVarClear(DinoEventPackedVar(DinoEventArg(data, i)))) {
                return TRUE;
            }
        }
        return FALSE;
    case 19:
        for (u32 i = 0; i < condition->count; i++) {
            if (!gDinoBackup->GetVarFlag(DinoEventPackedVar(DinoEventArg(data, i)))) {
                return FALSE;
            }
        }
        return TRUE;
    case 20:
        for (u32 i = 0; i < condition->count; i++) {
            if (gDinoBackup->GetVarFlag(DinoEventPackedVar(DinoEventArg(data, i)))) {
                return TRUE;
            }
        }
        return FALSE;
    case 21:
        for (u32 i = 0; i < condition->count; i++) {
            if (gDinoBackup->GetVarFlag(DinoEventPackedVar(DinoEventArg(data, i)))) {
                return FALSE;
            }
        }
        return TRUE;
    case 22:
        for (u32 i = 0; i < condition->count; i++) {
            if (!gDinoBackup->GetVarFlag(DinoEventPackedVar(DinoEventArg(data, i)))) {
                return TRUE;
            }
        }
        break;
    case 40:
        return gDinoBackup->IsUnk98Full();
    case 41: {
        DinoTeam team;
        gDinoBackup->GetTeam(gDinoBackup->GetUnk34_26(), &team);
        for (u32 i = 0; i < 5; i++) {
            if (team.slots[i] == condition->GetArgs()[0]) {
                return TRUE;
            }
        }
        return FALSE;
    }
    }
    return FALSE;
}

void DinoEventLoop::ReleasePicture() {
    if (mHasPicture) {
        mHasPicture = FALSE;
        mFont->Release();
        mPicture = MPictureFile();
    }
}

void DinoEventLoop::FinishCommands() {
    ReleasePicture();
    mMessages = MResource<DinoMessageData>();
    if (!mUnkBE) {
        mField->SetUnk258(FALSE);
    }
    mUnkBE = FALSE;
    if (mCameraUsed && !mUnkBD) {
        mCameraUsed = FALSE;
        mField->GetCamera()->SetTracking(TRUE);
    }
    mUnkBD = FALSE;
}

// Loads the talk message's file (in the background, before a talk) and, the first time, the talk window's font and
// picture.
void DinoEventLoop::Talk(u32 id) {
    if (id != 0) {
        mMessages = gDinoResource->LoadMessage(gDinoResource->GetTalkMessageMatch(id));
    }
    if (!mHasPicture) {
        mHasPicture = TRUE;
        mFont = new DinoCommonFont(0, 0, 0);
        mFont->AddRef();
        mPicture = MPictureFile(std::string("/image/talk_base"), 7);
    }
}

// The text of the talk message, or "".
std::string DinoEventLoop::GetTalkText(u32 id) {
    mMessages = gDinoResource->LoadMessage(gDinoResource->GetTalkMessageMatch(id));
    for (u32 i = 0; i < ((const u32 *)mMessages.Get())[1]; i++) {
        if (id == *(const u32 *)DinoEventBlock((const u8 *)mMessages.Get(), 2, i)) {
            const u8 *entry = DinoEventBlock((const u8 *)mMessages.Get(), 2, i);
            return std::string((const char *)entry + ((const u32 *)entry)[1]);
        }
    }
    return std::string("");
}

// Opens the episode's file (looking it up the first time) and keeps it.
void DinoEventLoop::AddEpisode(u32 id) {
    if (mEpisodes[id].arc == NULL) {
        char path[0x100];
        OS_SPrintf(path, data_020bd9d0.EpisodePath_04, id);
        MGetFileID(std::string(path), &mEpisodes[id]);
    }
    mEpisodeFiles[id] = DinoEventEpisode(mEpisodes[id]);
}

void DinoEventLoop::ClearEpisodes() {
    mEpisodeFiles.clear();
}

// Saves the field's state and deletes it. Unless closing for good, the episodes are reloaded if they changed.
void DinoEventLoop::CloseField(bool arg) {
    mField->Write(gDinoBackup->GetUnk140());
    mParallel.Remove(mField);
    delete mField;
    MUnloadOverlay(0, M_OVERLAY_ID(ov001));
    ReleasePicture();
    mMessages = MResource<DinoMessageData>();
    mCommands = MResource<DinoEpisodeData>();
    if (arg) {
        ClearEpisodes();
        mEpisodesChanged = FALSE;
    } else if (mEpisodesChanged) {
        mEpisodesChanged = FALSE;
        ClearEpisodes();
        AddSeenEpisodes();
    }
}

// The file's static initialization (see dino_static_init.h): the common layout header.
// NONMATCHING: the game schedules the stores differently.
extern "C" u8 data_020bdb50[];
#pragma always_inline on

static struct DinoEventStaticInit {
    DinoEventStaticInit() {
        DINO_POOL_NV(MPoint, data_020bd9d0, 0x10) = MPoint(256, 192);
        DINO_POOL_NV(MRect, data_020bdb50, 0x64) = DinoSizeRect(DINO_POOL_NV(MPoint, data_020bd9d0, 0x10));
        DINO_POOL_NV(MPoint, data_020bd9d0, 0x28) = MPoint(2, 2);
        DINO_POOL_NV(MPoint, data_020bd9d0, 0x48) = MPoint(36, 28);
        DINO_POOL_NV(MRect, data_020bdb50, 0x34) = DinoSizeRect(DINO_POOL_NV(MPoint, data_020bd9d0, 0x48));
        DINO_POOL_NV(MPoint, data_020bdb50, 0x44) = MPoint(0, 24);
        DINO_POOL_NV(MPoint, data_020bdb50, 0x4c) = MPoint(256, 192);
        DINO_POOL_NV(MRect, data_020bdb50, 0x54) = MRect(0, 0, 256, 24);
    }
} sDinoEventStaticInit;
