// Sequence player: plays one sequence (or sequence archive entry) through an NNS sound handle.
// Built as ARM code.

#pragma thumb off

#include "sound.h"

extern "C" {
void NNS_SndHandleInit(NNSSndHandle *handle);
void NNS_SndHandleReleaseSeq(NNSSndHandle *handle);
BOOL NNS_SndArcPlayerStartSeq(NNSSndHandle *handle, int seqNo);
BOOL NNS_SndArcPlayerStartSeqArc(NNSSndHandle *handle, int seqArcNo, int index);
void NNS_SndPlayerSetVolume(NNSSndHandle *handle, int volume);
void NNS_SndPlayerMoveVolume(NNSSndHandle *handle, int targetVolume, int frames);
void NNS_SndPlayerSetTempoRatio(NNSSndHandle *handle, int ratio);
void NNS_SndPlayerSetTrackPitch(NNSSndHandle *handle, u32 trackBitMask, int pitch);
void NNS_SndPlayerSetTrackPan(NNSSndHandle *handle, u32 trackBitMask, int pan);
void NNS_SndPlayerStopSeqBySeqNo(int seqNo, int fadeFrames);
void NNS_SndPlayerStopSeqBySeqArcNo(int seqArcNo, int fadeFrames);
void NNS_SndPlayerStopSeqBySeqArcIdx(int seqArcNo, int index, int fadeFrames);
int NNS_SndPlayerCountPlayingSeqBySeqNo(int seqNo);
int NNS_SndPlayerCountPlayingSeqBySeqArcIdx(int seqArcNo, int index);
}

enum {
    SEQ_KIND_NONE = -1,
    SEQ_KIND_SEQ = 0,       // plain sequence
    SEQ_KIND_SEQARC = 1,    // sequence archive entry
};

SeqPlayer::SeqPlayer() {
    ClearIds();
    ResetParams();
    NNS_SndHandleInit(&handle);
}

SeqPlayer::~SeqPlayer() {
    NNS_SndHandleReleaseSeq(&handle);
}

void SeqPlayer::ClearIds() {
    seqNo = -1;
    index = -1;
}

void SeqPlayer::Update() {
}

BOOL SeqPlayer::StartSeq(int no, int fadeFrames) {
    BOOL started = TRUE;

    if (!NNS_SndArcPlayerStartSeq(&handle, no)) {
        started = FALSE;
    }
    if (started) {
        if (fadeFrames == 0) {
            if (volumeSet) {
                NNS_SndPlayerSetVolume(&handle, volume);
            } else {
                NNS_SndPlayerSetVolume(&handle, 127);
            }
        } else {
            if (!volumeSet) {
                volume = 127;
            }
            NNS_SndPlayerMoveVolume(&handle, 0, 0);
            NNS_SndPlayerMoveVolume(&handle, volume, fadeFrames);
        }
        if (tempoSet) {
            NNS_SndPlayerSetTempoRatio(&handle, tempo);
        }
        if (pitchSet) {
            NNS_SndPlayerSetTrackPitch(&handle, 0xFFFF, pitch);
        }
        if (panSet) {
            NNS_SndPlayerSetTrackPan(&handle, 0xFFFF, pan);
        }
    }
    if (started) {
        seqNo = no;
        index = -1;
    }
    ResetParams();
    return started;
}

BOOL SeqPlayer::StartSeqArc(int seqArcNo, int idx, int fadeFrames) {
    BOOL started = TRUE;

    if (!NNS_SndArcPlayerStartSeqArc(&handle, seqArcNo, idx)) {
        started = FALSE;
    }
    if (started) {
        if (fadeFrames == 0) {
            if (volumeSet) {
                NNS_SndPlayerSetVolume(&handle, volume);
            } else {
                NNS_SndPlayerSetVolume(&handle, 127);
            }
        } else {
            if (!volumeSet) {
                volume = 127;
            }
            NNS_SndPlayerMoveVolume(&handle, 0, 0);
            NNS_SndPlayerMoveVolume(&handle, volume, fadeFrames);
        }
        if (tempoSet) {
            NNS_SndPlayerSetTempoRatio(&handle, tempo);
        }
        if (pitchSet) {
            NNS_SndPlayerSetTrackPitch(&handle, 0xFFFF, pitch);
        }
        if (panSet) {
            NNS_SndPlayerSetTrackPan(&handle, 0xFFFF, pan);
        }
    }
    if (started) {
        seqNo = seqArcNo;
        index = idx;
    }
    ResetParams();
    return started;
}

