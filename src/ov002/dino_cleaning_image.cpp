// DinoCleaningImage: the picture being cleaned and its layers.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_image.h"
#include "game/dino_cleaning_image_layer.h"
#include "game/dino_cleaning.h"
#include "game/dino_common_live.h"
#include "game/dino_sound.h"
#include "m/pal.h"
#include "nitro/mi.h"
#include "types.h"
#include "m/fx32.h"
#include "game/dino_static_init.h"
#include "game/dino_cleaning_neighbour.h"
#include "m/archive.h"
#include "m/seed_generator.h"

#pragma auto_inline off

void DinoCleaningImageLayer::SetUnk14(u8 value) {
    mUnk14 = value;
}

u32 DinoCleaningImageLayer::GetUnk1C3C() {
    return mUnk1C3C;
}

u8 DinoCleaningImageLayer::GetUnk800() {
    return mUnk800;
}

// std::reverse (our <algorithm> doesn't have it yet).
template <class T> static inline void DinoReverse(T *first, T *last) {
    if (first != last) {
        for (--last; first < last; ++first, --last) {
            T t = *first;
            *first = *last;
            *last = t;
        }
    }
}

// The pooled constants (shared with the image view's file).
extern "C" u32 data_ov002_021646e4[];
// (The pool from +0x80, by its own symbol.)
extern "C" struct {
    u8 unk00[0x38];
    MRect mCellBounds;          // 0x38: cells outside it start uncovered
} data_ov002_02164764;
extern "C" const u8 data_ov002_021632c4[10][8];     // a cell's footprint, from a pixel above its corner
extern "C" const u8 data_ov002_02163364[6][80];  // the footprint's edge towards each neighbour

// A cell's corner on the screen.
static inline MPoint DinoCellCorner(MPoint cell) {
    cell.x -= DINO_POOL(MPoint, data_ov002_021646e4, 0x3C).x;
    cell.y -= DINO_POOL(MPoint, data_ov002_021646e4, 0x3C).y;
    return MPoint(cell.x * 8 + (cell.y & 1) * 4, cell.y * 8);
}

static inline bool DinoFxLe(MFx32 a, const MFx32 &b) {
    return a.v <= b.v;
}

