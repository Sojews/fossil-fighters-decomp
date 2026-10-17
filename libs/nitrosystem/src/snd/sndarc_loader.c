// NitroSystem sound: loading sequences, banks and wave archives from the sound archive into a sound heap.

#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/snd.h"
#include "nnsys/snd/sndarc.h"
#include "nnsys/snd/sndarc_loader.h"

#define WAVEARC_HEADER_SIZE 0x3c

enum {
    GROUP_ITEM_SEQ,
    GROUP_ITEM_BANK,
    GROUP_ITEM_WAVEARC,
    GROUP_ITEM_SEQARC
};

static SNDWaveArc sWaveArcHeader;   // Only the header (WAVEARC_HEADER_SIZE bytes) is read into it

static void *LoadSeq(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr);
static void *LoadSeqArc(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr);
static void *LoadBank(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr);
static void *LoadWaveArc(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr);
static void *LoadWaveArcTable(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr);
static void DisposeCallback(void *mem, NNSSndArc *arc, u32 fileId);
static void SeqDisposeCallback(void *mem, u32 size, u32 data1, u32 data2);
static void BankDisposeCallback(void *mem, u32 size, u32 data1, u32 data2);
static void WaveArcDisposeCallback(void *mem, u32 size, u32 data1, u32 data2);
static void WaveArcTableDisposeCallback(void *mem, u32 size, u32 data1, u32 data2);
static void SingleWaveDisposeCallback(void *mem, u32 size, u32 data1, u32 data2);
static BOOL LoadSingleWave(SNDWaveArc *waveArc, u32 index, u32 fileId, NNSSndHeapHandle heap);
static BOOL LoadSingleWaves(SNDWaveArc *waveArc, SNDBankData *bank, int waveArcIndex, u32 fileId,
                            NNSSndHeapHandle heap);

BOOL NNS_SndArcLoadGroup(int groupNo, NNSSndHeapHandle heap) {
    return NNSi_SndArcLoadGroup(groupNo, heap) == NNS_SND_ARC_LOAD_SUCCESS;
}

int NNSi_SndArcLoadGroup(int groupNo, NNSSndHeapHandle heap) {
    const NNSSndArcGroupInfo *info = NNS_SndArcGetGroupInfo(groupNo);
    u32 i;
    int result;
    if (info == NULL) {
        return NNS_SND_ARC_LOAD_ERROR_INVALID_GROUP_NO;
    }
    for (i = 0; i < info->count; i++) {
        const NNSSndArcGroupItem *item = &info->item[i];
        switch (item->type) {
        case GROUP_ITEM_SEQ:
            result = NNSi_SndArcLoadSeq(item->index, item->loadFlag, heap, TRUE, NULL);
            if (result != NNS_SND_ARC_LOAD_SUCCESS) {
                return result;
            }
            break;
        case GROUP_ITEM_SEQARC:
            result = NNSi_SndArcLoadSeqArc(item->index, item->loadFlag, heap, TRUE, NULL);
            if (result != NNS_SND_ARC_LOAD_SUCCESS) {
                return result;
            }
            break;
        case GROUP_ITEM_BANK:
            result = NNSi_SndArcLoadBank(item->index, item->loadFlag, heap, TRUE, NULL);
            if (result != NNS_SND_ARC_LOAD_SUCCESS) {
                return result;
            }
            break;
        case GROUP_ITEM_WAVEARC:
            result = NNSi_SndArcLoadWaveArc(item->index, item->loadFlag, heap, TRUE, NULL);
            if (result != NNS_SND_ARC_LOAD_SUCCESS) {
                return result;
            }
            break;
        }
    }
    return NNS_SND_ARC_LOAD_SUCCESS;
}

