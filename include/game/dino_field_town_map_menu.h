#ifndef GAME_DINO_FIELD_TOWN_MAP_MENU_H
#define GAME_DINO_FIELD_TOWN_MAP_MENU_H

#include "m/fx32.h"
#include "game/dino_field.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoFieldTownMapManager : public MProcess, public DinoFieldCameraHook {
public:
    DinoFieldTownMapManager(DinoField *field);
    virtual ~DinoFieldTownMapManager();
    virtual void Idle();
    virtual void CameraUnk0(u32 map);
    virtual void CameraUnk1(u32 map);
    // Fades the town map out (to 0) or makes it and fades it in.
    MTask *CreateModeTask(u32 radarMode, u32 to, u32 frames);

    DinoField *mField;      // 0x78
    DinoFieldTownMap *mTownMap;  // 0x7C: while the camera allows it
    MPoint mOffset;         // 0x80: of the map on the screen
    MFx32 mScale;           // 0x88
    u8 mCentered;           // 0x8C: show the map's centre instead of the player
};

#endif
