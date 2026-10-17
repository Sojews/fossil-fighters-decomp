// DinoField: the field (the overworld).

#include "game/dino_field.h"

BOOL DinoField::IsUnk258() {
    if (mUnk258) {
        return TRUE;
    }
    return FALSE;
}

DinoFieldCamera *DinoField::GetCamera() {
    return mCamera;
}

DinoFieldMap *DinoField::GetMap() {
    return mMap;
}

u32 DinoField::GetUnk234() {
    return mUnk234;
}

u32 DinoField::GetUnk26C() {
    return mUnk26C;
}

void DinoField::SetUnk238(u32 value) {
    mUnk238 = value;
}

DinoFieldChara *DinoField::GetChara(u32 id) {
    for (u32 i = 0; i < 100; i++) {
        if (mCharas[i]->GetModel()->id == id) {
            if (!mCharas[i]->GetModel()->unk04 || i == mUnk238) {
                return mCharas[i];
            }
        }
    }
    return NULL;
}
