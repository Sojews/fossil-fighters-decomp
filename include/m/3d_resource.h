// The 3D resources: a heap for model data, caches of the models, animations and textures loaded from archives
// (shared by file and counted), and the model resources that hold one of each.
#ifndef M_3D_RESOURCE_H
#define M_3D_RESOURCE_H

#include <string>
#include <vector>
#include "m/archive.h"
#include "m/task.h"

class M3DModel;

struct M3DVec16 {
    s16 x;
    s16 y;
    s16 z;
};

// A model's triangles (as indices into its vertices) sorted into a grid of cells over the XZ plane, to find the
// ground under a point.
class M3DGrid {
public:
    M3DGrid();
    ~M3DGrid();

    std::vector<M3DVec16> mVerts;   // 0x00
    std::vector<u16> *mCells;       // 0x0C: mDim * mDim cells, three indices per triangle
    u32 mDim;                       // 0x10
    int mShiftX;                    // 0x14
    int mShiftZ;                    // 0x18
};

// Sets up the 3D heap for as long as it exists.
class M3DHeap : public MTask {
public:
    M3DHeap();
    M3DHeap(u32 base, u32 size);
    virtual ~M3DHeap();
    virtual void Execute();

    u8 *mBuffer;    // 0x10
};

void M3DInitHeap(u32 base, u32 size);
void M3DDestroyHeap();
void *M3DLoadModel(MArchiveFile file);
void M3DReleaseModel(MArchiveFile file);
void *M3DLoadAnim(MArchiveFile file);
void M3DReleaseAnim(MArchiveFile file);
void *M3DLoadTex(MArchiveFile file);
// Uploads a loaded texture file's texture and palette for the model's scene (once) and gets their keys.
void M3DUploadTex(MArchiveFile file, u32 *texKey, u32 *plttKey, M3DModel *model, u8 keepPalette);
void M3DReleaseTex(MArchiveFile file);
// Whether there's a model at a path: battle models are listed in a table, the others are files.
BOOL M3DModelExists(std::string path);

// Where a model's files are: an index and a path (an offset from here) for each.
struct M3DModelFiles {
    MArchiveFile GetModelFile(const MArchivePtr &archive) const {
        return archive.GetFile((const char *) this + mModelPath, mModelIndex);
    }
    MArchiveFile GetAnimFile(const MArchivePtr &archive) const {
        return archive.GetFile((const char *) this + mAnimPath, mAnimIndex);
    }
    MArchiveFile GetTexFile(const MArchivePtr &archive) const {
        return archive.GetFile((const char *) this + mTexPath, mTexIndex);
    }

    u32 mUnk0;          // 0x00
    u32 mModelIndex;    // 0x04
    u32 mModelPath;     // 0x08
    u32 mAnimIndex;     // 0x0C
    u32 mAnimPath;      // 0x10
    u32 mTexIndex;      // 0x14
    u32 mTexPath;       // 0x18
};

// A battle model's entry in the table: whether it exists, and its files.
struct M3DDinEntry {
    MArchiveFile GetModelFile(const MArchivePtr &archive) const {
        return archive.GetFile((const char *) this + mModelPath, mModelIndex);
    }
    MArchiveFile GetAnimFile(const MArchivePtr &archive) const {
        return archive.GetFile((const char *) this + mAnimPath, mAnimIndex);
    }
    MArchiveFile GetTexFile(const MArchivePtr &archive) const {
        return archive.GetFile((const char *) this + mTexPath, mTexIndex);
    }

    u8 mExists;         // 0x00
    u32 mModelIndex;    // 0x04
    u32 mModelPath;     // 0x08
    u32 mAnimIndex;     // 0x0C
    u32 mAnimPath;      // 0x10
    u32 mTexIndex;      // 0x14
    u32 mTexPath;       // 0x18
};

// A model archive held open, with its first file (the model's M3DModelFiles) loaded.
class M3DModelArchive {
public:
    M3DModelArchive() {}
    M3DModelArchive(std::string path) : mArchive(path) {
        mFile = mArchive.GetFile(0);
        mFile.Lock();
        mFile.GetData();
    }

    const M3DModelFiles *GetFiles() { return (const M3DModelFiles *) mFile.GetData(); }

    MArchivePtr mArchive;   // 0x00
    MArchiveFile mFile;     // 0x04
};

// The battle model table archive held open, with its first file (the table) loaded.
class M3DDinTable {
public:
    M3DDinTable() {}
    M3DDinTable(std::string path) : mArchive(path) {
        mFile = mArchive.GetFile(0);
        mFile.Lock();
        mFile.GetData();
    }

    const M3DDinEntry *GetEntry(int species, int variant) {
        const u8 *data = (const u8 *) mFile.GetData();
        const u8 *group = data + ((const u32 *) (data + ((const u32 *) data)[2]))[species];
        return (const M3DDinEntry *) (group + ((const u32 *) (group + ((const u32 *) group)[1]))[variant]);
    }

    MArchivePtr mArchive;   // 0x00
    MArchiveFile mFile;     // 0x04
};

// A model's files, loaded through the caches, by path.
class M3DModelResource {
public:
    M3DModelResource(std::string path);
    ~M3DModelResource();
    M3DModelResource &operator=(const M3DModelResource &other);

    void Setup(M3DModel *model, u8 keepPalette);
    void GetPalette(u32 *plttKey, u8 **palette, u32 *size);

    M3DModelArchive mArchive;   // 0x00
    M3DDinTable mDinTable;      // 0x10
    MArchiveFile mModelFile;    // 0x20
    MArchiveFile mAnimFile;     // 0x2C
    MArchiveFile mTexFile;      // 0x38
    std::string mPath;          // 0x44
    void *mModel;               // 0x50
    void *mAnim;                // 0x54
    void *mTex;                 // 0x58
    u32 mTexKey;                // 0x5C
    u32 mPlttKey;               // 0x60
};

#endif
