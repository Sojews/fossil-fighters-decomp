// NitroSystem sound: the sound archive (SDAT), its info block and file table.
#ifndef NNSYS_SND_SNDARC_H
#define NNSYS_SND_SNDARC_H

#include "nitro/fs.h"
#include "nnsys/snd/heap.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u32 signature;
    u16 byteOrder;
    u16 version;
    u32 fileSize;
    u16 headerSize;
    u16 dataBlocks;
    u32 symbolDataOffset;
    u32 symbolDataSize;
    u32 infoOffset;
    u32 infoSize;
    u32 fatOffset;
    u32 fatSize;
    u32 fileImageOffset;
    u32 fileImageSize;
} NNSSndArcHeader;

typedef struct {
    u32 offset;
    u32 size;
    void *mem;      // Where it's loaded, or NULL
    u32 reserved;
} NNSSndArcFileInfo;

typedef struct {
    u32 kind;
    u32 size;
    u32 numFiles;
    NNSSndArcFileInfo files[1];
} NNSSndArcFat;

typedef struct {
    u32 count;
    u32 offset[1];
} NNSSndArcOffsetTable;

typedef struct {
    u32 kind;
    u32 size;
    u32 seqOffset;
    u32 seqArcOffset;
    u32 bankOffset;
    u32 waveArcOffset;
    u32 playerOffset;
    u32 groupOffset;
    u32 strmPlayerOffset;
    u32 strmOffset;
} NNSSndArcInfo;

typedef struct {
    NNSSndArcHeader header;
    BOOL fileOpenFlag;
    FSFile file;
    FSFileID fileId;
    NNSSndArcFat *fat;
    void *symbol;
    NNSSndArcInfo *info;
} NNSSndArc;

typedef struct {
    u32 fileId;
    u16 bankNo;
    u8 volume;
    u8 channelPrio;
    u8 playerPrio;
    u8 playerNo;
    u16 reserved;
} NNSSndArcSeqInfo;

typedef struct {
    u32 fileId;
} NNSSndArcSeqArcInfo;

typedef struct {
    u32 fileId;
    u16 waveArcNo[4];
} NNSSndArcBankInfo;

typedef struct {
    u32 fileId : 24;
    u32 flags : 8;
} NNSSndArcWaveArcInfo;

#define NNS_SND_ARC_WAVEARC_SINGLE_LOAD (1 << 0)

typedef struct {
    u8 type;
    u8 loadFlag;
    u16 padding;
    u32 index;
} NNSSndArcGroupItem;

typedef struct {
    u32 count;
    NNSSndArcGroupItem item[1];
} NNSSndArcGroupInfo;

BOOL NNS_SndArcInit(NNSSndArc *arc, const char *filePath, NNSSndHeapHandle heap, BOOL symbolLoad);
BOOL NNS_SndArcSetup(NNSSndArc *arc, NNSSndHeapHandle heap, BOOL symbolLoad);
NNSSndArc *NNS_SndArcSetCurrent(NNSSndArc *arc);
NNSSndArc *NNS_SndArcGetCurrent(void);
const void *NNS_SndArcGetSeqInfo(int seqNo);
const void *NNS_SndArcGetSeqArcInfo(int seqArcNo);
const void *NNS_SndArcGetBankInfo(int bankNo);
const void *NNS_SndArcGetWaveArcInfo(int waveArcNo);
const void *NNS_SndArcGetStrmInfo(int strmNo);
const void *NNS_SndArcGetPlayerInfo(int playerNo);
const void *NNS_SndArcGetStrmPlayerInfo(int playerNo);
const void *NNS_SndArcGetGroupInfo(int groupNo);
u32 NNS_SndArcGetFileOffset(u32 fileId);
u32 NNS_SndArcGetFileSize(u32 fileId);
s32 NNS_SndArcReadFile(u32 fileId, void *buffer, s32 size, s32 offset);
void NNS_SndArcGetFileID(FSFileID *fileId);
void *NNS_SndArcGetFileAddress(u32 fileId);
void NNS_SndArcSetFileAddress(u32 fileId, void *address);

#ifdef __cplusplus
}
#endif

#endif
