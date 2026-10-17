// NitroSDK SND: banks (instrument tables) and the wave archives they link to.

#include "nitro/mi.h"
#include "nitro/snd.h"

#define SND_BANK_HEADER_SIZE 0x3c
#define SND_WAVE_ARC_HEADER_SIZE 0x3c

enum {
    SND_INST_INVALID,
    SND_INST_PCM,
    SND_INST_PSG,
    SND_INST_NOISE,
    SND_INST_DIRECTPCM,
    SND_INST_NULL,
    SND_INST_DRUM_SET = 0x10,
    SND_INST_KEY_SPLIT
};

typedef struct {
    u8 min;
    u8 max;
    SNDInstData instOffset[1];
} SNDDrumSet;

typedef struct {
    u8 key[8];
    SNDInstData instOffset[8];
} SNDKeySplit;

void SND_AssignWaveArc(SNDBankData *bank, int index, SNDWaveArc *waveArc) {
    SNDWaveArcLink *top;
    SNDi_LockMutex();
    if (bank->waveArcLink[index].waveArc != NULL) {
        if (waveArc == bank->waveArcLink[index].waveArc) {
            SNDi_UnlockMutex();
            return;
        }
        if (&bank->waveArcLink[index] == bank->waveArcLink[index].waveArc->topLink) {
            bank->waveArcLink[index].waveArc->topLink = bank->waveArcLink[index].next;
            DC_StoreRange(bank->waveArcLink[index].waveArc, SND_WAVE_ARC_HEADER_SIZE);
        } else {
            SNDWaveArcLink *link;
            for (link = bank->waveArcLink[index].waveArc->topLink; link != NULL; link = link->next) {
                if (&bank->waveArcLink[index] == link->next) {
                    break;
                }
            }
            link->next = bank->waveArcLink[index].next;
            DC_StoreRange(link, sizeof(SNDWaveArcLink));
        }
    }
    top = waveArc->topLink;
    waveArc->topLink = &bank->waveArcLink[index];
    bank->waveArcLink[index].next = top;
    bank->waveArcLink[index].waveArc = waveArc;
    SNDi_UnlockMutex();
    DC_StoreRange(bank, SND_BANK_HEADER_SIZE);
    DC_StoreRange(waveArc, SND_WAVE_ARC_HEADER_SIZE);
}

void SND_DestroyBank(SNDBankData *bank) {
    int i;
    SNDi_LockMutex();
    for (i = 0; i < 4; i++) {
        SNDWaveArc *waveArc = bank->waveArcLink[i].waveArc;
        SNDWaveArcLink *target = &bank->waveArcLink[i];
        if (waveArc == NULL) {
            continue;
        }
        if (target == waveArc->topLink) {
            waveArc->topLink = bank->waveArcLink[i].next;
            DC_StoreRange(waveArc, SND_WAVE_ARC_HEADER_SIZE);
        } else {
            SNDWaveArcLink *link;
            for (link = waveArc->topLink; link != NULL; link = link->next) {
                if (target == link->next) {
                    break;
                }
            }
            link->next = bank->waveArcLink[i].next;
            DC_StoreRange(link, sizeof(SNDWaveArcLink));
        }
    }
    SNDi_UnlockMutex();
}

void SND_DestroyWaveArc(SNDWaveArc *waveArc) {
    SNDWaveArcLink *link;
    SNDi_LockMutex();
    link = waveArc->topLink;
    while (link != NULL) {
        SNDWaveArcLink *next = link->next;
        link->waveArc = NULL;
        link->next = NULL;
        DC_StoreRange(link, sizeof(SNDWaveArcLink));
        link = next;
    }
    SNDi_UnlockMutex();
}

SNDInstPos SND_GetFirstInstDataPos(const SNDBankData *bank) {
    SNDInstPos pos;
    pos.prgNo = 0;
    pos.index = 0;
    return pos;
}

BOOL SND_GetNextInstData(const SNDBankData *bank, SNDInstData *inst, SNDInstPos *pos) {
    for (; pos->prgNo < bank->instCount; pos->prgNo++, pos->index = 0) {
        u32 instOffset = bank->instOffset[pos->prgNo];
        u32 offset;
        inst->type = (u8) instOffset;
        offset = instOffset >> 8;
        switch (inst->type) {
        case SND_INST_PCM:
        case SND_INST_PSG:
        case SND_INST_NOISE:
        case SND_INST_DIRECTPCM:
        case SND_INST_NULL:
            inst->param = *(const SNDInstParam *) ((const u8 *) bank + offset);
            pos->prgNo++;
            return TRUE;
        case SND_INST_DRUM_SET: {
            const SNDDrumSet *drumSet = (const SNDDrumSet *) ((const u8 *) bank + offset);
            while (pos->index < drumSet->max - drumSet->min + 1) {
                *inst = drumSet->instOffset[pos->index];
                pos->index++;
                return TRUE;
            }
            break;
        }
        case SND_INST_KEY_SPLIT: {
            const SNDKeySplit *keySplit = (const SNDKeySplit *) ((const u8 *) bank + offset);
            while (pos->index < 8) {
                if (keySplit->key[pos->index] == 0) {
                    break;
                }
                *inst = keySplit->instOffset[pos->index];
                pos->index++;
                return TRUE;
            }
            break;
        }
        }
    }
    return FALSE;
}

u32 SND_GetWaveDataCount(const SNDWaveArc *waveArc) {
    return waveArc->waveCount;
}

void SND_SetWaveDataAddress(SNDWaveArc *waveArc, int index, const SNDWaveData *waveData) {
    SNDi_LockMutex();
    waveArc->waveOffset[index] = (u32) waveData;
    DC_StoreRange(&waveArc->waveOffset[index], sizeof(u32));
    SNDi_UnlockMutex();
}

const SNDWaveData *SND_GetWaveDataAddress(const SNDWaveArc *waveArc, int index) {
    u32 offset;
    const SNDWaveData *waveData;
    SNDi_LockMutex();
    offset = waveArc->waveOffset[index];
    if (offset != 0) {
        if (offset < 0x2000000) {
            waveData = (const SNDWaveData *) ((u32) waveArc + offset);
        } else {
            waveData = (const SNDWaveData *) offset;
        }
    } else {
        waveData = NULL;
    }
    SNDi_UnlockMutex();
    return waveData;
}
