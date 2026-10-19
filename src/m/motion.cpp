// Motions: animated OBJ graphics loaded from a motion file, shown as a view.

#pragma ipa file

#include "m/motion.h"
#include <algorithm>
#include <map>
#include "m/display.h"
#include "m/heap.h"
#include "m/obj.h"
#include "m/chr.h"
#include "m/pal.h"

static std::map<FSFileID, MMotionData *> sMotionDatas;
static OSMutex sMotionDatasMutex;

// Opens the files of a list (in the directory it names).
static inline void OpenFiles(const MArchivePtr &archive, const void *header, const MMotionFileList *list,
                             std::vector<MArchiveFile> &files) {
    MArchivePtr dir = archive.Open((const char *) header + list->path);
    files.reserve(list->count);
    for (u32 i = 0; i < list->count; i++) {
        files.push_back(dir.GetFile(((const u32 *) ((const u8 *) header + list->table))[i]));
    }
}

// Opens the motion file and every file it uses (unlocked, but for the ones asked for).
MMotionData::MMotionData(FSFileID id, int unk) : MMotionArchive(id) {
    mRefCount = 0;
    for (u32 i = 0; i < 2; i++) {
        mPalRefs[i] = 0;
        mChrRefs[i] = 0;
        mPals[i] = NULL;
        mChrs[i] = NULL;
    }
    MMotionHeader *header = (MMotionHeader *) mFile.GetData();
    OpenFiles(mArchive, header, &header->cellFiles, mAnimeFiles);
    header = (MMotionHeader *) mFile.GetData();
    OpenFiles(mArchive, header, &header->chrs, mChrFiles);
    header = (MMotionHeader *) mFile.GetData();
    OpenFiles(mArchive, header, &header->pals, mPalFiles);
    LockFiles(unk);
}

MMotionData::~MMotionData() {
}

void MMotionData::LockFiles(int which) {
    if (which & 1) {
        for (std::vector<MArchiveFile>::iterator it = mChrFiles.begin(); it != mChrFiles.end(); it++) {
            it->Lock();
        }
    }
    if (which & 2) {
        for (std::vector<MArchiveFile>::iterator it = mPalFiles.begin(); it != mPalFiles.end(); it++) {
            it->Lock();
        }
    }
    if (which & 4) {
        for (std::vector<MArchiveFile>::iterator it = mAnimeFiles.begin(); it != mAnimeFiles.end(); it++) {
            it->Lock();
        }
    }
}

MChrBlock **MMotionData::AcquireChrs(MDisplay *display) {
    MScopedLock lock(&mMutex.mutex, TRUE);
    int d = display->GetRegs00();
    LockFiles(1);
    if (mChrRefs[d]++ == 0) {
        const MMotionGfxHeader *data;
        u32 i;
        MChr *chr;
        u32 count;
        u32 tiles;
        u32 boundary;
        u32 size;
        count = ((MMotionHeader *) mFile.GetData())->chrs.count;
        chr = display->mObj->mChr;
        mChrs[d] = new MChrBlock *[count];
        for (i = 0; i < count; i++) {
            data = (const MMotionGfxHeader *) mChrFiles[i].GetData();
            tiles = data->tiles;
            boundary = chr->mBoundary;
            if (chr->mMode == 0) {
                size = boundary * ((tiles + boundary - 1) / boundary);
            } else {
                size = ((boundary + tiles * 2 - 1) / boundary * boundary) >> 1;
            }
            mChrs[d][i] = chr->AllocShared(data->mode, data + 1, size, 0, TRUE);
        }
    }
    return mChrs[d];
}

void MMotionData::ReleaseChrs(MDisplay *display) {
    MScopedLock lock(&mMutex.mutex, TRUE);
    int d = display->GetRegs00();
    mChrRefs[d]--;
    if (mChrRefs[d] > 0) {
        return;
    }
    MChr *chr = display->mObj->mChr;
    u32 count = ((MMotionHeader *) mFile.GetData())->chrs.count;
    for (u32 i = 0; i < count; i++) {
        chr->FreeShared(mChrs[d][i]);
    }
    delete[] mChrs[d];
    mChrs[d] = NULL;
}

