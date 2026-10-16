// MArchiveFile and MResource.

#define M_RESOURCE_OUT_OF_LINE
#include "m/resource.h"

MArchiveFile MArchivePtr::GetFile(u32 index) const {
    return MArchiveFile(mArchive, index);
}

MResource::MResource(std::string path) : mArchive(path) {
    mFile = mArchive.GetFile(0);
    mFile.Lock();
    mFile.GetData();
}
