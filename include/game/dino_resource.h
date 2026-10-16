// The game's resources: the archives it keeps loaded, the file paths it opens by name, and its fonts.
#ifndef GAME_DINO_RESOURCE_H
#define GAME_DINO_RESOURCE_H

#include "m/font.h"

class DinoResource {
public:
    MFontSet *CreateFont24(int color, int shadow, int back);
    MFontSet *CreateFont16(int color, int shadow, int back);
    MFontSet *CreateFont12(int color, int shadow, int back);
};

extern DinoResource *gDinoResource;

#endif
