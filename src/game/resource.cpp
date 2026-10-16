// MArchiveFile and MResource.

#include "game/resource.h"

MArchiveFile MArchivePtr::GetFile(u32 index) const {
    return MArchiveFile(mArchive, index);
}

MResource::MResource(const std::string &path) : mArchive(path) {
    mFile = mArchive.GetFile(0);
    mFile.Lock();
    mFile.GetData();
}