MPalUnit **MMotionData::AcquirePalettes(MDisplay *display) {
    MScopedLock lock(&mMutex.mutex, TRUE);
    LockFiles(2);
    int d = display->GetRegs00();
    if (mPalRefs[d]++ == 0) {
        u32 i;
        u32 count = ((MMotionHeader *) mFile.GetData())->pals.count;
        MPal *pal = display->mObj->mPal;
        MPal *extPal = display->mObj->mExtPal;
        mFile.GetData();
        mPals[d] = new MPalUnit *[count];
        for (i = 0; i < count; i++) {
            const MMotionGfxHeader *data = (const MMotionGfxHeader *) mPalFiles[i].GetData();
            if (data->extPalette == 0) {
                mPals[d][i] = pal->Alloc(1, data + 1, TRUE);
            } else {
                mPals[d][i] = extPal->Alloc(1, data + 1, TRUE);
            }
        }
    }
    return mPals[d];
}

void MMotionData::ReleasePalettes(MDisplay *display) {
    MScopedLock lock(&mMutex.mutex, TRUE);
    int d = display->GetRegs00();
    mPalRefs[d]--;
    if (mPalRefs[d] > 0) {
        return;
    }
    u32 count = ((MMotionHeader *) mFile.GetData())->pals.count;
    for (u32 i = 0; i < count; i++) {
        mPals[d][i]->pal->Free(mPals[d][i]);
    }
    delete[] mPals[d];
    mPals[d] = NULL;
}

MMotionData *MMotionData::Get(FSFileID id, int unk) {
    MScopedLock lock(&sMotionDatasMutex, TRUE);
    MMotionData *data;
    std::map<FSFileID, MMotionData *>::iterator it = sMotionDatas.find(id);
    if (it != sMotionDatas.end()) {
        data = it->second;
    } else {
        data = new MMotionData(id, unk);
        sMotionDatas[id] = data;
    }
    data->mRefCount++;
    return data;
}

void MMotionData::AddRef() {
    MScopedLock lock(&sMotionDatasMutex, TRUE);
    mRefCount++;
}

void MMotionData::Release() {
    MScopedLock lock(&sMotionDatasMutex, TRUE);
    if (--mRefCount <= 0) {
        FSFileID id = mArchive.mArchive->mId;
        sMotionDatas.erase(id);
        delete this;
    }
}

MMotionFile::MMotionFile(std::string path, int unk) {
    FSFileID id;
    if (!MGetFileID(path, &id)) {
        OS_Terminate();
    }
    mData = MMotionData::Get(id, unk);
}

MMotionFile::MMotionFile(const MMotionFile &other) {
    mData = other.mData;
    mData->AddRef();
}

MMotionFile::~MMotionFile() {
    mData->Release();
}

MMotionFile &MMotionFile::operator=(const MMotionFile &other) {
    MMotionData *old = mData;
    mData = other.mData;
    mData->AddRef();
    old->Release();
    return *this;
}

u32 MMotionFile::GetUnk04() {
    return ((MMotionHeader *) mData->mFile.GetData())->unk04;
}

u32 MMotionFile::GetAnimeCount() {
    return ((MMotionHeader *) mData->mFile.GetData())->animeCount;
}

u32 MMotionFile::GetCellCount() {
    return ((MMotionHeader *) mData->mFile.GetData())->cellCount;
}

const MMotionCellDef *MMotionFile::GetCell(u32 index) {
    MMotionHeader *header = (MMotionHeader *) mData->mFile.GetData();
    return (const MMotionCellDef *) ((u8 *) header + header->cellOffset) + index;
}

