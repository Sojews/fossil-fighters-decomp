#ifndef GAME_DINO_CLEANING_IMAGE_LAYER_H
#define GAME_DINO_CLEANING_IMAGE_LAYER_H

#include <string>
#include "m/picture.h"

// One layer of a cleaning image.
class DinoCleaningImageLayer {
public:
    virtual ~DinoCleaningImageLayer();

    void SetUnk14(u8 value);
    u32 GetUnk1C3C();
    u8 GetUnk800();
    void CreatePalAnime(MTaskSet *set, MPalUnit *block, u32 start, u16 color);
    void LoadPalette(MPalUnit *block, u32 start);


    u8 mShared;             // 0x04: mPixels belongs to someone else
    std::string mName;      // 0x08
    u8 mUnk14;              // 0x14
    u8 *mPixels;            // 0x18
    u8 unk1C[0x410 - 0x1C];
    u8 mCells410[36][28];   // 0x410
    u8 mUnk800;             // 0x800
    u8 mCells[36][28];      // 0x801
    s32 mGrid[36][28];      // 0xBF4
    MPictureFile mPicture;  // 0x1BB4
    u16 mPalette[42];       // 0x1BE8
    u32 mUnk1C3C;           // 0x1C3C: a sound effect
    u32 unk1C40[2];
    u8 mUnk1C48;            // 0x1C48
};

#endif
