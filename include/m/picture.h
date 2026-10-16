// Pictures: an archive holding an image's palette, characters and screen, and views that show one.
#ifndef M_PICTURE_H
#define M_PICTURE_H

#include "m/resource.h"
#include "m/geometry.h"

// The header of a picture file. Its parts are files of other archives, named relative to its own.
struct MPictureData {
    struct FileRef {
        u32 index;
        u32 nameOffset;     // from the header
    };

    u32 unk00;
    u32 flags;              // 0x04: bit 0: ...
    u32 type;               // 0x08: which parts the picture has
    u32 unk0C;
    int width;              // 0x10: in pixels
    int height;             // 0x14
    u32 paletteSize;        // 0x18
    u32 chrSize;            // 0x1C
    u32 unk20;
    FileRef palette;        // 0x24
    FileRef chr;            // 0x2C
    FileRef screen;         // 0x34

    MArchiveFile GetPaletteFile(const MArchivePtr &dir) const {
        return dir.GetFile((const char *)this + palette.nameOffset, palette.index);
    }
    MArchiveFile GetChrFile(const MArchivePtr &dir) const {
        return dir.GetFile((const char *)this + chr.nameOffset, chr.index);
    }
    MArchiveFile GetScreenFile(const MArchivePtr &dir) const {
        return dir.GetFile((const char *)this + screen.nameOffset, screen.index);
    }
};

class MPictureFile {
public:
    enum {
        LOCK_CHR = 1,
        LOCK_PALETTE = 2,
        LOCK_SCREEN = 4,
    };

    MPictureFile(std::string path, int lock);
    MPictureFile();

    // Finds the picture's parts and locks the ones asked for (LOCK_*).
    void Open(int lock);
    const void *GetChr();
    const void *GetPalette();
    const void *GetScreen();
    MArchiveFile GetChrFile();
    bool IsUnk04();
    u32 GetPaletteSize();
    u32 GetChrSize();
    MPoint CalcSize();
    int GetWidth();
    int GetHeight();
    MPoint CalcTileSize();
    MPoint GetSize();
    MPoint GetTileSize();

    const MPictureData *GetData() { return (const MPictureData *)mResource.mFile.GetData(); }

    MResource mResource;    // 0x00
    MArchiveFile mPalette;  // 0x10
    MArchiveFile mChr;      // 0x1C
    MArchiveFile mScreen;   // 0x28
};

#endif
