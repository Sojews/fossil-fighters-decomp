// Motions: animated OBJ graphics loaded from a motion file, shown as a view.

#include "m/motion.h"
#include <map>
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

void *MMotion::GetAnimeData(int anime) {
    return mFile.GetAnime(anime, mUnk78 != NULL ? (MArchiveFile *) mUnk78->file : NULL);
}

void MMotion::Play(int anime, int flags) {
    if (mAnime == anime && !(flags & 1) && (!(flags & 4) || mPlaying)) {
        return;
    }
    mAnime = anime;
    mUnk74 = GetAnimeData(anime);
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

void MMotion::Unk44() {
}

void MMotion::Unk48() {
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