// The palette file's data, locked; with file, also a reference to the file (which keeps it locked).
void *MMotionFile::GetPalette(u32 index, MArchiveFile *file) {
    if (file != NULL) {
        MArchiveFile f = mData->mPalFiles[index];
        f.Lock();
        *file = f;
        return f.GetData();
    }
    MArchiveFile &f = mData->mPalFiles[index];
    f.Lock();
    return f.GetData();
}

// The chr file's data, as for GetPalette.
void *MMotionFile::GetChr(u32 index, MArchiveFile *file) {
    if (file != NULL) {
        MArchiveFile f = mData->mChrFiles[index];
        f.Lock();
        *file = f;
        return f.GetData();
    }
    MArchiveFile &f = mData->mChrFiles[index];
    f.Lock();
    return f.GetData();
}

u32 MMotionFile::GetUnk18() {
    return ((u32 *) mData->mFile.GetData())[6];
}

// The anime file's data, as for GetPalette.
void *MMotionFile::GetAnime(int anime, MArchiveFile *file) {
    if (file != NULL) {
        MArchiveFile f = mData->mAnimeFiles[anime];
        f.Lock();
        *file = f;
        return f.GetData();
    }
    MArchiveFile &f = mData->mAnimeFiles[anime];
    f.Lock();
    return f.GetData();
}

MPalUnit **MMotionFile::AcquirePalettes(MDisplay *display) {
    return mData->AcquirePalettes(display);
}

void MMotionFile::ReleasePalettes(MDisplay *display) {
    mData->ReleasePalettes(display);
}

MChrBlock **MMotionFile::AcquireChrs(MDisplay *display) {
    return mData->AcquireChrs(display);
}

void MMotionFile::ReleaseChrs(MDisplay *display) {
    mData->ReleaseChrs(display);
}

MMotion::MMotion(const MMotionFile &file, MSmartOam *oam, int flags, int fromEnd)
    : mDisplay(oam->mDisplay), mFile(file) {
    mOam = oam;
    mFlags = flags;
    if (mFile.GetCellCount() != 0) {
        mUnit = mOam->Alloc(mFile.GetCellCount(), fromEnd);
        mEntries = mUnit->GetEntries();
    }
    Init(fromEnd);
}

MMotion::MMotion(const std::string &path, MSmartOam *oam, int flags, int fromEnd)
    : mDisplay(oam->mDisplay), mFile(path, 0) {
    mOam = oam;
    mFlags = flags;
    if (mFile.GetCellCount() != 0) {
        mUnit = mOam->Alloc(mFile.GetCellCount(), fromEnd);
        mEntries = mUnit->GetEntries();
    }
    Init(fromEnd);
}

