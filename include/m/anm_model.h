// Animated 3D objects played by a scene script: MAnmModelBase and the kinds of object the scripts make (models,
// particle emitters, camera targets, shadows, sounds and trails).
#ifndef M_ANM_MODEL_H
#define M_ANM_MODEL_H

#include <string>
#include <vector>
#include "m/task.h"
#include "m/seed_generator.h"
#include "nitro/fx.h"

class M3DScene;
class M3DModel;
class M3DModelResource;

// A key of an animated value: from this frame on, a random value in [min, max).
struct MAnmKey {
    int mFrame;     // 0x0
    int mMin;       // 0x4
    int mMax;       // 0x8
    int mUnkC;      // 0xC
};

class MAnmModelBase : public MTask {
public:
    virtual ~MAnmModelBase();
    virtual void Execute();
    virtual void Unk0C();
    virtual void Unk10();

    void Init();
    void Load(const char *path, u8 keepPalette);
    void Unload();
    int GetValue(int track, int frame);
    void PlaySounds(int frame);
    void TintPalette(u16 color, fx32 amount);
    void ClearTransformTracks();

    std::vector<MAnmKey> mTracks[16];   // 0x010
    std::vector<MAnmKey> mSounds;       // 0x0D0: the frame, then DinoSoundFunc_0202016c's arguments
    std::string mName;                  // 0x0DC
    fx32 mFrame;                        // 0x0E8
    fx32 mStartFrame;                   // 0x0EC
    fx32 mEndFrame;                     // 0x0F0
    u32 mUnkF4;                         // 0x0F4
    fx32 mLoopFrame;                    // 0x0F8
    fx32 mLoopEnd;                      // 0x0FC
    u32 unk100;
    M3DScene *mScene;                   // 0x104
    M3DModel *mModel;                   // 0x108
    MAnmModelBase *mParent;             // 0x10C
    int mJoint;                         // 0x110: the parent's joint to follow, or -1
    u8 mLoaded;                         // 0x114
    u8 mFollowParent;                   // 0x115
    u8 mActive;                         // 0x116
    u8 mUnk117;                         // 0x117
    u8 mOwnsResource;                   // 0x118
    u8 mVisible;                        // 0x119
    u8 mUnk11A;                         // 0x11A
    u8 mUnk11B;                         // 0x11B
    u8 mUnk11C;                         // 0x11C
    u32 mUnk120;                        // 0x120
    u32 mUnk124;                        // 0x124
    u32 unk128[3];
    u32 mUnk134;                        // 0x134
    u16 mUnk138;                        // 0x138
    u32 mUnk13C;                        // 0x13C
    u32 mUnk140;                        // 0x140
    u8 mUnk144;                         // 0x144
    u8 mUnk145;                         // 0x145
    fx32 mSpeed;                        // 0x148
    MtxFx44 mMatrix;                    // 0x14C
    u8 *mUnk18C;                        // 0x18C
    u8 *mUnk190;                        // 0x190
    u8 *mUnk194;                        // 0x194
    M3DModelResource *mResource;        // 0x198
    MRandom mRandom;                    // 0x19C
    u32 mUnk1A4;                        // 0x1A4
};

class MAnmModel : public MAnmModelBase {
public:
    virtual ~MAnmModel();
    virtual void Unk0C();
    virtual void Unk10();
};

class MAnmEmit : public MAnmModelBase {
public:
    virtual ~MAnmEmit();
    virtual void Unk0C();
    virtual void Unk10();

    M3DModel *mParticles[32];           // 0x1A8
};

class MAnmTargetCamera : public MAnmModelBase {
public:
    virtual ~MAnmTargetCamera();
    virtual void Unk0C();
};

class MAnmSE : public MAnmModelBase {
public:
    virtual ~MAnmSE();
    virtual void Unk0C();
};

class M3DRyusen;

class MAnmRyusen : public MAnmModelBase {
public:
    virtual ~MAnmRyusen();
    virtual void Unk0C();
    virtual void Unk10();

    u8 unk1A8[0x1290 - 0x1A8];
    M3DRyusen *mTrails;                 // 0x1290: 180 of them
};

class MAnmModelSh : public MAnmModelBase {
public:
    virtual ~MAnmModelSh();
    virtual void Unk0C();
    virtual void Unk10();
};

#endif
