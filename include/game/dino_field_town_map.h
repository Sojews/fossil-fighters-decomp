#ifndef GAME_DINO_FIELD_TOWN_MAP_H
#define GAME_DINO_FIELD_TOWN_MAP_H

#include "m/fx32.h"
#include "m/task.h"

class DinoFieldTownMap : public MProcess {
public:
    virtual ~DinoFieldTownMap();
    virtual void Idle();

    MTask *CreateModeTask(u32 radarMode, u32 to, u32 a);
    void Update(MFx32Point pos, MFx32Point offset, MFx32 scale);

    u32 mUnk74;         // 0x74: blink while set
    u32 unk78[2];
    MTaskSet *mBlinks;  // 0x80
};

#endif