BOOL SeqPlayer::Play(int no, int idx, int fadeFrames) {
    if (fadeFrames < 0) {
        fadeFrames = 0;
    }
    switch (GetKind(no, idx)) {
    case SEQ_KIND_NONE:
        return FALSE;
    case SEQ_KIND_SEQ:
        return StartSeq(no, fadeFrames);
    case SEQ_KIND_SEQARC:
        return StartSeqArc(no, idx, fadeFrames);
    }
    return FALSE;
}

void SeqPlayer::Stop(int no, int idx, int fadeFrames) {
    int kind = GetKind(no, idx);

    if (no == seqNo && idx == index) {
        StopCurrent(fadeFrames);
        return;
    }
    switch (kind) {
    case SEQ_KIND_SEQ:
        NNS_SndPlayerStopSeqBySeqArcNo(no, fadeFrames);
        break;
    case SEQ_KIND_SEQARC:
        NNS_SndPlayerStopSeqBySeqArcIdx(no, idx, fadeFrames);
        break;
    }
}

void SeqPlayer::StopCurrent(int fadeFrames) {
    if (fadeFrames < 0) {
        fadeFrames = 0;
    }
    switch (GetKind(seqNo, index)) {
    case SEQ_KIND_SEQ:
        NNS_SndPlayerStopSeqBySeqNo(seqNo, fadeFrames);
        break;
    case SEQ_KIND_SEQARC:
        NNS_SndPlayerStopSeqBySeqArcIdx(seqNo, index, fadeFrames);
        break;
    }
    ClearIds();
}

// Sets the volume for the next start. If one is already set, the new value scales it.
void SeqPlayer::SetVolume(int vol) {
    if (vol < 0) {
        vol = 0;
    } else if (vol > 127) {
        vol = 127;
    }
    if (volumeSet) {
        if (vol == 0) {
            volume = 0;
        } else {
            volume = volume * (vol + 1) / 128;
        }
    } else {
        volume = vol;
    }
    volumeSet = TRUE;
}

void SeqPlayer::SetPitch(int p) {
    if (p < -0x8000) {
        p = -0x8000;
    } else if (p > 0x7FFF) {
        p = 0x7FFF;
    }
    pitch = p;
    pitchSet = TRUE;
}

void SeqPlayer::SetTempo(int t) {
    if (t <= 0) {
        t = 1;
    }
    tempo = t;
    tempoSet = TRUE;
}

void SeqPlayer::SetPan(int p) {
    if (p < -1) {
        p = -1;
    } else if (p > 127) {
        p = -1;
    }
    pan = p;
    panSet = TRUE;
}

void SeqPlayer::ResetParams() {
    volume = 127;
    pitch = 0;
    tempo = 0x100;
    pan = 0x40;
    tempoSet = FALSE;
    pitchSet = FALSE;
    volumeSet = FALSE;
    panSet = FALSE;
}

BOOL SeqPlayer::IsPlaying() {
    switch (GetKind(seqNo, index)) {
    case SEQ_KIND_NONE:
        return FALSE;
    case SEQ_KIND_SEQ:
        return NNS_SndPlayerCountPlayingSeqBySeqNo(seqNo) != 0;
    case SEQ_KIND_SEQARC:
        return NNS_SndPlayerCountPlayingSeqBySeqArcIdx(seqNo, index) != 0;
    }
    return FALSE;
}

void SeqPlayer::GetPlaying(int *no, int *idx) {
    *no = -1;
    *idx = -1;
    switch (GetKind(seqNo, index)) {
    case SEQ_KIND_SEQ:
        if (NNS_SndPlayerCountPlayingSeqBySeqNo(seqNo) != 0) {
            *no = seqNo;
            *idx = -1;
            break;
        }
        // fallthrough
    case SEQ_KIND_SEQARC:
        if (NNS_SndPlayerCountPlayingSeqBySeqArcIdx(seqNo, index) != 0) {
            *no = seqNo;
            *idx = index;
        }
        break;
    }
}

int SeqPlayer::GetKind(int no, int idx) {
    if (no < 0) {
        return SEQ_KIND_NONE;
    }
    if (idx == -1) {
        return SEQ_KIND_SEQ;
    }
    if (idx >= -2) {
        return SEQ_KIND_SEQARC;
    }
    return SEQ_KIND_NONE;
}
