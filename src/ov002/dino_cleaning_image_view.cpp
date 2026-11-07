// DinoCleaningImageView, DinoCleaningNewDust, DinoCleaningLastDust.

#pragma ipa file

#include "types.h"
#include "game/dino_cleaning_image_view.h"
#include "game/dino_cleaning_image.h"
#include "m/smart_oam.h"
#include "m/bg.h"
#include "m/display.h"
#include "m/pal.h"
#include "m/bitmap.h"
#include "nitro/mi.h"
#include "m/seed_generator.h"
#include "game/dino_cleaning_new_dust.h"
#include "game/dino_cleaning_last_dust.h"
#include "game/dino_cleaning_image_view_scale_anime.h"
#include "game/dino_cleaning_status.h"
#include "game/dino_cleaning_image_layer.h"
#include "game/dino_cleaning_neighbour.h"

#pragma auto_inline off

DinoCleaningImageView::~DinoCleaningImageView() {
    delete mOamTasks;
    operator delete(mSeed);
    delete mPalTasks;
    for (std::vector<DinoCleaningImage *>::iterator it = mImages.begin(); it != mImages.end(); it++) {
        delete *it;
    }
    MBg *bg = mDisplay->mBg;
    bg->DeleteIndexBitmap((MIndexBitmap *) mScreen);
    if (!mKeepPal) {
        bg->mPal->Free(mPal);
    }
}

// (The push through a static inline taking the pointer by const reference gives push_back's tag temporaries the
// game's stack slots.)
static inline void DinoPushImage(std::vector<DinoCleaningImage *> &v, DinoCleaningImage *const &image) {
    v.push_back(image);
}

void DinoCleaningImageView::AddImage(DinoCleaningImage *image) {
    image->mView = this;
    DinoPushImage(mImages, image);
}

DinoCleaningImage *DinoCleaningImageView::GetImage(int index) {
    return mImages[index];
}

// (start is set twice, so the game keeps its 0 in a register.)
void DinoCleaningImageView::CreatePalAnimes() {
    u32 start;

    mPalTasks->Clear();
    start = 0;
    mImages[0]->CreatePalAnimes(mPalTasks, mPal, start, 0x8000);
    start += mImages[0]->CountPalettes();
    mImages[1]->CreatePalAnimes(mPalTasks, mPal, start, 0xFFFF);
}

void DinoCleaningImageView::LoadPalettes() {
    u32 start;

    mPalTasks->Clear();
    start = 0;
    mImages[0]->LoadPalettes(mPal, start);
    start += mImages[0]->CountPalettes();
    mImages[1]->LoadPalettes(mPal, start);
}

void DinoCleaningImageView::Move(int x, int y) {
    SetPosition(MPoint(x, y));
}

void DinoCleaningImageView::SetPosition(MPoint pos) {
    mPos.x = pos.x;
    mPos.y = pos.y;
    mScreen->SetOffset(MPoint(-pos.x, 1 - pos.y));
}

MPoint DinoCleaningImageView::GetPosition() {
    return mPos;
}

void DinoCleaningImageView::SetPriority(int priority) {
    mScreen->mBgCntBits.priority = priority;
}

int DinoCleaningImageView::GetPriority() {
    return mScreen->mBgCntBits.priority;
}

void DinoCleaningImageView::Show() {
    mScreen->Show();
}

void DinoCleaningImageView::Hide() {
    mScreen->Hide();
}

BOOL DinoCleaningImageView::IsHidden() {
    return mScreen->IsHidden();
}

u32 DinoCleaningImageView::GetPlaneMask() {
    return 1 << mScreen->mBg;
}

void DinoCleaningImageView::CreateOam(MRandom *random) {
    mOam = new MSmartOam(300, 50, TRUE);
    mOamTasks->AddTail(mOam);
}

void DinoCleaningImageView::DestroyOam() {
    mOamTasks->Clear();
    mOam = NULL;
}

BOOL DinoCleaningImageView::IsUnkD9E0Zero() {
    if (mUnkD9E0 == 0) {
        return TRUE;
    }
    return FALSE;
}