void MMotion::Init(int fromEnd) {
    mCellCount = mFile.GetCellCount();
    mPlaying = FALSE;
    mHidden = TRUE;
    mUnk2B = FALSE;
    mUnk2C = FALSE;
    mUnk38 = -1;
    mAnime = -1;
    mTime = 0;
    mSpeed = 0x100;
    mPos.x = 0;
    mPos.y = 0;
    mUnk50 = 0;
    mUnk78 = NULL;
    mCells = new MMotionCell[mCellCount];
    mUnk64 = new MMotionAnimeState[mFile.GetAnimeCount()];
    if (mFlags & 1) {
        mPalettes = new MPalUnit *[mCellCount];
        for (u32 i = 0; i < mCellCount; i++) {
            if (mFile.GetCell(i)->pal16 != 0) {
                mPalettes[i] = mDisplay->mObj->mPal->Alloc(1, NULL, fromEnd);
            } else if (mFile.GetCell(i)->pal256 != 0) {
                mPalettes[i] = mDisplay->mObj->mExtPal->Alloc(1, NULL, fromEnd);
            } else {
                mPalettes[i] = NULL;
            }
        }
    } else {
        mPalettes = mFile.AcquirePalettes(mDisplay);
    }
    if (mFlags & 2) {
        mChrs = new MChrBlock *[mCellCount];
        for (u32 i = 0; i < mCellCount; i++) {
            if (mFile.GetCell(i)->chr16 != 0) {
                u32 b1;
                u32 b2;
                u32 tiles = mFile.GetCell(i)->chr16;
                MChr *chr = mDisplay->mObj->mChr;
                u32 size;
                if (chr->mMode == 0) {
                    b1 = chr->mBoundary;
                    size = b1 * ((tiles + b1 - 1) / b1);
                } else {
                    b2 = chr->mBoundary;
                    size = ((b2 + tiles * 2 - 1) / b2 * b2) >> 1;
                }
                mChrs[i] = chr->Alloc(0, size, fromEnd);
            } else if (mFile.GetCell(i)->chr256 != 0) {
                u32 tiles = mFile.GetCell(i)->chr256;
                MChr *chr = mDisplay->mObj->mChr;
                u32 boundary = chr->mBoundary;
                mChrs[i] = chr->Alloc(1, ((boundary + tiles * 2 - 1) / boundary * boundary) >> 1, fromEnd);
            } else {
                mChrs[i] = NULL;
            }
        }
    } else {
        mChrs = mFile.AcquireChrs(mDisplay);
    }
    mUnk2A = mFile.GetUnk04() != 0 && !(mFlags & 4) ? TRUE : FALSE;
    if (mFile.GetUnk04() != 0) {
        mAffines = new MAfnBlock *[mCellCount];
        for (u32 i = 0; i < mCellCount; i++) {
            mAffines[i] = NULL;
            if (!mUnk2A && mAffines[i] == NULL && (mFile.GetUnk04() & (1 << i))) {
                mAffines[i] = mDisplay->mObj->mAfn->Alloc(1, TRUE);
            }
        }
    }
    ResetAnime();
    SetPosition(0, 0);
}

MMotionCell::MMotionCell() {
}

MMotion::~MMotion() {
    SetOverride(TRUE);
    for (u32 i = 0; i < mCellCount; i++) {
        mEntries[i].objMode = 2;
    }
    if (mFlags & 1) {
        for (u32 i = 0; i < mCellCount; i++) {
            if (mPalettes[i] != NULL) {
                mPalettes[i]->pal->Free(mPalettes[i]);
            }
        }
        delete[] mPalettes;
    } else {
        mFile.ReleasePalettes(mDisplay);
    }
    if (mFlags & 2) {
        for (u32 i = 0; i < mCellCount; i++) {
            if (mChrs[i] != NULL) {
                mDisplay->mObj->mChr->FreeShared(mChrs[i]);
            }
        }
        delete[] mChrs;
    } else {
        mFile.ReleaseChrs(mDisplay);
    }
    if (mFile.GetUnk04() != 0) {
        for (u32 i = 0; i < mCellCount; i++) {
            if (mAffines[i] != NULL) {
                mDisplay->mObj->mAfn->Free(mAffines[i]);
                mAffines[i] = NULL;
            }
        }
        delete[] mAffines;
    }
    delete[] mUnk64;
    delete[] mCells;
    if (mOam != NULL && mCellCount != 0) {
        mOam->Free(mUnit);
    }
}

void MMotion::FreeAffine(u32 index) {
    if (mAffines[index] != NULL) {
        mDisplay->mObj->mAfn->Free(mAffines[index]);
        mAffines[index] = NULL;
    }
}

void MMotion::SetOverride(BOOL off) {
    if (off == (mUnk78 == NULL)) {
        return;
    }
    if (off) {
        delete[] mUnk78->animes;
        delete[] mUnk78->palettes;
        delete mUnk78;
        mUnk78 = NULL;
    } else {
        mUnk78 = new Override;
        mUnk78->animes = new MArchiveFile[mCellCount];
        mUnk78->palettes = new MArchiveFile[mCellCount];
    }
}

void MMotion::SetPosition(int x, int y) {
    SetPosition(MPoint(x, y));
}

void MMotion::SetPosition(MPoint pos) {
    mPos = pos;
    for (u32 i = 0; i < mCellCount; i++) {
        MMotionCell *cell = &mCells[i];
        MSmartOamCell *entry = &mEntries[cell->entry];
        int y = pos.y + cell->offsetY;
        entry->posX = pos.x;
        entry->posY = y;
        entry->depth = 0x7FFF - y;
    }
}

