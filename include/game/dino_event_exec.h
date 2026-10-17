// The event script commands: each runs as a task in the event process, one per command.
#ifndef GAME_DINO_EVENT_EXEC_H
#define GAME_DINO_EVENT_EXEC_H

#include "m/task.h"
#include "m/input.h"
#include "game/dino_backup.h"
#include "game/dino_field.h"

class DinoEventExecVarValue : public MTransfer {
public:
    DinoEventExecVarValue(u32 op, DinoEventVar var, s32 value);
    virtual ~DinoEventExecVarValue();
    virtual void Transfer();

    u32 mOp;            // 0x10: set, add, subtract, multiply, divide
    DinoEventVar mVar;  // 0x14
    s32 mValue;         // 0x1C
};

class DinoEventExecVarVar : public MTransfer {
public:
    DinoEventExecVarVar(u32 op, DinoEventVar var, DinoEventVar source);
    virtual ~DinoEventExecVarVar();
    virtual void Transfer();

    u32 mOp;                // 0x10
    DinoEventVar mVar;      // 0x14
    DinoEventVar mSource;   // 0x1C
};

class DinoEventExecMapConnect : public MTask {
public:
    DinoEventExecMapConnect(u32 cmd, u32 from, u32 to);
    virtual ~DinoEventExecMapConnect();
    virtual void Execute();

    u32 mCmd;   // 0x10
    u32 mFrom;  // 0x14
    u32 mTo;    // 0x18
};

class DinoEventExecMapConnectDirectional : public MTask {
public:
    DinoEventExecMapConnectDirectional(u32 cmd, u32 from, u32 to);
    virtual ~DinoEventExecMapConnectDirectional();
    virtual void Execute();

    u32 mCmd;   // 0x10
    u32 mFrom;  // 0x14
    u32 mTo;    // 0x18
};

class DinoEventExecMapDisconnect : public MTask {
public:
    DinoEventExecMapDisconnect(u32 cmd, u32 from, u32 to);
    virtual ~DinoEventExecMapDisconnect();
    virtual void Execute();

    u32 mCmd;   // 0x10
    u32 mFrom;  // 0x14
    u32 mTo;    // 0x18
};

class DinoEventExecMapDisconnectDirectional : public MTask {
public:
    DinoEventExecMapDisconnectDirectional(u32 cmd, u32 from, u32 to);
    virtual ~DinoEventExecMapDisconnectDirectional();
    virtual void Execute();

    u32 mCmd;   // 0x10
    u32 mFrom;  // 0x14
    u32 mTo;    // 0x18
};

class DinoEventExecFlagSet : public MTask {
public:
    DinoEventExecFlagSet(u32 cmd, DinoEventVar var, u8 value);
    virtual ~DinoEventExecFlagSet();
    virtual void Execute();

    u32 mCmd;           // 0x10
    DinoEventVar mVar;  // 0x14
    u8 mValue;          // 0x1C
};

class DinoEventExecEventClear : public MTask {
public:
    DinoEventExecEventClear(u32 cmd, u32 id, u8 value);
    virtual ~DinoEventExecEventClear();
    virtual void Execute();

    u32 mCmd;   // 0x10
    u32 mId;    // 0x14
    u8 mValue;  // 0x18
};

class DinoEventExecSetEpisodeClear : public MTask {
public:
    DinoEventExecSetEpisodeClear(u32 cmd, u32 id, u8 value);
    virtual ~DinoEventExecSetEpisodeClear();
    virtual void Execute();

    u32 mCmd;   // 0x10
    u32 mId;    // 0x14
    u8 mValue;  // 0x18
};

class DinoEventExecAppendCharaRegion : public MTask {
public:
    virtual ~DinoEventExecAppendCharaRegion();
};

class DinoEventExecAppendCharaChara : public MProcess {
public:
    virtual ~DinoEventExecAppendCharaChara();
};

