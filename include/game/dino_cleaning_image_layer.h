#ifndef GAME_DINO_CLEANING_IMAGE_LAYER_H
#define GAME_DINO_CLEANING_IMAGE_LAYER_H

#include <string>
#include "m/picture.h"
#include "m/fx32.h"

// One layer of a cleaning image.
class DinoCleaningImageLayer {
public:
    DinoCleaningImageLayer(u32 unk1C44, std::string name, u8 unk800, u32 sound, MFx32 unk1C40);
    // A layer sharing the other's pixels.
    DinoCleaningImageLayer(const DinoCleaningImageLayer &other);
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
    u8 *mUnk1C;             // 0x1C: past the first 0x100 bytes
    u8 mHard[36][28];       // 0x20: cells the tools can't break
    u8 mCells410[36][28];   // 0x410
    u8 mUnk800;             // 0x800
    u8 mCells[36][28];      // 0x801
    s32 mGrid[36][28];      // 0xBF4
    MPictureFile mPicture;  // 0x1BB4
    u16 mPalette[32];       // 0x1BE8
    u32 mUnk1C28;           // 0x1C28
    u32 mUnk1C2C;           // 0x1C2C
    u32 mUnk1C30;           // 0x1C30
    u32 mUnk1C34;           // 0x1C34
    u32 mUnk1C38;           // 0x1C38
    u32 mUnk1C3C;           // 0x1C3C: a sound effect
    MFx32 mUnk1C40;         // 0x1C40
    u32 mUnk1C44;           // 0x1C44
    u8 mUnk1C48;            // 0x1C48
};

#endif