MPoint MMotion::GetPosition() {
    return mPos;
}

void MMotion::SetPriority(int priority) {
    for (u32 i = 0; i < mCellCount; i++) {
        mEntries[i].priority = priority;
    }
}

int MMotion::GetPriority() {
    return mEntries[0].priority;
}

void MMotion::SetAnchor(int anchor) {
    for (u32 i = 0; i < mCellCount; i++) {
        mEntries[i].links = (mEntries[i].links & 0x3FFFFFFF) | (anchor << 30);
    }
}

int MMotion::GetAnchor() {
    return (mEntries[0].links & 0xC0000000) >> 30;
}

void MMotion::SetFlip(u8 hflip, u8 vflip) {
    mUnk2B = hflip;
    mUnk2C = vflip;
}

void MMotion::Show() {
    mHidden = FALSE;
    for (u32 i = 0; i < mCellCount; i++) {
        MMotionCell *cell = &mCells[i];
        if (cell->visible) {
            int mode = cell->objMode;
            MSmartOamCell *entry = &mEntries[cell->entry];
            entry->objMode = mode;
            if (mode != 3) {
                MPoint size = gObjSizes[0][entry->GetShape()];
                entry->width = size.x;
                entry->height = size.y;
            } else {
                MPoint size = gObjSizes[0][entry->GetShape()];
                MPoint doubled(size.x * 2, size.y * 2);
                entry->width = doubled.x;
                entry->height = doubled.y;
            }
        }
    }
}

void MMotion::Hide() {
    mHidden = TRUE;
    for (u32 i = 0; i < mCellCount; i++) {
        mEntries[mCells[i].entry].objMode = 2;
    }
}

BOOL MMotion::IsHidden() {
    return mHidden;
}

MRect MMotion::GetFrame() {
    MRect frame(0x7FFFFFFF, 0x7FFFFFFF, 0x80000000, 0x80000000);
    for (u32 i = 0; i < mCellCount; i++) {
        if (mCells[i].visible) {
            frame.Unite(GetCellRect(i));
        }
    }
    if (frame.left == 0x7FFFFFFF && frame.top == 0x7FFFFFFF && frame.right == 0x80000000 && frame.bottom == 0x80000000) {
        return MRect(GetPosition(), GetPosition());
    }
    return frame;
}

void MMotion::ResetAnime() {
    mUnk40 = -1;
    mFrame = 0;
    mUnk38 = -1;
    for (u32 i = 0; i < mCellCount; i++) {
        mCells[i].entry = mCellCount - i - 1;
        mCells[i].unk04 = 0;
        mCells[i].visible = FALSE;
        mCells[i].objMode = 0;
        mCells[i].offsetY = 0;
        mCells[i].unk14 = MPoint(0, 0);
        MSmartOamCell *entry = &mEntries[i];
        entry->unk08 = 0;
        entry->unk0C = mUnk50;
        entry->SetCoord(mPos);
        entry->objMode = 2;
        entry->SetFlip(mUnk2B, mUnk2C);
    }
    if (mUnk2A) {
        for (u32 i = 0; i < mCellCount; i++) {
            if (mAffines[i] != NULL) {
                mDisplay->mObj->mAfn->Free(mAffines[i]);
                mAffines[i] = NULL;
            }
        }
    }
    for (u32 i = 0; i < mFile.GetAnimeCount(); i++) {
        mUnk64[i].Set(0, 0);
    }
}

MSmartOamUnit *MMotion::GetUnit() {
    return mUnit;
}

MPalUnit *MMotion::GetPalette(u32 index) {
    return mPalettes[index];
}

void *MMotion::GetAnimeData(int anime) {
    return mFile.GetAnime(anime, mUnk78 != NULL ? &mUnk78->file : NULL);
}