class DinoEventExecRemoveChara : public MTask {
public:
    DinoEventExecRemoveChara(DinoEvent *event, u32 chara);
    virtual ~DinoEventExecRemoveChara();
    virtual void Execute();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecCharaAngle : public MProcess {
public:
    DinoEventExecCharaAngle(DinoEvent *event, u32 chara, u32 angle, u32 a, u32 b);
    virtual ~DinoEventExecCharaAngle();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u32 mAngle;         // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
};

class DinoEventExecCharaAngleToChara : public MProcess {
public:
    virtual ~DinoEventExecCharaAngleToChara();
};

class DinoEventExecCharaAngleOffset : public MProcess {
public:
    virtual ~DinoEventExecCharaAngleOffset();
};

class DinoEventExecCharaAngleOffsetToChara : public MProcess {
public:
    virtual ~DinoEventExecCharaAngleOffsetToChara();
};

class DinoEventExecCharaMoveToChara : public MProcess {
public:
    virtual ~DinoEventExecCharaMoveToChara();
};

class DinoEventExecCharaMoveToRegion : public MProcess {
public:
    DinoEventExecCharaMoveToRegion(DinoEvent *event, u32 chara, u32 region, u32 a, u32 b, u8 c);
    virtual ~DinoEventExecCharaMoveToRegion();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u32 mRegion;        // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
    u8 mC;              // 0x88
};

class DinoEventExecCharaMoveToCoord : public MProcess {
public:
    DinoEventExecCharaMoveToCoord(DinoEvent *event, u32 chara, MPoint coord, u32 a, u32 b, u8 c);
    virtual ~DinoEventExecCharaMoveToCoord();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mChara;         // 0x78
    MPoint mPoint;      // 0x7C
    u32 mA;             // 0x84
    u32 mB;             // 0x88
    u8 mC;              // 0x8C
};

class DinoEventExecCharaMoveOffset : public MProcess {
public:
    DinoEventExecCharaMoveOffset(DinoEvent *event, u32 chara, MPoint offset, u32 a, u32 b, u8 c);
    virtual ~DinoEventExecCharaMoveOffset();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mChara;         // 0x78
    MPoint mPoint;      // 0x7C
    u32 mA;             // 0x84
    u32 mB;             // 0x88
    u8 mC;              // 0x8C
};

class DinoEventExecCharaMoveVector : public MProcess {
public:
    DinoEventExecCharaMoveVector(DinoEvent *event, u32 chara, u16 direction, u32 a, u32 b, u32 c, u8 d);
    virtual ~DinoEventExecCharaMoveVector();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u16 mDirection;     // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
    u32 mC;             // 0x88
    u8 mD;              // 0x8C
};

class DinoEventExecCharaWaitMove : public MProcess {
public:
    DinoEventExecCharaWaitMove(DinoEvent *event, u32 chara);
    virtual ~DinoEventExecCharaWaitMove();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mChara;         // 0x78
};

class DinoEventExecCharaFocus : public MProcess {
public:
    DinoEventExecCharaFocus(DinoEvent *event, u32 chara);
    virtual ~DinoEventExecCharaFocus();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mChara;         // 0x78
};

class DinoEventExecCleaning : public MProcess {
public:
    virtual ~DinoEventExecCleaning();
};

class DinoEventExecAutoWarp : public MProcess {
public:
    virtual ~DinoEventExecAutoWarp();
};

class DinoEventExecCamera : public MTransfer {
public:
    DinoEventExecCamera(DinoEvent *event, u32 x, u32 y, u32 z, u32 a, u32 b, u32 c);
    virtual ~DinoEventExecCamera();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mX;             // 0x14
    u32 mY;             // 0x18
    u32 mZ;             // 0x1C
    u32 mA;             // 0x20
    u32 mB;             // 0x24
    u32 mC;             // 0x28
};

class DinoEventExecProhibitCameraRelease : public MTransfer {
public:
    DinoEventExecProhibitCameraRelease(u8 *flag);
    virtual ~DinoEventExecProhibitCameraRelease();
    virtual void Transfer();

