// Sequence player with a "recently played" list, so a sound effect isn't restarted every frame.
// Built as ARM code.

#pragma thumb off

#include "sound.h"

extern "C" void OS_Terminate(void);

SeqPlayerEx::SeqPlayerEx() {
    InitList();
}

void SeqPlayerEx::Update() {
    int i;
    int next;

    for (i = entries[SEQ_ENTRY_HEAD].next; i != SEQ_ENTRY_TAIL; i = next) {
        entries[i].timer--;
        next = entries[i].next;
        if (entries[i].timer <= 0) {
            RemoveEntry(i);
        }
    }
    player.Update();
}

// Plays the sequence unless it's already in the list. With a duration, adds it to the list for that many frames.
BOOL SeqPlayerEx::PlayOnce(int no, int index, int duration, int fadeFrames) {
    BOOL started;

    if (Contains(no, index)) {
        return TRUE;
    }
    player.Stop(no, index, 0);
    started = player.Play(no, index, fadeFrames);
    if (duration <= 0) {
        return started;
    }
    if (started && !AddEntry(no, index, duration)) {
        OS_Terminate();
    }
    return started;
}

void SeqPlayerEx::InitList() {
    int i;

    for (i = 0; i < SEQ_ENTRY_COUNT; i++) {
        InitEntry(i);
    }
    entries[SEQ_ENTRY_HEAD].next = SEQ_ENTRY_TAIL;
    entries[SEQ_ENTRY_TAIL].prev = SEQ_ENTRY_HEAD;
}

int SeqPlayerEx::FindFreeEntry() {
    int i;

    for (i = 2; i < SEQ_ENTRY_COUNT; i++) {
        if (entries[i].timer == 0) {
            return i;
        }
    }
    return -1;
}

void SeqPlayerEx::InsertBefore(int at, int entry) {
    int prev;

    if (at <= 0 || at >= SEQ_ENTRY_COUNT) {
        return;
    }
    if (entry <= 1 || entry >= SEQ_ENTRY_COUNT) {
        return;
    }
    prev = entries[at].prev;
    entries[entry].prev = prev;
    entries[entry].next = at;
    entries[prev].next = entry;
    entries[at].prev = entry;
}

#ifdef NONMATCHING
// Equivalent, but mwcc swaps the registers holding prev and next (ip/lr).
void SeqPlayerEx::RemoveEntry(int entry) {
    int prev;
    int next;

    if (entry <= 1 || entry >= SEQ_ENTRY_COUNT) {
        return;
    }
    prev = entries[entry].prev;
    next = entries[entry].next;
    entries[prev].next = next;
    entries[next].prev = prev;
    InitEntry(entry);
}
#else
asm void SeqPlayerEx::RemoveEntry(int entry) {
    nofralloc
    stmdb sp!, {r3, lr}
    cmp r1, #1
    ldmleia sp!, {r3, pc}
    cmp r1, #0x12
    ldmgeia sp!, {r3, pc}
    mov r2, #0x14
    mla r3, r1, r2, r0
    ldr lr, [r3, #0x2c]
    ldr ip, [r3, #0x30]
    mla r3, lr, r2, r0
    mla r2, ip, r2, r0
    str ip, [r3, #0x30]
    str lr, [r2, #0x2c]
    bl InitEntry
    ldmia sp!, {r3, pc}
}
#endif

void SeqPlayerEx::InitEntry(int entry) {
    if (entry <= 1 || entry >= SEQ_ENTRY_COUNT) {
        return;
    }
    entries[entry].no = 0;
    entries[entry].index = 0;
    entries[entry].timer = 0;
    entries[entry].next = -1;
    entries[entry].prev = -1;
}

int SeqPlayerEx::FindEntry(int no, int index) {
    int i;

    for (i = entries[SEQ_ENTRY_HEAD].next; i != SEQ_ENTRY_TAIL; i = entries[i].next) {
        if (entries[i].no == no && entries[i].index == index) {
            return i;
        }
    }
    return -1;
}

BOOL SeqPlayerEx::Contains(int no, int index) {
    return FindEntry(no, index) != -1;
}

BOOL SeqPlayerEx::AddEntry(int no, int index, int duration) {
    int i = FindFreeEntry();

    if (i == -1) {
        return FALSE;
    }
    InsertBefore(SEQ_ENTRY_TAIL, i);
    entries[i].no = no;
    entries[i].index = index;
    entries[i].timer = duration;
    return TRUE;
}
