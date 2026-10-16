// A file loaded on its own: an archive's first file, locked in memory.
#ifndef M_RESOURCE_H
#define M_RESOURCE_H

#include "m/archive.h"

class MResource {
public:
    MResource() {}
    MResource(std::string path);

    MArchivePtr mArchive;   // 0x00
    MArchiveFile mFile;     // 0x04
};

// Every file that uses the constructor has its own copy of it.
#ifndef M_RESOURCE_OUT_OF_LINE
inline MResource::MResource(std::string path) : mArchive(path) {
    mFile = mArchive.GetFile(0);
    mFile.Lock();
    mFile.GetData();
}
#endif

#endif