int NNSi_SndArcLoadSeq(int seqNo, u32 loadFlag, NNSSndHeapHandle heap, BOOL bSetAddr, void **pData) {
    const NNSSndArcSeqInfo *info = NNS_SndArcGetSeqInfo(seqNo);
    void *data;
    int result;
    if (info == NULL) {
        return NNS_SND_ARC_LOAD_ERROR_INVALID_SEQ_NO;
    }
    result = NNSi_SndArcLoadBank(info->bankNo, loadFlag, heap, bSetAddr, NULL);
    if (result != NNS_SND_ARC_LOAD_SUCCESS) {
        return result;
    }
    if (loadFlag & NNS_SND_ARC_LOAD_SEQ) {
        data = LoadSeq(info->fileId, heap, bSetAddr);
        if (data == NULL) {
            return NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_SEQ;
        }
    } else {
        data = NNS_SndArcGetFileAddress(info->fileId);
    }
    if (pData != NULL) {
        *pData = data;
    }
    return NNS_SND_ARC_LOAD_SUCCESS;
}

int NNSi_SndArcLoadSeqArc(int seqArcNo, u32 loadFlag, NNSSndHeapHandle heap, BOOL bSetAddr, void **pData) {
    const NNSSndArcSeqArcInfo *info = NNS_SndArcGetSeqArcInfo(seqArcNo);
    void *data;
    if (info == NULL) {
        return NNS_SND_ARC_LOAD_ERROR_INVALID_SEQARC_NO;
    }
    if (loadFlag & NNS_SND_ARC_LOAD_SEQARC) {
        data = LoadSeqArc(info->fileId, heap, bSetAddr);
        if (data == NULL) {
            return NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_SEQARC;
        }
    } else {
        data = NNS_SndArcGetFileAddress(info->fileId);
    }
    if (pData != NULL) {
        *pData = data;
    }
    return NNS_SND_ARC_LOAD_SUCCESS;
}

int NNSi_SndArcLoadBank(int bankNo, u32 loadFlag, NNSSndHeapHandle heap, BOOL bSetAddr, void **pData) {
    const NNSSndArcBankInfo *info = NNS_SndArcGetBankInfo(bankNo);
    SNDBankData *bank;
    int i;
    int result;
    if (info == NULL) {
        return NNS_SND_ARC_LOAD_ERROR_INVALID_BANK_NO;
    }
    if (loadFlag & NNS_SND_ARC_LOAD_BANK) {
        bank = LoadBank(info->fileId, heap, bSetAddr);
        if (bank == NULL) {
            return NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_BANK;
        }
    } else {
        bank = NNS_SndArcGetFileAddress(info->fileId);
    }
    for (i = 0; i < 4; i++) {
        const NNSSndArcWaveArcInfo *waveArcInfo;
        SNDWaveArc *waveArc;
        if (info->waveArcNo[i] == 0xffff) {
            continue;
        }
        waveArcInfo = NNS_SndArcGetWaveArcInfo(info->waveArcNo[i]);
        if (waveArcInfo == NULL) {
            return NNS_SND_ARC_LOAD_ERROR_INVALID_WAVEARC_NO;
        }
        result = NNSi_SndArcLoadWaveArc(info->waveArcNo[i], loadFlag, heap, bSetAddr, (void **) &waveArc);
        if (result != NNS_SND_ARC_LOAD_SUCCESS) {
            return result;
        }
        if ((waveArcInfo->flags & NNS_SND_ARC_WAVEARC_SINGLE_LOAD) && (loadFlag & NNS_SND_ARC_LOAD_WAVE)) {
            if (!LoadSingleWaves(waveArc, bank, i, waveArcInfo->fileId, heap)) {
                return NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_WAVE;
            }
        }
        if (bank != NULL && waveArc != NULL) {
            SND_AssignWaveArc(bank, i, waveArc);
        }
    }
    if (pData != NULL) {
        *pData = bank;
    }
    return NNS_SND_ARC_LOAD_SUCCESS;
}

