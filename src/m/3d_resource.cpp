// The 3D resources: the 3D heap, the model, animation and texture caches, and M3DModelResource.
// Not decompiled yet: M3DGrid::Load.

#include <ctype.h>
#include <map>
#include <scanf.h>
#include "m/3d_resource.h"
#include "m/3d_model.h"
#include "m/3d_scene.h"
#include "m/file.h"
#include "m/heap.h"
#include "nitro/mi.h"

// A loaded file, and how many hold it.
struct M3DCacheEntry {
    int mRefs;      // 0x0
    void *mData;    // 0x4
};

// A loaded texture file, and its texture and palette once uploaded.
struct M3DTexEntry {
    int mRefs;          // 0x00
    void *mData;        // 0x04
    u32 mTexKey;        // 0x08: -1 until uploaded
    u32 mPlttKey;       // 0x0C
    u32 mPaletteSize;   // 0x10
    u8 *mPalette;       // 0x14: a copy of the palette, if asked for
    M3DScene *mScene;   // 0x18
};

// A texture file: the sizes of the texture and palette that follow its entries.
struct M3DTexHeader {
    struct Entry {
        u32 unk[7];
    };

    int mCount;         // 0x00
    u32 mTexSize;       // 0x04
    u32 mPlttSize;      // 0x08
    Entry mEntries[1];  // 0x0C
};

typedef std::map<MArchiveFile, M3DCacheEntry> M3DCache;
typedef std::map<MArchiveFile, M3DTexEntry> M3DTexCache;

static MHeap *sHeap;
static M3DTexCache sTexCache;
static M3DCache sModelCache;
static M3DCache sAnimCache;
static MMutex sMutex;

// Holds the caches' mutex for the rest of the scope.
class M3DLock {
public:
    M3DLock() { OS_LockMutex(&sMutex.mutex); }
    ~M3DLock() { OS_UnlockMutex(&sMutex.mutex); }

    u32 unk[2];
};

M3DGrid::M3DGrid() {
    mDim = 32;
    mCells = new std::vector<u16>[mDim * mDim];
}

M3DGrid::~M3DGrid() {
    u32 count = mDim * mDim;
    for (u32 i = 0; i < count; i++) {
        mCells[i].clear();
    }
    mVerts.clear();
    delete[] mCells;
}

// Whether (x, z) is inside the triangle tris[i], tris[i + 1], tris[i + 2].
#define M3D_GRID_IN_TRIANGLE(verts, tris, i, x, z, miss)                                                         \
    {                                                                                                           \
        int x0 = verts[tris[i]].x - x;                                                                          \
        int z0 = verts[tris[i]].z - z;                                                                          \
        int x1 = verts[tris[i + 1]].x - x;                                                                      \
        int z1 = verts[tris[i + 1]].z - z;                                                                      \
        int x2 = verts[tris[i + 2]].x - x;                                                                      \
        int z2 = verts[tris[i + 2]].z - z;                                                                      \
        if (x0 < 0 && x1 < 0 && x2 < 0) goto miss;                                                              \
        if (x0 > 0 && x1 > 0 && x2 > 0) goto miss;                                                              \
        if (z0 < 0 && z1 < 0 && z2 < 0) goto miss;                                                              \
        if (z0 > 0 && z1 > 0 && z2 > 0) goto miss;                                                              \
        if (x0 * z1 - x1 * z0 > 0) goto miss;                                                                   \
        if (z0 * x2 - x0 * z2 > 0) goto miss;                                                                   \
        if (x1 * z2 - x2 * z1 > 0) goto miss;                                                                   \
    }

