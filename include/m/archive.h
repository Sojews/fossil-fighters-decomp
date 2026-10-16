// Archives: files loaded from the ROM file system, read in pieces through an optional filter.
#ifndef M_ARCHIVE_H
#define M_ARCHIVE_H

#include "types.h"

// Transforms data as it's read.
class MArchiveFilter {
public:
    virtual ~MArchiveFilter() {}
    virtual void Filter(const void *src, void *dest, u32 size) = 0;
};

class MArchive {
public:
    // Reads part of a file into dest, through filter if there is one (which is deleted afterwards).
    void Read(u32 index, u32 offset, void *dest, u32 size, MArchiveFilter *filter);
};

// A file in an archive.
struct MFileRef {
    MArchive *archive;
    u32 index;
};

#endif
