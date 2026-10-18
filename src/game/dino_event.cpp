// DinoEventLoop: runs the event scripts.
// Only a few small methods are decompiled so far.

#include "game/dino_field.h"
#include "m/bg_new_task.h"

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
