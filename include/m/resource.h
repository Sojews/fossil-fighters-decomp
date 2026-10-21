// A file loaded on its own: an archive's first file, locked in memory, read as a T. Each data type has its own
// instantiation (the game keeps one copy of each constructor).
#ifndef M_RESOURCE_H
#define M_RESOURCE_H

#include "m/archive.h"

template <class T>
class MResource {
public:
    MResource() {}
    MResource(std::string path) : mArchive(path) {
        mFile = mArchive.GetFile(0);
        mFile.Lock();
        mFile.GetData();
    }

    const T *Get() { return (const T *)mFile.GetData(); }
    // Takes another type's resource (the same file, read as a T).
    template <class U> MResource &operator=(const MResource<U> &o) {
        mArchive = o.mArchive;
        mFile = o.mFile;
        return *this;
    }

    MArchivePtr mArchive;   // 0x00
    MArchiveFile mFile;     // 0x04
};

#endif
