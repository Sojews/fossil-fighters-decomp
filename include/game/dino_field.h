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
#include "game/dino_ui_island_map.h"
#include "m/archive.h"
#include "m/resource.h"
#include "m/serializer.h"
#include "m/3d_resource.h"
#include "m/picture.h"
#include "game/dino_resource.h"
#include "game/dino_field_town_map.h"
#include "game/dino_backup.h"
#include "game/dino_field_player_control.h"
struct DinoCharaTask;
class MScreen;
class DinoField;
class DinoFieldCameraHook;
class MTextPicture;
class DinoGradationBg;
class MSmartOam;
class MDisplay;

// The panel that shows the area's name.
class DinoFieldNamePanel : public MProcess {
public:
    // Not decompiled yet.
    DinoFieldNamePanel();
    virtual ~DinoFieldNamePanel();
    virtual void Idle();
    void SetMode(int mode, int arg);

    u8 unk74[0x88 - 0x74];
};

class DinoFieldCamera : public MProcess, public MSerializer {
public:
    // With a name panel unless namePanel is 0.
    DinoFieldCamera(DinoField *field, u32 namePanel);
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
    void SetUnk15C(u8 value);
    MFx32 GetUnk148();
    void SetUnk94(u32 a, u32 b, u32 c);
    void SetUnkD4(u16 a, u16 b, u16 c, MFx32 d);
    // Keeps where a rotation in progress is going (for Write), or clears it (all 0).
    void SaveRotation(u16 fromX, u16 fromY, u16 fromZ, MFx32 fromScale, u16 toX, u16 toY, u16 toZ, MFx32 toScale,
                      u32 duration, int ease, u32 frame);
    void AddHook(DinoFieldCameraHook *hook);
    void NotifyHooks0();
    void NotifyHooks1();
    // Not decompiled yet: Idle runs them after following the focus (the second after a map change, else the third).
    void IdleUnk0();
    void IdleUnk1();
    void IdleUnk2();

    DinoField *mField;              // 0x8C
    u32 mUnk90;                     // 0x90
    u32 mUnk94;                     // 0x94
    u32 mUnk98;                     // 0x98
    u32 mUnk9C;                     // 0x9C
    VecFx32 mUnkA0;                 // 0xA0
    VecFx32 mUnkAC;                 // 0xAC
    VecFx32 mUnkB8;                 // 0xB8
    u32 mUnkC4;                     // 0xC4
    u32 mUnkC8;                     // 0xC8
    u32 mUnkCC;                     // 0xCC
    u32 mUnkD0;                     // 0xD0
    u16 mUnkD4;                     // 0xD4
    u16 mUnkD6;                     // 0xD6
    u16 mUnkD8;                     // 0xD8
    u8 unkDA[0xDC - 0xDA];
    MFx32 mUnkDC;                   // 0xDC
    u32 mUnkE0;                     // 0xE0
    u32 mFocus;                     // 0xE4: the character followed
    MTaskSet *mUnkE8;               // 0xE8: the rotation anime
    s32 mUnkEC;                     // 0xEC
    u32 mUnkF0[2];                  // 0xF0
    u8 mTracking;                   // 0xF8
    u8 unkF9[0xFC - 0xF9];
    u32 mUnkFC;                     // 0xFC: cleared on a map change
    // The rotation and scale the map's camera setting 0 means.
    u16 mUnk100;                    // 0x100
    u16 mUnk102;                    // 0x102
    u16 mUnk104;                    // 0x104
    u8 unk106[2];
    MFx32 mUnk108;                  // 0x108
    MFx32Point mFocusPos;           // 0x10C: where the followed character stands
    MFx32Point mUnk114;             // 0x114: mFocusPos when the setting was last chosen
    // What Write saved of a rotation in progress (Read restarts it).
    u16 mSavedD4;                   // 0x11C
    u16 mSavedD6;                   // 0x11E
    u16 mSavedD8;                   // 0x120
    u8 unk122[2];
    u32 mSavedDC;                   // 0x124
    u16 mSavedRotate[3];            // 0x128
    u8 unk12E[2];
    u32 mSavedRotateArgs[3];        // 0x130
    s32 mSavedSteps;                // 0x13C
    u32 mUnk140;                    // 0x140
    DinoFieldNamePanel *mNamePanel; // 0x144
    MFx32 mUnk148;                  // 0x148
    u8 mHeightSet;                  // 0x14C
    std::vector<DinoFieldCameraHook *> mHooks;  // 0x150: told when the field changes maps
    u8 mHooksMuted;                 // 0x15C
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

class M3DScene;
class MAnmModelBase;
class MPolygonChar;

class DinoFieldChara : public MProcess, public MSerializer {
public:
    virtual ~DinoFieldChara();
    virtual void Read(const void *buf);

