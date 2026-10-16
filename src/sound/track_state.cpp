// Per-track state for a BGM sequence: which tracks are active, which are muted, and their volume fades.
// Built as ARM code.

#pragma thumb off

#include "sound.h"

Unk1A8::Unk1A8() {
    int i;

    active = FALSE;
    activeBits = 0;
    activeTracks.Init(&activeBits, 16);
    muteBits = 0;
    mutedTracks.Init(&muteBits, 16);
    mutedTracks.SetAll(FALSE);
    for (i = 0; i < 16; i++) {
        fades[i].Reset();
    }
}

void Unk1A8::Update() {
    int i;

    if (!active) {
        return;
    }
    for (i = 0; i < 16; i++) {
        fades[i].Update();
        if (!mutedTracks.Test(i) && fades[i].value <= 0) {
            mutedTracks.Set(i);
        } else if (mutedTracks.Test(i) && fades[i].value > 0) {
            mutedTracks.Clear(i);
        }
    }
}

void Unk1A8::Start(int trackMask) {
    int i;

    for (i = 0; i < 16; i++) {
        if (trackMask & (u16)(1 << i)) {
            fades[i].unk4 = fades[i].unk8 = fades[i].value = 127;
            activeTracks.Set(i);
        }
    }
    active = TRUE;
}

// Fades the tracks in trackMask out (mute) or back to full volume.
#ifdef NONMATCHING
// Equivalent, but mwcc doesn't hoist `i = 0` and the fade pointer above the branch like the original does.
void Unk1A8::FadeTracks(u16 trackMask, int frames, int mute) {
    int i;

    if (mute) {
        for (i = 0; i < 16; i++) {
            if (trackMask & (u16)(1 << i)) {
                fades[i].Start(fades[i].value, 0, frames);
            }
        }
    } else {
        for (i = 0; i < 16; i++) {
            if (trackMask & (u16)(1 << i)) {
                fades[i].Start(fades[i].value, 127, frames);
            }
        }
    }
}
#else
// Unk18::Start, by its symbol name, for the asm below.
extern "C" void _ZN5Unk185StartEiii(Unk18 *fade, int from, int to, int frames);

asm void Unk1A8::FadeTracks(u16 trackMask, int frames, int mute) {
    nofralloc
    stmdb sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    mov r6, r0
    mov r5, r1
    mov r4, r2
    cmp r3, #0
    add r10, r6, #0x28
    mov r9, #0
    mov r8, #1
    beq unmute
    mov r7, r9
mute_loop:
    mov r0, r8, lsl r9
    mov r0, r0, lsl #16
    tst r5, r0, lsr #16
    beq mute_next
    ldr r1, [r6, #0x34]
    mov r0, r10
    mov r2, r7
    mov r3, r4
    bl _ZN5Unk185StartEiii
mute_next:
    add r9, r9, #1
    cmp r9, #16
    add r6, r6, #0x18
    add r10, r10, #0x18
    blt mute_loop
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, pc}
unmute:
    mov r7, #0x7f
unmute_loop:
    mov r0, r8, lsl r9
    mov r0, r0, lsl #16
    tst r5, r0, lsr #16
    beq unmute_next
    ldr r1, [r6, #0x34]
    mov r0, r10
    mov r2, r7
    mov r3, r4
    bl _ZN5Unk185StartEiii
unmute_next:
    add r9, r9, #1
    cmp r9, #16
    add r6, r6, #0x18
    add r10, r10, #0x18
    blt unmute_loop
    ldmia sp!, {r4, r5, r6, r7, r8, r9, r10, pc}
}
#endif

void Unk1A8::Reset() {
    int i;

    activeTracks.SetAll(FALSE);
    mutedTracks.SetAll(FALSE);
    for (i = 0; i < 16; i++) {
        fades[i].Reset();
    }
    active = FALSE;
}

int Unk1A8::GetTrackVolume(int track) {
    return fades[track].value;
}

BOOL Unk1A8::IsActive(int track) {
    return activeTracks.Test(track);
}

BOOL Unk1A8::IsMuted(int track) {
    return mutedTracks.Test(track);
}
