// A file loaded on its own: an archive's first file, locked in memory.
#ifndef GAME_RESOURCE_H
#define GAME_RESOURCE_H

#include "m/archive.h"

class MResource {
public:
    MResource(const std::string &path);

    MArchivePtr mArchive;   // 0x00
    MArchiveFile mFile;     // 0x04
};

#endif
