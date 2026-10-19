// A character drawn as animated 3D objects played from a scene script (MAnmModelBase and the other MAnm kinds).
#ifndef M_POLYGON_CHAR_H
#define M_POLYGON_CHAR_H

#include "m/task.h"
#include "nitro/fx.h"

class MAnmModelBase;

class MPolygonChar : public MProcess {
public:
    MPolygonChar(void *unk80, u16 unkA4, void *unk84, void *unk88);
    virtual ~MPolygonChar();
    virtual void Idle();

    void SetPosition(fx32 x, fx32 y, fx32 z) {
        mPos.x = x;
        mPos.y = y;
        mPos.z = z;
    }
    void SetRotation(fx32 x, fx32 y, fx32 z) {
        mRot.x = x;
        mRot.y = y;
        mRot.z = z;
    }
    void SetAngle(u16 angle) {
        mRot.x = 0;
        mRot.y = (angle * 360) >> 4;
        mRot.z = 0;
    }
    void Reset();
    // Stops the models following their parents, or deactivates them (the game inlines these in DinoField::Step).
    void DetachModels();
    void DeactivateModels();
    // Loads a scene script and the model it plays.
    void Load(const char *script, const char *model);

    MAnmModelBase *mUnk74;      // 0x74
    MAnmModelBase *mUnk78;      // 0x78
    MAnmModelBase *mUnk7C;      // 0x7C: restarted whenever mUnk74 loops
    void *mUnk80;               // 0x80
    void *mUnk84;               // 0x84
    void *mUnk88;               // 0x88
    VecFx32 mPos;               // 0x8C
    VecFx32 mRot;               // 0x98
    u16 mUnkA4;                 // 0xA4
    u16 mUnkA6;                 // 0xA6
    u32 mUnkA8;                 // 0xA8
    u8 mUnkAC;                  // 0xAC
    int mUnkB0;                 // 0xB0
    int mEndFrame;              // 0xB4
    int mFrame;                 // 0xB8
    u8 unkBC[0x43C - 0xBC];
    u32 mUnk43C;                // 0x43C
    OSMutex mMutex;             // 0x440
    int mPauseCount;            // 0x458: the task sets stay suspended while this isn't 0
};

#endif