int NNSi_SndArcLoadWaveArc(int waveArcNo, u32 loadFlag, NNSSndHeapHandle heap, BOOL bSetAddr, void **pData) {
    const NNSSndArcWaveArcInfo *info = NNS_SndArcGetWaveArcInfo(waveArcNo);
    void *waveArc;
    if (info == NULL) {
        return NNS_SND_ARC_LOAD_ERROR_INVALID_WAVEARC_NO;
    }
    if (loadFlag & NNS_SND_ARC_LOAD_WAVE) {
        if (info->flags & NNS_SND_ARC_WAVEARC_SINGLE_LOAD) {
            waveArc = LoadWaveArcTable(info->fileId, heap, bSetAddr);
        } else {
            waveArc = LoadWaveArc(info->fileId, heap, bSetAddr);
        }
        if (waveArc == NULL) {
            return NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_WAVE;
        }
    } else {
        waveArc = NNS_SndArcGetFileAddress(info->fileId);
    }
    if (pData != NULL) {
        *pData = waveArc;
    }
    return NNS_SND_ARC_LOAD_SUCCESS;
}

void *NNSi_SndArcLoadFile(u32 fileId, NNSSndHeapDisposeCallback callback, u32 data1, u32 data2,
                          NNSSndHeapHandle heap) {
    u32 size = NNS_SndArcGetFileSize(fileId);
    void *mem;
    if (size == 0) {
        return NULL;
    }
    if (heap == NULL) {
        return NULL;
    }
    mem = NNS_SndHeapAlloc(heap, size + 32, callback, data1, data2);
    if (mem == NULL) {
        return NULL;
    }
    if (size != NNS_SndArcReadFile(fileId, mem, size, 0)) {
        return NULL;
    }
    DC_StoreRange(mem, size);
    return mem;
}

static void *LoadSeq(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr) {
    void *data = NNS_SndArcGetFileAddress(fileId);
    if (data == NULL) {
        data = NNSi_SndArcLoadFile(fileId, SeqDisposeCallback, bSetAddr ? (u32) NNS_SndArcGetCurrent() : 0, fileId, heap);
        if (bSetAddr && data != NULL) {
            NNS_SndArcSetFileAddress(fileId, data);
        }
    }
    return data;
}

static void *LoadSeqArc(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr) {
    void *data = NNS_SndArcGetFileAddress(fileId);
    if (data == NULL) {
        data = NNSi_SndArcLoadFile(fileId, SeqDisposeCallback, bSetAddr ? (u32) NNS_SndArcGetCurrent() : 0, fileId, heap);
        if (bSetAddr && data != NULL) {
            NNS_SndArcSetFileAddress(fileId, data);
        }
    }
    return data;
}

static void *LoadBank(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr) {
    void *data = NNS_SndArcGetFileAddress(fileId);
    if (data == NULL) {
        data = NNSi_SndArcLoadFile(fileId, BankDisposeCallback, bSetAddr ? (u32) NNS_SndArcGetCurrent() : 0, fileId,
                                   heap);
        if (bSetAddr && data != NULL) {
            NNS_SndArcSetFileAddress(fileId, data);
        }
    }
    return data;
}

static void *LoadWaveArc(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr) {
    void *data = NNS_SndArcGetFileAddress(fileId);
    if (data == NULL) {
        data = NNSi_SndArcLoadFile(fileId, WaveArcDisposeCallback, bSetAddr ? (u32) NNS_SndArcGetCurrent() : 0, fileId,
                                   heap);
        if (bSetAddr && data != NULL) {
            NNS_SndArcSetFileAddress(fileId, data);
        }
    }
    return data;
}

// Loads just the header and offset table of a wave archive whose waves load one at a time. The offsets are
// moved to after the table, which becomes the per-wave address table.
static void *LoadWaveArcTable(u32 fileId, NNSSndHeapHandle heap, BOOL bSetAddr) {
    SNDWaveArc *waveArc = NNS_SndArcGetFileAddress(fileId);
    if (waveArc == NULL) {
        u32 tableSize;
        u32 size;
        if (NNS_SndArcReadFile(fileId, &sWaveArcHeader, WAVEARC_HEADER_SIZE, 0) != WAVEARC_HEADER_SIZE) {
            return NULL;
        }
        tableSize = sWaveArcHeader.waveCount * 4;
        size = tableSize * 2;
        size += WAVEARC_HEADER_SIZE;
        if (heap == NULL) {
            return NULL;
        }
        waveArc = NNS_SndHeapAlloc(heap, size + 32, WaveArcTableDisposeCallback,
                                   bSetAddr ? (u32) NNS_SndArcGetCurrent() : 0, fileId);
        if (waveArc == NULL) {
            return NULL;
        }
        if (NNS_SndArcReadFile(fileId, waveArc, tableSize + WAVEARC_HEADER_SIZE, 0) !=
            tableSize + WAVEARC_HEADER_SIZE) {
            return NULL;
        }
        MI_CpuCopy8(waveArc->waveOffset, &waveArc->waveOffset[waveArc->waveCount], tableSize);
        MI_CpuFill8(waveArc->waveOffset, 0, tableSize);
        DC_StoreRange(waveArc, size);
        if (bSetAddr) {
            NNS_SndArcSetFileAddress(fileId, waveArc);
        }
    }
    return waveArc;
}