void DinoCleaningImage::SetCellLayer(int row, int col, u32 layer) {
    u32 cur = mGrid[row][col];
    if (layer == cur) {
        return;
    }
    for (; cur > layer; cur--) {
        DinoCleaningImageLayer *l = mLayers[cur];
        if (l->mHard[row][col] == 0 && l->mCells410[row][col] == 0) {
            mUnk13FC--;
            mLayers[cur]->mUnk1C48 = TRUE;
        }
    }
    mGrid[row][col] = layer;
    mView->mDirty[row][col] = TRUE;
    for (u32 i = 0; i < 6; i++) {
        MPoint n = GetNeighbour(MPoint(row, col), i);
        mView->mDirty[n.x][n.y] = TRUE;
    }
}

// Moves the screen down a line. (The offset point is built inside the inline, which gives the game's temp slots.)
static inline void DinoOffsetScreen(MIndexBitmap *screen) { screen->SetOffset(MPoint(0, 1)); }

void DinoCleaningImageView::CreateScreen() {
    std::vector<DinoCleaningImage *>::iterator it;
    u32 count = 0;
    for (it = mImages.begin(); it != mImages.end(); ++it) {
        count += (*it)->CountPalettes();
    }
    MBg *bg = mDisplay->mBg;
    mPal = bg->mPal->Alloc(count, NULL, TRUE);
    MIndexBitmap *screen = bg->CreateIndexBitmap(1, TRUE);
    mScreen = screen;
    DinoOffsetScreen(screen);
    u32 start = 0;
    for (it = mImages.begin(); it != mImages.end(); ++it) {
        (*it)->Build((u32)mPal, start);
        start += (*it)->CountPalettes();
    }
    mPalBytes = start << 4;
    mUnkD9EC = 0;
    MIi_CpuClearFast(0, mUnk820, sizeof(mUnk820));
    ((MIndexBitmap *)mScreen)->Fill(MRect(0, 0, 256, 256), 0);
    for (u32 col = 0; col < 28; col++) {
        for (u32 row = 0; row < 36; row++) {
            mMasked[row][col] = TRUE;
            for (it = mImages.begin(); it != mImages.end(); ++it) {
                if (!(*it)->GetMask(row, col)) {
                    mMasked[row][col] = FALSE;
                    break;
                }
            }
        }
    }
    for (u32 col = 0; col < 28; col++) {
        for (u32 row = 0; row < 36; row++) {
            mDirty[row][col] = mMasked[row][col] ? FALSE : TRUE;
        }
    }
    MI_CpuFill8(mCells, 0, sizeof(mCells));
    mUnkD9E0 = 0;
    mDirtSum.x = 0;
    mDirtSum.y = 0;
}

// The pooled constants the code reads.
extern "C" struct {
    u8 unk00[0xC];
    int mMinDirt;           // 0x0C: the least dirt a cell keeps when the last dust is taken off
    const char *mMotionPath;    // 0x10
    u8 unk14[0x3C - 0x14];
    MPoint mCellOrigin;     // 0x3C: (2, 2)
    u8 unk44[0xB8 - 0x44];
    MRect mDustBounds;      // 0xB8: where landing dust dirties the image (built by the __sinit)
} data_ov002_021646e4;
extern "C" const int data_ov002_021646f0;   // (the pool's +0xC, by its own symbol)

DinoCleaningImageView::DinoCleaningImageView()
    : MFixedSizeView(MPoint(256, 192)), mKeepPal(FALSE), mDisplay(gCurrentDisplay), mPos(0, 0), mDirtSum(0, 0),
      mOam(NULL), mMotion(std::string(data_ov002_021646e4.mMotionPath), 7) {
    mSeed = new u32(SeedGenerator::GenerateSeed());
    mOamTasks = new MParallelTaskSet(FALSE);
    mPalTasks = new MParallelTaskSet(FALSE);
}

// (Arrays copy as wholes, element by element.)
template <class T> static inline void DinoCopyBlock(T &dst, const T &src) {
    struct Block {
        T a;
    };
    *(Block *)&dst = *(const Block *)&src;
}

