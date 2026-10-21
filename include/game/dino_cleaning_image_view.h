// The view showing the cleaning image on a BG screen.
#ifndef GAME_DINO_CLEANING_IMAGE_VIEW_H
#define GAME_DINO_CLEANING_IMAGE_VIEW_H

#include "m/view.h"
class MRandom;
#include <vector>
#include "m/affine_screen.h"
#include "m/fx32.h"
#include "m/motion.h"
#include "game/dino_cleaning_status.h"

class DinoCleaningImage;
class MPalUnit;
class MSmartOam;

class DinoCleaningImageView : public MFixedSizeView {
public:
    DinoCleaningImageView();
    // A copy with copies of the images, its own seed, task sets and screen (the palette stays the other's).
    DinoCleaningImageView(const DinoCleaningImageView &other);
    void Assign(const DinoCleaningImageView &src);
    // Makes the screen and palettes for the images, and clears the dirt.
    void CreateScreen();
    virtual ~DinoCleaningImageView();
    virtual void Execute();  // (in ITCM: func_01ff9a4c)
    virtual void SetPosition(MPoint pos);
    virtual MPoint GetPosition();
    virtual void SetPriority(int priority);
    virtual int GetPriority();
    virtual void Show();
    virtual void Hide();
    virtual BOOL IsHidden();
    virtual u32 GetPlaneMask();

    void AddImage(DinoCleaningImage *image);
    DinoCleaningImage *GetImage(int index);
    void CreatePalAnimes();
    void LoadPalettes();
    void Move(int x, int y);
    void CreateOam(MRandom *random);
    void DestroyOam();
    BOOL IsUnkD9E0Zero();
    MFx32 GetDirtRatio();
    void AddDirt(int x, int y);
    void SetScale(MFx32 scale);
    MTask *CreateScaleAnime(MFx32 from, MFx32 to, int duration, int ease);

    u8 mKeepPal;                                // 0x18: the palette isn't the view's to free
    MDisplay *mDisplay;                         // 0x1C
    MPoint mPos;                                // 0x20
    MAffineScreen *mScreen;                     // 0x28
    MPalUnit *mPal;                            // 0x2C
    u8 mUnk30;                                  // 0x30
    std::vector<DinoCleaningImage *> mImages;   // 0x34
    u8 mMasked[36][28];                         // 0x40: cells every image masks
    u8 mDirty[36][28];                          // 0x430: cells that have had dirt
    u8 mUnk820[0xCA20 - 0x820];                 // 0x820
    u32 mCells[36][28];                         // 0xCA20: the dirt left on each cell
    u32 mUnkD9E0;                               // 0xD9E0: dirt added
    MPoint mDirtSum;                            // 0xD9E4: the added dirt's positions, summed
    u32 mUnkD9EC;                               // 0xD9EC
    u32 mPalBytes;                              // 0xD9F0: the images' palettes' size
    MSmartOam *mOam;                            // 0xD9F4
    u32 *mSeed;                                 // 0xD9F8
    MParallelTaskSet *mOamTasks;                // 0xD9FC
    MMotionFile mMotion;                        // 0xDA00
    std::vector<DinoCleaningStatusEntry> mUnkDA04;  // 0xDA04
    MParallelTaskSet *mPalTasks;                // 0xDA10
};

#endif