    DinoFieldCharaModel *GetModel();
    void ShowManpu(u32 manpu, MPoint offset);
    void HideManpu();
    const char *GetMotionModelPath(u32 motion);
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
    const char *GetHeadModelPath();
    void ChangeMotion(u32 motion, u8 loop, MFx32 speed);
    void SetAngle(u32 angle, u32 frames, u32 ease);
    void SetDirection(u16 angle);
    BOOL IsMoving();
    BOOL IsUnkA1Motion();
    MFx32 GetHeight();
    DinoCharaTask *GetTask() { return &mTask; }
    void ResetState();
    // Not decompiled yet: clears the tasks and motion state and redraws the character.
    void Refresh();
    MFx32 GetMoveSpeed();
    u8 GetUnk2C();
    MFx32Rect GetRect30();
    u8 GetUnk2D();
    MRect GetRect40();
    MFx32 GetUnk50();
    void SetUnk2D8(MPoint pos, u32 a, u32 b);
    void ClearUnk2E0();
    void StopWalking();
    void LeaveUnk258();
    void PlayReaction();
    BOOL IsUnk24Reacting();
    void SetUnk2D8Pos(MPoint pos) { mUnk2D8 = pos; }
    BOOL IsUnk20Two();
    // Not decompiled yet: what a character with no task does (unless the field's unk258 is set).
    void Unk02162154();
    // The idle step of each task kind (DinoCharaTask::unk4).
    void IdleNoTask();
    void MoveTo(MFx32Point coord, u32 frames, u32 ease, u8 silent);
    MTask *CreateWaitMoveTask();
    MTask *CreateWaitStopTask();
    MTask *CreateAngleToTask(u32 angle, u32 frames, u32 ease);
    MTask *CreateMoveToTask(MFx32Point coord, u32 frames, u32 ease, u8 silent);
    MTask *CreateShowManpuTask(u32 manpu, MPoint offset);
    MTask *CreateHideManpuTask();
    MTask *CreateSetMotionTask(u32 motion, u8 loop, MFx32 speed);
    MTask *CreateChangeMotionTask(u32 motion, u8 loop, MFx32 speed);

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
    M3DScene *mScene;           // 0x11C
    MAnmModelBase *mBody;       // 0x120
    MAnmModelBase *mHead;       // 0x124: on the body's head joint
    u32 mUnk128;                // 0x128
    MPolygonChar *mEffect;      // 0x12C
    u32 mUnk130[9];             // 0x130
    std::map<u32, M3DModelResource> mMotionModels;      // 0x154: by motion, cached ahead
    std::map<u32, M3DModelResource> mMotionModelsAlt;   // 0x164: (when the field's unk258 is set)
    M3DModelResource mModelResource;                    // 0x174
    char mMotionModelPath[256]; // 0x1D8
    MPoint mUnk2D8;             // 0x2D8
    u32 mUnk2E0;                // 0x2E0
    u32 mUnk2E4;                // 0x2E4
    MResource<DinoHeadMaskDefs> mHeadMasks; // 0x2E8: loaded for the player's head mask
    MTask *mManpu;              // 0x2F8: the emotion bubble shown over the character
    u32 mUnk2FC[2];             // 0x2FC
    BOOL mHasFootSe;            // 0x304
    u32 mFootSe;                // 0x308
    u32 mBeholdTarget;          // 0x30C: the character looked at
    u32 mUnk310;                // 0x310
    u32 mUnk314;                // 0x314
    u32 mMoveSe;                // 0x318: the footstep sound when walking
    u32 mMoveSe2;               // 0x31C
};

// Where the chara is on the screen.
BOOL DinoFieldCharaGetScreenPos(MPoint *pos, DinoFieldChara *chara);

struct MAfnBlock;

// An 8-byte entry of the radar's list (only its size is known).
struct DinoFieldRadarEntry {
    u32 unk0;
    u32 unk4;
};

class DinoFieldRadarManager;
class DinoFieldTownMapManager;

class DinoFieldRadar : public MProcess {
public:
    DinoFieldRadar(DinoField *field, u32 map);
    virtual ~DinoFieldRadar();
    virtual void Idle();
    // Not decompiled yet: switches the radar to the mode.
    MTask *CreateModeTask(u32 mode, u32 frames, u8 b);
    void SetPlayer(MFx32Point pos, u16 angle, MFx32 height);
    void SetUnk5A8(u8 value);
    // Beeps and plays the marker's signal motion (unless in mode 1 or already signalling).
    void Signal();
    // Where the container gauge's top is: lower the fuller the containers are.
    MPoint GetContainerGaugePos();