// (Vectors of pointers copy as vectors of words.)
template <class T> static inline void DinoCopyVector(std::vector<T *> &dst, const std::vector<T *> &src) {
    *(std::vector<u32> *)&dst = *(const std::vector<u32> *)&src;
}

inline void DinoCleaningImageView::Assign(const DinoCleaningImageView &src) {
    MView::operator=(src);
    mSize = src.mSize;
    mKeepPal = src.mKeepPal;
    mDisplay = src.mDisplay;
    mPos = src.mPos;
    mScreen = src.mScreen;
    mPal = src.mPal;
    mUnk30 = src.mUnk30;
    DinoCopyVector(mImages, src.mImages);
    DinoCopyBlock(mMasked, src.mMasked);
    DinoCopyBlock(mDirty, src.mDirty);
    DinoCopyBlock(mUnk820, src.mUnk820);
    DinoCopyBlock(mCells, src.mCells);
    mUnkD9E0 = src.mUnkD9E0;
    mDirtSum = src.mDirtSum;
    mUnkD9EC = src.mUnkD9EC;
    mPalBytes = src.mPalBytes;
    mOam = src.mOam;
    mSeed = src.mSeed;
    mOamTasks = src.mOamTasks;
    mMotion = src.mMotion;
    mUnkDA04 = src.mUnkDA04;
    mPalTasks = src.mPalTasks;
}

DinoCleaningImageView::DinoCleaningImageView(const DinoCleaningImageView &other) : mMotion(other.mMotion) {
    Assign(other);
    mKeepPal = TRUE;
    mSeed = new u32(SeedGenerator::GenerateSeed());
    mOamTasks = new MParallelTaskSet(FALSE);
    mPalTasks = new MParallelTaskSet(FALSE);
    mImages.clear();
    for (std::vector<DinoCleaningImage *>::iterator it =
             const_cast<std::vector<DinoCleaningImage *> &>(other.mImages).begin();
         it != const_cast<std::vector<DinoCleaningImage *> &>(other.mImages).end(); ++it) {
        AddImage(new DinoCleaningImage(**it));
    }
    MIndexBitmap *screen = mDisplay->mBg->CreateIndexBitmap(1, TRUE);
    mScreen = screen;
    DinoOffsetScreen(screen);
    ((MIndexBitmap *)mScreen)->Fill(MRect(0, 0, 256, 256), 0);
}