// Loads the layer: its pixels (mirrored for mUnk14), its palette sorted by brightness (the pixels remapped to the
// palettes from start on), then for each cell whether any of its pixels is set, its edges, how hard it is, and
// which cells are surrounded.
// NONMATCHING: 99.8%, the mirrored rows' reverse loop gets other registers (tried std::swap, while and helper forms).
extern "C" void func_ov002_02158b74(DinoCleaningImageLayer *layer, MPalUnit *block, u32 start) {
    layer->mUnk1C28 = start;
    layer->mSize = layer->mPicture.CalcSize();
    layer->mTileSize = layer->mPicture.CalcTileSize();
    MI_CpuFill8(layer->mPixels, 0, 0xC200);
    layer->mPicture.GetChrFile().Read(layer->mUnk1C + 0x1800, 0xA800);
    if (layer->mUnk14) {
        for (int row = 0; row < 0xC0; row++) {
            DinoReverse(layer->mUnk1C + row * 0x100, layer->mUnk1C + (row + 1) * 0x100);
        }
    }
    MI_CpuCopy8(layer->mPicture.GetPalette(), layer->mPalette, sizeof(layer->mPalette));
    u32 order[32];
    u32 bright[32];
    for (u32 i = 0; i < 32; i++) {
        order[i] = i;
        u16 c = layer->mPalette[i];
        u32 b = (c >> 10) & 0x1F;
        bright[i] = b + ((c & 0x1F) * 3 + ((c >> 5) & 0x1F) * 6);
    }
    for (u32 i = 1; i < 31; i++) {
        for (u32 j = i + 1; j < 32; j++) {
            if (bright[order[i]] > bright[order[j]]) {
                std::swap(order[i], order[j]);
            }
        }
    }
    for (u32 i = 0; i < 32; i++) {
        layer->mPalette[i] = ((const u16 *)layer->mPicture.GetPalette())[order[i]];
    }
    block->Load(layer->mPalette, start, 2);
    u32 remap[32];
    remap[0] = 0;
    for (u32 i = 1; i < 32; i++) {
        remap[order[i]] = start * 16 + i;
    }
    for (u32 i = 0; i < 0xC000; i++) {
        layer->mUnk1C[i] = remap[layer->mUnk1C[i]];
    }
    MI_CpuFill8(layer->mGrid, 0, sizeof(layer->mGrid));
    u8 edges[6][10];
    for (u32 dir = 0; dir < 6; dir++) {
        for (u32 row = 0; row < 10; row++) {
            edges[dir][row] = 0;
            for (u32 bit = 0; bit < 8; bit++) {
                if (data_ov002_02163364[dir][row * 8 + bit]) {
                    edges[dir][row] |= 1 << bit;
                }
            }
        }
    }
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 36; x++) {
            layer->mHard[x][y] = 1;
            layer->mCells410[x][y] = 0;
            MPoint pos = DinoCellCorner(MPoint(x, y));
            MRect rect(pos, pos + MPoint(8, 8));
            rect.top--;
            rect.bottom++;
            if (!data_ov002_02164764.mCellBounds.Contains(rect)) {
                continue;
            }
            const u8 *shape = &data_ov002_021632c4[0][0];
            int count = 0;
            u8 rows[10];
            for (int py = rect.top; py < rect.bottom; py++) {
                rows[py - rect.top] = 0;
                for (int px = rect.left; px < rect.right; px++) {
                    if (*shape++ && layer->mUnk1C[px + py * 0x100]) {
                        rows[py - rect.top] |= 1 << (px - rect.left);
                        layer->mHard[x][y] = 0;
                        count++;
                    }
                }
            }
            if (DinoFxLe(MFx32(count) / MFx32(64), DINO_POOL(MFx32, data_ov002_021646e4, 0x4))) {
                layer->mCells410[x][y] = 1;
            }
            int sides = 0;
            if (!layer->mHard[x][y]) {
                for (u32 dir = 0; dir < 6; dir++) {
                    for (int row = 0; row < 10; row++) {
                        if (edges[dir][row] & rows[row]) {
                            sides |= 1 << dir;
                            break;
                        }
                    }
                }
            }
            layer->mCells[x][y] = sides;
            if (layer->mUnk1C44 && !layer->mHard[x][y]) {
                layer->mGrid[x][y] = (layer->mUnk1C40 * (((MRandom *)layer->mUnk1C44)->NextFraction() *
                                       MFx32::FromRaw(0x333) + MFx32::FromRaw(0xE66))).v;
            }
        }
    }
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 36; x++) {
            if (layer->mCells410[x][y]) {
                BOOL surrounded = TRUE;
                for (u32 dir = 0; dir < 6; dir++) {
                    MPoint n = GetNeighbour(MPoint(x, y), dir);
                    if (!layer->mHard[n.x][n.y] && !layer->mCells410[n.x][n.y]) {
                        surrounded = FALSE;
                        break;
                    }
                }
                if (surrounded) {
                    layer->mCells410[x][y] = 2;
                }
            }
        }
    }
}

void DinoCleaningImageLayer::CreatePalAnime(MTaskSet *set, MPalUnit *block, u32 start, u16 color) {
    MPalAnime *anime = new MPalAnime(block, start, 2);
    anime->AddColor(color);
    anime->AddFrame(mPalette);
    anime->SetDuration(60);
    set->AddTail(anime);
}

void DinoCleaningImageLayer::LoadPalette(MPalUnit *block, u32 start) {
    block->Load(mPalette, start, 2);
}

DinoCleaningImage::DinoCleaningImage(DinoCleaning *cleaning)
    : mCleaning(cleaning), mUnk08(FALSE), mPicture(std::string(""), 0) {
}

// (Arrays copy as wholes, element by element.)
template <class T> static inline void DinoCopyBlock(T &dst, const T &src) {
    struct Block {
        T a;
    };
    *(Block *)&dst = *(const Block *)&src;
}

DinoCleaningImage::DinoCleaningImage(const DinoCleaningImage &other) {
    mCleaning = other.mCleaning;
    mUnk08 = other.mUnk08;
    *(std::vector<u32> *)&mLayers = *(const std::vector<u32> *)&other.mLayers;
    mPicture = other.mPicture;
    DinoCopyBlock(mGrid, other.mGrid);
    DinoCopyBlock(mMask, other.mMask);
    mUnk13FC = other.mUnk13FC;
    mUnk1400 = other.mUnk1400;
    mView = other.mView;
    mLayers.clear();
    for (std::vector<DinoCleaningImageLayer *>::iterator it =
             const_cast<std::vector<DinoCleaningImageLayer *> &>(other.mLayers).begin();
         it != const_cast<std::vector<DinoCleaningImageLayer *> &>(other.mLayers).end(); ++it) {
        mLayers.push_back(new DinoCleaningImageLayer(**it));
    }
}