    u32 mUnk74[3];              // 0x74
    MSmartOam *mOam;            // 0x80
    u32 mUnk84[2];              // 0x84
    MMotion *mMarker;           // 0x8C
    u32 mUnk90[4];              // 0x90
    MAfnBlock *mAffine;         // 0xA0
    u8 unkA4[0x568 - 0xA4];
    u32 mMode;                  // 0x568
    MFx32 mScale;               // 0x56C: map units to radar pixels
    u8 unk570[0x584 - 0x570];
    u32 mSignalTime;            // 0x584
    u8 mSignalling;             // 0x588
    std::vector<DinoFieldRadarEntry> mEntries;  // 0x58C
    MFx32Point mPlayerPos;      // 0x598
    u16 mPlayerAngle;           // 0x5A0
    MFx32 mPlayerHeight;        // 0x5A4
    u8 mUnk5A8;                 // 0x5A8
};


struct DinoFieldMapInfo {
    u32 id;
};

// The map file's header.
struct DinoFieldMapData {
    u8 unk00[0x30];
    MFx32 mWalkSpeed;   // 0x30: the speed characters' walk motion plays at
    u8 unk34[0x3C - 0x34];
    u32 mAreas;         // 0x3C: offset of the areas (0x14 bytes each)
    u32 mCameraCount;   // 0x40: of the camera settings
    u32 mCameras;       // 0x44: offset of the camera settings (DinoFieldMapCamera)
    u32 unk48;
    u32 mEntries4C;     // 0x4C: offset of a table of offsets
};

// A camera setting of the map: a rotation and a scale.
struct DinoFieldMapCamera {
    u16 x;
    u16 y;
    u16 z;
    u8 unk06[2];
    MFx32 scale;    // 0x08
};

// The map's area grid: an area index per 2x2 cell.
struct DinoFieldMapGrid {
    u32 unk00;
    u32 width;          // 0x04: in cells
    u32 height;         // 0x08
    u32 unk0C;
    u32 cells;          // 0x10: offset of width * height u8 area indexes

    const u8 *GetCells() const { return (const u8 *) this + cells; }
};

// The field's settings file.
struct DinoFieldDefs {
    u8 unk00[0x9C];
    u32 mRockMap;       // 0x9C: the map the kaseki rocks are on

