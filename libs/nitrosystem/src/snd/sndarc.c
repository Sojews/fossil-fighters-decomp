// NitroSystem sound: the sound archive (SDAT), its info block and file table.

#include "nnsys/snd/sndarc.h"

#define OFFSET_TO_ADDR(base, offset) ((offset) == 0 ? NULL : (void *) ((u32) (base) + (offset)))

static NNSSndArc *sCurrent;

static inline const void *GetAddress(const void *base, u32 offset) {
    if (offset == 0) {
        return NULL;
    }
    return (const void *) ((u32) base + offset);
}

static void InfoDisposeCallback(void *mem, u32 size, u32 data1, u32 data2);
static void FatDisposeCallback(void *mem, u32 size, u32 data1, u32 data2);
static void SymbolDisposeCallback(void *mem, u32 size, u32 data1, u32 data2);

BOOL NNS_SndArcInit(NNSSndArc *arc, const char *filePath, NNSSndHeapHandle heap, BOOL symbolLoad) {
    arc->info = NULL;
    arc->fat = NULL;
    arc->symbol = NULL;
    if (!FS_ConvertPathToFileID(&arc->fileId, filePath)) {
        return FALSE;
    }
    FS_InitFile(&arc->file);
    if (!FS_OpenFileFast(&arc->file, arc->fileId)) {
        return FALSE;
    }
    arc->fileOpenFlag = TRUE;
    if (!NNS_SndArcSetup(arc, heap, symbolLoad)) {
        return FALSE;
    }
    sCurrent = arc;
    return TRUE;
}

