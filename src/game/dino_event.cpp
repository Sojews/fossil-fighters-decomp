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
#include "game/set_episode_task.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_sound.h"
#include "game/dino_event_atm.h"
#include "game/dino_event_shop_buy.h"
#include "game/dino_event_shop_sell.h"
#include "game/dino_event_shop_mask.h"
#include "game/dino_event_mask_equip.h"
#include "game/dino_event_canon_putin.h"
#include "game/dino_event_canon_takeout.h"
#include "game/dino_event_name_entry.h"
#include "game/dino_event_senryu_entry.h"
#include "game/dino_event_senryu_list_entry.h"
#include "game/dino_event_license_card.h"
#include "game/dino_event_ending.h"
#include "game/dino_event_save.h"
#include "game/dino_event_museum.h"
#include "game/dino_event_revive_effect_caller.h"
#include "game/dino_event_revive.h"

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
    mPictPos.x = 0;
    mPictPos.y = 0;
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

extern "C" void _ZNSt13__vec_deleterImSaImEE19reallocate_copy_altEm(void *vector, size_t n);
extern "C" void _ZNSt13__vec_deleterImSaImEE9init_copyEPKmS3_N10Metrowerks8int2typeILi1EEE(void *vector, const u32 *first,
                                                                                        const u32 *last, u8 tag);

// (vector::reserve and the copy constructor, through the u32 vector's out-of-line members the game calls here; see
// dino_ui_title.cpp.)
static inline void DinoEventReservePoints(std::vector<void *> &points, size_t n) {
    if (n > points.capacity()) {
        _ZNSt13__vec_deleterImSaImEE19reallocate_copy_altEm(&points, n);
    }
}

// Degrees as an angle (0x10000 a turn).
static inline s32 DinoEventDegree(u32 degrees) {
    return (s32)(degrees << 16) / 360;
}

