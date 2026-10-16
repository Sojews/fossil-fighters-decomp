// Stream player: plays one stream from the sound archive, with pause/resume by position.
// Built as ARM code.

#pragma thumb off

#include "sound.h"

extern "C" {
void NNS_SndStrmHandleInit(NNSSndStrmHandle *handle);
void NNS_SndStrmHandleRelease(NNSSndStrmHandle *handle);
BOOL NNS_SndArcStrmStart(NNSSndStrmHandle *handle, int strmNo, u32 offset);
void NNS_SndArcStrmStop(NNSSndStrmHandle *handle, int fadeFrames);
void NNS_SndArcStrmMoveVolume(NNSSndStrmHandle *handle, int targetVolume, int frames);
u32 NNS_SndArcStrmGetCurrentPlayingPos(NNSSndStrmHandle *handle);
}

static inline BOOL NNS_SndStrmHandleIsValid(NNSSndStrmHandle *handle) {
    return handle->strm != NULL;
}

StrmPlayer::StrmPlayer() {
    Init();
}

StrmPlayer::~StrmPlayer() {
    NNS_SndStrmHandleRelease(&handle);
}

void StrmPlayer::Init() {
    strmNo = -1;
    volume = 127;
    pos = 0;
    playing = FALSE;
    paused = FALSE;
    flag = FALSE;
    NNS_SndStrmHandleInit(&handle);
    NNS_SndStrmHandleInit(&handle2);
}

void StrmPlayer::Update() {
    if (playing & !paused) {
        if (NNS_SndStrmHandleIsValid(&handle)) {
            pos = NNS_SndArcStrmGetCurrentPlayingPos(&handle);
        } else {
            pos = 0;
            playing = FALSE;
        }
    }
    if (flag) {
        if (!playing) {
            flag = FALSE;
        }
    }
}

BOOL StrmPlayer::Start(int no, int startPos, int unused) {
    if (startPos == -1) {
        startPos = 0;
    }
    if (!NNS_SndArcStrmStart(&handle, no, startPos)) {
        return FALSE;
    }
    strmNo = no;
    pos = startPos;
    playing = TRUE;
    paused = FALSE;
    volume = 127;
    return TRUE;
}

void StrmPlayer::StopStream(int fadeFrames) {
    if (fadeFrames < 0) {
        fadeFrames = 0;
    }
    NNS_SndArcStrmStop(&handle, fadeFrames);
}

void StrmPlayer::Stop(int fadeFrames) {
    StopStream(fadeFrames);
    strmNo = -1;
    pos = 0;
    playing = FALSE;
    paused = FALSE;
    flag = FALSE;
    volume = 127;
}

void StrmPlayer::Pause() {
    if (!playing) {
        return;
    }
    if (paused) {
        return;
    }
    StopStream(0);
    paused = TRUE;
}

// Restarts the stream where it was paused, at vol/127 of its volume.
void StrmPlayer::Resume(int vol) {
    if (!playing || !paused) {
        return;
    }
    if (vol < 0) {
        vol = 0;
    } else if (vol > 127) {
        vol = 127;
    }
    Start(strmNo, pos, 0);
    MoveVolume(volume * vol / 127, 0);
    paused = FALSE;
}

void StrmPlayer::SetFlag() {
    if (playing) {
        flag = TRUE;
    }
}

void StrmPlayer::ClearFlag() {
    if (playing != TRUE) {
        flag = FALSE;
    }
}

void StrmPlayer::MoveVolume(int vol, int frames) {
    if (frames < 0) {
        frames = 0;
    }
    NNS_SndArcStrmMoveVolume(&handle, vol, frames);
    volume = vol;
}

BOOL StrmPlayer::GetFlag() {
    return (flag & playing) != 0;
}