BOOL NNS_SndArcSetup(NNSSndArc *arc, NNSSndHeapHandle heap, BOOL symbolLoad) {
    if (!FS_SeekFile(&arc->file, 0, FS_SEEK_SET)) {
        return FALSE;
    }
    if (FS_ReadFile(&arc->file, &arc->header, sizeof(NNSSndArcHeader)) != sizeof(NNSSndArcHeader)) {
        return FALSE;
    }
    if (heap != NULL) {
        arc->info = NNS_SndHeapAlloc(heap, arc->header.infoSize, InfoDisposeCallback, (u32) arc, 0);
        if (arc->info == NULL) {
            return FALSE;
        }
        if (!FS_SeekFile(&arc->file, arc->header.infoOffset, FS_SEEK_SET)) {
            return FALSE;
        }
        if (FS_ReadFile(&arc->file, arc->info, arc->header.infoSize) != arc->header.infoSize) {
            return FALSE;
        }
        arc->fat = NNS_SndHeapAlloc(heap, arc->header.fatSize, FatDisposeCallback, (u32) arc, 0);
        if (arc->fat == NULL) {
            return FALSE;
        }
        if (!FS_SeekFile(&arc->file, arc->header.fatOffset, FS_SEEK_SET)) {
            return FALSE;
        }
        if (FS_ReadFile(&arc->file, arc->fat, arc->header.fatSize) != arc->header.fatSize) {
            return FALSE;
        }
        if (symbolLoad && arc->header.symbolDataSize != 0) {
            arc->symbol = NNS_SndHeapAlloc(heap, arc->header.symbolDataSize, SymbolDisposeCallback, (u32) arc, 0);
            if (arc->symbol == NULL) {
                return FALSE;
            }
            if (!FS_SeekFile(&arc->file, arc->header.symbolDataOffset, FS_SEEK_SET)) {
                return FALSE;
            }
            if (FS_ReadFile(&arc->file, arc->symbol, arc->header.symbolDataSize) != arc->header.symbolDataSize) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

NNSSndArc *NNS_SndArcSetCurrent(NNSSndArc *arc) {
    NNSSndArc *old = sCurrent;
    sCurrent = arc;
    return old;
}

NNSSndArc *NNS_SndArcGetCurrent(void) {
    return sCurrent;
}

const void *NNS_SndArcGetSeqInfo(int seqNo) {
    NNSSndArc *arc = sCurrent;
    const NNSSndArcOffsetTable *table = OFFSET_TO_ADDR(arc->info, arc->info->seqOffset);
    if (table == NULL) {
        return NULL;
    }
    if (seqNo < 0) {
        return NULL;
    }
    if (seqNo >= table->count) {
        return NULL;
    }
    return GetAddress(arc->info, table->offset[seqNo]);
}

const void *NNS_SndArcGetSeqArcInfo(int seqArcNo) {
    NNSSndArc *arc = sCurrent;
    const NNSSndArcOffsetTable *table = OFFSET_TO_ADDR(arc->info, arc->info->seqArcOffset);
    if (table == NULL) {
        return NULL;
    }
    if (seqArcNo < 0) {
        return NULL;
    }
    if (seqArcNo >= table->count) {
        return NULL;
    }
    return GetAddress(arc->info, table->offset[seqArcNo]);
}

const void *NNS_SndArcGetBankInfo(int bankNo) {
    NNSSndArc *arc = sCurrent;
    const NNSSndArcOffsetTable *table = OFFSET_TO_ADDR(arc->info, arc->info->bankOffset);
    if (table == NULL) {
        return NULL;
    }
    if (bankNo < 0) {
        return NULL;
    }
    if (bankNo >= table->count) {
        return NULL;
    }
    return GetAddress(arc->info, table->offset[bankNo]);
}

const void *NNS_SndArcGetWaveArcInfo(int waveArcNo) {
    NNSSndArc *arc = sCurrent;
    const NNSSndArcOffsetTable *table = OFFSET_TO_ADDR(arc->info, arc->info->waveArcOffset);
    if (table == NULL) {
        return NULL;
    }
    if (waveArcNo < 0) {
        return NULL;
    }
    if (waveArcNo >= table->count) {
        return NULL;
    }
    return GetAddress(arc->info, table->offset[waveArcNo]);
}

const void *NNS_SndArcGetStrmInfo(int strmNo) {
    NNSSndArc *arc = sCurrent;
    const NNSSndArcOffsetTable *table = OFFSET_TO_ADDR(arc->info, arc->info->strmOffset);
    if (table == NULL) {
        return NULL;
    }
    if (strmNo < 0) {
        return NULL;
    }
    if (strmNo >= table->count) {
        return NULL;
    }
    return GetAddress(arc->info, table->offset[strmNo]);
}

const void *NNS_SndArcGetPlayerInfo(int playerNo) {
    NNSSndArc *arc = sCurrent;
    const NNSSndArcOffsetTable *table = OFFSET_TO_ADDR(arc->info, arc->info->playerOffset);
    if (table == NULL) {
        return NULL;
    }
    if (playerNo < 0) {
        return NULL;
    }
    if (playerNo >= table->count) {
        return NULL;
    }
    return GetAddress(arc->info, table->offset[playerNo]);
}

const void *NNS_SndArcGetStrmPlayerInfo(int playerNo) {
    NNSSndArc *arc = sCurrent;
    const NNSSndArcOffsetTable *table = OFFSET_TO_ADDR(arc->info, arc->info->strmPlayerOffset);
    if (table == NULL) {
        return NULL;
    }
    if (playerNo < 0) {
        return NULL;
    }
    if (playerNo >= table->count) {
        return NULL;
    }
    return GetAddress(arc->info, table->offset[playerNo]);
}

const void *NNS_SndArcGetGroupInfo(int groupNo) {
    NNSSndArc *arc = sCurrent;
    const NNSSndArcOffsetTable *table = OFFSET_TO_ADDR(arc->info, arc->info->groupOffset);
    if (table == NULL) {
        return NULL;
    }
    if (groupNo < 0) {
        return NULL;
    }
    if (groupNo >= table->count) {
        return NULL;
    }
    return GetAddress(arc->info, table->offset[groupNo]);
}

u32 NNS_SndArcGetFileOffset(u32 fileId) {
    NNSSndArcFat *fat = sCurrent->fat;
    if (fileId >= fat->numFiles) {
        return 0;
    }
    return fat->files[fileId].offset;
}

u32 NNS_SndArcGetFileSize(u32 fileId) {
    NNSSndArcFat *fat = sCurrent->fat;
    if (fileId >= fat->numFiles) {
        return 0;
    }
    return fat->files[fileId].size;
}

s32 NNS_SndArcReadFile(u32 fileId, void *buffer, s32 size, s32 offset) {
    NNSSndArc *arc = sCurrent;
    NNSSndArcFat *fat = arc->fat;
    const NNSSndArcFileInfo *info;
    if (fileId >= fat->numFiles) {
        return -1;
    }
    info = &fat->files[fileId];
    if (size > info->size - offset) {
        size = info->size - offset;
    }
    if (!FS_SeekFile(&arc->file, info->offset + offset, FS_SEEK_SET)) {
        return -1;
    }
    return FS_ReadFile(&arc->file, buffer, size);
}

void NNS_SndArcGetFileID(FSFileID *fileId) {
    *fileId = sCurrent->fileId;
}

void *NNS_SndArcGetFileAddress(u32 fileId) {
    NNSSndArcFat *fat = sCurrent->fat;
    if (fileId >= fat->numFiles) {
        return NULL;
    }
    return fat->files[fileId].mem;
}

void NNS_SndArcSetFileAddress(u32 fileId, void *address) {
    sCurrent->fat->files[fileId].mem = address;
}

static void InfoDisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    ((NNSSndArc *) data1)->info = NULL;
}

static void FatDisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    ((NNSSndArc *) data1)->fat = NULL;
}

static void SymbolDisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    ((NNSSndArc *) data1)->symbol = NULL;
}