    u32 GetRockMap() const { return mRockMap; }
};

class DinoFieldMap : public MProcess, public MSerializer {
public:
    const u8 *GetMapData() { return (const u8 *)mData.mFile.GetData(); }
    DinoFieldMap(void *unkC4);
    virtual ~DinoFieldMap();
    virtual void Idle();
    virtual void Read(const void *buf);

    DinoFieldMapInfo *GetInfo();
    fx32 GetGroundHeight(fx32 x, fx32 y);
    BOOL IsWalkable(fx32 x, fx32 y, fx32 *height);
    void Load();
    MFx32 GetWalkSpeed();
    void ClearGradation();
    DinoFieldMapCamera *GetCamera(int index);
    // The area at the position (the first one off the grid).
    void *GetArea(MFx32Point pos);
    void *GetEntry4C(int index);
    // The camera setting at the position (the nearest cell's, off the grid; 0 if the map has just one).
    int GetCameraIndex(MFx32Point pos);

    DinoFieldMapInfo mInfo; // 0x8C
    u32 mUnk90;             // 0x90
    MResource<u8> mData;    // 0x94
    MResource<DinoFieldMapGrid> mGrid;  // 0xA4
    MResource<DinoFieldMapGrid> mCameraGrid;    // 0xB4: a camera setting index per 2x2 cell
    void *mUnkC4;           // 0xC4: with a gradation behind the map
    MAnmModelBase *mModel;  // 0xC8: the map's model
    DinoGradationBg *mGradation;    // 0xCC
    u32 mUnkD0;             // 0xD0
};

class DinoFieldPoint;

class DinoFieldKasekiRock;

// One of the field's walk animation states.
struct DinoFieldStep {
    MPolygonChar *effects[6];   // (the last step's run on past it, to 8)
};

class DinoFieldHole;
class MMotion;

class DinoField : public MProcess, public MSerializer, public DinoHelpBase {
public:
    DinoField(MScreen **screen, u32 arg1);
    virtual ~DinoField();
    virtual void Idle();
    void CreateEffects(MPolygonChar **effects, u32 count, u32 id);
    fx32 GetGroundHeight(fx32 x, fx32 y);
    // The highest of the ground heights around pos (a tile each way), a little above it.
    fx32 GetMaxGroundHeight(MFx32Point pos);
    DinoFieldHole *CreateHole(MFx32Point pos);
    void CreateRockHole(MFx32Point pos, u32 arg);
    // The open rock hole within 1.5 of pos, or NULL.
    DinoFieldHole *FindRockHole(MFx32Point pos);
    BOOL IsWalkable(fx32 x, fx32 y, fx32 *height);
    BOOL IsWalkable(MFx32Point pos, fx32 *height);
    // Where the character ends up moving by move: pushed out of the other characters (and, on the map in the
    // field defs, the kaseki rocks), then stopped or slid along walls.
    MFx32Point ResolveMove(DinoFieldChara *chara, MFx32Point move);
    virtual void Write(void *buf);
    virtual void Read(const void *buf);
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
    MTask *CreateCharaMoveVectorTask(u32 chara, u16 direction, MFx32 a, u32 b, u32 c, u8 d);
    MTask *CreateCharaWaitMoveTask(u32 chara);
    MTask *CreateCharaFocusTask(u32 chara);
    void Step(u32 dir, DinoFieldStep *step, int a, int frames, int c);
    // Steps the player one way when walking, another when running.
    // Steps the player one way when walking, another when running.
    void Walk(u32 dir, u32 run, DinoFieldStep *step, int a, int frames, int runA, int runFrames, int c) {
        if (!run) {
            Step(dir, step, a, frames, c);
        } else {
            if (!(mUnk37C & 1)) {
                dir = run;
            }
            Step(dir, step, runA, runFrames, c);
        }
    }
    void WalkOnLand0(u32 dir);
    void CreateKasekiRock(u32 index);
    // Makes the kaseki rocks the backup has, if this is their map.
    void CreateKasekiRocks();
    // Clears away the last map's things and loads the map, with its step effects.
    void LoadMap(u32 id);
    // Reloads the map the field is on: the charas, the fellows' places (with whom they follow), the kaseki rocks.
    void ReloadMap();
    void HideUnk2B4();
    void WalkOnLand1(u32 dir);
    void WalkInWater(u32 dir, u32 run);
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
    void FadeInPict(MDisplay *display, u32 *id, MExtAffinePicture **pict, u32 newId, MPoint pos, u32 frames, long unk);
    void FadeOutPict(MDisplay *display, MExtAffinePicture **pict, u32 id, long frames);
    void SlideInPict(MDisplay *display, u32 *id, MExtAffinePicture **pict, u32 newId, MPoint pos, u32 a, u32 b, long c);
    void SlideOutPict(MDisplay *display, MExtAffinePicture **pict, u32 id, u32 a, long b);
    void ZoomInPict(MDisplay *display, u32 *id, MExtAffinePicture **pict, u32 newId, MPoint pos, MFx32 scale, u32 a, long b);
    void ZoomOutPict(MDisplay *display, MExtAffinePicture **pict, MFx32 scale, u32 a, long b);

