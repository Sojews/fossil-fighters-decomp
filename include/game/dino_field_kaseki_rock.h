#ifndef GAME_DINO_FIELD_KASEKI_ROCK_H
#define GAME_DINO_FIELD_KASEKI_ROCK_H

#include "m/task.h"
#include "nitro/fx.h"

class DinoField;
class M3DScene;
class MAnmModelBase;

class DinoFieldKasekiRock : public MProcess {
public:
    DinoFieldKasekiRock(DinoField *field, M3DScene *scene, u32 index);
    virtual ~DinoFieldKasekiRock();
    virtual void Idle();

    DinoField *mField;          // 0x74
    M3DScene *mScene;           // 0x78
    u32 mIndex;                 // 0x7C
    MAnmModelBase *mModel;      // 0x80
    VecFx32 mPos;               // 0x84
    VecFx32 mRot;               // 0x90
};

#endif