    u8 *mFlag;  // 0x10
};

class DinoEventExecFinish : public MTransfer {
public:
    DinoEventExecFinish(void *event);
    virtual ~DinoEventExecFinish();
    virtual void Transfer();

    void *mEvent;   // 0x10
};

class DinoEventExecStartFellow : public MTransfer {
public:
    DinoEventExecStartFellow(DinoEvent *event, u32 chara, u32 target);
    virtual ~DinoEventExecStartFellow();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mTarget;        // 0x18
};

class DinoEventExecFinishFellow : public MTransfer {
public:
    DinoEventExecFinishFellow(DinoEvent *event, u32 chara);
    virtual ~DinoEventExecFinishFellow();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecStartWander : public MTransfer {
public:
    DinoEventExecStartWander(DinoEvent *event, u32 chara, u32 a, u32 b, u32 c, u32 d);
    virtual ~DinoEventExecStartWander();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mArgs[4];       // 0x18
};

class DinoEventExecStartWatch : public MTransfer {
public:
    DinoEventExecStartWatch(DinoEvent *event, u32 chara, DinoCharaWatch watch);
    virtual ~DinoEventExecStartWatch();
    virtual void Transfer();

    DinoEvent *mEvent;      // 0x10
    u32 mChara;             // 0x14
    DinoCharaWatch mWatch;  // 0x18
};

class DinoEventExecFinishWatch : public MTransfer {
public:
    DinoEventExecFinishWatch(DinoEvent *event, u32 chara);
    virtual ~DinoEventExecFinishWatch();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecStartBehold : public MTransfer {
public:
    DinoEventExecStartBehold(DinoEvent *event, u32 chara, u32 target);
    virtual ~DinoEventExecStartBehold();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mTarget;        // 0x18
};

class DinoEventExecFinishBehold : public MTransfer {
public:
    DinoEventExecFinishBehold(DinoEvent *event, u32 chara);
    virtual ~DinoEventExecFinishBehold();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecChoice : public MProcess {
public:
    virtual ~DinoEventExecChoice();
};

class DinoEventExecChoice2 : public MProcess {
public:
    virtual ~DinoEventExecChoice2();
};

class DinoEventExecYesNo : public MProcess {
public:
    virtual ~DinoEventExecYesNo();
};

class DinoEventExecShopChoice : public MProcess {
public:
    virtual ~DinoEventExecShopChoice();
};

class DinoEventExecShopChoice2 : public MProcess {
public:
    virtual ~DinoEventExecShopChoice2();
};

class DinoEventExecFade : public MProcess {
public:
    DinoEventExecFade(DinoEvent *event, u32 a, u32 b, u8 wait);
    virtual ~DinoEventExecFade();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mA;             // 0x78
    u32 mB;             // 0x7C
    u8 mWait;           // 0x80
};

class DinoEventExecLicenseGet : public MTransfer {
public:
    DinoEventExecLicenseGet(u32 license);
    virtual ~DinoEventExecLicenseGet();
    virtual void Transfer();

    u32 mValue; // 0x10
};

class DinoEventExecHoriderLevel : public MTransfer {
public:
    DinoEventExecHoriderLevel(u32 level);
    virtual ~DinoEventExecHoriderLevel();
    virtual void Transfer();

    u32 mValue; // 0x10
};

class DinoEventExecContainerNum : public MTransfer {
public:
    DinoEventExecContainerNum(u32 count);
    virtual ~DinoEventExecContainerNum();
    virtual void Transfer();

    u32 mValue; // 0x10
};

class DinoEventExecSystemFlag : public MTransfer {
public:
    DinoEventExecSystemFlag(u32 id, u8 value);
    virtual ~DinoEventExecSystemFlag();
    virtual void Transfer();

