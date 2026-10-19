// DinoEventLoop: runs the event scripts.
// Only a few small methods are decompiled so far.

#include "game/dino_field.h"
#include "m/bg_new_task.h"
#include "m/overlay.h"
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

extern "C" void func_0203a4f4(DinoEventLoop *event);
M_EXTERN_OVERLAY(ov001);

// Loads the field overlay and makes the field from the saved state.
void DinoEventLoop::OpenField(BOOL arg) {
    if (arg) {
        func_0203a4f4(this);
    }
    MLoadOverlay(0, M_OVERLAY_ID(ov001));
    mField = new DinoField(&mUnk110, 0);
    mParallel.AddTail(mField);
    mField->Read(gDinoBackup->GetUnk140());
}
