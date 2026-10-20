#ifndef GAME_DINO_FIELD_TOWN_MAP_H
#define GAME_DINO_FIELD_TOWN_MAP_H

#include "m/fx32.h"
#include "m/task.h"

class TownMapIcon;
class MTextPicture;
class MParallelTaskSet;

class DinoField;

class DinoFieldTownMap : public MProcess {
public:
    // With the map's picture and the player's icon, when there is a town map.
    DinoFieldTownMap(DinoField *field, u32 map);
    virtual ~DinoFieldTownMap();
    virtual void Idle();

    // Puts the icon at the character's scaled position (or hides it if off).
    void Update(MFx32Point pos, MPoint offset, MFx32 scale);

    MTextPicture *mPicture; // 0x74: the map (none: no town map)
    TownMapIcon *mIcon; // 0x78: the player's
    MParallelTaskSet *mIcons;   // 0x7C
    MTaskSet *mBlinks;  // 0x80
    DinoField *mField;  // 0x84
};

#endif
