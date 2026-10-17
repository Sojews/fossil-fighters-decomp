#ifndef GAME_DINO_FIELD_CAMERA_HOOK_H
#define GAME_DINO_FIELD_CAMERA_HOOK_H

#include "types.h"

// Told about the field camera (by the field's managers).
class DinoFieldCameraHook {
public:
    virtual void CameraUnk0() = 0;
    virtual void CameraUnk1() = 0;
};

#endif