    u32 mId;    // 0x10
    u8 mValue;  // 0x14
};

class DinoEventExecReviverSet : public MTransfer {
public:
    virtual ~DinoEventExecReviverSet();
};

class DinoEventExecMedalChange : public MTransfer {
public:
    DinoEventExecMedalChange(u32 from, u32 to);
    virtual ~DinoEventExecMedalChange();
    virtual void Transfer();

    u32 mFrom;  // 0x10: the creature in the team to swap out
    u32 mTo;    // 0x14
};

class DinoEventExecBattlePointSet : public MTransfer {
public:
    DinoEventExecBattlePointSet(u32 creature, u32 points);
    virtual ~DinoEventExecBattlePointSet();
    virtual void Transfer();

    u32 mCreature;  // 0x10
    u32 mPoints;    // 0x14
};

class DinoEventExecShopSet : public MTransfer {
public:
    virtual ~DinoEventExecShopSet();
};

// Shows a kaseki rock being got.
class DinoEventKasekiRockGet : public MProcess {
public:
    DinoEventKasekiRockGet(DinoEvent *event, u32 mode, const DinoBackupEntry *entry, u8 *result);
    virtual void Idle();

    u8 unk74[0x58];
};

class DinoEventExecKasekiRockGet : public MProcess {
public:
    DinoEventExecKasekiRockGet(DinoEvent *event, u32 flag, u32 id, u32 a, u32 b, u8 c);
    virtual ~DinoEventExecKasekiRockGet();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mFlag;          // 0x78: if set, gets the result
    u32 mId;            // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
    u32 mStep;          // 0x88
    u8 mResult;         // 0x8C
    u8 mC;              // 0x8D
};

class DinoEventExecKasekiRockShot : public MProcess {
public:
    DinoEventExecKasekiRockShot(DinoEvent *event, u32 id, u32 a, u32 b);
    virtual ~DinoEventExecKasekiRockShot();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mId;            // 0x78
    u32 mA;             // 0x7C
    u32 mB;             // 0x80
    u32 mStep;          // 0x84
};

class DinoEventExecCharaBodyModelIndex : public MTransfer {
public:
    DinoEventExecCharaBodyModelIndex(DinoEvent *event, u32 chara, u32 model);
    virtual ~DinoEventExecCharaBodyModelIndex();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mModel;         // 0x18
};

class DinoEventExecCharaHeadModelIndex : public MTransfer {
public:
    DinoEventExecCharaHeadModelIndex(DinoEvent *event, u32 chara, u32 model);
    virtual ~DinoEventExecCharaHeadModelIndex();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mModel;         // 0x18
};

class DinoEventExecCharaMotionSet : public MTransfer {
public:
    DinoEventExecCharaMotionSet(DinoEvent *event, u32 chara, u32 motion, u8 loop);
    virtual ~DinoEventExecCharaMotionSet();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mMotion;        // 0x18
    u8 mLoop;           // 0x1C
};

class DinoEventExecCharaMotionCache : public MTransfer {
public:
    DinoEventExecCharaMotionCache(DinoEvent *event, u32 chara, u32 motion);
    virtual ~DinoEventExecCharaMotionCache();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mMotion;        // 0x18
};

class DinoEventExecCharaMotionCacheClear : public MTransfer {
public:
    DinoEventExecCharaMotionCacheClear(DinoEvent *event, u32 chara);
    virtual ~DinoEventExecCharaMotionCacheClear();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecManpuShow : public MTransfer {
public:
    DinoEventExecManpuShow(DinoEvent *event, u32 chara, u32 manpu, MPoint offset);
    virtual ~DinoEventExecManpuShow();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mManpu;         // 0x18
    MPoint mOffset;     // 0x1C
};

class DinoEventExecManpuHide : public MTransfer {
public:
    DinoEventExecManpuHide(DinoEvent *event, u32 chara);
    virtual ~DinoEventExecManpuHide();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecPictShow : public MProcess {
public:
    virtual ~DinoEventExecPictShow();
};

class DinoEventExecPictHide : public MProcess {
public:
    virtual ~DinoEventExecPictHide();
};

class DinoEventExecCmnMotion : public MTransfer {
public:
    DinoEventExecCmnMotion(DinoEvent *event, u32 chara, u32 motion);
    virtual ~DinoEventExecCmnMotion();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mMotion;        // 0x18
};

class DinoEventExecChrEffect : public MTransfer {
public:
    DinoEventExecChrEffect(DinoEvent *event, u32 chara, u32 effect);
    virtual ~DinoEventExecChrEffect();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mEffect;        // 0x18
};

class DinoEventExecKeyWait : public MProcess {
public:
    DinoEventExecKeyWait();
    virtual ~DinoEventExecKeyWait();
    virtual void Idle();

