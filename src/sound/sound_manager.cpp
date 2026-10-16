// Sound manager: owns the sound heap and archive, and queues sound commands for the main loop.
// Built as ARM code.

#pragma thumb off

#include <stdarg.h>
#include "types.h"

typedef struct NNSSndHeap *NNSSndHeapHandle;

extern "C" {
void NNS_SndInit(void);
void NNS_SndMain(void);
BOOL NNS_SndPlayerPauseByPlayerNo(int playerNo, BOOL flag);
void NNS_SndPlayerSetPlayerVolume(int playerNo, int volume);
void NNS_SndPlayerStopSeqByPlayerNo(int playerNo, int fadeFrames);
void NNS_SndArcStrmInit(u32 priority, NNSSndHeapHandle heap);
void NNS_SndArcStrmCreatePrepareThread(void *stack, u32 priority);
NNSSndHeapHandle NNS_SndHeapCreate(void *startAddress, u32 size);
BOOL NNS_SndArcInit(void *arc, const char *filePath, NNSSndHeapHandle heap, BOOL symbolLoad);
BOOL NNS_SndArcPlayerSetup(NNSSndHeapHandle heap);
void *NNS_SndArcSetCurrent(void *arc);
}

// Loads groups of sound data into the heap, using heap save states so they can be released in reverse order.
class SoundLoader {
public:
    SoundLoader();
    void Init(NNSSndHeapHandle heap);
    int Load(int count, const int *groups);
    void Unload(int level);

    u32 unk0[0x5C / 4];
};

// A sequence player (BGM/jingles). The second one at 0x2A24 has extra state.
class SeqPlayer {
public:
    SeqPlayer();
    ~SeqPlayer();
    int func_02001eb0(int a, int b, int c);
    void func_02001f34(int a, int b, int c);
    void func_02001fa8(int a);
    void func_02002000(int a);
    void func_0200208c(int a);
    void func_02002060(int a);
    void func_020020a4(int a);
    void func_020020cc();
    int func_02002100();
    void Update();

    u32 unk0[0x20 / 4];
};

class SeqPlayerEx : public SeqPlayer {
public:
    SeqPlayerEx();
    int func_02002288(int a, int b, int c, int d);
    void Update();

    u8 unk20[0x188 - 0x20];
};

class SndBase {
public:
    SndBase();
    ~SndBase();
    u32 unk0[0x20 / 4];
};

class Unk18 {
public:
    Unk18();
    u32 unk0[0x18 / 4];
};

class Unk10 {
public:
    Unk10();
    ~Unk10();
    u32 unk0[0x10 / 4];
};

class Unk1A8 {
public:
    Unk1A8();
    u8 unk0;
    u16 unk2;
    Unk10 unk4;
    u16 unk14;
    Unk10 unk18;
    Unk18 unk28[16];
};

class Unk2BCC : public SndBase {
public:
    void Update();
    int func_02002618(int a, int b, int c);
    void func_020026b4(int a);
    int func_02002708(int a, int b, int c);
    int func_02002744(int a, int b, int c, u16 d, int e);
    void func_02002854(int a, int b);
    int func_02002938(int a, int b);
    int func_020029b8();
    int func_020029e8();

    Unk18 unk20;
    Unk18 unk38;
    Unk18 unk50;
    Unk1A8 unk68;
    u8 unk210[4];
};

class Unk2FF4 : public Unk2BCC {
public:
    void func_02002cc0(int a, int b, int c);
    void Update();
    u8 unk214[0x10];
};

// Stream player.
class Unk3218 {
public:
    Unk3218();
    ~Unk3218();
    void func_02002d3c();
    void Update();
    void func_02002dec();
    void func_02002e5c(int a);
    void func_02002e90();
    void func_02002ec4(int a);
    void func_02002f44();
    void func_02002f58();
    void func_02002f6c(int a, int b);
    void func_02002f94();
    u8 unk0[0x14];
    u8 unk14;
    u8 unk15[3];
};

struct SoundCommand {
    u32 type;
    s32 arg0;
    s32 arg1;
    s32 arg2;
};

