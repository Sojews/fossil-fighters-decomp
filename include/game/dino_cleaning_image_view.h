// The view showing the cleaning image on a BG screen.
#ifndef GAME_DINO_CLEANING_IMAGE_VIEW_H
#define GAME_DINO_CLEANING_IMAGE_VIEW_H

#include "m/view.h"
#include <vector>
#include "m/affine_screen.h"
#include "m/fx32.h"
#include "m/motion.h"

class DinoCleaningImage;
class MPalBlock;
class MSmartOam;

class DinoCleaningImageView : public MFixedSizeView {
public:
    virtual ~DinoCleaningImageView();
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
    void CreateOam();
    void DestroyOam();
    BOOL IsUnkD9E0Zero();
    void SetScale(MFx32 scale);
    MTask *CreateScaleAnime(MFx32 from, MFx32 to, int duration, int ease);

    u8 mKeepPal;                                // 0x18: the palette isn't the view's to free
    MDisplay *mDisplay;                         // 0x1C
    MPoint mPos;                                // 0x20
    MAffineScreen *mScreen;                     // 0x28
    MPalBlock *mPal;                            // 0x2C
    u32 unk30;
    std::vector<DinoCleaningImage *> mImages;   // 0x34
    u8 unk40[0xD9E0 - 0x40];
    u32 mUnkD9E0;                               // 0xD9E0
    u32 unkD9E4[4];
    MSmartOam *mOam;                            // 0xD9F4
    void *mUnkD9F8;                             // 0xD9F8
    MTaskSet *mOamTasks;                        // 0xD9FC
    MMotionFile mMotion;                        // 0xDA00
    std::vector<u16> mUnkDA04;                  // 0xDA04
    MTaskSet *mPalTasks;                        // 0xDA10
};

#endif
