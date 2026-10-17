// The field (the overworld, in overlay 1), as the main program's event commands use it. Only what they call is
// declared.
#ifndef GAME_DINO_FIELD_H
#define GAME_DINO_FIELD_H

#include "types.h"
#include "m/fx32.h"
#include "m/geometry.h"

#include <string>
#include <vector>
#include "m/task.h"
#include "m/serializer.h"
#include "game/dino_field_town_map.h"
struct DinoCharaTask;

class DinoFieldNamePanel {
public:
    void SetMode(int mode, int arg);
};

class DinoFieldCamera : public MProcess, public MSerializer {
public:
    virtual ~DinoFieldCamera();
    virtual void Idle();
    virtual void Read(const void *buf);
    virtual void UnkV14();

    void ResetHeight();
    void SetTracking(u8 tracking);
    void StartRotate(u16 x, u16 y, u16 z, u32 a, u32 b, u32 c);

    u8 unk8C[0x144 - 0x8C];
    DinoFieldNamePanel *mNamePanel; // 0x144
    u32 mUnk148[2];                 // 0x148
    std::vector<void *> mUnk150;    // 0x150
};

// A character's model: where it stands and which way it faces.
struct DinoFieldCharaModel {
    u8 unk00[8];
    MFx32Point pos;     // 0x08
    u8 unk10[6];
    u16 angle;          // 0x16
};

class DinoFieldChara {
public:
    DinoFieldCharaModel *GetModel();
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

struct MAfnBlock;

class DinoFieldRadar : public MProcess {
public:
    virtual ~DinoFieldRadar();
    virtual void Idle();
    MTask *CreateModeTask(u32 from, u32 to, u32 a, u8 b);

    u32 mUnk74[11];             // 0x74
    MAfnBlock *mAffine;         // 0xA0
    u8 unkA4[0x4E8];
    std::vector<short> mUnk58C; // 0x58C: (the element type is a guess, distinct from other overlays')
};


struct DinoFieldMapInfo {
    u32 id;
};

class DinoFieldMap {
public:
    DinoFieldMapInfo *GetInfo();
};

class DinoField {
public:
    DinoFieldMap *GetMap();
    DinoFieldCamera *GetCamera();
    DinoFieldChara *GetChara(u32 id);
    void PlayBgm(u32 bgm, u32 arg);
    void StopBgm(u32 fadeFrames);
    void PlayBgmFade(u32 bgm, u32 arg);
    void Quake(u32 a, u32 b, u32 c);
    void SetUnk258(BOOL value);
    void SyncChara(u32 id);
    MTask *CreateCharaAngleTask(u32 chara, u32 angle, u32 a, u32 b);
    MTask *CreateCharaMoveToRegionTask(u32 chara, u32 region, u32 a, u32 b, u8 c);
    MTask *CreateCharaMoveToCoordTask(u32 chara, MFx32Point coord, u32 a, u32 b, u8 c);
    MTask *CreateCharaMoveOffsetTask(u32 chara, MFx32Point offset, u32 a, u32 b, u8 c);
    MTask *CreateCharaMoveVectorTask(u32 chara, u16 direction, u32 a, u32 b, u32 c, u8 d);
    MTask *CreateCharaWaitMoveTask(u32 chara);
    MTask *CreateCharaFocusTask(u32 chara);
    MTask *CreateMoneyShowTask();
    MTask *CreateMoneyHideTask();
    MTask *CreateDonateShowTask();
    MTask *CreateDonateHideTask();
    MTask *CreatePictWaitTask();
    BOOL IsBgmChanging();
    void ShowPict(u32 pict, MPoint pos, u32 frames, long unk);
    void HidePict(u32 frames, long unk);
    void PlayMapBgm(u32 bgm, u32 arg);
    void FadeInLowerPict(u32 pict, MPoint pos, u32 frames, long unk);
    void FadeOutLowerPict(u32 pict, long frames);
    void SlideInLowerPict(u32 pict, MPoint pos, u32 a, u32 b, long c);
    void SlideOutLowerPict(u32 pict, u32 a, long b);
    void ZoomInLowerPict(u32 pict, MPoint pos, MFx32 scale, u32 a, long b);
    void ZoomOutLowerPict(MFx32 scale, u32 a, long b);
    void FadeInUpperPict(u32 pict, MPoint pos, u32 frames, long unk);
    void FadeOutUpperPict(u32 pict, long frames);
    void SlideInUpperPict(u32 pict, MPoint pos, u32 a, u32 b, long c);
    void SlideOutUpperPict(u32 pict, u32 a, long b);
    void ZoomInUpperPict(u32 pict, MPoint pos, MFx32 scale, u32 a, long b);
    void ZoomOutUpperPict(MFx32 scale, u32 a, long b);

    u8 unk000[0x244];
    DinoFieldRadar *mRadar;     // 0x244
    DinoFieldTownMap *mTownMap; // 0x248
};

// The event being run.
// The dance minigame.
class DinoFieldDance : public MTask {
public:
    DinoFieldDance(DinoField *field, u32 a, u32 b, u32 c, u32 d, u32 e);
    virtual void Execute();

    u8 unk10[0x124];
};

// The map select menu (in overlay 4).
class DinoMapSelectMenu : public MTask {
public:
    DinoMapSelectMenu(s32 *result, u32 unlocked);
    virtual void Execute();

    u8 unk10[0x6C];
};

class DinoEvent : public MProcess {
public:
    MTask *CreateTalkTask(u32 id);
    std::string GetTalkText(u32 id);
    void SetResult(u32 result);
    void CloseField(BOOL arg);
    void OpenField(BOOL arg);
    void MarkCameraUsed();
    DinoField *GetField();
    void Fade(u32 a, u32 b);
    MTask *CreateFadeWaitTask();
};

#endif