extern int data_02093060;   // volume for players 2 and 3 when resuming

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
    int func_02000fd4(int a, int b, int c);
    void func_02001060(int a);
    void func_02001080(int a, int b, int c);
    int func_020010a0();
    int func_020010c4(int a, int b, int c);
    int func_02001158(int a, int b, int c, u16 d, int e);
    void func_02001198(int a);
    void func_020011c4(int a, int b, int c);
    void func_0200120c(int a, int b);
    int func_0200122c();
    int func_02001250(int a, int b, int c);
    void func_02001274(int a);
    void func_02001294(int a, int b);
    void func_020012b4(int a, int b, int c);
    void InitStream();
    void func_0200131c(int a, int b, int c);
    void func_02001348(int a);
    void func_0200135c();
    void func_02001370();
    u8 func_02001388();
    void func_02001394(int a, int b, int c);
    void func_020013cc(int a);
    void func_020013f0();
    void func_02001404(int a, int b, int c);
    int func_02001488();
    void SetVolume8(int volume);
    void SetVolumeC(int volume);
    void func_020014dc();
    void Pause();
    void Resume();
    void StopPlayer0(int fadeFrames);
    void StopPlayer1(int fadeFrames);
    void PushCommand(u32 type, s32 arg0, s32 arg1, s32 arg2);

    u8 enabled;
    u8 heapCreated;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    NNSSndHeapHandle heap;              // 0x10
    u8 arc[0x90];                       // 0x14: NNSSndArc
    u8 strmHeapMem[0x2800];             // 0xA4
    s32 numCommands;                    // 0x28A4
    SoundCommand commands[16];          // 0x28A8
    SoundLoader loader;                 // 0x29A8
    SeqPlayer player0;                  // 0x2A04
    SeqPlayerEx player1;                // 0x2A24
    SeqPlayer player2;                  // 0x2BAC
    Unk2BCC unk2BCC;
    Unk2BCC unk2DE0;
    Unk2FF4 unk2FF4;
    Unk3218 strm;                       // 0x3218
    NNSSndHeapHandle strmHeap;          // 0x3230
    u8 strmThreadStack[0x3720 - 0x3234];
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
    strm.Update();
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

int SoundManager::func_02000fd4(int a, int b, int c) {
    int result;

    if (!enabled) {
        return 0;
    }
    result = player2.func_02001eb0(a, b, c);
    if (result && !unk2) {
        unk4 = unk2BCC.func_020029e8();
        unk2BCC.func_02002854(1, 6);
        if (unk3) {
            func_02001348(0);
            unk3 = 0;
        }
        unk2 = 1;
    }
    return result;
}

void SoundManager::func_02001060(int a) {
    if (!enabled) {
        return;
    }
    player2.func_02001fa8(a);
}

void SoundManager::func_02001080(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    player2.func_02001f34(a, b, c);
}

int SoundManager::func_020010a0() {
    if (!enabled) {
        return 0;
    }
    return player2.func_02002100();
}

int SoundManager::func_020010c4(int a, int b, int c) {
    int result;

    if (!enabled) {
        return 0;
    }
    result = unk2BCC.func_02002708(a, b, c);
    if (result) {
        if (unk2 && func_020010a0()) {
            func_02001060(0);
            unk2 = 0;
        }
        if (unk3 && func_02001488()) {
            func_02001348(0);
            unk3 = 0;
        }
    }
    return result;
}

int SoundManager::func_02001158(int a, int b, int c, u16 d, int e) {
    if (!enabled) {
        return 0;
    }
    return unk2BCC.func_02002744(a, b, c, d, e);
}

void SoundManager::func_02001198(int a) {
    if (!enabled) {
        return;
    }
    unk2BCC.func_020026b4(a);
    unk4 = 0x7F;
}

void SoundManager::func_020011c4(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    if (unk2BCC.func_02002938(a, b)) {
        unk2BCC.func_020026b4(c);
        unk4 = 0x7F;
    }
}

void SoundManager::func_0200120c(int a, int b) {
    if (!enabled) {
        return;
    }
    unk2BCC.func_02002854(a, b);
}

int SoundManager::func_0200122c() {
    if (!enabled) {
        return 0;
    }
    return unk2BCC.func_020029b8();
}

int SoundManager::func_02001250(int a, int b, int c) {
    if (!enabled) {
        return 0;
    }
    return unk2DE0.func_02002618(a, b, c);
}

void SoundManager::func_02001274(int a) {
    if (!enabled) {
        return;
    }
    unk2DE0.func_020026b4(a);
}