    MInput *mInput; // 0x74
};

class DinoEventExecBgmPlay : public MTransfer {
public:
    DinoEventExecBgmPlay(DinoEvent *event, u32 bgm, u32 arg, u8 fade);
    virtual ~DinoEventExecBgmPlay();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mBgm;           // 0x14
    u32 mArg;           // 0x18
    u8 mFade;           // 0x1C
};

class DinoEventExecMapBgmPlay : public MProcess {
public:
    virtual ~DinoEventExecMapBgmPlay();
};

class DinoEventExecBgmStop : public MTransfer {
public:
    DinoEventExecBgmStop(DinoEvent *event, u32 fadeFrames);
    virtual ~DinoEventExecBgmStop();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mFadeFrames;    // 0x14
};

class DinoEventExecQuake : public MTransfer {
public:
    DinoEventExecQuake(DinoEvent *event, u32 a, u32 b, u32 c);
    virtual ~DinoEventExecQuake();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mA;             // 0x14
    u32 mB;             // 0x18
    u32 mC;             // 0x1C
};

class DinoEventExecDance : public MProcess {
public:
    DinoEventExecDance(DinoEvent *event, u32 a, u32 b, u32 c, u32 d, u32 e);
    virtual ~DinoEventExecDance();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mA;             // 0x78
    u32 mB;             // 0x7C
    u32 mC;             // 0x80
    u32 mD;             // 0x84
    u32 mE;             // 0x88
};

class DinoEventExecMapSelect : public MProcess {
public:
    DinoEventExecMapSelect(DinoEvent *event, u32 var);
    virtual ~DinoEventExecMapSelect();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mVar;           // 0x78: the local variable that gets the chosen map (0 if cancelled)
    s32 mResult;        // 0x7C
    u32 mStep;          // 0x80
};

class DinoEventExecEraseAllExcrement : public MTransfer {
public:
    DinoEventExecEraseAllExcrement(u32 cmd);
    virtual ~DinoEventExecEraseAllExcrement();
    virtual void Transfer();

    u32 mCmd;   // 0x10
};

class DinoEventExecEraseExcrement : public MTransfer {
public:
    DinoEventExecEraseExcrement(u32 cmd, DinoEventVar count);
    virtual ~DinoEventExecEraseExcrement();
    virtual void Transfer();

