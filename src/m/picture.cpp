// MPictureFile and the views that show pictures.

#include "m/picture.h"

MPictureFile::MPictureFile(std::string path, int lock) : mResource(path) {
    Open(lock);
}

MPictureFile::MPictureFile() {
}

void MPictureFile::Open(int lock) {
    switch (GetData()->type) {
    case 0:
        mPalette = GetData()->GetPaletteFile(mResource.mArchive);
        mChr = GetData()->GetChrFile(mResource.mArchive);
        mScreen = GetData()->GetScreenFile(mResource.mArchive);
        lock &= LOCK_CHR | LOCK_PALETTE | LOCK_SCREEN;
        break;
    case 3:
    case 4:
        mPalette = GetData()->GetPaletteFile(mResource.mArchive);
        mChr = GetData()->GetChrFile(mResource.mArchive);
        lock &= LOCK_CHR | LOCK_PALETTE;
        break;
    case 1:
    case 2:
        mChr = GetData()->GetChrFile(mResource.mArchive);
        lock &= LOCK_CHR;
        break;
    }
    if (lock & LOCK_CHR) {
        mChr.Lock();
    }
    if (lock & LOCK_PALETTE) {
        mPalette.Lock();
    }
    if (lock & LOCK_SCREEN) {
        mScreen.Lock();
    }
}

const void *MPictureFile::GetChr() {
    mChr.Lock();
    return mChr.GetData();
}

const void *MPictureFile::GetPalette() {
    mPalette.Lock();
    return mPalette.GetData();
}

const void *MPictureFile::GetScreen() {
    mScreen.Lock();
    return mScreen.GetData();
}

MArchiveFile MPictureFile::GetChrFile() {
    return mChr;
}

bool MPictureFile::IsUnk04() {
    return GetData()->flags & 1;
}

u32 MPictureFile::GetPaletteSize() {
    return GetData()->paletteSize;
}

u32 MPictureFile::GetChrSize() {
    return GetData()->chrSize;
}

MPoint MPictureFile::CalcSize() {
    return MPoint(GetData()->width, GetData()->height);
}

int MPictureFile::GetWidth() {
    return GetData()->width;
}

int MPictureFile::GetHeight() {
    return GetData()->height;
}

MPoint MPictureFile::CalcTileSize() {
    MPoint size(GetData()->width, GetData()->height);
    return MPoint(size.x >> 3, size.y >> 3);
}

MPoint MPictureFile::GetSize() {
    return CalcSize();
}

MPoint MPictureFile::GetTileSize() {
    return CalcTileSize();
}
