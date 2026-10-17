// Motions: animated OBJ graphics loaded from a motion file, shown as a view.

#include "m/motion.h"
#include <map>
#include "m/display.h"
#include "m/heap.h"
#include "m/obj.h"

static std::map<FSFileID, MMotionData *> sMotionDatas;
static OSMutex sMotionDatasMutex;

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
        sMotionDatas.erase(mArchive.mArchive->mId);
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

void *MMotionFile::GetCell(u32 index) {
    MMotionHeader *header = (MMotionHeader *) mData->mFile.GetData();
    return (u8 *) header + header->cellOffset + index * 16;
}

u32 MMotionFile::GetUnk18() {
    return ((u32 *) mData->mFile.GetData())[6];
}

MPalBlock **MMotionFile::AcquirePalettes(MDisplay *display) {
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

void MMotion::SetPosition(MPoint pos) {
    mPos = pos;
    for (u32 i = 0; i < mCellCount; i++) {
        MMotionCell *cell = &mCells[i];
        MSmartOamEntry *entry = &mEntries[cell->entry];
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
            MSmartOamEntry *entry = &mEntries[cell->entry];
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

MSmartOamUnit *MMotion::GetUnit() {
    return mUnit;
}

MPalBlock *MMotion::GetPalette(u32 index) {
    return mPalettes[index];
}

inline void MMotion::FreeAffine(u32 index) {
    if (mAffines[index] != NULL) {
        mDisplay->mObj->mAfn->Free(mAffines[index]);
        mAffines[index] = NULL;
    }
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
        MSmartOamEntry *entry = &mEntries[i];
        entry->unk08 = 0;
        entry->unk0C = mUnk50;
        entry->SetCoord(mPos);
        entry->objMode = 2;
        entry->SetFlip(mUnk2B, mUnk2C);
    }
    if (mUnk2A) {
        for (u32 i = 0; i < mCellCount; i++) {
            FreeAffine(i);
        }
    }
    for (u32 i = 0; i < mFile.GetAnimeCount(); i++) {
        mUnk64[i] = MMotionAnimeState(0, 0);
    }
}

void *MMotion::GetAnimeData(int anime) {
    return mFile.GetAnime(anime, mUnk78 != NULL ? (MArchiveFile *) mUnk78->file : NULL);
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
        u32 chr;
        u32 pal;
    } changes[16];
    for (u32 i = 0; i < mCellCount; i++) {
        changes[i].chr = -1;
        changes[i].pal = -1;
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
            changes[command >> 8].chr = *mScript++;
            break;
        case 1:
            changes[command >> 8].pal = *mScript++;
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
        if (changes[i].chr != -1) {
            SetCellChr(i, changes[i].chr);
        }
        if (changes[i].pal != -1) {
            SetCellPalette(i, changes[i].pal);
        }
    }
}

void MMotion::SetCellOffsetY(int cell, int y) {
    mCells[cell].offsetY = y;
    int posY = mPos.y + y;
    MSmartOamEntry *entry = &mEntries[mCells[cell].entry];
    entry->posX = mPos.x;
    entry->posY = posY;
    entry->depth = 0x7FFF - posY;
}

void MMotion::SetAnimeState(int index, int a, int b) {
    mUnk64[index] = MMotionAnimeState(a, b);
}

void MMotion::ShowCell(int cell) {
    mCells[cell].visible = TRUE;
    if (!mHidden) {
        MSmartOamEntry *entry = &mEntries[mCells[cell].entry];
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