    u32 mUnk98;                 // 0x98
    DinoFieldMap *mMap;         // 0x9C
    DinoFieldChara *mCharas[100]; // 0xA0
    u32 mUnk230;                // 0x230
    u32 mUnk234;                // 0x234
    u32 mUnk238;                // 0x238: for characters with the model flag, the one GetChara returns
    M3DScene *mScene;           // 0x23C
    DinoFieldCamera *mCamera;   // 0x240
    DinoFieldRadarManager *mRadar;      // 0x244
    DinoFieldTownMapManager *mTownMap;  // 0x248
    u32 mUnk24C[1];             // 0x24C
    MTaskSet *mHoles;           // 0x250
    MTaskSet *mRockHoles;       // 0x254
    u8 mUnk258;                 // 0x258: the player can't move (an event is running)
    u8 unk259[3];
    u32 mUnk25C;                // 0x25C
    u32 mUnk260;                // 0x260
    u8 mUnk264;                 // 0x264
    u8 unk265[3];
    MTaskSet *mFadeTasks;       // 0x268
    u32 mUnk26C;                // 0x26C
    MTaskSet *mPictTasks;       // 0x270
    MExtAffinePicture *mLowerPict;   // 0x274: the event picture on each screen
    MExtAffinePicture *mUpperPict;   // 0x278
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
    MMotion *mUnk2B4;           // 0x2B4
    MTask *mUnk2B8;             // 0x2B8
    MMotion *mUnk2BC;           // 0x2BC
    MTask *mUnk2C0;             // 0x2C0
    MTask *mUnk2C4;             // 0x2C4
    BOOL *mUnk2C8;              // 0x2C8
    DinoFieldKasekiRock *mKasekiRocks[20]; // 0x2CC
    MTaskSet *mKasekiRockTasks; // 0x31C
    DinoFieldStep mSteps[3];    // 0x320: the walk animations
    u8 unk368[0x370 - 0x368];
    MTaskSet *mUnk370;          // 0x370
    MTaskSet *mEffectTasks;     // 0x374
    u32 mUnk378;                // 0x378
    u32 mUnk37C;                // 0x37C: bit 0: running keeps the direction
    MResource<DinoFieldDefs> mDefs; // 0x380
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

    void Start();

    void (DinoFieldDance::*mState)(); // 0x74
    DinoField *mField;                // 0x7C
    u32 mUnk80;                       // 0x80
    MResource<DinoDanceData> mDance;  // 0x84
    u32 mUnk94;                       // 0x94
    u32 mUnk98;                       // 0x98
    u32 mUnk9C;                       // 0x9C
    u32 unkA0;
    std::vector<MTask *> mUnkA4;      // 0xA4
    u32 unkB0[14];
    MResource<DinoDanceData> mUnkE8;  // 0xE8
    u32 unkF8[2];
    MPictureFile mPicture;            // 0x100
};


// A loaded episode file and the archive holding it.
struct DinoEventEpisode {
    DinoEventEpisode() {}
    DinoEventEpisode(FSFileID id) : mArchive(id) {
        mFile = mArchive.GetFile(0);
        mFile.Lock();
        mFile.GetData();
    }

