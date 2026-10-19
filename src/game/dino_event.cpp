// DinoEventLoop: runs the event scripts.
// Only a few small methods are decompiled so far.

#include "game/dino_field.h"
#include "m/bg_new_task.h"
#include "m/overlay.h"
#include "game/dino_event_save_option.h"
#include "game/dino_ui_save_option.h"
#include "game/dino_backup.h"
#include "game/dino_event_exec.h"

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
void DinoEventLoop::OpenField(BOOL arg) {
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
