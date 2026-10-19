#ifndef GAME_DINO_FIELD_TOWN_MAP_H
#define GAME_DINO_FIELD_TOWN_MAP_H

#include "m/fx32.h"
#include "m/task.h"

class TownMapIcon;

class DinoFieldTownMap : public MProcess {
public:
    virtual ~DinoFieldTownMap();
    virtual void Idle();

    // Puts the icon at the character's scaled position (or hides it if off).
    void Update(MFx32Point pos, MPoint offset, MFx32 scale);

    u32 mUnk74;         // 0x74: blink while set
    TownMapIcon *mIcon; // 0x78: the player's
    u32 unk7C;
    MTaskSet *mBlinks;  // 0x80
};

#endif