// The height of the highest triangle under (x, z), or the lowest fx32 if there's none.
// NONMATCHING: register allocation and the order of the loop's spill slots.
fx32 M3DGrid::GetHeight(fx32 px, fx32 pz) {
    int best;
    std::vector<u16> *tris;
    int i;
    int ya, yb, za, zb;
    int x0, y0, z0, x1, y1, z1, x2, y2, z2;
    int x, z;
    int cx, cz;
    int h;
    best = -0x8000;
    x = px >> mShiftX;
    z = pz >> mShiftZ;
    cx = (x >> 11) + 16;
    cz = (z >> 11) + 16;
    if (cx < 0) {
        return best;
    }
    if (cx >= mDim) {
        return best;
    }
    if (cz < 0) {
        return best;
    }
    if (cz >= mDim) {
        return best;
    }
    tris = &mCells[cx + cz * mDim];
    for (i = 0; i < tris->size(); i += 3) {
        x0 = mVerts[(*tris)[i]].x - x;
        y0 = mVerts[(*tris)[i]].y;
        z0 = mVerts[(*tris)[i]].z - z;
        x1 = mVerts[(*tris)[i + 1]].x - x;
        y1 = mVerts[(*tris)[i + 1]].y;
        z1 = mVerts[(*tris)[i + 1]].z - z;
        x2 = mVerts[(*tris)[i + 2]].x - x;
        y2 = mVerts[(*tris)[i + 2]].y;
        z2 = mVerts[(*tris)[i + 2]].z - z;
        if (x0 < 0 && x1 < 0 && x2 < 0) continue;
        if (x0 > 0 && x1 > 0 && x2 > 0) continue;
        if (z0 < 0 && z1 < 0 && z2 < 0) continue;
        if (z0 > 0 && z1 > 0 && z2 > 0) continue;
        if (x0 * z1 - x1 * z0 > 0) continue;
        if (z0 * x2 - x0 * z2 > 0) continue;
        if (x1 * z2 - x2 * z1 > 0) continue;
        if (x0 == x1 && x0 == x2) {
            h = y0 + -x0 * (y1 - y0) / (z1 - z0);
        } else {
            int a = x1 - x0;
            if (a < 0) {
                a = -a;
            }
            int b = x2 - x0;
            if (b < 0) {
                b = -b;
            }
            int c = x2 - x1;
            if (c < 0) {
                c = -c;
            }
            if (a > c && b > c) {
                ya = y0 + -x0 * (y1 - y0) / (x1 - x0);
                yb = y0 + -x0 * (y2 - y0) / (x2 - x0);
                za = z0 + -x0 * (z1 - z0) / (x1 - x0);
                zb = z0 + -x0 * (z2 - z0) / (x2 - x0);
            } else if (b > a && c > a) {
                ya = y2 + -x2 * (y1 - y2) / (x1 - x2);
                yb = y2 + -x2 * (y0 - y2) / (x0 - x2);
                za = z2 + -x2 * (z1 - z2) / (x1 - x2);
                zb = z2 + -x2 * (z0 - z2) / (x0 - x2);
            } else {
                ya = y1 + -x1 * (y0 - y1) / (x0 - x1);
                yb = y1 + -x1 * (y2 - y1) / (x2 - x1);
                za = z1 + -x1 * (z0 - z1) / (x0 - x1);
                zb = z1 + -x1 * (z2 - z1) / (x2 - x1);
            }
            h = ya + -za * (yb - ya) / (zb - za);
        }
        if (best < h) {
            best = h;
        }
    }
    if (best == -0x8000) {
        return best << 16;
    }
    return best << mShiftX;
}

// Whether (x, z) is on a triangle of the grid; hint caches the last hit (cell << 16 | first index).
// NONMATCHING: register allocation (the game keeps this in r6 and the cell on the stack).
BOOL M3DGrid::IsOnGround(fx32 x, fx32 z, fx32 *hint) {
    x >>= mShiftX;
    z >>= mShiftZ;
    int cx = (x >> 11) + 16;
    int cz = (z >> 11) + 16;
    if (cx < 0) {
        return FALSE;
    }
    if (cx >= mDim) {
        return FALSE;
    }
    if (cz < 0) {
        return FALSE;
    }
    if (cz >= mDim) {
        return FALSE;
    }
    u32 cell = cx + cz * mDim;
    if (*(u32 *) hint >> 16 == cell) {
        u32 i = *(u32 *) hint & 0xFFFF;
        std::vector<u16> &tris = mCells[*(u32 *) hint >> 16];
        M3D_GRID_IN_TRIANGLE(mVerts, tris, i, x, z, scan);
        *(u32 *) hint = (u16) i | (cell << 16);
        return TRUE;
    }
scan:
    std::vector<u16> &tris = mCells[cell];
    for (u32 i = 0; i < tris.size(); i += 3) {
        M3D_GRID_IN_TRIANGLE(mVerts, tris, i, x, z, next);
        *(u32 *) hint = (u16) i | (cell << 16);
        return TRUE;
    next:;
    }
    return FALSE;
}

inline bool operator<(const MArchiveFile &a, const MArchiveFile &b) {
    if (&b == &a) {
        return false;
    }
    if ((u32) a.archive < (u32) b.archive) {
        return true;
    }
    if ((u32) a.archive > (u32) b.archive) {
        return false;
    }
    if (a.index < b.index) {
        return true;
    }
    if (a.index > b.index) {
        return false;
    }
    return a.locked < b.locked;
}

void *M3DLoadModel(MArchiveFile file) {
    MScopedLock lock(&sMutex.mutex, TRUE);
    M3DCache::iterator it = sModelCache.find(file);
    if (it != sModelCache.end()) {
        it->second.mRefs++;
        return it->second.mData;
    }
    void *data = (void *) sHeap->AllocAddr(file.GetSize(), FALSE, 4, 0, 0, 0);
    file.archive->Read(file.index, 0, data, file.GetSize(), NULL);
    M3DCacheEntry &entry = sModelCache[file];
    entry.mRefs = 1;
    entry.mData = data;
    return data;
}

