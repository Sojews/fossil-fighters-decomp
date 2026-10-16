// Sound API: plays sounds by ID from the sound table, with delayed requests, volume settings
// and two scripted fade sequences. Built as ARM code.

#pragma thumb off

#include "sound.h"

extern "C" {
void _s32_div_f(void);   // runtime division, used by the asm below
void NNS_SndPlayerSetPlayerVolume(int playerNo, int volume);
void func_0206e2d4(int a, s16 b);
}

// An entry of the sound table. What the parameters mean depends on the type.
struct SoundEntry {
    u16 unk0;
    s16 no;
    s16 index;
    u8 type;
    u8 unk7;
    s32 volume;
    s32 delay;
    s16 param10;
    u16 param12;
    s16 param14;
};

struct SoundTimeline {
    s32 state;
    s32 timer;
};

extern SoundEntry data_0209fae4[];

SoundSettings gSoundSettings = {127, 127, {127, 0, 0x100}};

static SoundTimeline sTimeline;
static SoundParams sNextParams = gSoundSettings.defaults;

void func_020032b8();
void func_020032c4(void *heapMem, u32 heapSize, const char *arcPath);
void func_020032d8();
void func_020033a8();
void func_02003554(int id, int fadeFrames, int delay);
void func_0200363c(int id, int fadeFrames, int delay);
void func_02003688(int type, int id, int a, int b, int delay);
void func_020037d0(int scene, int delay);
void func_020037f8(int id, int fadeFrames, int unused);
void func_02003b40(int id, int fadeFrames);
void func_02003cb4(int id, int arg);
void func_02003cf0(int a);
void func_02003cfc(int a);
void func_02003d08(int a);
void func_02003d14();
void func_02003d20(int volume);
void func_02003d7c(int volume);
void func_02003db8();
void func_02003dc8();
void func_02003dd8(int fadeFrames);
void func_02003de4(int fadeFrames);
void func_02003e08(int mode, int id, int fadeFrames, int value);
void func_02003f10(int scene);
BOOL func_02003f34(int id);
SoundEntry *func_02003f5c(s16 id);

void func_020031d0() {
    func_020032b8();
}

void func_020031dc(void *heapMem, u32 heapSize, const char *arcPath) {
    func_020032c4(heapMem, heapSize, arcPath);
}

void func_020031e8() {
    func_020032d8();
}

void func_020031f4() {
    func_020033a8();
}

void func_02003200(int id, int fadeFrames, int delay) {
    func_02003554(id, fadeFrames, delay);
}

void func_0200320c(int id, int fadeFrames) {
    func_0200363c(id, fadeFrames, 0);
}

void func_0200321c(int id, int arg) {
    func_02003cb4(id, arg);
}

void func_02003228(int volume) {
    func_02003d20(volume);
}

void func_02003234(int volume) {
    func_02003d7c(volume);
}

void func_02003240() {
    func_02003db8();
}

void func_0200324c() {
    func_02003dc8();
}

void func_02003258(int fadeFrames) {
    func_02003dd8(fadeFrames);
}

void func_02003264(int fadeFrames) {
    func_02003de4(fadeFrames);
}

void func_02003270(int type, int id, int a, int b) {
    func_02003688(type, id, a, b, 0);
}

void func_02003284(int scene) {
    func_020037d0(scene, 0);
}

BOOL func_02003294(int id) {
    return func_02003f5c(id)->unk7 != 1;
}

void func_020032b8() {
    func_02001624();
}

void func_020032c4(void *heapMem, u32 heapSize, const char *arcPath) {
    SoundInit(heapMem, heapSize, arcPath);
    func_02003f10(15);
}

// Runs the queued requests whose delay has run out.
void func_020032d8() {
    BeginSoundRequests();
    while (!AtSoundRequestsEnd()) {
        TickSoundRequest();
        if (GetSoundRequestDelay() == 0) {
            SoundRequest *req = GetCurrentSoundRequest();
            switch (req->type) {
            case 0:
                func_020037f8(req->args[0], req->args[1], 0);
                break;
            case 1:
                func_02003b40(req->args[0], req->args[1]);
                break;
            case 2:
                func_02003e08(req->args[0], req->args[1], req->args[2], req->args[3]);
                break;
            case 3:
                func_02003f10(req->args[0]);
                break;
            case 4:
                func_02003cf0(req->args[0]);
                break;
            case 5:
                func_02003cfc(req->args[0]);
                break;
            case 6:
                func_02003d08(req->args[0]);
                break;
            }
            RemoveSoundRequest(GetSoundRequestCursor());
        } else {
            NextSoundRequest();
        }
    }
}

