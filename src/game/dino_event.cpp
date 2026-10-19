// DinoEventLoop: runs the event scripts.
// Only a few small methods are decompiled so far.

#include "game/dino_field.h"
#include "m/bg_new_task.h"
#include "m/overlay.h"
#include "game/dino_event_save_option.h"
#include "game/dino_ui_save_option.h"
#include "game/dino_backup.h"

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
