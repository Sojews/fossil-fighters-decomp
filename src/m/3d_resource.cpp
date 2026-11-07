// The 3D resources: the 3D heap, the model, animation and texture caches, and M3DModelResource.

#pragma ipa file

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

// The caches' destructors, which __sinit registers.
extern "C" inline M3DCache *_ZNSt3mapI12MArchiveFile13M3DCacheEntrySt4lessIS0_ESaISt4pairIKS0_S1_EEED1Ev(
    M3DCache *self) {
    self->~M3DCache();
    return self;
}
extern "C" inline M3DTexCache *_ZNSt3mapI12MArchiveFile11M3DTexEntrySt4lessIS0_ESaISt4pairIKS0_S1_EEED1Ev(
    M3DTexCache *self) {
    self->~M3DTexCache();
    return self;
}
extern "C" void *__register_global_object(void *object, void *destructor, void *link);

// The game constructs the model cache, the anim cache, the texture cache, then the mutex, but lays .bss out as
// the texture cache, the model cache, then the anim cache. No declaration order of ordinary statics gives both, so
// the caches and the mutex have no constructors and are set up by sStaticInit (at the end of the file). This
// declaration order gives the game's layout (the anim, model, then texture cache's destructor-chain link).
// static_order: begin
static MHeap *sHeap;
static struct { u32 w[3]; } sModelLink;
static struct { u32 w[3]; } sAnimLink;
static struct { u32 w[3]; } sTexLink;
static struct { u32 w[4]; } sTexStore;
static struct { u32 w[4]; } sModelStore;
static struct { u32 w[4]; } sAnimStore;
static struct { OSMutex mutex; } sMutex;
// static_order: end
#define sModelCache (*(M3DCache *) &sModelStore)
#define sAnimCache (*(M3DCache *) &sAnimStore)
#define sTexCache (*(M3DTexCache *) &sTexStore)
// A cache's end(), from its header's own address (through the cast the compiler adds 4 to the cache's address).
#define M3D_CACHE_END(type, store) type::iterator((std::__red_black_tree_common<true>::node_base *) &store.w[1])

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

// Widens the cell bounds to the last n vertices'.
#define M3D_GRID_BOUND(n, minX, minZ, maxX, maxZ)                                                               \
    for (int v = count - (n); v < count; v++) {                                                                 \
        int vx = mVerts[v].x >> 11;                                                                             \
        int vz = mVerts[v].z >> 11;                                                                             \
        if (minX > vx) {                                                                                        \
            minX = vx;                                                                                          \
        }                                                                                                       \
        if (minZ > vz) {                                                                                        \
            minZ = vz;                                                                                          \
        }                                                                                                       \
        if (maxX < vx) {                                                                                        \
            maxX = vx;                                                                                          \
        }                                                                                                       \
        if (maxZ < vz) {                                                                                        \
            maxZ = vz;                                                                                          \
        }                                                                                                       \
    }

// The cell (x, z), from (-16, -16).
#define M3D_GRID_CELL(x, z) mCells[(x) + 16 + ((z) + 16) * mDim]

