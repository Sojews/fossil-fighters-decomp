// DinoFieldMap.
// Only the destructor and GetInfo are decompiled so far.

#include "game/dino_field.h"

DinoFieldMap::~DinoFieldMap() {
}

DinoFieldMapInfo *DinoFieldMap::GetInfo() {
    return &mInfo;
}

void DinoFieldMap::Idle() {
}

void DinoFieldMap::Read(const void *buf) {
    MSerializer::Read(buf);
    Load();
}
