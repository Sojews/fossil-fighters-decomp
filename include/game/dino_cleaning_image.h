#ifndef GAME_DINO_CLEANING_IMAGE_H
#define GAME_DINO_CLEANING_IMAGE_H

#include <vector>
#include "m/picture.h"
#include "game/dino_cleaning_image_layer.h"
#include "m/fx32.h"

class DinoCleaningKasekiRock;

// A cleaning image: a picture and the layers drawn over it.
class DinoCleaningImage {
public:
    DinoCleaningImage(DinoCleaningKasekiRock *rock);
    virtual ~DinoCleaningImage();

    void AddLayer(DinoCleaningImageLayer *layer);
    DinoCleaningImageLayer *GetLayer(int index);
    int CountPalettes();
    void CreatePalAnimes(MTaskSet *set, MPalBlock *block, u32 start, u16 color);
    void LoadPalettes(MPalBlock *block, u32 start);
    void PlayLayerSounds();
    MFx32 GetRatio();
    u8 GetMask(int row, int col);
    u8 GetCell(int row, int col);
    void ClearGrid();

    DinoCleaningKasekiRock *mRock;                  // 0x04
    u8 mUnk08;                                      // 0x08
    std::vector<DinoCleaningImageLayer *> mLayers;  // 0x0C
    MPictureFile mPicture;                          // 0x18
    s32 mGrid[36][28];                              // 0x4C: which layer each cell shows
    u8 mMask[36][28];                               // 0x100C
    u32 mUnk13FC;                                   // 0x13FC
    u32 mUnk1400;                                   // 0x1400
};

#endif