void MMotion::Play(int anime, int flags) {
    if (mAnime == anime && !(flags & 1) && (!(flags & 4) || mPlaying)) {
        return;
    }
    mAnime = anime;
    mScript = (const u16 *) GetAnimeData(anime);
    mPlaying = TRUE;
    u32 frame;
    if (flags & 2) {
        frame = mFrame;
    } else {
        frame = 0;
    }
    ResetAnime();
    Advance(frame);
}

BOOL MMotion::IsPlaying() {
    return mPlaying;
}

int MMotion::GetAnime() {
    return mAnime;
}

// Runs the anime's commands up to the next wait that outlasts the frames given.
void MMotion::Advance(u32 frames) {
    BOOL done = FALSE;
    struct {
        u32 pal;
        u32 chr;
    } changes[16];
    for (u32 i = 0; i < mCellCount; i++) {
        changes[i].pal = -1;
        changes[i].chr = -1;
    }
    do {
        u16 command = *mScript++;
        switch (command & 0xFF) {
        default:
            OS_Terminate();
            break;
        case 9:
            mScript += *(const s16 *) mScript;
            break;
        case 11:
            if (mUnk40 == -1) {
                mUnk40 = mFrame;
            } else {
                mFrame = mUnk40;
            }
            break;
        case 8: {
            u32 wait = command >> 8;
            if (wait == 0) {
                break;
            }
            mFramesLeft = wait;
            if (frames != 0) {
                if (frames >= mFramesLeft) {
                    frames -= mFramesLeft;
                    mFrame += mFramesLeft;
                } else {
                    mFramesLeft -= frames;
                    mFrame += frames;
                    done = TRUE;
                }
            } else {
                done = TRUE;
            }
            break;
        }
        case 12:
            mPlaying = FALSE;
            done = TRUE;
            if (mUnk2A) {
                for (u32 i = 0; i < mCellCount; i++) {
                    if (!mCells[i].visible) {
                        FreeAffine(i);
                    }
                }
            }
            break;
        case 5: {
            int x = *(const s16 *) mScript++;
            int y = *(const s16 *) mScript++;
            SetCellOffset(command >> 8, x, y);
            break;
        }
        case 15:
            SetCellOffsetY(command >> 8, *(const s16 *) mScript++);
            break;
        case 7: {
            int length = command >> 8;
            const char *data = (const char *) mScript;
            mScript = (const u16 *) ((const u8 *) mScript + length + 1);
            Command(data, length);
            break;
        }
        case 6: {
            mScript++;
            int a = *(const s16 *) mScript++;
            int b = *(const s16 *) mScript++;
            SetAnimeState(command >> 8, a, b);
            break;
        }
        case 10:
            Unk44(command >> 8);
            break;
        case 14:
            mUnk38 = command >> 8;
            Unk48(command >> 8);
            break;
        case 0:
            changes[command >> 8].pal = *mScript++;
            break;
        case 1:
            changes[command >> 8].chr = *mScript++;
            break;
        case 2:
            ShowCell(command >> 8);
            break;
        case 3:
            HideCell(command >> 8);
            break;
        case 4:
            SetCellAttr(command >> 8, *mScript++);
            break;
        case 13: {
            int left = *(const s16 *) mScript++ << 4;
            int top = *(const s16 *) mScript++ << 4;
            int right = *(const s16 *) mScript++ << 4;
            int bottom = *(const s16 *) mScript++ << 4;
            SetCellRect(command >> 8, MRect(left, top, right, bottom));
            break;
        }
        }
    } while (!done);
    for (u32 i = 0; i < mCellCount; i++) {
        if (changes[i].pal != -1) {
            SetCellPalette(i, changes[i].pal);
        }
        if (changes[i].chr != -1) {
            SetCellChr(i, changes[i].chr);
        }
    }
}

