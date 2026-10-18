// The field (the overworld, in overlay 1), as the main program's event commands use it. Only what they call is
// declared.
#ifndef GAME_DINO_FIELD_H
#define GAME_DINO_FIELD_H

#include "types.h"
#include "m/fx32.h"
#include "m/geometry.h"

#include <string>
#include <vector>
#include <map>
#include "m/task.h"
#include "game/help.h"
#include "m/archive.h"
#include "m/serializer.h"
#include "game/dino_field_town_map.h"
#include "game/dino_backup.h"
struct DinoCharaTask;
class DinoField;
class MTextPicture;
class MSmartOam;
class MDisplay;

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
    u32 GetFocus();
    u16 GetUnkD8();

    u8 unk8C[0xD8 - 0x8C];
    u16 mUnkD8;                     // 0xD8
    u8 unkDA[0xE4 - 0xDA];
    u32 mFocus;                     // 0xE4: the character followed
    u32 mUnkE8;                     // 0xE8
    s32 mUnkEC;                     // 0xEC
    u32 mUnkF0[2];                  // 0xF0
    u8 mTracking;                   // 0xF8
    u8 unkF9[0x144 - 0xF9];
    DinoFieldNamePanel *mNamePanel; // 0x144
    u32 mUnk148;                    // 0x148
    u8 mHeightSet;                  // 0x14C
    std::vector<void *> mUnk150;    // 0x150
};

// A character's model: where it stands and which way it faces.
struct DinoFieldCharaModel {
    u32 id;             // 0x00: the character id
    u8 unk04;           // 0x04: several characters share the id; see DinoField::GetChara
    u8 unk05[3];
    MFx32Point pos;     // 0x08
    u32 unk10;          // 0x10
    u8 unk14[2];
    u16 angle;          // 0x16
};

class DinoFieldChara : public MProcess, public MSerializer {
public:
    virtual ~DinoFieldChara();
    virtual void Read(const void *buf);

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
    void SetAngle(u32 angle, u32 frames, u32 ease);
    void SetDirection(u16 angle);
    BOOL IsMoving();
    BOOL IsUnkA1Motion();
    MFx32 GetHeight();
    void ResetState();
    MFx32 GetMoveSpeed();
    u8 GetUnk2C();
    MRect GetRect30();
    u8 GetUnk2D();
    MRect GetRect40();
    MFx32 GetUnk50();
    void SetUnk2D8(MPoint pos, u32 a, u32 b);
    void ClearUnk2E0();
    void MoveTo(MFx32Point coord, u32 frames, u32 ease, u8 silent);
    MTask *CreateWaitMoveTask();

    MArchivePtr mArchive;       // 0x8C
    MArchiveFile mFile;         // 0x90
    DinoFieldCharaModel mModel; // 0x9C
    u32 mMotion;                // 0xB4: the motion playing
    MFx32 mHomeX;               // 0xB8: where the character was placed
    MFx32 mHomeY;               // 0xBC
    u32 mUnkC0;                 // 0xC0
    u32 mUnkC4;                 // 0xC4
    DinoCharaTask mTask;        // 0xC8: what the character does on its own
    MTaskSet *mAngleTasks;      // 0x108
    MTaskSet *mMoveTasks;       // 0x10C
    MTaskSet *mTaskSet;         // 0x110: where the manpu task runs
    u32 mUnk114;                // 0x114
    DinoField *mField;          // 0x118
    u32 mUnk11C;                // 0x11C
    void *mUnk120;              // 0x120
    u32 mUnk124[109];           // 0x124
    MPoint mUnk2D8;             // 0x2D8
    u32 mUnk2E0;                // 0x2E0
    u32 mUnk2E4;                // 0x2E4
    u32 mUnk2E8[4];             // 0x2E8
    MTask *mManpu;              // 0x2F8: the emotion bubble shown over the character
    u32 mUnk2FC[2];             // 0x2FC
    BOOL mHasFootSe;            // 0x304
    u32 mFootSe;                // 0x308
    u32 mBeholdTarget;          // 0x30C: the character looked at
    u32 mUnk310;                // 0x310
    u32 mUnk314;                // 0x314
    u32 mMoveSe;                // 0x318: the footstep sound when walking
};

struct MAfnBlock;

class DinoFieldRadar : public MProcess {
public:
    DinoFieldRadar(DinoField *field, u32 map);
    virtual ~DinoFieldRadar();
    virtual void Idle();
    MTask *CreateModeTask(u32 from, u32 to, u32 a, u8 b);
    void SetPlayer(MFx32Point pos, u16 angle, MFx32 height);

    u32 mUnk74[3];              // 0x74
    MSmartOam *mOam;            // 0x80
    u32 mUnk84[7];              // 0x84
    MAfnBlock *mAffine;         // 0xA0
    u8 unkA4[0x56C - 0xA4];
    MFx32 mScale;               // 0x56C: map units to radar pixels
    u8 unk570[0x58C - 0x570];
    std::vector<short> mUnk58C; // 0x58C: (the element type is a guess, distinct from other overlays')
    MFx32Point mPlayerPos;      // 0x598
    u16 mPlayerAngle;           // 0x5A0
    MFx32 mPlayerHeight;        // 0x5A4
    u32 unk5A8;
};


