#ifndef GAME_DINO_CLEANING_IMAGE_LAYER_H
#define GAME_DINO_CLEANING_IMAGE_LAYER_H

#include <string>
#include "m/picture.h"

// One layer of a cleaning image.
class DinoCleaningImageLayer {
public:
    virtual ~DinoCleaningImageLayer();

    u8 mShared;             // 0x04: mPixels belongs to someone else
    std::string mName;      // 0x08
    u32 mUnk14;             // 0x14
    u8 *mPixels;            // 0x18
    u8 unk1C[0x1B98];
    MPictureFile mPicture;  // 0x1BB4
};

#endif