// Runs one command: almost all of them queue the matching task on the serial set, from the command's arguments.
// NONMATCHING
void DinoEventLoop::RunCommand(const u8 *command) {
    const DinoEventCondition *cmd = (const DinoEventCondition *)command;
    MFx32Point pos7;
    MFx32Point posB;
    switch (cmd->type) {
    case 0x7: {
        pos7.x.v = DinoEventArg(command, 2);
        pos7.y.v = DinoEventArg(command, 3);
        mSerial.AddTail(new DinoEventExecAppendCharaRegion(this, DinoEventArg(command, 0), DinoEventArg(command, 1), pos7, DinoEventDegree(DinoEventArg(command, 4)), 0));
        break;
    }
    case 0xb: {
        posB.x.v = DinoEventArg(command, 2);
        posB.y.v = DinoEventArg(command, 3);
        mSerial.AddTail(new DinoEventExecAppendCharaRegion(this, DinoEventArg(command, 0), DinoEventArg(command, 1), posB, 0x4000, DinoEventArg(command, 4)));
        break;
    }
    case 0x8:
        mSerial.AddTail(new DinoEventExecAppendCharaRegion(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 0x4000, 0));
        break;
    case 0xc:
        mSerial.AddTail(new DinoEventExecAppendCharaRegion(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 0x4000, DinoEventArg(command, 2)));
        break;
    case 0x9:
        mSerial.AddTail(new DinoEventExecAppendCharaRegion(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventDegree(DinoEventArg(command, 2)), 0));
        break;
    case 0xa:
        mSerial.AddTail(new DinoEventExecAppendCharaChara(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0xe:
        mSerial.AddTail(new DinoEventExecRemoveChara(this, DinoEventArg(command, 0)));
        break;
    case 0x10:
        mSerial.AddTail(new DinoEventExecMapConnect((u32)this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x11:
        mSerial.AddTail(new DinoEventExecMapConnectDirectional((u32)this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x12:
        mSerial.AddTail(new DinoEventExecMapDisconnect((u32)this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x13:
        mSerial.AddTail(new DinoEventExecMapDisconnectDirectional((u32)this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x1:
        mSerial.AddTail(new DinoEventExecTalk(this, DinoEventArg(command, 0), 0));
        break;
    case 0x2:
        mSerial.AddTail(new DinoEventExecTalk(this, DinoEventArg(command, 0), 1));
        break;
    case 0x22:
        mSerial.AddTail(new DinoEventExecCharaAngle(this, DinoEventArg(command, 0), DinoEventDegree(DinoEventArg(command, 1)), 1, 0));
        break;
    case 0x23:
        mSerial.AddTail(new DinoEventExecCharaAngle(this, DinoEventArg(command, 0), DinoEventDegree(DinoEventArg(command, 1)), DinoEventArg(command, 2), DinoEventArg(command, 3)));
        break;
    case 0x24:
        mSerial.AddTail(new DinoEventExecCharaAngleToChara(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3)));
        break;
    case 0x25:
        mSerial.AddTail(new DinoEventExecCharaAngleOffset(this, DinoEventArg(command, 0), DinoEventDegree(DinoEventArg(command, 1)), DinoEventArg(command, 2), DinoEventArg(command, 3)));
        break;
    case 0x26:
        mSerial.AddTail(new DinoEventExecCharaAngleOffsetToChara(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventDegree(DinoEventArg(command, 2)), DinoEventArg(command, 3), DinoEventArg(command, 4)));
        break;
    case 0x27:
        mSerial.AddTail(new DinoEventExecCharaMoveToChara(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), 0));
        break;
    case 0x28:
        mSerial.AddTail(new DinoEventExecCharaMoveToChara(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), 1));
        break;
    case 0x29:
        mSerial.AddTail(new DinoEventExecCharaMoveToRegion(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), 0));
        break;
    case 0x2a:
        mSerial.AddTail(new DinoEventExecCharaMoveToRegion(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), 1));
        break;
    case 0x2b:
        mSerial.AddTail(new DinoEventExecCharaMoveToCoord(this, DinoEventArg(command, 0), MFx32Point(MFx32::FromRaw(DinoEventArg(command, 1)), MFx32::FromRaw(DinoEventArg(command, 2))), DinoEventArg(command, 3), DinoEventArg(command, 4), 0));
        break;
    case 0x2c:
        mSerial.AddTail(new DinoEventExecCharaMoveToCoord(this, DinoEventArg(command, 0), MFx32Point(MFx32::FromRaw(DinoEventArg(command, 1)), MFx32::FromRaw(DinoEventArg(command, 2))), DinoEventArg(command, 3), DinoEventArg(command, 4), 1));
        break;
    case 0x2d:
        mSerial.AddTail(new DinoEventExecCharaMoveOffset(this, DinoEventArg(command, 0), MFx32Point(MFx32::FromRaw(DinoEventArg(command, 1)), MFx32::FromRaw(DinoEventArg(command, 2))), DinoEventArg(command, 3), DinoEventArg(command, 4), 0));
        break;
    case 0x2e:
        mSerial.AddTail(new DinoEventExecCharaMoveOffset(this, DinoEventArg(command, 0), MFx32Point(MFx32::FromRaw(DinoEventArg(command, 1)), MFx32::FromRaw(DinoEventArg(command, 2))), DinoEventArg(command, 3), DinoEventArg(command, 4), 1));
        break;
    case 0x2f:
        mSerial.AddTail(new DinoEventExecCharaMoveVector(this, DinoEventArg(command, 0), DinoEventDegree(DinoEventArg(command, 1)), MFx32::FromRaw(DinoEventArg(command, 2)), DinoEventArg(command, 3), DinoEventArg(command, 4), 0));
        break;
    case 0x30:
        mSerial.AddTail(new DinoEventExecCharaMoveVector(this, DinoEventArg(command, 0), DinoEventDegree(DinoEventArg(command, 1)), MFx32::FromRaw(DinoEventArg(command, 2)), DinoEventArg(command, 3), DinoEventArg(command, 4), 1));
        break;
    case 0x32:
        mSerial.AddTail(new DinoEventExecCharaWaitMove(this, DinoEventArg(command, 0)));
        break;
    case 0x38:
        mSerial.AddTail(new MTimer(DinoEventArg(command, 0)));
        break;
    case 0x33:
        mSerial.AddTail(new DinoEventExecCharaFocus(this, DinoEventArg(command, 0)));
        break;
    case 0x14:
        mSerial.AddTail(new DinoEventExecFade(this, 1, DinoEventArg(command, 0), 0));
        break;
    case 0x15:
        mSerial.AddTail(new DinoEventExecFade(this, 0, DinoEventArg(command, 0), 0));
        break;
    case 0x16:
        mSerial.AddTail(new DinoEventExecFade(this, 3, DinoEventArg(command, 0), 0));
        break;
    case 0x17:
        mSerial.AddTail(new DinoEventExecFade(this, 2, DinoEventArg(command, 0), 0));
        break;
    case 0x18:
        mSerial.AddTail(new DinoEventExecFade(this, 5, DinoEventArg(command, 0), 0));
        break;
    case 0x19:
        mSerial.AddTail(new DinoEventExecFade(this, 4, DinoEventArg(command, 0), 0));
        break;
    case 0x5:
        for (u32 i = 0; i < cmd->count; i++) {
            MTaskSet *serial = &mSerial;
            serial->AddTail(new DinoEventExecFlagSet((u32)this, DinoEventPackedVar(DinoEventArg(command, i)), 1));
        }
        break;
    case 0x6:
        for (u32 i = 0; i < cmd->count; i++) {
            MTaskSet *serial = &mSerial;
            serial->AddTail(new DinoEventExecFlagSet((u32)this, DinoEventPackedVar(DinoEventArg(command, i)), 0));
        }
        break;
    case 0x46: case 0x47: case 0x48: case 0x49: case 0x4a:
        mSerial.AddTail(new DinoEventExecVarValue(cmd->type, DinoEventPackedVar(DinoEventArg(command, 0)), DinoEventArg(command, 1)));
        break;
    case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
        mSerial.AddTail(new DinoEventExecVarVar(cmd->type, DinoEventPackedVar(DinoEventArg(command, 0)), DinoEventPackedVar(DinoEventArg(command, 1))));
        break;
    case 0x3:
        mSerial.AddTail(new DinoEventExecEventClear((u32)this, DinoEventArg(command, 0), 1));
        break;
    case 0x4:
        mSerial.AddTail(new DinoEventExecEventClear((u32)this, DinoEventArg(command, 0), 0));
        mEventCleared = TRUE;
        break;
    case 0x1a:
        {
            SetEpisodeTask *seen = new SetEpisodeTask(this, DinoEventArg(command, 0), 0);
            mSerial.AddTail(new DinoEventExecSetEpisodeClear((u32)this, DinoEventArg(command, 0), 1), seen);
        }
        break;
    case 0x1b:
        mSerial.AddTail(new SetEpisodeTask(this, DinoEventArg(command, 0), 1));
        mEventCleared = TRUE;
        break;
    case 0x20:
        {
            MTask *box = new DinoCommonWindowMessageBox(gLcd->GetBottomDisplay(), MPoint(0x80, 0x60), GetTalkText(DinoEventArg(command, 0)), 4);
            mSerial.AddTail(box);
        }
        break;
    case 0x21:
        mSerial.AddTail(new DinoEventExecYesNo(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x3a:
        mSerial.AddTail(new DinoEventExecCleaning(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 0, 0));
        break;
    case 0x3b:
        mSerial.AddTail(new DinoEventExecCleaning(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 1, 0));
        break;
    case 0x3c:
        mSerial.AddTail(new DinoEventExecCleaning(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 0, 1));
        break;
    case 0x39:
        {
            u32 arg2 = 0;
            if (cmd->count >= 3) {
                arg2 = DinoEventArg(command, 2);
            }
            mSerial.AddTail(new DinoEventExecBattle(this, DinoEventArg(command, 0), DinoEventArg(command, 1), arg2, -1));
        }
        break;
    case 0x3d:
        mSerial.AddTail(new DinoEventExecCamera(this, (s32)(DinoEventArg(command, 0) << 4) / 360, (s32)(DinoEventArg(command, 1) << 4) / 360, (s32)(DinoEventArg(command, 2) << 4) / 360, DinoEventArg(command, 3), DinoEventArg(command, 4), DinoEventArg(command, 5)));
        break;
    case 0x3e:
        mSerial.AddTail(new DinoEventExecProhibitCameraRelease(&mUnkBD));
        break;
    case 0x3f:
        mSerial.AddTail(new DinoEventExecProhibitCameraRelease(&mUnkBE));
        break;
    case 0x50:
        mSerial.AddTail(new DinoEventExecStartFellow(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x52:
        mSerial.AddTail(new DinoEventExecStartWander(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), MFx32::FromRaw(DinoEventArg(command, 3)), MFx32::FromRaw(DinoEventArg(command, 4))));
        break;
    case 0x54:
        {
            u32 count = cmd->count - 4;
            if (count == 0) {
                OS_Terminate();
            }
            if (count > 8) {
                OS_Terminate();
            }
            std::vector<void *> points;
            DinoEventReservePoints(points, count);
            for (u32 i = 0; i < count; i++) {
                points.push_back((void *)(cmd->GetArgs() + 4)[i]);
            }
            mSerial.AddTail(new DinoEventExecStartPatrol(this, DinoEventArg(command, 0), DinoEventArg(command, 1),
                                                         DinoEventArg(command, 2), DinoEventArg(command, 3),
                                                         std::vector<u32>(*(const std::vector<u32> *)&points)));
        }
        break;
    case 0x51: case 0x53: case 0x55:
        mSerial.AddTail(new DinoEventExecFinishFellow(this, DinoEventArg(command, 0)));
        break;
    case 0x56:
        mSerial.AddTail(new DinoEventExecStartWatch(this, DinoEventArg(command, 0), MFx32::FromRaw(DinoEventArg(command, 1)), MFx32::FromRaw(DinoEventArg(command, 2)), MFx32::FromRaw(DinoEventArg(command, 3)), MFx32::FromRaw(DinoEventArg(command, 4)), cmd->count == 5 ? 60 : DinoEventArg(command, 5)));
        break;
    case 0x57:
        mSerial.AddTail(new DinoEventExecFinishWatch(this, DinoEventArg(command, 0)));
        break;
    case 0xc8:
        mSerial.AddTail(new DinoEventExecStartBehold(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0xc9:
        mSerial.AddTail(new DinoEventExecFinishBehold(this, DinoEventArg(command, 0)));
        break;
    case 0x34:
        mSerial.AddTail(new DinoEventExecChoice(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 0, 0));
        break;
    case 0x35:
        mSerial.AddTail(new DinoEventExecChoice2(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x90:
        mSerial.AddTail(new DinoEventExecChoice(this, DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 0), 0));
        break;
    case 0x91:
        mSerial.AddTail(new DinoEventExecChoice(this, DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 0), DinoEventArg(command, 3)));
        break;
    case 0x36:
        mSerial.AddTail(new DinoEventExecShopChoice(this, DinoEventArg(command, 0), DinoEventPackedVar(DinoEventArg(command, 1))));
        break;
    case 0x37:
        mSerial.AddTail(new DinoEventExecShopChoice2(this, DinoEventArg(command, 0), DinoEventPackedVar(DinoEventArg(command, 1)), DinoEventArg(command, 2)));
        break;
    case 0x58:
        mSerial.AddTail(new DinoEventExecLicenseGet(1));
        break;
    case 0x59:
        mSerial.AddTail(new DinoEventExecLicenseGet(2));
        break;
    case 0x5c:
        mSerial.AddTail(new DinoEventATM(this));
        break;
    case 0x5d:
        mSerial.AddTail(new DinoEventRevive(this));
        break;
    case 0x5a:
        mSerial.AddTail(new DinoEventExecHoriderLevel(DinoEventArg(command, 0)));
        break;
    case 0x5b:
        mSerial.AddTail(new DinoEventExecContainerNum(DinoEventArg(command, 0)));
        break;
    case 0x5e:
        mSerial.AddTail(new DinoEventExecSystemFlag(1, 1));
        break;
    case 0x5f:
        mSerial.AddTail(new DinoEventExecSystemFlag(1, 0));
        break;
    case 0x63:
        mSerial.AddTail(new DinoEventExecSystemFlag(2, 1));
        break;
    case 0x64:
        mSerial.AddTail(new DinoEventExecSystemFlag(2, 0));
        break;
    case 0x60:
        mSerial.AddTail(new DinoEventExecReviverSet(DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), DinoEventArg(command, 4), 1));
        break;
    case 0x61:
        mSerial.AddTail(new DinoEventExecReviverSet(DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), DinoEventArg(command, 4), 0));
        break;
    case 0x62:
        mSerial.AddTail(new DinoEventExecMedalChange(DinoEventArg(command, 0), DinoEventArg(command, 1)));
        // (No break: the medal change also sets the battle points.)
    case 0xce:
        mSerial.AddTail(new DinoEventExecBattlePointSet(DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x69:
        mSerial.AddTail(new DinoEventExecShopSet(DinoEventArg(command, 0)));
        break;
    case 0x66:
        mSerial.AddTail(new DinoEventShopBuy(this));
        break;
    case 0x67:
        mSerial.AddTail(new DinoEventShopSell(this));
        break;
    case 0x68:
        mSerial.AddTail(new DinoEventShopMask(this));
        break;
    case 0x6a:
        mSerial.AddTail(new DinoEventMaskEquip(this));
        break;
    case 0xc4:
        mSerial.AddTail(new DinoEventCanonPutin(this));
        break;
    case 0xc5:
        mSerial.AddTail(new DinoEventCanonTakeout(this));
        break;
    case 0x6b:
        mSerial.AddTail(new DinoEventExecKasekiRockGet(this, 0, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), 0));
        break;
    case 0x6c:
        mSerial.AddTail(new DinoEventExecKasekiRockGet(this, 0, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), 1));
        break;
    case 0xc0:
        mSerial.AddTail(new DinoEventExecKasekiRockGet(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), 0));
        break;
    case 0x6d:
        mSerial.AddTail(new DinoEventExecKasekiRockShot(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2)));
        break;
    case 0x72:
        mSerial.AddTail(new DinoEventExecCharaBodyModelIndex(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x73:
        mSerial.AddTail(new DinoEventExecCharaHeadModelIndex(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x70:
        mSerial.AddTail(new DinoEventExecCharaMotionSet(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 0));
        break;
    case 0x71:
        mSerial.AddTail(new DinoEventExecCharaMotionSet(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 1));
        break;
    case 0x6e:
        mSerial.AddTail(new DinoEventExecCharaMotionCache(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x6f:
        mSerial.AddTail(new DinoEventExecCharaMotionCacheClear(this, DinoEventArg(command, 0)));
        break;
    case 0x75:
        mSerial.AddTail(new DinoEventExecBgmPlay(this, DinoEventArg(command, 0), 0, 0));
        break;
    case 0x79:
        mSerial.AddTail(new DinoEventExecBgmPlay(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 0));
        break;
    case 0x77:
        mSerial.AddTail(new DinoEventExecBgmPlay(this, DinoEventArg(command, 0), 0, 1));
        break;
    case 0x7b:
        mSerial.AddTail(new DinoEventExecBgmPlay(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 1));
        break;
    case 0x76:
        mSerial.AddTail(new DinoEventExecMapBgmPlay(this, 0));
        break;
    case 0x7a:
        mSerial.AddTail(new DinoEventExecMapBgmPlay(this, DinoEventArg(command, 0)));
        break;
    case 0x78:
        mSerial.AddTail(new DinoEventExecBgmStop(this, 0));
        break;
    case 0x7c:
        mSerial.AddTail(new DinoEventExecBgmStop(this, DinoEventArg(command, 0)));
        break;
    case 0x7d:
        mSerial.AddTail(DinoSoundCreatePlaySeTask(DinoEventArg(command, 0), 0));
        break;
    case 0x7f:
        mSerial.AddTail(DinoSoundCreatePlaySeTask(DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x7e:
        mSerial.AddTail(DinoSoundCreateStopSeTask(DinoEventArg(command, 0), 0));
        break;
    case 0x80:
        mSerial.AddTail(DinoSoundCreateStopSeTask(DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x81:
        mSerial.AddTail(new DinoEventExecManpuShow(this, DinoEventArg(command, 0), DinoEventArg(command, 1), MPoint(0, 0)));
        break;
    case 0x82:
        mSerial.AddTail(new DinoEventExecManpuShow(this, DinoEventArg(command, 0), DinoEventArg(command, 1), MPoint(DinoEventArg(command, 2), DinoEventArg(command, 3))));
        break;
    case 0x83:
        mSerial.AddTail(new DinoEventExecManpuHide(this, DinoEventArg(command, 0)));
        break;
    case 0x84:
        mSerial.AddTail(new DinoEventExecPictShow(this, DinoEventArg(command, 0)));
        break;
    case 0x85:
        mSerial.AddTail(new DinoEventExecPictHide(this));
        break;
    case 0x87:
        mSerial.AddTail(new DinoEventExecCmnMotion(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x88:
        mSerial.AddTail(new DinoEventExecChrEffect(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0x89:
        mSerial.AddTail(new DinoEventExecChrEffect(this, DinoEventArg(command, 0), 0));
        break;
    case 0x86:
        mSerial.AddTail(new DinoEventExecKeyWait());
        break;
    case 0x8a:
        mSerial.AddTail(new DinoEventExecQuake(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2)));
        break;
    case 0x8b:
        mSerial.AddTail(new DinoEventExecDance(this, 0, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3)));
        break;
    case 0x8c:
        mSerial.AddTail(new DinoEventExecDance(this, 1, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3)));
        break;
    case 0x8d:
        mSerial.AddTail(new DinoEventExecMapSelect(this, DinoEventArg(command, 0)));
        break;
    case 0x8e:
        mSerial.AddTail(new DinoEventNameEntry(this, 1));
        break;
    case 0x8f:
        mSerial.AddTail(new DinoEventNameEntry(this, 0));
        break;
    case 0xc6:
        mSerial.AddTail(new DinoEventSenryuEntry(this));
        break;
    case 0xc7:
        mSerial.AddTail(new DinoEventSenryuListEntry(this));
        break;
    case 0x93:
        mSerial.AddTail(new DinoEventExecEraseAllExcrement((u32)this));
        break;
    case 0x94:
        mSerial.AddTail(new DinoEventExecEraseExcrement((u32)this, DinoEventPackedVar(DinoEventArg(command, 0))));
        break;
    case 0x95:
        mSerial.AddTail(new DinoEventExecGameOver(this));
        break;
    case 0xbd:
        mSerial.AddTail(new DinoEventExecExit(this));
        break;
    case 0x96:
        mSerial.AddTail(new DinoEventLicenseCard(this));
        break;
    case 0x97:
        mSerial.AddTail(new DinoEventExecFieldName(this, DinoEventArg(command, 0)));
        break;
    case 0x98:
        mPictPos = MPoint(DinoEventArg(command, 0), DinoEventArg(command, 1));
        break;
    case 0x99:
        mSerial.AddTail(new DinoEventExecLowerPictFadeIn(this, DinoEventArg(command, 0), mPictPos, DinoEventArg(command, 1), DinoEventArg(command, 2), 1));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0x9a:
        mSerial.AddTail(new DinoEventExecLowerPictFadeOut(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 1));
        break;
    case 0x9b:
        mSerial.AddTail(new DinoEventExecLowerPictSlideIn(this, DinoEventArg(command, 0), mPictPos, DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), 1));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0x9c:
        mSerial.AddTail(new DinoEventExecLowerPictSlideOut(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), 1));
        break;
    case 0x9d:
        mSerial.AddTail(new DinoEventExecLowerPictZoomIn(this, DinoEventArg(command, 0), mPictPos, MFx32::FromRaw(DinoEventArg(command, 1)), DinoEventArg(command, 2), DinoEventArg(command, 3), 1));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0x9e:
        mSerial.AddTail(new DinoEventExecLowerPictZoomOut(this, MFx32::FromRaw(DinoEventArg(command, 0)), DinoEventArg(command, 1), DinoEventArg(command, 2), 1));
        break;
    case 0x9f:
        mSerial.AddTail(new DinoEventExecUpperPictFadeIn(this, DinoEventArg(command, 0), mPictPos, DinoEventArg(command, 1), DinoEventArg(command, 2), 1));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0xa0:
        mSerial.AddTail(new DinoEventExecUpperPictFadeOut(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 1));
        break;
    case 0xa1:
        mSerial.AddTail(new DinoEventExecUpperPictSlideIn(this, DinoEventArg(command, 0), mPictPos, DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), 1));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0xa2:
        mSerial.AddTail(new DinoEventExecUpperPictSlideOut(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), 1));
        break;
    case 0xa3:
        mSerial.AddTail(new DinoEventExecUpperPictZoomIn(this, DinoEventArg(command, 0), mPictPos, MFx32::FromRaw(DinoEventArg(command, 1)), DinoEventArg(command, 2), DinoEventArg(command, 3), 1));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0xa4:
        mSerial.AddTail(new DinoEventExecUpperPictZoomOut(this, MFx32::FromRaw(DinoEventArg(command, 0)), DinoEventArg(command, 1), DinoEventArg(command, 2), 1));
        break;
    case 0xa5:
        mSerial.AddTail(new DinoEventExecLowerPictFadeIn(this, DinoEventArg(command, 0), mPictPos, DinoEventArg(command, 1), DinoEventArg(command, 2), 0));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0xa6:
        mSerial.AddTail(new DinoEventExecLowerPictFadeOut(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 0));
        break;
    case 0xa7:
        mSerial.AddTail(new DinoEventExecLowerPictSlideIn(this, DinoEventArg(command, 0), mPictPos, DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), 0));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0xa8:
        mSerial.AddTail(new DinoEventExecLowerPictSlideOut(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), 0));
        break;
    case 0xa9:
        mSerial.AddTail(new DinoEventExecLowerPictZoomIn(this, DinoEventArg(command, 0), mPictPos, MFx32::FromRaw(DinoEventArg(command, 1)), DinoEventArg(command, 2), DinoEventArg(command, 3), 0));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0xaa:
        mSerial.AddTail(new DinoEventExecLowerPictZoomOut(this, MFx32::FromRaw(DinoEventArg(command, 0)), DinoEventArg(command, 1), DinoEventArg(command, 2), 0));
        break;
    case 0xab:
        mSerial.AddTail(new DinoEventExecUpperPictFadeIn(this, DinoEventArg(command, 0), mPictPos, DinoEventArg(command, 1), DinoEventArg(command, 2), 0));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0xac:
        mSerial.AddTail(new DinoEventExecUpperPictFadeOut(this, DinoEventArg(command, 0), DinoEventArg(command, 1), 0));
        break;
    case 0xad:
        mSerial.AddTail(new DinoEventExecUpperPictSlideIn(this, DinoEventArg(command, 0), mPictPos, DinoEventArg(command, 1), DinoEventArg(command, 2), DinoEventArg(command, 3), 0));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0xae:
        mSerial.AddTail(new DinoEventExecUpperPictSlideOut(this, DinoEventArg(command, 0), DinoEventArg(command, 1), DinoEventArg(command, 2), 0));
        break;
    case 0xaf:
        mSerial.AddTail(new DinoEventExecUpperPictZoomIn(this, DinoEventArg(command, 0), mPictPos, MFx32::FromRaw(DinoEventArg(command, 1)), DinoEventArg(command, 2), DinoEventArg(command, 3), 0));
        mPictPos.x = 0;
        mPictPos.y = 0;
        break;
    case 0xb0:
        mSerial.AddTail(new DinoEventExecUpperPictZoomOut(this, MFx32::FromRaw(DinoEventArg(command, 0)), DinoEventArg(command, 1), DinoEventArg(command, 2), 0));
        break;
    case 0xb2:
        mSerial.AddTail(new DinoEventExecDelAreaNamePanel(this));
        break;
    case 0xb3:
        mSerial.AddTail(new DinoEventExecRadarMode(this, 0, cmd->count == 0 ? 60 : DinoEventArg(command, 0), 1));
        break;
    case 0xb5:
        mSerial.AddTail(new DinoEventExecRadarMode(this, 2, cmd->count == 0 ? 60 : DinoEventArg(command, 0), 1));
        break;
    case 0xb4:
        mSerial.AddTail(new DinoEventExecRadarMode(this, 1, cmd->count == 0 ? 60 : DinoEventArg(command, 0), 1));
        break;
    case 0xb6:
        mSerial.AddTail(new DinoEventExecRadarMode(this, 0, cmd->count == 0 ? 60 : DinoEventArg(command, 0), 0));
        break;
    case 0xb8:
        mSerial.AddTail(new DinoEventExecRadarMode(this, 2, cmd->count == 0 ? 60 : DinoEventArg(command, 0), 0));
        break;
    case 0xb7:
        mSerial.AddTail(new DinoEventExecRadarMode(this, 1, cmd->count == 0 ? 60 : DinoEventArg(command, 0), 0));
        break;
    case 0xb9:
        mSerial.AddTail(new DinoEventExecTownMapMode(this, 0, cmd->count == 0 ? 60 : DinoEventArg(command, 0)));
        break;
    case 0xba:
        mSerial.AddTail(new DinoEventExecTownMapMode(this, 1, cmd->count == 0 ? 60 : DinoEventArg(command, 0)));
        break;
    case 0x74:
        mSerial.AddTail(DinoSoundCreateLoadSceneTask(DinoEventArg(command, 0)));
        break;
    case 0xbb:
        mSerial.AddTail(new DinoEventEnding(this));
        break;
    case 0xbc:
        mSerial.AddTail(new DinoEventSave(this));
        break;
    case 0xbe:
        mSerial.AddTail(new DinoEventMuseum(this));
        break;
    case 0xbf:
        mSerial.AddTail(new DinoEventReviveEffectCaller(this, DinoEventArg(command, 0)));
        break;
    case 0xc1:
        mSerial.AddTail(new DinoEventExecHeightReset(this));
        break;
    case 0xc2:
        mSerial.AddTail(new DinoEventExecCharaFootSeOff(this, DinoEventArg(command, 0)));
        break;
    case 0xc3:
        mSerial.AddTail(new DinoEventExecCharaFootSePreserve(this, DinoEventArg(command, 0), DinoEventArg(command, 1)));
        break;
    case 0xca:
        mSerial.AddTail(new DinoEventExecMoneyShow(this));
        break;
    case 0xcb:
        mSerial.AddTail(new DinoEventExecMoneyHide(this));
        break;
    case 0xcc:
        mSerial.AddTail(new DinoEventExecDonateShow(this));
        break;
    case 0xcd:
        mSerial.AddTail(new DinoEventExecDonateHide(this));
        break;
    case 0xcf:
        mSerial.AddTail(new DinoEventExecProhibitNisReset());
        break;
    case 0x0:
        break;
    }
}