#pragma auto_inline on

DinoCleaningImage::~DinoCleaningImage() {
    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        delete *it;
    }
}

#pragma auto_inline off

void DinoCleaningImage::AddLayer(DinoCleaningImageLayer *layer) {
    mLayers.push_back(layer);
}

DinoCleaningImageLayer *DinoCleaningImage::GetLayer(int index) {
    return mLayers[index];
}

int DinoCleaningImage::CountPalettes() {
    int count = 0;

    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        if (!(*it)->GetUnk800()) {
            count += 2;
        }
    }
    return count;
}

void DinoCleaningImage::CreatePalAnimes(MTaskSet *set, MPalUnit *block, u32 start, u16 color) {
    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        (*it)->CreatePalAnime(set, block, start, color);
        if (!(*it)->GetUnk800()) {
            start += 2;
        }
    }
}

void DinoCleaningImage::LoadPalettes(MPalUnit *block, u32 start) {
    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        (*it)->LoadPalette(block, start);
        if (!(*it)->GetUnk800()) {
            start += 2;
        }
    }
}


// NONMATCHING: 81%, the game recomputes the layer cell index for every layer (strength-reducing only the mask row).
void DinoCleaningImage::Build(u32 arg, u32 start) {
    mUnk08 = TRUE;
    std::vector<DinoCleaningImageLayer *>::iterator it;
    for (it = mLayers.begin(); it != mLayers.end(); ++it) {
        func_ov002_02158b74(*it, (MPalUnit *)arg, start);
        if (!(*it)->GetUnk800()) {
            start += 2;
        }
    }
    for (u32 col = 0; col < 28; col++) {
        for (u32 row = 0; row < 36; row++) {
            mMask[row][col] = TRUE;
            for (it = mLayers.begin(); it != mLayers.end(); ++it) {
                if ((*it)->mHard[row][col] == 0) {
                    mMask[row][col] = FALSE;
                    break;
                }
            }
        }
    }
    mUnk13FC = 0;
    for (u32 col = 0; col < 28; col++) {
        for (u32 row = 0; row < 36; row++) {
            if (GetMask(row, col)) {
                mGrid[row][col] = 0;
            } else {
                int top = 0;
                for (it = mLayers.begin(); it != mLayers.end(); ++it) {
                    if ((*it)->mHard[row][col] == 0 && (*it)->mCells410[row][col] == 0) {
                        top = it.ptr_ - mLayers.begin().ptr_;
                        mUnk13FC++;
                    }
                }
                mGrid[row][col] = top;
            }
        }
    }
    mUnk1400 = mUnk13FC;
}

void DinoCleaningImage::PlayLayerSounds() {
    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        if ((*it)->mUnk1C48) {
            (*it)->mUnk1C48 = FALSE;
            u32 se = (*it)->GetUnk1C3C();
            if (se != 0) {
                if ((u32)(se - 0x2A) <= 1) {
                    mCleaning->GetUnkE4()->SetUnkAC(9);
                }
                DinoSoundPlaySe((*it)->GetUnk1C3C(), 0);
            }
        }
    }
}

MFx32 DinoCleaningImage::GetRatio() {
    return MFx32(100) * MFx32(mUnk13FC) / MFx32(mUnk1400);
}

u8 DinoCleaningImage::GetMask(int row, int col) {
    return mMask[row][col];
}

u8 DinoCleaningImage::GetCell(int row, int col) {
    return mLayers[mGrid[row][col]]->mCells[row][col];
}

void DinoCleaningImage::ClearGrid() {
    MI_CpuFill8(mGrid, 0, sizeof(mGrid));
}

#pragma auto_inline off
// A vector member the game emits in this file (made explicitly until the code that copies the vector is decompiled).
template void std::__vec_deleter<u32, std::allocator<u32> >::assign<u32 *>(u32 *first, u32 *last, Metrowerks::int2type<true>);

// The file's static initialization (see dino_static_init.h), with the screen center and a half.
// NONMATCHING: the dead stack copies of the layout and the center take other slots.
#pragma always_inline on
static struct DinoCleaningImageStaticInit {
    DinoCleaningImageStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_021646e4, 0x34, 0xb8, 0x3c, 0x1c, 0xc8, 0xe8, 0xf0, 0xd8);
        DINO_POOL(MFx32Point, data_ov002_021646e4, 0x14).Set(MFx32(128), MFx32(96));
        DINO_POOL(MFx32, data_ov002_021646e4, 0x4) = MFx32::FromRaw(0x800);
    }
} sDinoCleaningImageStaticInit;
