#ifndef GAME_DINO_CLEANING_IMAGE_H
#define GAME_DINO_CLEANING_IMAGE_H

#include <vector>
#include "m/picture.h"
#include "game/dino_cleaning_image_layer.h"

// A cleaning image: a picture and the layers drawn over it.
class DinoCleaningImage {
public:
    virtual ~DinoCleaningImage();

    u32 mUnk04[2];                                  // 0x04
    std::vector<DinoCleaningImageLayer *> mLayers;  // 0x0C
    MPictureFile mPicture;                          // 0x18
};

#endif