inline DinoCleaningNewDust::DinoCleaningNewDust(MSmartOam *oam, DinoCleaningImageView *view, MFx32Point from,
                                                MFx32Point to, MFx32 height)
    : MProcess(NULL) {
    mView = view;
    mFrom = from;
    mTo = to;
    mHeight = height;
    mFrame = MFx32(0);
    mDuration = MFx32(20);
    mMotion = new MMotion(MMotionFile(view->mMotion), oam, 0, 1);
    mMotion->SetPriority(2);
    mMotion->SetAnchor(3);
    mMotion->Play(1, 0);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

// The view's random numbers (a linear congruential generator).
static inline u32 DinoCleaningRand(u32 *seed) {
    *seed = *seed * 0x41C64E6D + 0x3039;
    return *seed;
}

// The unit vector at angle.
static inline MFx32Point DinoDirection(u16 angle) {
    return MFx32Point(MFx32::FromRaw(FX_CosIdx(angle)), MFx32::FromRaw(FX_SinIdx(angle)));
}

// The point in fixed point.
static inline MFx32Point DinoToFx(MPoint p) {
    return MFx32Point(MFx32(p.x), MFx32(p.y));
}

// Brushes a bit of dust off at pos, flying a random distance in a random direction.
// (The dust's ctor takes the oam first, which puts its load between the end point's y and x, as in the game.)
extern "C" void func_ov002_0215a4fc(DinoCleaningImageView *view, MPoint pos) {
    u16 angle = (int)(u16)DinoCleaningRand(view->mSeed);  // (truncated twice, as the game does)
    MFx32Point move = MFx32Point(MFx32::FromRaw(FX_CosIdx(angle)), MFx32::FromRaw(FX_SinIdx(angle))) *
                      MFx32((int)((DinoCleaningRand(view->mSeed) & 0x7FFFFFFF) % 40));
    MFx32 height = MFx32((int)((DinoCleaningRand(view->mSeed) & 0x7FFFFFFF) % 10));
    view->mOamTasks->AddHead(new DinoCleaningNewDust(view->mOam, view, DinoToFx(pos), DinoToFx(pos) + move, height));
}

// a - b. (Taking a by value and returning it keeps both points on the stack, as the game's code does.)
static inline MPoint DinoPointSub(MPoint a, const MPoint &b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

// Adds dirt to a cell.
void DinoCleaningImageView::AddDirt(int x, int y) {
    mCells[x][y]++;
    mUnkD9E0++;
    mDirty[x][y] = TRUE;
    MPoint d = DinoPointSub(MPoint(x, y), data_ov002_021646e4.mCellOrigin);
    mDirtSum.x += d.x * 8 + (d.y & 1) * 4;
    mDirtSum.y += d.y * 8;
}

// How much of the image is still dirty, in percent.
MFx32 DinoCleaningImageView::GetDirtRatio() {
    int count = 0;
    for (int col = 0; col < 28; col++) {
        for (int row = 0; row < 36; row++) {
            if (mCells[row][col] != 0) {
                count++;
            }
        }
    }
    return MFx32(100) * MFx32(count) / MFx32(36 * 28);
}

// std::max on a pooled operand: its value read through the pool, the operand returned by its own symbol.
static inline const int &DinoMaxPooled(const int &a, const int &aNamed, const int &b) {
    return a < b ? b : aNamed;
}

inline DinoCleaningLastDust::DinoCleaningLastDust(MSmartOam *oam, DinoCleaningImageView *view, MFx32Point from,
                                                  MFx32Point to)
    : MProcess(NULL) {
    unk74 = (u32)view;
    mFrom = from;
    mTo = to;
    mFrame = MFx32(0);
    mDuration = MFx32(10);
    mMotion = new MMotion(MMotionFile(view->mMotion), oam, 0, 1);
    mMotion->SetPriority(2);
    mMotion->SetAnchor(3);
    mMotion->Play(1, 0);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

// Takes up to 20 random bits of dirt off (a cell keeps mMinDirt), the first 6 flying off away from the dirt's centre.
// NONMATCHING: 95.2%, stack slots and some registers differ.
extern "C" void func_ov002_0215a7b4(DinoCleaningImageView *view) {
    std::vector<DinoCleaningStatusEntry>::iterator it;
    DinoCleaningStatusEntry cell;
    MFx32Point centre;
    MFx32Point from;
    MFx32Point away;
    MFx32Point to;
    DinoCleaningStatusEntry dirty;
    if (view->mOam == NULL) {
        return;
    }
    view->mUnkDA04.clear();
    for (int col = 0; col < 28; col++) {
        for (int row = 0; row < 36; row++) {
            if (view->mCells[row][col] != 0) {
                dirty.unk4 = col;
                dirty.unk0 = row;
                const DinoCleaningStatusEntry &c = dirty;
                view->mUnkDA04.push_back(c);
            }
        }
    }
    for (u32 n = 0; n < 20; n++) {
        u32 size = view->mUnkDA04.size();
        if (size == 0) {
            return;
        }
        it = view->mUnkDA04.begin() + (DinoCleaningRand(view->mSeed) & 0x7FFFFFFF) % size;
        cell = *it;
        centre = DinoToFx(view->mDirtSum / (int)view->mUnkD9E0);
        MPoint d = DinoPointSub(*(const MPoint *)&cell, data_ov002_021646e4.mCellOrigin);
        int x = d.x * 8 + (d.y & 1) * 4;
        int y = d.y * 8;
        u32 row = cell.unk0;
        u32 col = cell.unk4;
        u32 left = DinoMaxPooled(data_ov002_021646e4.mMinDirt, data_ov002_021646f0,
                                 view->mCells[row][col] - 3);
        int taken = view->mCells[row][col] - left;
        view->mDirtSum.x -= x * taken;
        view->mDirtSum.y -= y * taken;
        view->mUnkD9E0 -= view->mCells[row][col] - left;
        view->mCells[row][col] = left;
        view->mDirty[row][col] = TRUE;
        if (view->mCells[row][col] == 0) {
            view->mUnkDA04.erase(it);
        }
        if (n < 6) {
            MPoint e = DinoPointSub(*(const MPoint *)&cell, data_ov002_021646e4.mCellOrigin);
            from = DinoToFx(MPoint(e.x * 8 + (e.y & 1) * 4, e.y * 8));
            away = from - centre;
            u32 spread = DinoCleaningRand(view->mSeed) & 0x3FFF;
            int angle = FX_Atan2Idx(away.y.v, away.x.v);
            to = from + DinoDirection(angle + spread - 0x2000) * MFx32(50);
            view->mOamTasks->AddHead(new DinoCleaningLastDust(view->mOam, view, from, to));
        }
    }
}

void DinoCleaningImageView::SetScale(MFx32 scale) {
    mScreen->SetCenter(128, 108);
    mScreen->SetScale(scale, scale);
}

MTask *DinoCleaningImageView::CreateScaleAnime(MFx32 from, MFx32 to, int duration, int ease) {
    return new DinoCleaningImageViewScaleAnime(this, from, to, duration, ease);
}

void DinoCleaningImageViewScaleAnime::Apply(int step) {
    MFx32ValueAnime::Apply(step);
    MFx32 scale = mValue;
    mView->SetScale(scale);
}

// mFrame++: steps a frame on, giving the frame it was.
static inline MFx32 operator++(MFx32 &a, int) {
    MFx32 old = a;
    a += MFx32(1);
    return old;
}

// The point in whole pixels.
static inline MPoint DinoToPoint(MFx32Point p) {
    return MPoint(MFx32ToInt(p.x), MFx32ToInt(p.y));
}

// The image cell under a pixel (odd rows of cells sit half a cell to the right).
static inline MPoint DinoToCell(MPoint p) {
    int x = p.x - ((p.y & 8) >> 1);
    return MPoint((x >> 3) + data_ov002_021646e4.mCellOrigin.x, (p.y >> 3) + data_ov002_021646e4.mCellOrigin.y);
}

// Flies in an arc from mFrom to mTo (mHeight at the top), then dirties the cell it lands on.
void DinoCleaningNewDust::Idle() {
    if (mFrame++ >= mDuration) {
        if (data_ov002_021646e4.mDustBounds.Contains(DinoToPoint(mTo))) {
            MPoint cell = DinoToCell(DinoToPoint(mTo));
            mView->AddDirt(cell.x, cell.y);
        }
        Exit();
        return;
    }
    MFx32Point pos = mFrom + (mTo - mFrom) * mFrame / mDuration;
    MFx32 s = mFrame / (mDuration / MFx32(2)) - MFx32(1);
    MFx32 h = (MFx32(1) - s * s) * mHeight;
    mMotion->SetPosition(MFx32ToInt(pos.x), MFx32ToInt(pos.y - h));
}

#pragma auto_inline off

DinoCleaningNewDust::~DinoCleaningNewDust() {
}

DinoCleaningLastDust::~DinoCleaningLastDust() {
}

// Slides from mFrom to mTo.
void DinoCleaningLastDust::Idle() {
    if (mFrame++ >= mDuration) {
        Exit();
        return;
    }
    MFx32Point pos = mFrom + (mTo - mFrom) * mFrame / mDuration;
    mMotion->SetPosition(DinoToPoint(pos));
}

DinoCleaningImageViewScaleAnime::~DinoCleaningImageViewScaleAnime() {
}

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template class std::__vec_deleter<DinoCleaningStatusEntry, std::allocator<DinoCleaningStatusEntry> >;
// (A vector<DinoCleaningStatusEntry>'s operator= copies with this.)
template void std::__vec_deleter<DinoCleaningStatusEntry, std::allocator<DinoCleaningStatusEntry> >::assign<DinoCleaningStatusEntry *>(
    DinoCleaningStatusEntry *first, DinoCleaningStatusEntry *last, Metrowerks::int2type<true>);
template class std::__vec_deleter<u32, std::allocator<u32> >;
