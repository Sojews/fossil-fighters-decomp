// Sound manager: owns the sound heap and archive, and queues sound commands for the main loop.
// Built as ARM code.

#pragma thumb off

#include "types.h"

typedef struct NNSSndHeap *NNSSndHeapHandle;

extern "C" {
void NNS_SndInit(void);
void NNS_SndMain(void);
NNSSndHeapHandle NNS_SndHeapCreate(void *startAddress, u32 size);
BOOL NNS_SndArcInit(void *arc, const char *filePath, NNSSndHeapHandle heap, BOOL symbolLoad);
BOOL NNS_SndArcPlayerSetup(NNSSndHeapHandle heap);
void *NNS_SndArcSetCurrent(void *arc);
}

// Loads groups of sound data into the heap, using heap save states so they can be released in reverse order.
class SoundLoader {
public:
    void Init(NNSSndHeapHandle heap);
    int Load(int count, const int *groups);
    void Unload(int level);
};

// A sequence player (BGM/jingles). The second one at 0x2A24 has extra state.
class SeqPlayer {
public:
    int func_02001eb0(int a, int b, int c);
    void func_02001f34(int a, int b, int c);
    void func_02001fa8(int a);
    void func_02002000(int a);
    void func_0200208c(int a);
    void func_02002060(int a);
    void func_020020a4(int a);
    void func_020020cc();
    void Update();

    u8 unk0[0x20];
};

class SeqPlayerEx : public SeqPlayer {
public:
    int func_02002288(int a, int b, int c, int d);
    void Update();

    u8 unk20[0x188 - 0x20];
};

class Unk2BCC {
public:
    void Update();
    u8 unk0[0x428];
};

class Unk2FF4 {
public:
    void Update();
    u8 unk0[0x224];
};

class Unk3218 {
public:
    void Update();
    u8 unk0[0x508];
};

struct SoundCommand {
    u32 type;
    s32 arg0;
    s32 arg1;
    s32 arg2;
};

class SoundManager {
public:
    void Init();
    void CreateHeap(void *mem, u32 size);
    void LoadArchive(const char *path);
    void InitLoader();
    int LoadGroups(int count, const int *groups);
    void Unload(int level);
    void Update();
    int func_02000e64(int a, int b, int c);
    void func_02000e88(int a, int b, int c);
    void func_02000ea8(int a);
    void func_02000ec8(int a);
    void func_02000ee8(int a);
    void func_02000f08(int a);
    void func_02000f28(int a);
    void func_02000f48();
    int func_02000f68(int a, int b, int c, int d);
    void func_02000f94(int a, int b, int c);
    void func_02000fb4(int a);
    void func_02001080(int a, int b, int c);
    int func_020010a0();
    void func_02001198(int a);
    void func_020011c4(int a, int b, int c);
    void func_0200120c(int a, int b);
    void func_02001274(int a);
    int func_02001488();

    u8 enabled;
    u8 heapCreated;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    NNSSndHeapHandle heap;              // 0x10
    u8 arc[0x190];                      // 0x14: NNSSndArc
    u8 unk1A4[0x28A4 - 0x1A4];
    s32 numCommands;                    // 0x28A4
    SoundCommand commands[16];          // 0x28A8
    SoundLoader loader;                 // 0x29A8
    u8 unk29A9[0x2A04 - 0x29A9];
    SeqPlayer player0;                  // 0x2A04
    SeqPlayerEx player1;                // 0x2A24
    SeqPlayer player2;                  // 0x2BAC
    Unk2BCC unk2BCC;
    Unk2FF4 unk2FF4;
    Unk3218 unk3218;
    u8 unk3720;
    u8 unk3721;
};

void SoundManager::Init() {
    enabled = 0;
    heapCreated = 0;
    numCommands = 0;
    unk4 = 0x7F;
    unk8 = 0x7F;
    unkC = 0x7F;
    unk2 = 0;
    unk3 = 0;
    unk3720 = 0;
    unk3721 = 0;
    NNS_SndInit();
}

void SoundManager::CreateHeap(void *mem, u32 size) {
    heap = NNS_SndHeapCreate(mem, size);
    heapCreated = 1;
}

void SoundManager::LoadArchive(const char *path) {
    NNS_SndArcInit(arc, path, heap, FALSE);
    NNS_SndArcPlayerSetup(heap);
    NNS_SndArcSetCurrent(arc);
}

void SoundManager::InitLoader() {
    loader.Init(heap);
}

int SoundManager::LoadGroups(int count, const int *groups) {
    return loader.Load(count, groups);
}

void SoundManager::Unload(int level) {
    loader.Unload(level);
}

void SoundManager::Update() {
    int i;

    NNS_SndMain();
    for (i = 0; i < numCommands; i++) {
        SoundCommand *cmd = &commands[i];
        switch (cmd->type) {
        case 0:
            if (cmd->arg1 == -1) {
                func_02000ea8(cmd->arg0);
            } else {
                func_02000e88(cmd->arg1, cmd->arg2, cmd->arg0);
            }
            break;
        case 1:
            if (cmd->arg1 == -1) {
                func_02000fb4(cmd->arg0);
            } else {
                func_02000f94(cmd->arg1, cmd->arg2, cmd->arg0);
            }
            break;
        case 2:
            if (cmd->arg1 == -1) {
                func_02001198(cmd->arg0);
            } else {
                func_020011c4(cmd->arg1, cmd->arg2, cmd->arg0);
            }
            break;
        case 3:
            func_02001080(cmd->arg1, cmd->arg2, cmd->arg0);
            break;
        case 4:
            func_02001274(cmd->arg0);
            break;
        }
    }
    numCommands = 0;
    player0.Update();
    player1.Update();
    unk2BCC.Update();
    unk2FF4.Update();
    unk3218.Update();
    if (unk2) {
        if (func_020010a0() == 0) {
            func_0200120c(unk4, 6);
            unk2 = 0;
        }
    } else if (unk3) {
        if (func_02001488() == 0) {
            func_0200120c(unk4, 6);
            unk3 = 0;
        }
    }
}

int SoundManager::func_02000e64(int a, int b, int c) {
    if (!enabled) {
        return 0;
    }
    return player0.func_02001eb0(a, b, c);
}

void SoundManager::func_02000e88(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    player0.func_02001f34(a, b, c);
}

void SoundManager::func_02000ea8(int a) {
    if (!enabled) {
        return;
    }
    player0.func_02001fa8(a);
}

void SoundManager::func_02000ec8(int a) {
    if (!enabled) {
        return;
    }
    player0.func_02002000(a);
}

void SoundManager::func_02000ee8(int a) {
    if (!enabled) {
        return;
    }
    player0.func_0200208c(a);
}

void SoundManager::func_02000f08(int a) {
    if (!enabled) {
        return;
    }
    player0.func_02002060(a);
}

void SoundManager::func_02000f28(int a) {
    if (!enabled) {
        return;
    }
    player0.func_020020a4(a);
}

void SoundManager::func_02000f48() {
    if (!enabled) {
        return;
    }
    player0.func_020020cc();
}

int SoundManager::func_02000f68(int a, int b, int c, int d) {
    if (!enabled) {
        return 0;
    }
    return player1.func_02002288(a, b, c, d);
}

void SoundManager::func_02000f94(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    player1.func_02001f34(a, b, c);
}

void SoundManager::func_02000fb4(int a) {
    if (!enabled) {
        return;
    }
    player1.func_02001fa8(a);
}
