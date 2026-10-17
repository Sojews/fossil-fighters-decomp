// DinoField: the field (the overworld).

#include "game/dino_field.h"
#include "game/dino_field_chara_focus_task.h"
#include "game/dino_field_chara_wait_move_task.h"
#include "game/dino_field_chara_move_vector_task.h"
#include "game/dino_field_chara_move_offset_task.h"
#include "game/dino_field_chara_move_to_coord_task.h"
#include "game/dino_field_chara_move_to_region_task.h"
#include "game/dino_field_chara_angle_task.h"

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

MTask *DinoField::CreateCharaAngleTask(u32 chara, u32 angle, u32 a, u32 b) {
    return new DinoFieldCharaAngleTask(this, chara, angle, a, b);
}

MTask *DinoField::CreateCharaMoveToRegionTask(u32 chara, u32 region, u32 a, u32 b, u8 c) {
    return new DinoFieldCharaMoveToRegionTask(this, chara, region, a, b, c);
}

MTask *DinoField::CreateCharaMoveToCoordTask(u32 chara, MFx32Point coord, u32 a, u32 b, u8 c) {
    return new DinoFieldCharaMoveToCoordTask(this, chara, coord, a, b, c);
}

MTask *DinoField::CreateCharaMoveOffsetTask(u32 chara, MFx32Point offset, u32 a, u32 b, u8 c) {
    return new DinoFieldCharaMoveOffsetTask(this, chara, offset, a, b, c);
}

// NONMATCHING: the game spills the register arguments instead of keeping a in r7.
MTask *DinoField::CreateCharaMoveVectorTask(u32 chara, u16 direction, u32 a, u32 b, u32 c, u8 d) {
    return new DinoFieldCharaMoveVectorTask(this, chara, direction, a, b, c, d);
}

MTask *DinoField::CreateCharaWaitMoveTask(u32 chara) {
    return new DinoFieldCharaWaitMoveTask(this, chara);
}

MTask *DinoField::CreateCharaFocusTask(u32 chara) {
    return new DinoFieldCharaFocusTask(this, chara);
}
