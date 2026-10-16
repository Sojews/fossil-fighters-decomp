#ifndef SOUND_H
#define SOUND_H

// Sound system classes (ARM code, see src/sound/).

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

typedef struct NNSSndHandle {
    void *player;
} NNSSndHandle;

// A sequence player (BGM/jingles): one sound handle plus volume/pitch/tempo/pan to apply on start.
class SeqPlayer {
public:
    SeqPlayer();
    ~SeqPlayer();
    void ClearIds();
    void Update();
    BOOL StartSeq(int seqNo, int fadeFrames);
    BOOL StartSeqArc(int seqArcNo, int index, int fadeFrames);
    BOOL Play(int no, int index, int fadeFrames);
    void Stop(int no, int index, int fadeFrames);
    void StopCurrent(int fadeFrames);
    void SetVolume(int volume);
    void SetPitch(int pitch);
    void SetTempo(int tempo);
    void SetPan(int pan);
    void ResetParams();
    BOOL IsPlaying();
    void GetPlaying(int *no, int *index);
    int GetKind(int no, int index);

    NNSSndHandle handle;
    s32 seqNo;              // sequence number, or sequence archive number
    s32 index;              // index in the sequence archive, or -1 for a plain sequence
    s32 tempo;
    s32 pitch;
    s32 volume;
    s32 pan;
    u8 tempoSet;
    u8 pitchSet;
    u8 volumeSet;
    u8 panSet;
};

// A list entry: a sequence that was played recently and how many more frames it blocks replays.
struct SeqEntry {
    s32 no;
    s32 index;
    s32 timer;
    s32 prev;
    s32 next;
};

#define SEQ_ENTRY_HEAD 0
#define SEQ_ENTRY_TAIL 1
#define SEQ_ENTRY_COUNT 18

// A sequence player that won't restart a sequence while it's still in its recently-played list.
class SeqPlayerEx {
public:
    SeqPlayerEx();
    void Update();
    BOOL PlayOnce(int no, int index, int duration, int fadeFrames);
    void InitList();
    int FindFreeEntry();
    void InsertBefore(int at, int entry);
    void RemoveEntry(int entry);
    void InitEntry(int entry);
    int FindEntry(int no, int index);
    BOOL Contains(int no, int index);
    BOOL AddEntry(int no, int index, int duration);

    SeqPlayer player;
    SeqEntry entries[SEQ_ENTRY_COUNT];
};

// Moves a value towards a target over a number of frames (used for volume/pitch fades).
class Unk18 {
public:
    Unk18();
    void Update();
    void Start(int a, int b, int c);
    void Reset();

    u8 done;
    u8 unk1[0xB];
    s32 value;
    u32 unk10[2];
};

class Unk10 {
public:
    Unk10();
    ~Unk10();
    u32 unk0[0x10 / 4];
};

// Per-track state for the 16 sequence tracks.
class Unk1A8 {
public:
    Unk1A8();
    void Update();
    void Start(int trackMask);
    void func_02002ba0(u16 trackMask, int frames, int mute);
    void Reset();
    int func_02002c90(int track);
    BOOL IsActive(int track);
    int func_02002cb0(int track);
    u8 unk0;
    u16 unk2;
    Unk10 unk4;
    u16 unk14;
    Unk10 unk18;
    Unk18 unk28[16];
};

// Background-music player: a SeqPlayer with volume/pitch fades and per-track volume control.
class Unk2BCC : public SeqPlayer {
public:
    void Update();
    void UpdateVolume();
    void UpdatePitch();
    BOOL Play(int no, int index, int fadeFrames);
    void Stop(int fadeFrames);
    BOOL PlayIfNew(int no, int index, int fadeFrames);
    BOOL PlayTracks(int no, int index, int fadeFrames, u16 trackMask, int trackFadeFrames);
    void SetTracks(u16 trackMask, int frames, int mute);
    void ApplyVolume(int volume);
    void SetVolumeFade(int volume, int frames);
    void SetTrackVolume(int volume, u16 trackMask);
    void SetTrackPitch(int pitch, u16 trackMask);
    BOOL IsCurrent(int no, int index);
    BOOL IsPlaying();
    BOOL IsPlayingCurrent(int no, int index);
    int GetVolume();

    Unk18 volumeFade;   // 0x20
    Unk18 pitchFade;    // 0x38
    Unk18 unk50;
    Unk1A8 tracks;      // 0x68
    u16 trackMask;      // 0x210
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

#endif
