// The field (the overworld, in overlay 1), as the main program's event commands use it. Only what they call is
// declared.
#ifndef GAME_DINO_FIELD_H
#define GAME_DINO_FIELD_H

#include "types.h"
#include "m/fx32.h"
#include "m/geometry.h"

class MTask;
struct DinoCharaTask;

class DinoFieldNamePanel {
public:
    void SetMode(int mode, int arg);
};

class DinoFieldCamera {
public:
    void ResetHeight();
    void SetTracking(u8 tracking);
    void StartRotate(u16 x, u16 y, u16 z, u32 a, u32 b, u32 c);

    u8 unk00[0x144];
    DinoFieldNamePanel *mNamePanel; // 0x144
};

class DinoFieldChara {
public:
    void ShowManpu(u32 manpu, MPoint offset);
    void HideManpu();
    void CacheMotion(u32 motion);
    void ClearMotionCache();
    void SetMotion(u32 motion, u8 loop, MFx32 speed);
    void ApplyMotion();
    void SetEffect(u32 effect);
    void SetCommonMotion(u32 motion);
    void StartBehold(u32 target);
    void FinishBehold();
    void SetFootSe(u32 se);
    void SetTask(DinoCharaTask task);
    void UpdateBodyModel();
    void UpdateHeadModel();
};

class DinoFieldRadar {
public:
    MTask *CreateModeTask(u32 from, u32 to, u32 a, u8 b);
};

class DinoFieldTownMap {
public:
    MTask *CreateModeTask(u32 radarMode, u32 to, u32 a);
};

class DinoField {
public:
    DinoFieldCamera *GetCamera();
    DinoFieldChara *GetChara(u32 id);
    void PlayBgm(u32 bgm, u32 arg);
    void StopBgm(u32 fadeFrames);
    void PlayBgmFade(u32 bgm, u32 arg);
    void Quake(u32 a, u32 b, u32 c);
    void RemoveChara(u32 id);
    MTask *CreateCharaAngleTask(u32 chara, u32 angle, u32 a, u32 b);
    MTask *CreateCharaMoveToRegionTask(u32 chara, u32 region, u32 a, u32 b, u8 c);
    MTask *CreateCharaMoveToCoordTask(u32 chara, MPoint coord, u32 a, u32 b, u8 c);
    MTask *CreateCharaMoveOffsetTask(u32 chara, MPoint offset, u32 a, u32 b, u8 c);
    MTask *CreateCharaMoveVectorTask(u32 chara, u16 direction, u32 a, u32 b, u32 c, u8 d);
    MTask *CreateCharaWaitMoveTask(u32 chara);
    MTask *CreateCharaFocusTask(u32 chara);
    MTask *CreateMoneyShowTask();
    MTask *CreateMoneyHideTask();
    MTask *CreateDonateShowTask();
    MTask *CreateDonateHideTask();

    u8 unk000[0x244];
    DinoFieldRadar *mRadar;     // 0x244
    DinoFieldTownMap *mTownMap; // 0x248
};

// The event being run.
class DinoEvent {
public:
    void MarkCameraUsed();
    DinoField *GetField();
    void Fade(u32 a, u32 b);
    MTask *CreateFadeWaitTask();
};

#endif