static void DisposeCallback(void *mem, NNSSndArc *arc, u32 fileId) {
    OSIntrMode enabled;
    NNSSndArc *old;
    if (arc == NULL) {
        return;
    }
    enabled = OS_DisableInterrupts();
    old = NNS_SndArcSetCurrent(arc);
    if (mem == NNS_SndArcGetFileAddress(fileId)) {
        NNS_SndArcSetFileAddress(fileId, NULL);
    }
    NNS_SndArcSetCurrent(old);
    OS_RestoreInterrupts(enabled);
}

static void SeqDisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    DisposeCallback(mem, (NNSSndArc *) data1, data2);
    SND_InvalidateSeqData(mem, (u8 *) mem + size);
}

static void BankDisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    DisposeCallback(mem, (NNSSndArc *) data1, data2);
    SND_InvalidateBankData(mem, (u8 *) mem + size);
    SND_DestroyBank(mem);
}

static void WaveArcDisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    DisposeCallback(mem, (NNSSndArc *) data1, data2);
    SND_InvalidateWaveData(mem, (u8 *) mem + size);
    SND_DestroyWaveArc(mem);
}

static void WaveArcTableDisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    DisposeCallback(mem, (NNSSndArc *) data1, data2);
    SND_DestroyWaveArc(mem);
}

static void SingleWaveDisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    SNDWaveArc *waveArc = (SNDWaveArc *) data1;
    if (mem == SND_GetWaveDataAddress(waveArc, data2)) {
        SND_SetWaveDataAddress(waveArc, data2, NULL);
    }
    SND_InvalidateWaveData(mem, (u8 *) mem + size);
}

static BOOL LoadSingleWave(SNDWaveArc *waveArc, u32 index, u32 fileId, NNSSndHeapHandle heap) {
    u32 waveCount;
    const u32 *offsetTable;
    u32 offset;
    u32 size;
    void *mem;
    if (SND_GetWaveDataAddress(waveArc, index) != NULL) {
        return TRUE;
    }
    waveCount = SND_GetWaveDataCount(waveArc);
    offsetTable = &waveArc->waveOffset[waveArc->waveCount];
    offset = offsetTable[index];
    size = (index < waveCount - 1 ? offsetTable[index + 1] : waveArc->fileHeader.fileSize) - offset;
    if (heap == NULL) {
        return FALSE;
    }
    mem = NNS_SndHeapAlloc(heap, size + 32, SingleWaveDisposeCallback, (u32) waveArc, index);
    if (mem == NULL) {
        return FALSE;
    }
    if (size != NNS_SndArcReadFile(fileId, mem, size, offset)) {
        return FALSE;
    }
    DC_StoreRange(mem, size);
    SND_SetWaveDataAddress(waveArc, index, mem);
    return TRUE;
}

static BOOL LoadSingleWaves(SNDWaveArc *waveArc, SNDBankData *bank, int waveArcIndex, u32 fileId,
                            NNSSndHeapHandle heap) {
    SNDInstPos pos = SND_GetFirstInstDataPos(bank);
    SNDInstData inst;
    if (bank == NULL) {
        return FALSE;
    }
    while (SND_GetNextInstData(bank, &inst, &pos)) {
        if (inst.type == 1 && waveArcIndex == inst.wave[1]) {
            if (!LoadSingleWave(waveArc, inst.wave[0], fileId, heap)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