// Per-frame update, including the two scripted sequences started by sounds 0x7B and 0x459.
void func_020033a8() {
    func_02001694();
    if (sTimeline.state == 1) {
        switch (++sTimeline.timer) {
        case 0x5ACE:
            func_020019c8(150);
            break;
        case 0x5ACE + 60:
            func_02003554(0x14C, 150, 0);
            break;
        case 0x6A26:
            func_0200363c(0x14C, 240, 0);
            break;
        case 0x6A26 + 60:
            func_02003554(0x512, 240, 0);
            break;
        case 0x80E8:
            sTimeline.state = 0;
            sTimeline.timer = 0;
            break;
        }
    } else if (sTimeline.state == 2) {
        switch (++sTimeline.timer) {
        case 0x5B00:
            func_020019c8(150);
            break;
        case 0x5B1E:
            func_02003554(0x14B, 150, 0);
            break;
        case 0x697C:
            func_0200363c(0x14B, 240, 0);
            break;
        case 0x697C + 60:
            func_02003554(0x512, 240, 0);
            break;
        case 0x80E8:
            sTimeline.state = 0;
            sTimeline.timer = 0;
            break;
        }
    }
}

// Queues a sound. Type 4 entries repeat param10 times, param12 frames apart, getting quieter each time.
void func_02003554(int id, int fadeFrames, int delay) {
    SoundEntry *entry = func_02003f5c(id);
    int i;

    if (entry->type == 4) {
        for (i = 0; i < entry->param10 + 1; i++) {
            int base = entry->delay + delay;
            int step = entry->param12 * i;
            PushSoundRequest(4, base + step, entry->volume * (entry->param10 + 1 - i) / (entry->param10 + 1), 0, 0, 0);
            PushSoundRequest(0, base + step, id, fadeFrames, 0, 0);
        }
    } else {
        PushSoundRequest(0, entry->delay + delay, id, fadeFrames, 0, 0);
    }
}

void func_0200363c(int id, int fadeFrames, int delay) {
    SoundEntry *entry = func_02003f5c(id);

    PushSoundRequest(1, entry->delay + delay, id, fadeFrames, 0, 0);
}

