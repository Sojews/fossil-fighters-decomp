// Sound group loader: keeps the loaded groups in a stack that mirrors the sound heap's save states.
// Built as ARM code.

#pragma thumb off

#include "sound.h"

extern "C" {
BOOL NNS_SndArcLoadGroup(int groupNo, NNSSndHeapHandle heap);
int NNS_SndHeapSaveState(NNSSndHeapHandle heap);
void NNS_SndHeapLoadState(NNSSndHeapHandle heap, int level);
u32 NNS_SndHeapGetFreeSize(NNSSndHeapHandle heap);
}

SoundLoader::SoundLoader() {
    int i;

    initialized = FALSE;
    heap = NULL;
    lastGroup = -1;
    for (i = 0; i < 16; i++) {
        groups[i] = -1;
    }
}

BOOL SoundLoader::LoadGroup(int group) {
    BOOL loaded = NNS_SndArcLoadGroup(group, heap) ? TRUE : FALSE;

    stack.Push(group);
    return loaded;
}

// Releases every group after the first `count`.
void SoundLoader::Restore(int count) {
    if (count >= stack.Count()) {
        return;
    }
    NNS_SndHeapLoadState(heap, count + 1);
    stack.Truncate(count);
}

void SoundLoader::SaveState() {
    NNS_SndHeapSaveState(heap);
}

int SoundLoader::GetCount() {
    return stack.Count();
}

void SoundLoader::Init(NNSSndHeapHandle h) {
    heap = h;
    NNS_SndHeapSaveState(h);
    stack.Init(groups, 16);
    initialized = TRUE;
}

// Makes the loaded groups equal to `list`, keeping the ones already loaded in the same order.
// Returns the free heap size, or -1 if a group failed to load.
int SoundLoader::Load(int count, const int *list) {
    BOOL ok = TRUE;
    int i = CountLoaded(list, count);

    Restore(i);
    for (; i < count; i++) {
        if (!LoadGroup(list[i])) {
            ok = FALSE;
        }
        SaveState();
    }
    if (count != 0) {
        lastGroup = list[count - 1];
    }
    func_020031cc();
    if (!ok) {
        return -1;
    }
    return NNS_SndHeapGetFreeSize(heap);
}

int SoundLoader::CountLoaded(const int *list, int count) {
    int i;

    for (i = 0; i < count; i++) {
        if (list[i] != stack.Get(i)) {
            break;
        }
    }
    return i;
}

// Replaces the most recently loaded group.
void SoundLoader::ReplaceTop(int group) {
    if (group < 0) {
        return;
    }
    Restore(GetCount() - 1);
    LoadGroup(group);
    SaveState();
    func_020031cc();
}

void SoundLoader::func_020031cc() {
}
