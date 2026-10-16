// Background-music player: sequence playback with volume/pitch fades and per-track volumes.
// Built as ARM code.

#pragma thumb off

#include "sound.h"

extern "C" {
void NNS_SndPlayerSetVolume(NNSSndHandle *handle, int volume);
void NNS_SndPlayerMoveVolume(NNSSndHandle *handle, int targetVolume, int frames);
void NNS_SndPlayerSetTrackVolume(NNSSndHandle *handle, u16 trackBitMask, int volume);
void NNS_SndPlayerSetTrackPitch(NNSSndHandle *handle, u16 trackBitMask, int pitch);
void NNS_SndPlayerSetTrackMute(NNSSndHandle *handle, u16 trackBitMask, BOOL mute);
}

void Unk2BCC::Update() {
    if (!IsPlaying()) {
        return;
    }
    UpdatePitch();
    UpdateVolume();
}

void Unk2BCC::UpdateVolume() {
    int i;
    int volume;

    if (!volumeFade.done) {
        ApplyVolume(volumeFade.value);
    }
    volumeFade.Update();
    for (i = 0; i < 16; i++) {
        if (tracks.IsActive(i)) {
            volume = tracks.GetTrackVolume(i);
            if (volume != -1) {
                SetTrackVolume(volume, 1 << i);
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (tracks.IsActive(i)) {
            NNS_SndPlayerSetTrackMute(&handle, 1 << i, tracks.IsMuted(i));
        }
    }
    tracks.Update();
}

void Unk2BCC::UpdatePitch() {
    if (!pitchFade.done) {
        SetTrackPitch(pitchFade.value, 0);
    }
    pitchFade.Update();
}

BOOL Unk2BCC::Play(int no, int idx, int fadeFrames) {
    BOOL started;

    if (IsPlaying()) {
        Stop(0);
    }
    started = SeqPlayer::Play(no, idx, fadeFrames);
    if (started) {
        pitchFade.Start(0, 0, 0);
        unk50.Start(0x78, 0x78, 0);
        volumeFade.Start(0x7F, 0x7F, 0);
        tracks.Start(0xFFFF);
    }
    return started;
}

void Unk2BCC::Stop(int fadeFrames) {
    if (!IsPlaying()) {
        return;
    }
    StopCurrent(fadeFrames);
    pitchFade.Reset();
    unk50.Reset();
    volumeFade.Reset();
    tracks.Reset();
    trackMask = 0;
}

BOOL Unk2BCC::PlayIfNew(int no, int idx, int fadeFrames) {
    if (IsPlayingCurrent(no, idx)) {
        return TRUE;
    }
    return Play(no, idx, fadeFrames);
}

// Plays the sequence with only the tracks in mask audible. If it's already playing, crossfades the tracks instead.
BOOL Unk2BCC::PlayTracks(int no, int idx, int fadeFrames, u16 mask, int trackFadeFrames) {
    if (IsPlayingCurrent(no, idx)) {
        u16 current = trackMask;
        SetTracks(current & ~mask, trackFadeFrames, TRUE);
        SetTracks(~current & mask, trackFadeFrames, FALSE);
    } else {
        if (!Play(no, idx, fadeFrames)) {
            return FALSE;
        }
        SetTracks(~mask, 0, TRUE);
    }
    trackMask = mask;
    return TRUE;
}

void Unk2BCC::SetTracks(u16 mask, int frames, int mute) {
    tracks.FadeTracks(mask, frames, mute);
}

void Unk2BCC::ApplyVolume(int volume) {
    if (!IsPlaying()) {
        return;
    }
    if (volume < 0) {
        volume = 0;
    } else if (volume > 127) {
        volume = 127;
    }
    NNS_SndPlayerSetVolume(&handle, volume);
}

void Unk2BCC::SetVolumeFade(int volume, int frames) {
    if (!IsPlaying()) {
        return;
    }
    if (volume < 0) {
        volume = 0;
    } else if (volume > 127) {
        volume = 127;
    }
    if (frames <= 0) {
        NNS_SndPlayerSetVolume(&handle, volume);
    } else {
        NNS_SndPlayerMoveVolume(&handle, volume, frames);
    }
}

void Unk2BCC::SetTrackVolume(int volume, u16 mask) {
    if (!IsPlaying()) {
        return;
    }
    if (volume < 0) {
        volume = 0;
    } else if (volume > 127) {
        volume = 127;
    }
    NNS_SndPlayerSetTrackVolume(&handle, mask, volume);
}

void Unk2BCC::SetTrackPitch(int p, u16 mask) {
    if (!IsPlaying()) {
        return;
    }
    if (p < -0x8000) {
        p = -0x8000;
    } else if (p > 0x7FFF) {
        p = 0x7FFF;
    }
    NNS_SndPlayerSetTrackPitch(&handle, mask, p);
}

BOOL Unk2BCC::IsCurrent(int no, int idx) {
    int curNo;
    int curIdx;

    if (!IsPlaying()) {
        return FALSE;
    }
    if (no == -1) {
        return FALSE;
    }
    GetPlaying(&curNo, &curIdx);
    if (curNo == -1) {
        return FALSE;
    }
    return curNo == no && curIdx == idx;
}

BOOL Unk2BCC::IsPlaying() {
    return SeqPlayer::IsPlaying();
}

BOOL Unk2BCC::IsPlayingCurrent(int no, int idx) {
    if (!IsCurrent(no, idx)) {
        return FALSE;
    }
    return IsPlaying();
}

int Unk2BCC::GetVolume() {
    if (IsPlaying()) {
        return volumeFade.value;
    }
    return 0;
}