// Where the cell sits in the motion (mirrored with the motion's flips).
// NONMATCHING: register allocation (the game keeps this in r4 and the scaled index in r0) and when mCells is reloaded.
void MMotion::SetCellOffset(int cell, int x, int y) {
    mCells[cell].unk14 = MPoint(x, y);
    int ox = mCells[cell].unk14.x;
    int oy = mCells[cell].unk14.y;
    MSmartOamCell *entry = &mEntries[mCells[cell].entry];
    if (mUnk2B || mUnk2C) {
        int shape = entry->GetShape();
        int w = gObjSizes[0][shape].x;
        int h = gObjSizes[0][shape].y;
        if (mCells[cell].objMode == 3) {
            w *= 2;
            h *= 2;
        }
        if (mUnk2B) {
            ox = w - ox;
        }
        if (mUnk2C) {
            oy = h - oy;
        }
    }
    entry->unk08 = ox;
    entry->unk0C = oy + mUnk50;
}

void MMotion::SetCellOffsetY(int cell, int y) {
    mCells[cell].offsetY = y;
    int posY = mPos.y + y;
    MSmartOamCell *entry = &mEntries[mCells[cell].entry];
    entry->posX = mPos.x;
    entry->posY = posY;
    entry->depth = 0x7FFF - posY;
}

// A cell's entry, without its place in the smart OAM's lists (the low 10 bits of links, and the block).
static inline void CopyCellEntry(MSmartOamCell &to, const MSmartOamCell &from) {
    to.attr01 = from.attr01;
    to.attr2 = from.attr2;
    to.unk08 = from.unk08;
    to.unk0C = from.unk0C;
    to.posX = from.posX;
    to.posY = from.posY;
    to.width = from.width;
    to.height = from.height;
    to.links = (to.links & 0x3FF) | (from.links & ~0x3FF);
}

// Reorders the cells' entries: data lists the cells in their new order (the rest keep theirs).
void MMotion::Command(const char *data, int length) {
    // (Raw bytes: the entries aren't constructed.)
    u8 buf[sizeof(MSmartOamCell) * 16];
    MSmartOamCell *saved = (MSmartOamCell *) buf;
    u32 i;

    for (i = 0; i < mCellCount; i++) {
        CopyCellEntry(saved[i], mEntries[mCells[i].entry]);
    }
    for (i = 0; i < (u32) length; i++) {
        mCells[(u8) data[i]].entry = i;
    }
    for (; i < mCellCount; i++) {
        mCells[i].entry = i;
    }
    for (i = 0; i < mCellCount; i++) {
        CopyCellEntry(mEntries[mCells[i].entry], saved[i]);
    }
}

void MMotion::SetAnimeState(int index, int a, int b) {
    mUnk64[index].Set(a, b);
}

void MMotion::ShowCell(int cell) {
    mCells[cell].visible = TRUE;
    if (!mHidden) {
        MSmartOamCell *entry = &mEntries[mCells[cell].entry];
        int mode = mCells[cell].objMode;
        entry->objMode = mode;
        if (mode != 3) {
            int shape = entry->GetShape();
            entry->width = gObjSizes[0][shape].x;
            entry->height = gObjSizes[0][shape].y;
        } else {
            int shape = entry->GetShape();
            entry->width = gObjSizes[0][shape].x * 2;
            entry->height = gObjSizes[0][shape].y * 2;
        }
    }
}

void MMotion::HideCell(int cell) {
    mCells[cell].visible = FALSE;
    mEntries[mCells[cell].entry].objMode = 2;
}

void MMotion::SetCellRect(int cell, MRect rect) {
    MRect r = rect;
    MRect *p = &r;
    if (mUnk2B) {
        p->left = -p->left;
        p->top = -p->top;
    }
    if (mUnk2C) {
        p->right = -p->right;
        p->bottom = -p->bottom;
    }
    s16 *params = (s16 *) mAffines[cell]->addr;
    params[3] = r.left >> 4;
    params[7] = r.top >> 4;
    params[11] = r.right >> 4;
    params[15] = r.bottom >> 4;
}

