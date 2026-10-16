// Sound manager: owns the sound heap and archive, and queues sound commands for the main loop.
// Built as ARM code.

#pragma thumb off

#include <stdarg.h>
#include "sound.h"

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
    return player0.Play(a, b, c);
}

void SoundManager::func_02000e88(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    player0.Stop(a, b, c);
}

void SoundManager::func_02000ea8(int a) {
    if (!enabled) {
        return;
    }
    player0.StopCurrent(a);
}

void SoundManager::func_02000ec8(int a) {
    if (!enabled) {
        return;
    }
    player0.SetVolume(a);
}

void SoundManager::func_02000ee8(int a) {
    if (!enabled) {
        return;
    }
    player0.SetTempo(a);
}

void SoundManager::func_02000f08(int a) {
    if (!enabled) {
        return;
    }
    player0.SetPitch(a);
}

void SoundManager::func_02000f28(int a) {
    if (!enabled) {
        return;
    }
    player0.SetPan(a);
}

void SoundManager::func_02000f48() {
    if (!enabled) {
        return;
    }
    player0.ResetParams();
}

int SoundManager::func_02000f68(int a, int b, int c, int d) {
    if (!enabled) {
        return 0;
    }
    return player1.PlayOnce(a, b, c, d);
}

void SoundManager::func_02000f94(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    player1.player.Stop(a, b, c);
}

void SoundManager::func_02000fb4(int a) {
    if (!enabled) {
        return;
    }
    player1.player.StopCurrent(a);
}

int SoundManager::func_02000fd4(int a, int b, int c) {
    int result;

    if (!enabled) {
        return 0;
    }
    result = player2.Play(a, b, c);
    if (result && !unk2) {
        unk4 = unk2BCC.GetVolume();
        unk2BCC.SetVolumeFade(1, 6);
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
    player2.StopCurrent(a);
}

void SoundManager::func_02001080(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    player2.Stop(a, b, c);
}

int SoundManager::func_020010a0() {
    if (!enabled) {
        return 0;
    }
    return player2.IsPlaying();
}

int SoundManager::func_020010c4(int a, int b, int c) {
    int result;

    if (!enabled) {
        return 0;
    }
    result = unk2BCC.PlayIfNew(a, b, c);
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
    return unk2BCC.PlayTracks(a, b, c, d, e);
}

void SoundManager::func_02001198(int a) {
    if (!enabled) {
        return;
    }
    unk2BCC.Stop(a);
    unk4 = 0x7F;
}

void SoundManager::func_020011c4(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    if (unk2BCC.IsCurrent(a, b)) {
        unk2BCC.Stop(c);
        unk4 = 0x7F;
    }
}

void SoundManager::func_0200120c(int a, int b) {
    if (!enabled) {
        return;
    }
    unk2BCC.SetVolumeFade(a, b);
}

int SoundManager::func_0200122c() {
    if (!enabled) {
        return 0;
    }
    return unk2BCC.IsPlaying();
}

int SoundManager::func_02001250(int a, int b, int c) {
    if (!enabled) {
        return 0;
    }
    return unk2DE0.Play(a, b, c);
}

void SoundManager::func_02001274(int a) {
    if (!enabled) {
        return;
    }
    unk2DE0.Stop(a);
}

void SoundManager::func_02001294(int a, int b) {
    if (!enabled) {
        return;
    }
    unk2DE0.SetVolumeFade(a, b);
}

void SoundManager::func_020012b4(int a, int b, int c) {
    if (!enabled) {
        return;
    }
    unk2FF4.Request(a, b, c);
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
        unk4 = unk2BCC.GetVolume();
        unk2BCC.SetVolumeFade(1, 6);
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