struct DinoFieldMapInfo {
    u32 id;
};

// The map file's header.
struct DinoFieldMapData {
    u8 unk00[0x30];
    MFx32 mWalkSpeed;   // 0x30: the speed characters' walk motion plays at
};

class DinoFieldMap : public MProcess, public MSerializer {
public:
    virtual ~DinoFieldMap();
    virtual void Idle();
    virtual void Read(const void *buf);

    DinoFieldMapInfo *GetInfo();
    void Load();
    MFx32 GetWalkSpeed();

    DinoFieldMapInfo mInfo; // 0x8C
    u32 mUnk90;             // 0x90
    MArchivePtr mArchive;   // 0x94
    MArchiveFile mFile;     // 0x98
    MArchivePtr mArchive2;  // 0xA4
    MArchiveFile mFile2;    // 0xA8
    MArchivePtr mArchive3;  // 0xB4
    MArchiveFile mFile3;    // 0xB8
};

class DinoFieldPoint;

class DinoField : public MProcess, public MSerializer, public DinoHelpBase {
public:
    virtual ~DinoField();
    virtual void Read(const void *buf);
    virtual void Write(void *buf);
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
    u32 GetBgm();
    void FadeInPict(MDisplay *display, u32 *id, MTextPicture **pict, u32 newId, MPoint pos, u32 frames, long unk);
    void FadeOutPict(MDisplay *display, MTextPicture **pict, u32 id, long frames);
    void SlideInPict(MDisplay *display, u32 *id, MTextPicture **pict, u32 newId, MPoint pos, u32 a, u32 b, long c);
    void SlideOutPict(MDisplay *display, MTextPicture **pict, u32 id, u32 a, long b);
    void ZoomInPict(MDisplay *display, u32 *id, MTextPicture **pict, u32 newId, MPoint pos, MFx32 scale, u32 a, long b);
    void ZoomOutPict(MDisplay *display, MTextPicture **pict, MFx32 scale, u32 a, long b);

    u32 mUnk98;                 // 0x98
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
    u8 unk259[0xF];
    MTaskSet *mFadeTasks;       // 0x268
    u32 mUnk26C;                // 0x26C
    MTaskSet *mPictTasks;       // 0x270
    MTextPicture *mLowerPict;   // 0x274: the event picture on each screen
    MTextPicture *mUpperPict;   // 0x278
    MPoint mLowerPos;           // 0x27C
    MPoint mUpperPos;           // 0x284
    u32 mUpperPictId;           // 0x28C
    u32 mLowerPictId;           // 0x290
    u32 mBgm;                   // 0x294
    BOOL mBgmChanging;          // 0x298
    u32 mBgmScene;              // 0x29C: the sound scene when the BGM was set
    u8 mQuake;                  // 0x2A0
    u32 mQuakeArgs[3];          // 0x2A4
    u32 mQuakeTime;             // 0x2B0
    u32 mUnk2B4;                // 0x2B4
    MTask *mUnk2B8;             // 0x2B8
    u32 mUnk2BC;                // 0x2BC
    MTask *mUnk2C0;             // 0x2C0
    MTask *mUnk2C4;             // 0x2C4
    BOOL *mUnk2C8;              // 0x2C8
    u8 unk2CC[0xB4];
    MArchivePtr mArchive;       // 0x380
    MArchiveFile mFile;         // 0x384
    DinoFieldPoint *mMoney;     // 0x390: the money panel, while shown
    DinoFieldPoint *mDonate;    // 0x394
    u8 unk398[0x30];
    u8 mUnk3C8;                 // 0x3C8
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
class DinoMapSelectMenu : public MProcess {
public:
    DinoMapSelectMenu(s32 *result, u32 unlocked);
    virtual ~DinoMapSelectMenu();
    virtual void Idle();

    u32 mUnlocked;      // 0x74
    s32 *mResult;       // 0x78
};

// A loaded episode file and the archive holding it.
struct DinoEventEpisode {
    MArchivePtr mArchive;   // 0x00
    MArchiveFile mFile;     // 0x04
};

// Runs the event scripts (the game's class name; the event commands get it as their event).
class DinoEventLoop : public MProcess {
public:
    virtual ~DinoEventLoop();
    virtual void Idle();
    MTask *CreateTalkTask(u32 id);
    void Talk(u32 id);
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
    void ProcessEpisode(const std::map<u32, DinoEventEpisode>::iterator &it);

    void (DinoEventLoop::*mState)();    // 0x74
    u32 *mResult;           // 0x7C
    DinoField *mField;      // 0x80
    std::map<u32, DinoEventEpisode> mEpisodeFiles;  // 0x84
    u32 mUnk90;             // 0x90
    MArchivePtr mArchive;   // 0x94
    MArchiveFile mFile;     // 0x98
    u32 mUnkA4[6];          // 0xA4
    u8 mCameraUsed;         // 0xBC
    u8 unkBD[0x57];
    void *mEpisodes;        // 0x114
    u8 mEpisodesChanged;    // 0x118
};

#endif