void M3DReleaseModel(MArchiveFile file) {
    MScopedLock lock(&sMutex.mutex, TRUE);
    M3DCache::iterator it = sModelCache.find(file);
    if (--it->second.mRefs <= 0) {
        sHeap->Free((u32) it->second.mData, FALSE);
        sModelCache.erase(M3DCache::iterator(it.node_));
    }
}

void *M3DLoadAnim(MArchiveFile file) {
    MScopedLock lock(&sMutex.mutex, TRUE);
    M3DCache::iterator it = sAnimCache.find(file);
    if (it != sAnimCache.end()) {
        it->second.mRefs++;
        return it->second.mData;
    }
    void *data = (void *) sHeap->AllocAddr(file.GetSize(), FALSE, 4, 0, 0, 0);
    file.archive->Read(file.index, 0, data, file.GetSize(), NULL);
    M3DCacheEntry &entry = sAnimCache[file];
    entry.mRefs = 1;
    entry.mData = data;
    return data;
}

void M3DReleaseAnim(MArchiveFile file) {
    MScopedLock lock(&sMutex.mutex, TRUE);
    M3DCache::iterator it = sAnimCache.find(file);
    if (--it->second.mRefs <= 0) {
        sHeap->Free((u32) it->second.mData, FALSE);
        sAnimCache.erase(M3DCache::iterator(it.node_));
    }
}

void *M3DLoadTex(MArchiveFile file) {
    MScopedLock lock(&sMutex.mutex, TRUE);
    M3DTexCache::iterator it = sTexCache.find(file);
    if (it != sTexCache.end()) {
        it->second.mRefs++;
        return it->second.mData;
    }
    // Read the header with up to two entries first, then all of it if there are more.
    u32 size = sizeof(M3DTexHeader) + sizeof(M3DTexHeader::Entry);
    if (file.GetSize() < size) {
        size = file.GetSize();
    }
    M3DTexHeader *data = (M3DTexHeader *) sHeap->AllocAddr(size, FALSE, 4, 0, 0, 0);
    file.archive->Read(file.index, 0, data, size, NULL);
    if (data->mCount > 2) {
        size = (data->mCount - 1) * sizeof(M3DTexHeader::Entry) + sizeof(M3DTexHeader);
        sHeap->Free((u32) data, FALSE);
        data = (M3DTexHeader *) sHeap->AllocAddr(size, FALSE, 4, 0, 0, 0);
        file.archive->Read(file.index, 0, data, size, NULL);
    }
    DC_FlushRange(data, file.GetSize());
    M3DTexEntry entry;
    entry.mRefs = 1;
    entry.mData = data;
    entry.mTexKey = (u32) -1;
    entry.mPlttKey = (u32) -1;
    entry.mPalette = NULL;
    sTexCache[file] = entry;
    return data;
}

void M3DUploadTex(MArchiveFile file, u32 *texKey, u32 *plttKey, M3DModel *model, u8 keepPalette) {
    MScopedLock lock(&sMutex.mutex, TRUE);
    M3DTexCache::iterator it = sTexCache.find(file);
    M3DTexEntry *entry = &it->second;
    if (entry->mTexKey == (u32) -1) {
        M3DTexHeader *data =
            (M3DTexHeader *) sHeap->AllocAddr(file.GetSize(), FALSE, 4, 0, 0, 0);
        file.archive->Read(file.index, 0, data, file.GetSize(), NULL);
        u8 *tex = (u8 *) &data->mEntries[data->mCount];
        DC_FlushRange(data, file.GetSize());
        entry->mScene = model->mScene;
        entry->mTexKey = model->mScene->AllocTex(tex, data->mTexSize);
        entry->mPlttKey = model->mScene->AllocPltt(tex + data->mTexSize, data->mPlttSize);
        if (keepPalette) {
            entry->mPalette = new u8[data->mPlttSize];
            entry->mPaletteSize = data->mPlttSize;
            MI_CpuCopy8(tex + data->mTexSize, entry->mPalette, entry->mPaletteSize);
        }
        model->mScene->QueueE8(sHeap, (u32) data);
    }
    *texKey = entry->mTexKey;
    *plttKey = entry->mPlttKey;
}

void M3DReleaseTex(MArchiveFile file) {
    MScopedLock lock(&sMutex.mutex, TRUE);
    M3DTexCache::iterator it = sTexCache.find(file);
    if (--it->second.mRefs <= 0) {
        if (it->second.mTexKey != (u32) -1) {
            if (it->second.mPalette != NULL) {
                delete[] it->second.mPalette;
            }
            it->second.mScene->FreeTex(it->second.mTexKey);
            it->second.mScene->FreePltt(it->second.mPlttKey);
        }
        sHeap->Free((u32) it->second.mData, FALSE);
        sTexCache.erase(M3DTexCache::iterator(it.node_));
    }
}

