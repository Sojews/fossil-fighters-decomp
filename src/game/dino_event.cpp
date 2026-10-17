// DinoEvent: runs an event script.
// Only a few small methods are decompiled so far.

#include "game/dino_field.h"

void DinoEvent::SetResult(u32 result) {
    if (mResult) {
        *mResult = result;
    }
}

DinoField *DinoEvent::GetField() {
    return mField;
}

void DinoEvent::SetEpisodesChanged() {
    mEpisodesChanged = TRUE;
}

void DinoEvent::MarkCameraUsed() {
    mCameraUsed = TRUE;
}

void DinoEvent::Fade(u32 a, u32 b) {
    mField->Fade(a, b);
}

MTask *DinoEvent::CreateFadeWaitTask() {
    return mField->CreateFadeWaitTask();
}
