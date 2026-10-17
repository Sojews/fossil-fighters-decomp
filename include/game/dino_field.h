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
#include "m/archive.h"
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
    void SetFocus(u32 chara);
    void StartRotate(u16 x, u16 y, u16 z, u32 a, u32 b, u32 c);

    u8 unk8C[0xE4 - 0x8C];
    u32 mFocus;                     // 0xE4: the character followed
    u8 unkE8[0x144 - 0xE8];
    DinoFieldNamePanel *mNamePanel; // 0x144
    u32 mUnk148[2];                 // 0x148
    std::vector<void *> mUnk150;    // 0x150
};

// A character's model: where it stands and which way it faces.
struct DinoFieldCharaModel {
    u32 id;             // 0x00: the character id
    u8 unk04;           // 0x04: several characters share the id; see DinoField::GetChara
    u8 unk05[3];
    MFx32Point pos;     // 0x08
    u8 unk10[6];
    u16 angle;          // 0x16
};

class DinoFieldChara : public MProcess, public MSerializer {
public:
    virtual ~DinoFieldChara();

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
    void ApplyBodyModel();
    void ApplyHeadModel();
    void ChangeMotion(u32 motion, u8 loop, MFx32 speed);
    void SetAngle(u32 angle, u32 a, u32 b);
    void SetDirection(u16 angle);
    void MoveTo(MFx32Point coord, u32 a, u32 b, u8 c);
    MTask *CreateWaitMoveTask();

    MArchivePtr mArchive;       // 0x8C
    MArchiveFile mFile;         // 0x90
    DinoFieldCharaModel mModel; // 0x9C
    u32 mMotion;                // 0xB4: the motion playing
    u32 mUnkB8[22];             // 0xB8
    MTaskSet *mTaskSet;         // 0x110: where the manpu task runs
    u32 mUnk114[121];           // 0x114
    MTask *mManpu;              // 0x2F8: the emotion bubble shown over the character
    u32 mUnk2FC[2];             // 0x2FC
    BOOL mHasFootSe;            // 0x304
    u32 mFootSe;                // 0x308
    u32 mBeholdTarget;          // 0x30C: the character looked at
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

class DinoFieldMap : public MProcess, public MSerializer {
public:
    virtual ~DinoFieldMap();
    virtual void Idle();
    virtual void Read(const void *buf);

    DinoFieldMapInfo *GetInfo();

    DinoFieldMapInfo mInfo; // 0x8C
    u32 mUnk90;             // 0x90
    MArchivePtr mArchive;   // 0x94
    MArchiveFile mFile;     // 0x98
    MArchivePtr mArchive2;  // 0xA4
    MArchiveFile mFile2;    // 0xA8
    MArchivePtr mArchive3;  // 0xB4
    MArchiveFile mFile3;    // 0xB8
};

class DinoField {
public:
    DinoFieldMap *GetMap();
    DinoFieldCamera *GetCamera();
    DinoFieldChara *GetChara(u32 id);
    void PlayBgm(u32 bgm, u32 arg);
    void StopBgm(u32 fadeFrames);
    void PlayBgmFade(u32 bgm, u32 arg);
    void Fade(u32 a, u32 b);
    MTask *CreateFadeWaitTask();
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

    BOOL IsUnk258();
    u32 GetUnk234();
    u32 GetUnk26C();
    void SetUnk238(u32 value);

    u8 unk000[0x9C];            // (MProcess, and the bases at 0x74 and 0x8C)
    DinoFieldMap *mMap;         // 0x9C
    DinoFieldChara *mCharas[100]; // 0xA0
    u32 mUnk230;                // 0x230
    u32 mUnk234;                // 0x234
    u32 mUnk238;                // 0x238: for characters with the model flag, the one GetChara returns
    u32 mUnk23C;                // 0x23C
    DinoFieldCamera *mCamera;   // 0x240
    DinoFieldRadar *mRadar;     // 0x244
    DinoFieldTownMap *mTownMap; // 0x248
    u32 mUnk24C[3];             // 0x24C
    u8 mUnk258;                 // 0x258
    u8 unk259[0x13];
    u32 mUnk26C;                // 0x26C
};

// The event being run.
// The dance minigame.
class DinoFieldDance : public MProcess {
public:
    DinoFieldDance(DinoField *field, u32 a, u32 b, u32 c, u32 d, u32 e);
    virtual ~DinoFieldDance();
    virtual void Idle();

    void (DinoFieldDance::*mState)(); // 0x74
    u8 unk7C[184];
};

// The map select menu (in overlay 4).
class DinoMapSelectMenu : public MTask {
public:
    DinoMapSelectMenu(s32 *result, u32 unlocked);
    virtual void Execute();

    u8 unk10[0x6C];
};

// Runs the event scripts (the game's class name; the event commands get it as their event).
class DinoEventLoop : public MProcess {
public:
    virtual ~DinoEventLoop();
    virtual void Idle();
    MTask *CreateTalkTask(u32 id);
    std::string GetTalkText(u32 id);
    void SetResult(u32 result);
    void CloseField(BOOL arg);
    void OpenField(BOOL arg);
    void MarkCameraUsed();
    DinoField *GetField();
    void Fade(u32 a, u32 b);
    MTask *CreateFadeWaitTask();
    void SetEpisodesChanged();
    void AddEpisode(u32 id);
    void RemoveEpisode(u32 id);
    void ResetEpisode(u32 id);

    void (DinoEventLoop::*mState)();    // 0x74
    u32 *mResult;           // 0x7C
    DinoField *mField;      // 0x80
    u32 mUnk84[14];         // 0x84
    u8 mCameraUsed;         // 0xBC
    u8 unkBD[0x57];
    void *mEpisodes;        // 0x114
    u8 mEpisodesChanged;    // 0x118
};

#endif
