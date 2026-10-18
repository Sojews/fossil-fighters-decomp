#ifndef GAME_DINO_FIELD_CAMERA_HOOK_H
#define GAME_DINO_FIELD_CAMERA_HOOK_H

#include "types.h"

// Told about the field camera (by the field's managers).
class DinoFieldCameraHook {
public:
    virtual void CameraUnk0() = 0;
    virtual void CameraUnk1(u32 map) = 0;  // the field moved to a map
};

#endif