// Changes the cell's graphics (and shape), then places it again for the new size.
// NONMATCHING: register allocation (the game keeps the cell's scaled index in a register and spills the cell).
void MMotion::SetCellChr(int cell, u32 chr) {
    MSmartOamCell *entry = &mEntries[mCells[cell].entry];
    const MMotionGfxHeader *data =
        (const MMotionGfxHeader *) mFile.GetChr(chr, mUnk78 != NULL ? &mUnk78->animes[cell] : NULL);
    mCells[cell].unk04 = chr;
    entry->colors256 = data->mode;
    if (mFlags & 2) {
        entry->SetChr(mChrs[cell]);
        entry->SetShape(data->extPalette);
        mChrs[cell]->Load(data + 1, 0, data->tiles, 0);
    } else {
        entry->SetChr(mChrs[chr]);
        entry->SetShape(data->extPalette);
    }
    int ox = mCells[cell].unk14.x;
    int oy = mCells[cell].unk14.y;
    entry = &mEntries[mCells[cell].entry];
    if (mUnk2B || mUnk2C) {
        int shape = entry->GetShape();
        int w = gObjSizes[0][shape].x;
        int h = gObjSizes[0][shape].y;
        if (mCells[cell].objMode == 3) {
            w *= 2;
            h *= 2;
        }
        if (mUnk2B) {
            ox = w - ox;
        }
        if (mUnk2C) {
            oy = h - oy;
        }
    }
    entry->unk08 = ox;
    entry->unk0C = oy + mUnk50;
}

void MMotion::SetCellPalette(int cell, u32 pal) {
    MSmartOamCell *entry = &mEntries[mCells[cell].entry];
    void *data = mFile.GetPalette(pal, mUnk78 != NULL ? &mUnk78->palettes[cell] : NULL);
    if (mFlags & 1) {
        entry->SetPalette(mPalettes[cell]);
        mPalettes[cell]->Load((u8 *) data + 4, 0, -1);
    } else {
        entry->SetPalette(mPalettes[pal]);
    }
}

void MMotion::Execute() {
    if (mPlaying) {
        mTime += mSpeed;
        u32 frames = mTime >> 8;
        if (frames >= mFramesLeft) {
            mTime = ((frames - mFramesLeft) << 8) | (mTime & 0xFF);
            Advance(frames - mFramesLeft);
            mFrame += frames;
        } else {
            mTime &= 0xFF;
            mFramesLeft -= frames;
            mFrame += frames;
        }
    }
}

void MMotion::Unk44(int arg) {
}

void MMotion::Unk48(int arg) {
}

void MMotion::SetSize(MPoint size) {
    OS_Terminate();
}

MPoint MMotion::GetSize() {
    return GetFrame().GetSize();
}

u32 MMotion::GetPlaneMask() {
    return 0x10;
}

MTask *MMotion::CreateWaitStopTask() {
    return new MMotionWaitStopTask(this);
}

MTask *MMotion::CreatePlayTransfer(int anime, int flags) {
    return new MMotionPlayTransfer(this, anime, flags);
}

void MMotionPlayTransfer::Transfer() {
    mMotion->Play(mAnime, mFlags);
}

void MMotionWaitStopTask::Execute() {
    if (!mMotion->IsPlaying()) {
        Exit();
    }
}

#pragma auto_inline off
// Vector members the game emits at the end of this file (made explicitly until the code using them is decompiled).
template void std::__vec_deleter<MRect, std::allocator<MRect> >::append(size_t n, Metrowerks::int2type<true>);
template void std::__vec_deleter<MRect, std::allocator<MRect> >::append_realloc(size_t n);
template void std::__vec_deleter<MArchiveFile, std::allocator<MArchiveFile> >::append_realloc(size_t n, const MArchiveFile &x);
template void std::__vec_deleter<MArchiveFile, std::allocator<MArchiveFile> >::allocate(size_t n, Metrowerks::int2type<true>);
template void std::__vec_deleter<MArchiveFile, std::allocator<MArchiveFile> >::erase_at_end(size_t n, Metrowerks::int2type<false>);