#ifdef NONMATCHING
// Equivalent, but register allocation differs from the original.
void func_02003688(int type, int id, int a, int b, int delay) {
    int count;
    int i;
    int offset;
    int volume;

    switch (type) {
    case 0:
    case 1:
    case 2:
        PushSoundRequest(2, func_02003f5c(id)->delay + delay, type, id, a, b);
        break;
    case 3:
        PushSoundRequest(2, delay, type, id, a, b);
        break;
    case 4:
        count = a + 1;
        for (i = 0, offset = 0; i < count; i++, offset += b) {
            volume = (count - i) * 127 / count;
            if (i != 0) {
                PushSoundRequest(2, delay + offset, 2, id, 0, 0);
            }
            PushSoundRequest(2, delay + offset + 1, 3, volume, 0, 0x100);
            PushSoundRequest(2, delay + offset + 1, 0, id, 0, 0);
        }
        break;
    }
}
#else
asm void func_02003688(int type, int id, int a, int b, int delay) {
    nofralloc
    stmdb sp!, {r3, r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x8
    mov r4, r0
    ldr r5, [sp, #0x30]
    mov r7, r1
    mov r8, r2
    mov r6, r3
    cmp r4, #0x4
    addls pc, pc, r4, lsl #0x2
    b L_020037c8
L_020036b0:
    b L_020036c4
    b L_020036c4
    b L_020036c4
    b L_020036f8
    b L_0200371c
L_020036c4:
    mov r0, r7, lsl #0x10
    mov r0, r0, asr #0x10
    bl func_02003f5c
    str r8, [sp, #0x0]
    str r6, [sp, #0x4]
    ldr r0, [r0, #0xc]
    mov r2, r4
    add r1, r0, r5
    mov r3, r7
    mov r0, #0x2
    bl PushSoundRequest
    add sp, sp, #0x8
    ldmia sp!, {r3, r4, r5, r6, r7, r8, r9, r10, r11, pc}
L_020036f8:
    str r8, [sp, #0x0]
    mov r1, r5
    mov r2, r4
    mov r3, r7
    mov r0, #0x2
    str r6, [sp, #0x4]
    bl PushSoundRequest
    add sp, sp, #0x8
    ldmia sp!, {r3, r4, r5, r6, r7, r8, r9, r10, r11, pc}
L_0200371c:
    add r9, r8, #0x1
    cmp r9, #0x0
    mov r10, #0x0
    addle sp, sp, #0x8
    ldmleia sp!, {r3, r4, r5, r6, r7, r8, r9, r10, r11, pc}
    mov r4, r10
    mov r11, r10
L_02003738:
    sub r0, r9, r10
    rsb r0, r0, r0, lsl #0x7
    mov r1, r9
    bl _s32_div_f
    mov r8, r0
    cmp r10, #0x0
    beq L_02003770
    mov r0, #0x2
    str r11, [sp, #0x0]
    add r1, r5, r4
    mov r2, r0
    mov r3, r7
    str r11, [sp, #0x4]
    bl PushSoundRequest
L_02003770:
    mov r0, #0x0
    mov r3, r8
    str r0, [sp, #0x0]
    mov r0, #0x100
    add r8, r5, r4
    str r0, [sp, #0x4]
    mov r0, #0x2
    add r1, r8, #0x1
    mov r2, #0x3
    bl PushSoundRequest
    mov r0, #0x0
    str r0, [sp, #0x0]
    str r0, [sp, #0x4]
    add r1, r8, #0x1
    mov r0, #0x2
    mov r2, #0x0
    mov r3, r7
    bl PushSoundRequest
    add r10, r10, #0x1
    add r4, r4, r6
    cmp r10, r9
    blt L_02003738
L_020037c8:
    add sp, sp, #0x8
    ldmia sp!, {r3, r4, r5, r6, r7, r8, r9, r10, r11, pc}
}
#endif

void func_020037d0(int scene, int delay) {
    PushSoundRequest(3, delay, scene, 0, 0, 0);
}

void func_020037f8(int id, int fadeFrames, int unused) {
    SoundEntry *entry = func_02003f5c(id);

    func_02003294(id);
    if (entry == NULL) {
        return;
    }
    switch (entry->type) {
    case 2:
        func_020016f0(entry->no, entry->index, fadeFrames);
        // fallthrough
    case 0:
        func_02001718(entry->volume);
        func_020016a8(entry->no, entry->index, fadeFrames);
        break;
    case 1:
        func_02001718(entry->volume);
        func_02001730(entry->param10);
        func_02001748(entry->param12);
        func_02001760(entry->param14);
        func_020016a8(entry->no, entry->index, fadeFrames);
        break;
    case 3:
        func_02001718(entry->volume);
        func_0200178c(entry->no, entry->index, entry->param10, fadeFrames);
        break;
    case 4:
        func_020016f0(entry->no, entry->index, fadeFrames);
        func_02001718(entry->volume);
        func_020016a8(entry->no, entry->index, fadeFrames);
        break;
    case 5:
        if (func_02001a48()) {
            func_02001a30(0);
        }
        if (!func_0200182c(entry->no, entry->index, fadeFrames) && entry->param10 > 0) {
            SoundUnload(entry->param10);
            func_0200182c(entry->no, entry->index, fadeFrames);
        }
        func_020018d8(entry->volume, 0);
        break;
    case 6:
        if (func_02001a48()) {
            func_02001a30(0);
        }
        if (!func_02001854(entry->no, entry->index, fadeFrames, entry->param12, entry->param14) &&
            entry->param10 > 0) {
            SoundUnload(entry->param10);
            func_02001854(entry->no, entry->index, fadeFrames, entry->param12, entry->param14);
        }
        func_020018d8(entry->volume, 0);
        break;
    case 7:
        func_020017e4(entry->no, entry->index, fadeFrames);
        break;
    case 8:
        func_0200190c(entry->no, entry->index, fadeFrames);
        func_02001958(entry->volume, 0);
        break;
    case 10:
        func_020019a0(entry->no, 0, fadeFrames);
        break;
    case 11:
        if (func_020018f8()) {
            func_02001894(0);
        }
        func_02001a08(entry->no, 0, fadeFrames);
        break;
    case 12:
        func_02001a5c(entry->no, 0, fadeFrames);
        break;
    case 13:
        if (id == 0x7B) {
            sTimeline.timer = 0;
            sTimeline.state = 1;
            if (func_020018f8()) {
                func_02001894(0);
            }
            func_020019a0(entry->no, 0, fadeFrames);
        } else if (id == 0x459) {
            sTimeline.timer = 0;
            sTimeline.state = 2;
            if (func_020018f8()) {
                func_02001894(0);
            }
            func_020019a0(entry->no, 0, fadeFrames);
        }
        break;
    }
}

void func_02003b40(int id, int fadeFrames) {
    SoundEntry *entry = func_02003f5c(id);

    if (entry == NULL) {
        return;
    }
    switch (entry->type) {
    case 0:
    case 1:
    case 2:
        func_020016d0(entry->no, entry->index, fadeFrames);
        break;
    case 3:
        func_020017c4(entry->no, entry->index, fadeFrames);
        break;
    case 4:
        break;
    case 5:
        func_020018b8(entry->no, entry->index, fadeFrames);
        break;
    case 6:
        func_020018b8(entry->no, entry->index, fadeFrames);
        break;
    case 7:
        func_0200180c(entry->no, entry->index, fadeFrames);
        break;
    case 8:
        func_02001934(fadeFrames);
        break;
    case 9:
        break;
    case 10:
    case 11:
    case 12:
        func_020019c8(fadeFrames);
        break;
    case 13:
        if (id == 0x7B) {
            sTimeline.timer = 0;
            sTimeline.state = 0;
            entry = func_02003f5c(0x14C);
            func_020018b8(entry->no, entry->index, fadeFrames);
            func_020019c8(fadeFrames);
        } else if (id == 0x459) {
            sTimeline.timer = 0;
            sTimeline.state = 0;
            entry = func_02003f5c(0x14B);
            func_020018b8(entry->no, entry->index, fadeFrames);
            func_020019c8(fadeFrames);
        }
        break;
    }
}

void func_02003cb4(int id, int arg) {
    SoundEntry *entry = func_02003f5c(id);

    if (entry == NULL) {
        return;
    }
    if (entry->type != 9) {
        return;
    }
    func_02001978(entry->no, entry->index, arg);
}

void func_02003cf0(int a) {
    func_02001718(a);
}

void func_02003cfc(int a) {
    func_02001730(a);
}

void func_02003d08(int a) {
    func_02001748(a);
}

void func_02003d14() {
    func_02001778();
}

void func_02003d20(int volume) {
    if (volume < 0) {
        volume = 0;
    } else if (volume > 127) {
        volume = 127;
    }
    gSoundSettings.bgmVolume = volume;
    NNS_SndPlayerSetPlayerVolume(0, volume);
    NNS_SndPlayerSetPlayerVolume(2, gSoundSettings.bgmVolume);
    NNS_SndPlayerSetPlayerVolume(3, gSoundSettings.bgmVolume);
    func_02001a84(gSoundSettings.bgmVolume);
}

void func_02003d7c(int volume) {
    if (volume < 0) {
        volume = 0;
    } else if (volume > 127) {
        volume = 127;
    }
    gSoundSettings.seVolume = volume;
    NNS_SndPlayerSetPlayerVolume(1, volume);
    func_02001a9c(gSoundSettings.seVolume);
}

void func_02003db8() {
    SoundPause();
    func_020019e0();
}

void func_02003dc8() {
    func_020019f4();
    SoundResume();
}

void func_02003dd8(int fadeFrames) {
    func_02001adc(fadeFrames);
}

void func_02003de4(int fadeFrames) {
    func_02001af4(fadeFrames);
    if (func_02001a48()) {
        func_02001a30(fadeFrames);
    }
}

// Mode 3 sets the parameters for the next play; modes 0 and 1 play with them (1 overrides the volume).
#ifdef NONMATCHING
// Equivalent, but the original copies the default parameters in a different load/store order.
void func_02003e08(int mode, int id, int fadeFrames, int value) {
    SoundEntry *entry = func_02003f5c(id);
    SoundParams *params = &sNextParams;

    if (entry == NULL) {
        return;
    }
    switch (mode) {
    case 0:
        func_02001718(params->volume);
        func_02001748(params->pitch);
        func_02001730(params->tempo);
        func_020016a8(entry->no, entry->index, fadeFrames);
        params->volume = gSoundSettings.defaults.volume;
        params->pitch = gSoundSettings.defaults.pitch;
        params->tempo = gSoundSettings.defaults.tempo;
        func_02003d14();
        break;
    case 1:
        func_02001718(value);
        func_02001748(params->pitch);
        func_02001730(params->tempo);
        func_020016a8(entry->no, entry->index, fadeFrames);
        params->volume = gSoundSettings.defaults.volume;
        params->pitch = gSoundSettings.defaults.pitch;
        params->tempo = gSoundSettings.defaults.tempo;
        func_02003d14();
        break;
    case 2:
        func_020016d0(entry->no, entry->index, fadeFrames);
        break;
    case 3:
        params->volume = id;
        params->pitch = fadeFrames;
        params->tempo = value;
        break;
    }
}
#else
asm void func_02003e08(int mode, int id, int fadeFrames, int value) {
    nofralloc
    stmdb sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
    mov r5, r1
    mov r1, r5, lsl #0x10
    mov r6, r0
    mov r0, r1, asr #0x10
    mov r9, r2
    mov r8, r3
    bl func_02003f5c
    ldr r4, =sNextParams
    movs r7, r0
    ldmeqia sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
    cmp r6, #0x3
    addls pc, pc, r6, lsl #0x2
    ldmia sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
L_02003e40:
    b L_02003e50
    b L_02003e9c
    b L_02003ee8
    b L_02003efc
L_02003e50:
    ldr r0, [r4, #0x0]
    bl func_02001718
    ldr r0, [r4, #0x4]
    bl func_02001748
    ldr r0, [r4, #0x8]
    bl func_02001730
    ldrsh r0, [r7, #0x2]
    ldrsh r1, [r7, #0x4]
    mov r2, r9
    bl func_020016a8
    ldr r0, =gSoundSettings
    ldr r2, [r0, #0x8]
    ldr r1, [r0, #0xc]
    str r2, [r4, #0x0]
    str r1, [r4, #0x4]
    ldr r0, [r0, #0x10]
    str r0, [r4, #0x8]
    bl func_02003d14
    ldmia sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
L_02003e9c:
    mov r0, r8
    bl func_02001718
    ldr r0, [r4, #0x4]
    bl func_02001748
    ldr r0, [r4, #0x8]
    bl func_02001730
    ldrsh r0, [r7, #0x2]
    ldrsh r1, [r7, #0x4]
    mov r2, r9
    bl func_020016a8
    ldr r0, =gSoundSettings
    ldr r2, [r0, #0x8]
    ldr r1, [r0, #0xc]
    str r2, [r4, #0x0]
    str r1, [r4, #0x4]
    ldr r0, [r0, #0x10]
    str r0, [r4, #0x8]
    bl func_02003d14
    ldmia sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
L_02003ee8:
    ldrsh r0, [r7, #0x2]
    ldrsh r1, [r7, #0x4]
    mov r2, r9
    bl func_020016d0
    ldmia sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
L_02003efc:
    stmia r4, {r5, r9}
    str r8, [r4, #0x8]
    ldmia sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
}
#endif

void func_02003f10(int scene) {
    func_0206e2d4(1, scene);
    SetSoundScene(scene);
}

BOOL func_02003f34(int id) {
    if (id < 0 || id >= 0x572) {
        return FALSE;
    }
    return TRUE;
}

SoundEntry *func_02003f5c(s16 id) {
    if (!func_02003f34(id)) {
        return NULL;
    }
    return &data_0209fae4[id];
}