    u32 mCmd;               // 0x10
    DinoEventVar mCount;    // 0x14
};

class DinoEventExecGameOver : public MProcess {
public:
    DinoEventExecGameOver(DinoEvent *event);
    virtual ~DinoEventExecGameOver();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mStep;          // 0x78
};

class DinoEventExecExit : public MProcess {
public:
    DinoEventExecExit(DinoEvent *event);
    virtual ~DinoEventExecExit();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mStep;          // 0x78
};

class DinoEventExecTalk : public MProcess {
public:
    virtual ~DinoEventExecTalk();
};

class DinoEventExecFieldName : public MProcess {
public:
    virtual ~DinoEventExecFieldName();
};

class DinoEventExecLowerPictFadeIn : public MProcess {
public:
    virtual ~DinoEventExecLowerPictFadeIn();
};

class DinoEventExecLowerPictFadeOut : public MProcess {
public:
    virtual ~DinoEventExecLowerPictFadeOut();
};

class DinoEventExecLowerPictSlideIn : public MProcess {
public:
    virtual ~DinoEventExecLowerPictSlideIn();
};

class DinoEventExecLowerPictSlideOut : public MProcess {
public:
    virtual ~DinoEventExecLowerPictSlideOut();
};

class DinoEventExecLowerPictZoomIn : public MProcess {
public:
    virtual ~DinoEventExecLowerPictZoomIn();
};

class DinoEventExecLowerPictZoomOut : public MProcess {
public:
    virtual ~DinoEventExecLowerPictZoomOut();
};

class DinoEventExecUpperPictFadeIn : public MProcess {
public:
    virtual ~DinoEventExecUpperPictFadeIn();
};

class DinoEventExecUpperPictFadeOut : public MProcess {
public:
    virtual ~DinoEventExecUpperPictFadeOut();
};

class DinoEventExecUpperPictSlideIn : public MProcess {
public:
    virtual ~DinoEventExecUpperPictSlideIn();
};

class DinoEventExecUpperPictSlideOut : public MProcess {
public:
    virtual ~DinoEventExecUpperPictSlideOut();
};

class DinoEventExecUpperPictZoomIn : public MProcess {
public:
    virtual ~DinoEventExecUpperPictZoomIn();
};

class DinoEventExecUpperPictZoomOut : public MProcess {
public:
    virtual ~DinoEventExecUpperPictZoomOut();
};

class DinoEventExecRadarMode : public MProcess {
public:
    DinoEventExecRadarMode(DinoEvent *event, u32 mode, u32 a, u8 b);
    virtual ~DinoEventExecRadarMode();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mMode;          // 0x78
    u32 mA;             // 0x7C
    u8 mB;              // 0x80
};

class DinoEventExecTownMapMode : public MProcess {
public:
    DinoEventExecTownMapMode(DinoEvent *event, u32 mode, u32 a);
    virtual ~DinoEventExecTownMapMode();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mMode;          // 0x78
    u32 mA;             // 0x7C
};

class DinoEventExecDelAreaNamePanel : public MTask {
public:
    DinoEventExecDelAreaNamePanel(DinoEvent *event);
    virtual ~DinoEventExecDelAreaNamePanel();
    virtual void Execute();

    DinoEvent *mEvent;  // 0x10
};

class DinoEventExecHeightReset : public MTransfer {
public:
    DinoEventExecHeightReset(DinoEvent *event);
    virtual ~DinoEventExecHeightReset();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
};

class DinoEventExecCharaFootSeOff : public MTransfer {
public:
    DinoEventExecCharaFootSeOff(DinoEvent *event, u32 chara);
    virtual ~DinoEventExecCharaFootSeOff();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecCharaFootSePreserve : public MTransfer {
public:
    DinoEventExecCharaFootSePreserve(DinoEvent *event, u32 chara, u32 se);
    virtual ~DinoEventExecCharaFootSePreserve();
    virtual void Transfer();

    DinoEvent *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mSe;            // 0x18
};

class DinoEventExecMoneyShow : public MProcess {
public:
    DinoEventExecMoneyShow(DinoEvent *event);
    virtual ~DinoEventExecMoneyShow();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
};

class DinoEventExecMoneyHide : public MProcess {
public:
    DinoEventExecMoneyHide(DinoEvent *event);
    virtual ~DinoEventExecMoneyHide();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
};

class DinoEventExecDonateShow : public MProcess {
public:
    DinoEventExecDonateShow(DinoEvent *event);
    virtual ~DinoEventExecDonateShow();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
};

class DinoEventExecDonateHide : public MProcess {
public:
    DinoEventExecDonateHide(DinoEvent *event);
    virtual ~DinoEventExecDonateHide();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
};

class DinoEventExecProhibitNisReset : public MTransfer {
public:
    DinoEventExecProhibitNisReset();
    virtual ~DinoEventExecProhibitNisReset();
    virtual void Transfer();
};

#endif