// Builds the grid from a display list: the vertices it draws (scaled up as far as they fit in 16 bits) and, in each
// cell, the triangles over it. The list ends at its first matrix pop.
// NONMATCHING (99.35%): which zero the game reuses for the flags (the rescale flag, then each command's vertex flag
// from it), the list pointer's spill, and the 10-bit vertex's schedule. The C89 declaration order below gives the
// game's stack slots (named locals take the top slots in declaration order); the strip parity is a named local (its
// slot sits between minCX2 and maxCX2), set after the row loop's guard; maxX being an s32 gives the bounds' registers.
void M3DGrid::Load(const void *data) {
    int type;
    int strip;
    int count;
    int shift;
    int shiftZ;
    const u8 *cmds;
    const u32 *p;
    int c;
    int minCZ0;
    int cx0;
    int minCZ1;
    int cx1;
    int minCZ2;
    int cx2;
    int minCZ3;
    int cx3;
    int minZ;
    int minY;
    int minX;
    int maxZ;
    int maxY;
    s32 maxX;
    int x, y, z;
    int minCX3;
    int maxCX3;
    int minCX2;
    int odd;
    int maxCX2;
    int minCX1;
    int maxCX1;
    int minCX0;
    int maxCX0;
    int maxCZ0;
    int maxCZ1;
    int maxCZ2;
    int maxCZ3;
    BOOL again;
    minZ = 0;
    minY = 0;
    minX = 0;
    maxZ = 0;
    maxY = 0;
    shift = 0;
    shiftZ = 0;
    maxX = 0;
    do {
        cmds = (const u8 *)data;
        p = (const u32 *)data;
        u32 cells = mDim * mDim;
        for (u32 i = 0; i < cells; i++) {
            mCells[i].clear();
        }
        mVerts.clear();
        strip = 0;
        count = 0;
        type = -1;
        p++;
        again = FALSE;
        for (;;) {
            for (c = 0; c < 4; c++) {
                BOOL vertex = FALSE;
                switch (cmds[c]) {
                case 0x00:
                case 0x11:
                case 0x15:
                case 0x41:
                    break;
                case 0x10:
                case 0x13:
                case 0x14:
                case 0x20:
                case 0x21:
                case 0x22:
                case 0x29:
                case 0x2A:
                case 0x2B:
                case 0x30:
                case 0x31:
                case 0x32:
                case 0x33:
                case 0x50:
                case 0x60:
                case 0x72:
                    p += 1;
                    break;
                case 0x34:
                    p += 32;
                    break;
                case 0x16:
                case 0x18:
                    p += 16;
                    break;
                case 0x17:
                case 0x19:
                    p += 12;
                    break;
                case 0x1A:
                    p += 9;
                    break;
                case 0x1C:
                case 0x70:
                    p += 3;
                    break;
                case 0x1B: {
                    u32 scale = *p;
                    for (int k = 0; k < 12; k++) {
                        if (1 << (k + 12) == scale) {
                            mShiftX = k - shift;
                            mShiftZ = k - shiftZ;
                            break;
                        }
                    }
                    p += 3;
                    break;
                }
                case 0x71:
                    p += 2;
                    break;
                case 0x12:  // (the list's end)
                    goto done;
                case 0x23:
                    vertex = TRUE;
                    x = ((const s16 *)p)[0];
                    y = ((const s16 *)p)[1];
                    z = ((const s16 *)p)[2];
                    p += 2;
                    break;
                case 0x24:
                    vertex = TRUE;
                    x = (s16)((s32)(*p << 22) >> 16);
                    y = (s16)((s32)(*p << 12) >> 16);
                    z = (s16)((s32)(*p++ << 2) >> 16);
                    break;
                case 0x25:
                    vertex = TRUE;
                    x = ((const s16 *)p)[0];
                    y = ((const s16 *)p)[1];
                    p += 1;
                    break;
                case 0x26:
                    vertex = TRUE;
                    x = ((const s16 *)p)[0];
                    z = ((const s16 *)p)[1];
                    p += 1;
                    break;
                case 0x27:
                    vertex = TRUE;
                    y = ((const s16 *)p)[0];
                    z = ((const s16 *)p)[1];
                    p += 1;
                    break;
                case 0x28:
                    vertex = TRUE;
                    x = (s16)(x + ((s32)(*p << 22) >> 22));
                    y = (s16)(y + ((s32)(*p << 12) >> 22));
                    z = (s16)(z + ((s32)(*p << 2) >> 22));
                    p += 1;
                    break;
                case 0x40:
                    type = *p & 3;
                    strip = 0;
                    p += 1;
                    break;
                }
                if (!vertex) {
                    continue;
                }
                M3DVec16 vec;
                vec.x = x << shift;
                vec.y = y << shift;
                vec.z = z << shiftZ;
                mVerts.push_back(vec);
                if (minX > vec.x) {
                    minX = vec.x;
                }
                if (minY > vec.y) {
                    minY = vec.y;
                }
                if (minZ > vec.z) {
                    minZ = vec.z;
                }
                if (maxX < vec.x) {
                    maxX = vec.x;
                }
                if (maxY < vec.y) {
                    maxY = vec.y;
                }
                if (maxZ < vec.z) {
                    maxZ = vec.z;
                }
                count++;
                strip++;
                switch (type) {
                case 0:
                    if (strip >= 3) {
                        minCZ0 = 32;
                        minCX0 = 32;
                        maxCZ0 = -32;
                        maxCX0 = -32;
                        strip = 0;
                        M3D_GRID_BOUND(3, minCX0, minCZ0, maxCX0, maxCZ0);
                        for (; minCZ0 <= maxCZ0; minCZ0++) {
                            for (cx0 = minCX0; cx0 <= maxCX0; cx0++) {
                                M3D_GRID_CELL(cx0, minCZ0).push_back(count - 3);
                                M3D_GRID_CELL(cx0, minCZ0).push_back(count - 2);
                                M3D_GRID_CELL(cx0, minCZ0).push_back(count - 1);
                            }
                        }
                    }
                    break;
                case 1:
                    if (strip >= 4) {
                        minCZ1 = 32;
                        minCX1 = 32;
                        maxCZ1 = -32;
                        maxCX1 = -32;
                        strip = 0;
                        M3D_GRID_BOUND(4, minCX1, minCZ1, maxCX1, maxCZ1);
                        for (; minCZ1 <= maxCZ1; minCZ1++) {
                            for (cx1 = minCX1; cx1 <= maxCX1; cx1++) {
                                M3D_GRID_CELL(cx1, minCZ1).push_back(count - 4);
                                M3D_GRID_CELL(cx1, minCZ1).push_back(count - 3);
                                M3D_GRID_CELL(cx1, minCZ1).push_back(count - 1);
                                M3D_GRID_CELL(cx1, minCZ1).push_back(count - 1);
                                M3D_GRID_CELL(cx1, minCZ1).push_back(count - 3);
                                M3D_GRID_CELL(cx1, minCZ1).push_back(count - 2);
                            }
                        }
                    }
                    break;
                case 2:
                    if (strip >= 3) {
                        minCZ2 = 32;
                        minCX2 = 32;
                        maxCZ2 = -32;
                        maxCX2 = -32;
                        M3D_GRID_BOUND(3, minCX2, minCZ2, maxCX2, maxCZ2);
                        if (minCZ2 <= maxCZ2) {
                            odd = strip & 1;
                            do {
                                for (cx2 = minCX2; cx2 <= maxCX2; cx2++) {
                                    if (odd) {
                                        M3D_GRID_CELL(cx2, minCZ2).push_back(count - 3);
                                        M3D_GRID_CELL(cx2, minCZ2).push_back(count - 2);
                                        M3D_GRID_CELL(cx2, minCZ2).push_back(count - 1);
                                    } else {
                                        M3D_GRID_CELL(cx2, minCZ2).push_back(count - 3);
                                        M3D_GRID_CELL(cx2, minCZ2).push_back(count - 1);
                                        M3D_GRID_CELL(cx2, minCZ2).push_back(count - 2);
                                    }
                                }
                            } while (++minCZ2 <= maxCZ2);
                        }
                    }
                    break;
                case 3:
                    if (strip >= 4 && !(strip & 1)) {
                        minCZ3 = 32;
                        minCX3 = 32;
                        maxCZ3 = -32;
                        maxCX3 = -32;
                        M3D_GRID_BOUND(4, minCX3, minCZ3, maxCX3, maxCZ3);
                        for (; minCZ3 <= maxCZ3; minCZ3++) {
                            for (cx3 = minCX3; cx3 <= maxCX3; cx3++) {
                                M3D_GRID_CELL(cx3, minCZ3).push_back(count - 4);
                                M3D_GRID_CELL(cx3, minCZ3).push_back(count - 3);
                                M3D_GRID_CELL(cx3, minCZ3).push_back(count - 1);
                                M3D_GRID_CELL(cx3, minCZ3).push_back(count - 4);
                                M3D_GRID_CELL(cx3, minCZ3).push_back(count - 1);
                                M3D_GRID_CELL(cx3, minCZ3).push_back(count - 2);
                            }
                        }
                    }
                    break;
                }
            }
            cmds = (const u8 *)p;
            p++;
        }
    done:
        if (maxX < 0x3FFF && minX > -0x3FFF && maxY < 0x3FFF && minY > -0x3FFF) {
            shift++;
            again = TRUE;
        }
        if (maxZ < 0x3FFF && minZ > -0x3FFF) {
            shiftZ++;
            again = TRUE;
        }
    } while (again);
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
// NONMATCHING: the order of the loop's spill slots (two pairs swapped).
fx32 M3DGrid::GetHeight(fx32 px, fx32 pz) {
    int a;
    int b;
    int c;
    int best;
    std::vector<u16> *tris;
    int i;
    int ya, yb, za, zb;
    int h;
    s32 x0, y0, z0, x1, y1, z1, x2, y2, z2;
    int x, z;
    int cx, cz;
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
        if (z1 * x0 - x1 * z0 > 0) continue;
        if (z0 * x2 - x0 * z2 > 0) continue;
        if (z2 * x1 - x2 * z1 > 0) continue;
        if (x0 == x1 && x0 == x2) {
            h = y0 + -x0 * (y1 - y0) / (z1 - z0);
        } else {
            a = x1 - x0;
            b = x2 - x0;
            c = x2 - x1;
            if (a < 0) {
                a = -a;
            }
            if (b < 0) {
                b = -b;
            }
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
        return -0x8000 << 16;  // (the pooled -0x8000, shifted)
    }
    return best << mShiftX;
}

// Whether (x, z) is on a triangle of the grid; hint caches the last hit (cell << 16 | first index).
// NONMATCHING: register allocation (the game keeps this in r6 and the cell on the stack).
BOOL M3DGrid::IsOnGround(fx32 x, fx32 z, fx32 *hint) {
    int i;
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
        i = *(u32 *) hint & 0xFFFF;
        std::vector<u16> &tris = mCells[*(u32 *) hint >> 16];
        M3D_GRID_IN_TRIANGLE(mVerts, tris, i, x, z, scan);
        *(u32 *) hint = (u16) i | (cell << 16);
        return TRUE;
    }
scan:
    std::vector<u16> &tris = mCells[cell];
    for (i = 0; i < tris.size(); i += 3) {
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
    if (it != M3D_CACHE_END(M3DCache, sModelStore)) {
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
    if (it != M3D_CACHE_END(M3DCache, sAnimStore)) {
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
    if (it != M3D_CACHE_END(M3DTexCache, sTexStore)) {
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

// Constructs the caches (empty trees: no root, no size, the header linked to itself) and the mutex, in the game's
// order (see sModelLink).
static struct M3DStaticInit {
    M3DStaticInit() {
        sModelStore.w[0] = 0;
        sModelStore.w[1] = 0;
        sModelStore.w[3] = (u32) &sModelStore.w[1];
        __register_global_object(
            &sModelStore, (void *) _ZNSt3mapI12MArchiveFile13M3DCacheEntrySt4lessIS0_ESaISt4pairIKS0_S1_EEED1Ev,
            &sModelLink);
        sAnimStore.w[0] = 0;
        sAnimStore.w[1] = 0;
        sAnimStore.w[3] = (u32) &sAnimStore.w[1];
        __register_global_object(
            &sAnimStore, (void *) _ZNSt3mapI12MArchiveFile13M3DCacheEntrySt4lessIS0_ESaISt4pairIKS0_S1_EEED1Ev,
            &sAnimLink);
        sTexStore.w[0] = 0;
        sTexStore.w[1] = 0;
        sTexStore.w[3] = (u32) &sTexStore.w[1];
        __register_global_object(
            &sTexStore, (void *) _ZNSt3mapI12MArchiveFile11M3DTexEntrySt4lessIS0_ESaISt4pairIKS0_S1_EEED1Ev,
            &sTexLink);
        OS_InitMutex(&sMutex.mutex);
    }
} sStaticInit;
