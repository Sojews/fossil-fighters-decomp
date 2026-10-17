// NitroSystem foundation: NARC archives in memory, mounted as file system archives.
#ifndef NNSYS_FND_ARCHIVE_H
#define NNSYS_FND_ARCHIVE_H

#include "nitro/fs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u32 signature;      // 'NARC'
    u16 byteOrder;      // 0xFFFE
    u16 version;
    u32 fileSize;
    u16 headerSize;
    u16 dataBlocks;
} NNSiFndArchiveHeader;

typedef struct {
    u32 blockType;
    u32 blockSize;
} NNSiFndArchiveBlockHeader;

typedef struct {
    FSArchive fsArchive;
    NNSiFndArchiveHeader *arcBinary;
    NNSiFndArchiveBlockHeader *fatData;
    u32 fileImage;
} NNSFndArchive;

BOOL NNS_FndMountArchive(NNSFndArchive *archive, const char *arcName, void *arcBinary);
BOOL NNS_FndUnmountArchive(NNSFndArchive *archive);

#ifdef __cplusplus
}
#endif

#endif