void M3DInitHeap(u32 base, u32 size) {
    sHeap = new MHeap(base, size, 0xC);
    sHeap->SetUnk34(1);
}

void M3DDestroyHeap() {
    if (sHeap != NULL) {
        delete sHeap;
    }
    sHeap = NULL;
    if (sModelCache.size() != 0 || sAnimCache.size() != 0 || sTexCache.size() != 0) {
        OS_Terminate();
    }
}

M3DHeap::M3DHeap(u32 base, u32 size) {
    mBuffer = NULL;
    M3DInitHeap(base, size);
}

M3DHeap::M3DHeap() {
    mBuffer = new u8[0];
    M3DInitHeap((u32) mBuffer, 0);
}

M3DHeap::~M3DHeap() {
    M3DDestroyHeap();
    if (mBuffer != NULL) {
        delete[] mBuffer;
    }
}

void M3DHeap::Execute() {
}

BOOL M3DModelExists(std::string path) {
    for (u32 i = 0; i < path.size(); i++) {
        path[i] = tolower(path[i]);
    }
    int species;
    int variant;
    if (sscanf(path.c_str(), "/model/battle/din%03d_%02d", &species, &variant) != 2) {
        return MFileExists(path);
    }
    MResource<M3DDinTable> table("/model/battle/arcdin");
    return table.Get()->GetEntry(species, variant)->mExists;
}

M3DModelResource::M3DModelResource(std::string path) : mPath(path) {
    for (u32 i = 0; i < mPath.size(); i++) {
        mPath[i] = tolower(mPath[i]);
    }
    int species;
    int variant;
    if (sscanf(mPath.c_str(), "/model/battle/din%03d_%02d", &species, &variant) != 2) {
        mArchive = MResource<M3DModelFiles>(mPath);
        mModelFile = mArchive.Get()->GetModelFile(mArchive.mArchive);
        mAnimFile = mArchive.Get()->GetAnimFile(mArchive.mArchive);
        mTexFile = mArchive.Get()->GetTexFile(mArchive.mArchive);
    } else {
        mDinTable = MResource<M3DDinTable>("/model/battle/arcdin");
        const M3DDinEntry *entry = mDinTable.Get()->GetEntry(species, variant);
        mModelFile = entry->GetModelFile(mDinTable.mArchive);
        mAnimFile = entry->GetAnimFile(mDinTable.mArchive);
        mTexFile = entry->GetTexFile(mDinTable.mArchive);
    }
    mModel = M3DLoadModel(mModelFile);
    mAnim = M3DLoadAnim(mAnimFile);
    mTex = M3DLoadTex(mTexFile);
}

M3DModelResource::~M3DModelResource() {
    if (mModel != NULL) {
        M3DReleaseModel(mModelFile);
        M3DReleaseAnim(mAnimFile);
        M3DReleaseTex(mTexFile);
    }
}

M3DModelResource &M3DModelResource::operator=(const M3DModelResource &other) {
    if (mModel != NULL) {
        M3DReleaseModel(mModelFile);
        M3DReleaseAnim(mAnimFile);
        M3DReleaseTex(mTexFile);
        mModel = mAnim = mTex = NULL;
    }
    mArchive = other.mArchive;
    mDinTable = other.mDinTable;
    mPath = other.mPath;
    mModelFile = other.mModelFile;
    mAnimFile = other.mAnimFile;
    mTexFile = other.mTexFile;
    if (other.mModel != NULL) {
        mModel = M3DLoadModel(mModelFile);
        mAnim = M3DLoadAnim(mAnimFile);
        mTex = M3DLoadTex(mTexFile);
    }
    return *this;
}

void M3DModelResource::Setup(M3DModel *model, u8 keepPalette) {
    model->SetModelData(mModel, FALSE);
    model->SetAnimData(mAnim);
    M3DUploadTex(mTexFile, &mTexKey, &mPlttKey, model, keepPalette);
    model->SetTexture((u32) mTex, mTexKey, mPlttKey);
}

void M3DModelResource::GetPalette(u32 *plttKey, u8 **palette, u32 *size) {
    M3DLock lock;
    M3DTexCache::iterator it = sTexCache.find(mTexFile);
    M3DTexEntry &entry = it->second;
    *plttKey = entry.mPlttKey;
    *palette = entry.mPalette;
    *size = entry.mPaletteSize;
}

#pragma auto_inline off
template class std::__vec_deleter<u16, std::allocator<u16> >;
template class std::__vec_deleter<M3DVec16, std::allocator<M3DVec16> >;