    const void *Get() { return mFile.GetData(); }

    MArchivePtr mArchive;   // 0x00
    MArchiveFile mFile;     // 0x04
};

// What the player's control on the field asks the event loop for (a menu, an event, a warp...).
struct DinoEventLoopRequest {
    u32 type;       // 0x00: 0 for nothing
    u32 arg;        // 0x04
    MPoint pos;     // 0x08
};

// Runs the event scripts (the game's class name; the event commands get it as their event).
class DinoEventLoop : public MProcess {
public:
    DinoEventLoop(u32 *result);
    virtual ~DinoEventLoop();
    virtual void Idle();
    MTask *CreateTalkTask(u32 id);
    void Talk(u32 id);
    std::string GetTalkText(u32 id);
    void SetResult(u32 result);
    void CloseField(bool arg);
    void OpenField(bool arg);
    void CreateField();
    void MarkCameraUsed();
    DinoField *GetField();
    void Fade(u32 a, u32 b);
    MTask *CreateFadeWaitTask();
    void SetEpisodesChanged();
    void AddEpisode(u32 id);
    void AddSeenEpisodes();
    void RemoveEpisode(u32 id);
    void ResetEpisode(u32 id);
    void ClearEpisodes();
    // Makes the field (reading the saved state, if any) and fades it in.
    void Start();
    // Runs what the player's control asked for, or else the episodes that are ready, then hands control back.
    void StateRun();
    // After an event is cleared: notes the regions the events still open name in their conditions.
    void UpdateRegions();
    BOOL ProcessEpisode(const std::map<u32, DinoEventEpisode>::iterator &it);
    // TRUE if an entry's conditions hold (all of them, or any, by its mode).
    BOOL CheckConditions(const u8 *entry);
    BOOL CheckCondition(const u8 *condition);
    // Runs a block of commands, then lets the player move again.
    void RunCommands(const u8 *block);
    void RunCommand(const u8 *command);
    // When the commands are done: lets go of what they loaded, lets the player move and the camera follow again.
    void FinishCommands();
    // Lets go of the font and the picture, if loaded.
    void ReleasePicture();

    void (DinoEventLoop::*mState)();    // 0x74
    u32 *mResult;           // 0x7C
    DinoField *mField;      // 0x80
    std::map<u32, DinoEventEpisode> mEpisodeFiles;  // 0x84
    MResource<DinoEpisodeData> mCommands;  // 0x94: the commands of the episode being run
    MResource<DinoMessageData> mMessages;   // 0xA4: the talk messages last looked in
    DinoEventLoopRequest *mRequest;           // 0xB4
    DinoFieldPlayerControlResult *mControlResult; // 0xB8
    u8 mCameraUsed;         // 0xBC
    u8 mUnkBD;              // 0xBD
    u8 mUnkBE;              // 0xBE
    u8 unkBF;
    MBitVector mRegions;    // 0xC0: the regions the open events' conditions name, and the linked ones
    u8 mEventCleared;       // 0xCC: an event was just cleared
    u8 mHasPicture;         // 0xCD: the font and the picture below are loaded
    MFont *mFont;           // 0xD0
    MPictureFile mPicture;  // 0xD4
    u32 mUnk108;            // 0x108
    u32 mUnk10C;            // 0x10C
    MScreen *mScreen;       // 0x110: the field's screen
    FSFileID *mEpisodes;    // 0x114: each episode's file, once looked up
    u8 mEpisodesChanged;    // 0x118
};

#endif