void SoundManager::func_02001294(int a, int b) {
    if (!enabled) {
        return;
    }
    unk2DE0.func_02002854(a, b);
}

void SoundManager::func_020012b4(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    unk2FF4.func_02002cc0(a, b, c);
}

void SoundManager::InitStream() {
    strmHeap = NNS_SndHeapCreate(strmHeapMem, sizeof(strmHeapMem));
    NNS_SndArcStrmInit(10, strmHeap);
    NNS_SndArcStrmCreatePrepareThread(strmThreadStack, 11);
    strm.func_02002d3c();
}

void SoundManager::func_0200131c(int a, int b, int c) {
    strm.func_02002dec();
    strm.func_02002f6c(unk8, 0);
}

void SoundManager::func_02001348(int a) {
    strm.func_02002e5c(a);
}

void SoundManager::func_0200135c() {
    strm.func_02002e90();
}

void SoundManager::func_02001370() {
    strm.func_02002ec4(unk8);
}

u8 SoundManager::func_02001388() {
    return strm.unk14;
}

void SoundManager::func_02001394(int a, int b, int c) {
    strm.func_02002dec();
    strm.func_02002f6c(unkC, 0);
    strm.func_02002f44();
}

void SoundManager::func_020013cc(int a) {
    strm.func_02002e5c(a);
    strm.func_02002f58();
}

void SoundManager::func_020013f0() {
    strm.func_02002f94();
}

void SoundManager::func_02001404(int a, int b, int c) {
    strm.func_02002dec();
    strm.func_02002f6c(unkC, 0);
    if (!unk3) {
        unk3 = 1;
        unk4 = unk2BCC.func_020029e8();
        unk2BCC.func_02002854(1, 6);
        if (unk2) {
            func_02001060(0);
            unk2 = 0;
        }
    }
}

int SoundManager::func_02001488() {
    if (!unk3) {
        return 0;
    }
    return func_02001388();
}

void SoundManager::SetVolume8(int volume) {
    if (volume < 0) {
        volume = 0;
    } else if (volume > 127) {
        volume = 127;
    }
    unk8 = volume;
}

void SoundManager::SetVolumeC(int volume) {
    if (volume < 0) {
        volume = 0;
    } else if (volume > 127) {
        volume = 127;
    }
    unkC = volume;
}

void SoundManager::func_020014dc() {
}

void SoundManager::Pause() {
    if (!enabled) {
        return;
    }
    NNS_SndPlayerPauseByPlayerNo(0, TRUE);
    NNS_SndPlayerPauseByPlayerNo(1, TRUE);
    NNS_SndPlayerSetPlayerVolume(2, 0);
    NNS_SndPlayerSetPlayerVolume(3, 0);
    NNS_SndPlayerPauseByPlayerNo(4, TRUE);
}

void SoundManager::Resume() {
    if (!enabled) {
        return;
    }
    NNS_SndPlayerPauseByPlayerNo(0, FALSE);
    NNS_SndPlayerPauseByPlayerNo(1, FALSE);
    NNS_SndPlayerSetPlayerVolume(2, data_02093060);
    NNS_SndPlayerSetPlayerVolume(3, data_02093060);
    NNS_SndPlayerPauseByPlayerNo(4, FALSE);
}

void SoundManager::StopPlayer0(int fadeFrames) {
    if (!enabled) {
        return;
    }
    if (fadeFrames < 0) {
        fadeFrames = 0;
    }
    NNS_SndPlayerStopSeqByPlayerNo(0, fadeFrames);
    func_02000f48();
}

void SoundManager::StopPlayer1(int fadeFrames) {
    if (!enabled) {
        return;
    }
    if (fadeFrames < 0) {
        fadeFrames = 0;
    }
    NNS_SndPlayerStopSeqByPlayerNo(1, fadeFrames);
    func_02000f48();
}

void SoundManager::PushCommand(u32 type, s32 arg0, s32 arg1, s32 arg2) {
    SoundCommand *cmd = &commands[numCommands];

    cmd->type = type;
    cmd->arg0 = arg0;
    cmd->arg1 = arg1;
    cmd->arg2 = arg2;
    numCommands++;
}

static SoundManager gSoundManager;

void func_02001624() {
    gSoundManager.Init();
}

