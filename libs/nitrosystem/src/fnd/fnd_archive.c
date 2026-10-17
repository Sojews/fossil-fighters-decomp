// NitroSystem foundation: NARC archives in memory, mounted as file system archives. (Named so its object
// doesn't share a name with src/m/archive.cpp's, which the linker script can't tell apart.)

#include <string.h>
#include "nnsys/fnd/archive.h"

#define NNS_FND_ARCHIVE_SIGNATURE 0x4352414e        // 'NARC'
#define NNS_FND_ARCHIVE_BLOCK_FAT 0x46415442        // 'BTAF'
#define NNS_FND_ARCHIVE_BLOCK_FNT 0x464e5442        // 'BTNF'
#define NNS_FND_ARCHIVE_BLOCK_FIMG 0x46494d47       // 'GMIF'

static BOOL IsValidArchiveBinary(NNSiFndArchiveHeader *header) {
    if (header->signature != NNS_FND_ARCHIVE_SIGNATURE) {
        return FALSE;
    }
    if (header->byteOrder != 0xfffe) {
        return FALSE;
    }
    if (header->version != 0x100) {
        return FALSE;
    }
    return TRUE;
}

BOOL NNS_FndMountArchive(NNSFndArchive *archive, const char *arcName, void *arcBinary) {
    NNSiFndArchiveBlockHeader *fat = NULL;
    NNSiFndArchiveBlockHeader *fnt = NULL;
    NNSiFndArchiveBlockHeader *fimg = NULL;
    NNSiFndArchiveBlockHeader *block;
    int i;

    if (!IsValidArchiveBinary(arcBinary)) {
        return FALSE;
    }
    block = (NNSiFndArchiveBlockHeader *) ((u8 *) arcBinary + ((NNSiFndArchiveHeader *) arcBinary)->headerSize);
    for (i = 0; i < ((NNSiFndArchiveHeader *) arcBinary)->dataBlocks; i++) {
        switch (block->blockType) {
        case NNS_FND_ARCHIVE_BLOCK_FAT:
            fat = block;
            break;
        case NNS_FND_ARCHIVE_BLOCK_FNT:
            fnt = block;
            break;
        case NNS_FND_ARCHIVE_BLOCK_FIMG:
            fimg = block;
            break;
        }
        block = (NNSiFndArchiveBlockHeader *) ((u8 *) block + block->blockSize);
    }

    FS_InitArchive(&archive->fsArchive);
    archive->arcBinary = arcBinary;
    archive->fatData = fat;
    archive->fileImage = (u32) fimg + sizeof(NNSiFndArchiveBlockHeader);
    if (!FS_RegisterArchiveName(&archive->fsArchive, arcName, strlen(arcName))) {
        return FALSE;
    }
    if (!FS_LoadArchive(&archive->fsArchive, (u32) fimg + sizeof(NNSiFndArchiveBlockHeader),
                        (u32) fat + sizeof(NNSiFndArchiveBlockHeader) + 4 -
                            ((u32) fimg + sizeof(NNSiFndArchiveBlockHeader)),
                        fat->blockSize - sizeof(NNSiFndArchiveBlockHeader) - 4,
                        (u32) fnt + sizeof(NNSiFndArchiveBlockHeader) -
                            ((u32) fimg + sizeof(NNSiFndArchiveBlockHeader)),
                        fnt->blockSize - sizeof(NNSiFndArchiveBlockHeader), NULL, NULL)) {
        FS_ReleaseArchiveName(&archive->fsArchive);
        return FALSE;
    }
    return TRUE;
}

BOOL NNS_FndUnmountArchive(NNSFndArchive *archive) {
    if (!FS_UnloadArchive(&archive->fsArchive)) {
        return FALSE;
    }
    FS_ReleaseArchiveName(&archive->fsArchive);
    return TRUE;
}