void SoundInit(void *heapMem, u32 heapSize, const char *arcPath) {
    gSoundManager.CreateHeap(heapMem, heapSize);
    gSoundManager.LoadArchive(arcPath);
    gSoundManager.InitLoader();
    gSoundManager.InitStream();
    gSoundManager.func_020014dc();
    gSoundManager.enabled = 1;
}

void func_02001694() {
    gSoundManager.Update();
}

int func_020016a8(int a, int b, int c) {
    return gSoundManager.func_02000e64(a, b, c);
}

void func_020016d0(int a, int b, int c) {
    gSoundManager.PushCommand(0, c, a, b);
}

void func_020016f0(int a, int b, int c) {
    gSoundManager.func_02000e88(a, b, c);
}

void func_02001718(int a) {
    gSoundManager.func_02000ec8(a);
}

void func_02001730(int a) {
    gSoundManager.func_02000ee8(a);
}

void func_02001748(int a) {
    gSoundManager.func_02000f08(a);
}

void func_02001760(int a) {
    gSoundManager.func_02000f28(a);
}

void func_02001778() {
    gSoundManager.func_02000f48();
}

int func_0200178c(int a, int b, int c, int d) {
    return gSoundManager.func_02000f68(a, b, c, d);
}

void func_020017c4(int a, int b, int c) {
    gSoundManager.PushCommand(1, c, a, b);
}

int func_020017e4(int a, int b, int c) {
    return gSoundManager.func_02000fd4(a, b, c);
}

void func_0200180c(int a, int b, int c) {
    gSoundManager.PushCommand(3, c, a, b);
}

int func_0200182c(int a, int b, int c) {
    return gSoundManager.func_020010c4(a, b, c);
}

int func_02001854(int a, int b, int c, u16 d, int e) {
    return gSoundManager.func_02001158(a, b, c, d, e);
}

void func_02001894(int a) {
    gSoundManager.PushCommand(2, a, -1, -1);
}

void func_020018b8(int a, int b, int c) {
    gSoundManager.PushCommand(2, c, a, b);
}

void func_020018d8(int a, int b) {
    gSoundManager.func_0200120c(a, b);
}

int func_020018f8() {
    return gSoundManager.func_0200122c();
}

int func_0200190c(int a, int b, int c) {
    return gSoundManager.func_02001250(a, b, c);
}

void func_02001934(int a) {
    gSoundManager.PushCommand(4, a, -1, -1);
}

void func_02001958(int a, int b) {
    gSoundManager.func_02001294(a, b);
}

void func_02001978(int a, int b, int c) {
    gSoundManager.func_020012b4(a, b, c);
}

void func_020019a0(int a, int b, int c) {
    gSoundManager.func_0200131c(a, b, c);
}

void func_020019c8(int a) {
    gSoundManager.func_02001348(a);
}

void func_020019e0() {
    gSoundManager.func_0200135c();
}

void func_020019f4() {
    gSoundManager.func_02001370();
}

void func_02001a08(int a, int b, int c) {
    gSoundManager.func_02001394(a, b, c);
}

void func_02001a30(int a) {
    gSoundManager.func_020013cc(a);
}

void func_02001a48() {
    gSoundManager.func_020013f0();
}

void func_02001a5c(int a, int b, int c) {
    gSoundManager.func_02001404(a, b, c);
}

void func_02001a84(int volume) {
    gSoundManager.SetVolume8(volume);
}

void func_02001a9c(int volume) {
    gSoundManager.SetVolumeC(volume);
}

void SoundPause() {
    gSoundManager.Pause();
}

void SoundResume() {
    gSoundManager.Resume();
}

void func_02001adc(int fadeFrames) {
    gSoundManager.StopPlayer0(fadeFrames);
}

void func_02001af4(int fadeFrames) {
    gSoundManager.StopPlayer1(fadeFrames);
}

// Loads the sound groups given as arguments, terminated by -1.
void SoundLoadGroups(int group, ...) {
    int groups[128];
    int count;
    int next;
    va_list args;

    groups[0] = group;
    count = 1;
    va_start(args, group);
    for (;;) {
        next = va_arg(args, int);
        if (next == -1) {
            break;
        }
        groups[count++] = next;
    }
    va_end(args);
    gSoundManager.LoadGroups(count, groups);
}

void SoundUnload(int level) {
    gSoundManager.Unload(level);
}
