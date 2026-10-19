// The event script commands: each runs as a task in the event process, one per command.
#ifndef GAME_DINO_EVENT_EXEC_H
#define GAME_DINO_EVENT_EXEC_H

#include <vector>
#include "game/dino_event_capture_fade_ui_task.h"
#include "m/archive.h"
#include "game/dino_event_revive_effect.h"
#include "m/resource.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_common_tutorial_window.h"
#include "m/task.h"
#include "m/input.h"
#include "game/dino_backup.h"
#include "game/dino_field.h"
#include "game/dino_event_talk_window.h"

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

// Puts a chara on a map, at a region's center or a point, facing an angle or another chara.
class DinoEventExecAppendCharaRegion : public MTask {
public:
    DinoEventExecAppendCharaRegion(DinoEventLoop *event, u32 chara, u32 region, u32 angle, u32 target);
    DinoEventExecAppendCharaRegion(DinoEventLoop *event, u32 chara, u32 map, MFx32Point pos, u32 angle, u32 target);
    virtual ~DinoEventExecAppendCharaRegion();
    virtual void Execute();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mMap;           // 0x18
    MFx32Point mPos;    // 0x1C
    u32 mAngle;         // 0x24
    u32 mTarget;        // 0x28: a chara to face, or 0
};

// Puts a chara where another chara is.
class DinoEventExecAppendCharaChara : public MProcess {
public:
    DinoEventExecAppendCharaChara(DinoEventLoop *event, u32 chara, u32 other);
    virtual ~DinoEventExecAppendCharaChara();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u32 mOther;         // 0x7C
};

class DinoEventExecRemoveChara : public MTask {
public:
    DinoEventExecRemoveChara(DinoEventLoop *event, u32 chara);
    virtual ~DinoEventExecRemoveChara();
    virtual void Execute();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecCharaAngle : public MProcess {
public:
    DinoEventExecCharaAngle(DinoEventLoop *event, u32 chara, u32 angle, u32 a, u32 b);
    virtual ~DinoEventExecCharaAngle();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u32 mAngle;         // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
};

class DinoEventExecCharaAngleToChara : public MProcess {
public:
    DinoEventExecCharaAngleToChara(DinoEventLoop *event, u32 chara, u32 target, u32 a, u32 b);
    virtual ~DinoEventExecCharaAngleToChara();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u32 mTarget;        // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
};

class DinoEventExecCharaAngleOffset : public MProcess {
public:
    DinoEventExecCharaAngleOffset(DinoEventLoop *event, u32 chara, u32 offset, u32 a, u32 b);
    virtual ~DinoEventExecCharaAngleOffset();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u32 mOffset;        // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
};

class DinoEventExecCharaAngleOffsetToChara : public MProcess {
public:
    DinoEventExecCharaAngleOffsetToChara(DinoEventLoop *event, u32 chara, u32 target, u16 offset, u32 a, u32 b);
    virtual ~DinoEventExecCharaAngleOffsetToChara();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u32 mTarget;        // 0x7C
    u16 mOffset;        // 0x80
    u32 mA;             // 0x84
    u32 mB;             // 0x88
};

class DinoEventExecCharaMoveToChara : public MProcess {
public:
    DinoEventExecCharaMoveToChara(DinoEventLoop *event, u32 chara, u32 target, u32 a, u32 b, u8 c);
    virtual ~DinoEventExecCharaMoveToChara();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u32 mTarget;        // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
    u8 mC;              // 0x88
};

class DinoEventExecCharaMoveToRegion : public MProcess {
public:
    DinoEventExecCharaMoveToRegion(DinoEventLoop *event, u32 chara, u32 region, u32 a, u32 b, u8 c);
    virtual ~DinoEventExecCharaMoveToRegion();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u32 mRegion;        // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
    u8 mC;              // 0x88
};

class DinoEventExecCharaMoveToCoord : public MProcess {
public:
    DinoEventExecCharaMoveToCoord(DinoEventLoop *event, u32 chara, MFx32Point coord, u32 a, u32 b, u8 c);
    virtual ~DinoEventExecCharaMoveToCoord();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    MFx32Point mPoint;    // 0x7C
    u32 mA;             // 0x84
    u32 mB;             // 0x88
    u8 mC;              // 0x8C
};

class DinoEventExecCharaMoveOffset : public MProcess {
public:
    DinoEventExecCharaMoveOffset(DinoEventLoop *event, u32 chara, MFx32Point offset, u32 a, u32 b, u8 c);
    virtual ~DinoEventExecCharaMoveOffset();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    MFx32Point mPoint;    // 0x7C
    u32 mA;             // 0x84
    u32 mB;             // 0x88
    u8 mC;              // 0x8C
};

class DinoEventExecCharaMoveVector : public MProcess {
public:
    DinoEventExecCharaMoveVector(DinoEventLoop *event, u32 chara, u16 direction, u32 a, u32 b, u32 c, u8 d);
    virtual ~DinoEventExecCharaMoveVector();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
    u16 mDirection;     // 0x7C
    u32 mA;             // 0x80
    u32 mB;             // 0x84
    u32 mC;             // 0x88
    u8 mD;              // 0x8C
};

class DinoEventExecCharaWaitMove : public MProcess {
public:
    DinoEventExecCharaWaitMove(DinoEventLoop *event, u32 chara);
    virtual ~DinoEventExecCharaWaitMove();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
};

class DinoEventExecCharaFocus : public MProcess {
public:
    DinoEventExecCharaFocus(DinoEventLoop *event, u32 chara);
    virtual ~DinoEventExecCharaFocus();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mChara;         // 0x78
};

class DinoEventExecCleaning : public MProcess {
public:
    DinoEventExecCleaning(DinoEventLoop *event, u32 unk80, u32 unk84, u8 unk88, u8 unk89);
    virtual ~DinoEventExecCleaning();
    virtual void Idle();

    void StateFadeOut();
    void StateCloseField();
    void StateCleaning();
    void StateResult();
    void StateOpenField();

    void (DinoEventExecCleaning::*mState)(); // 0x74
    DinoEventLoop *mEvent;  // 0x7C
    u32 mUnk80;             // 0x80
    u32 mUnk84;             // 0x84
    u8 mUnk88;              // 0x88
    u8 mUnk89;              // 0x89
    u8 unk8A[2];
    u32 mScore;             // 0x8C: the cleaning's result (the cleaning gets this block)
    u8 unk90[0xD0 - 0x90];
};

class DinoEventExecAutoWarp : public MProcess {
public:
    DinoEventExecAutoWarp(DinoEventLoop *event, u32 unk80, u32 unk84, u32 unk88);
    virtual ~DinoEventExecAutoWarp();
    virtual void Idle();

    void StateWarp();

    void (DinoEventExecAutoWarp::*mState)(); // 0x74
    DinoEventLoop *mEvent;  // 0x7C
    u32 mUnk80;             // 0x80
    u32 mUnk84;             // 0x84
    u32 mUnk88;             // 0x88
    u8 unk8C[0x9C - 0x8C];
};

class DinoEventExecCamera : public MTransfer {
public:
    DinoEventExecCamera(DinoEventLoop *event, u32 x, u32 y, u32 z, u32 a, u32 b, u32 c);
    virtual ~DinoEventExecCamera();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
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
    DinoEventExecStartFellow(DinoEventLoop *event, u32 chara, u32 target);
    virtual ~DinoEventExecStartFellow();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mTarget;        // 0x18
};

class DinoEventExecFinishFellow : public MTransfer {
public:
    DinoEventExecFinishFellow(DinoEventLoop *event, u32 chara);
    virtual ~DinoEventExecFinishFellow();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecStartWander : public MTransfer {
public:
    DinoEventExecStartWander(DinoEventLoop *event, u32 chara, u32 a, u32 b, u32 c, u32 d);
    virtual ~DinoEventExecStartWander();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mArgs[4];       // 0x18
};

class DinoEventExecStartWatch : public MTransfer {
public:
    DinoEventExecStartWatch(DinoEventLoop *event, u32 chara, DinoCharaWatch watch);
    virtual ~DinoEventExecStartWatch();
    virtual void Transfer();

    DinoEventLoop *mEvent;      // 0x10
    u32 mChara;             // 0x14
    DinoCharaWatch mWatch;  // 0x18
};

class DinoEventExecFinishWatch : public MTransfer {
public:
    DinoEventExecFinishWatch(DinoEventLoop *event, u32 chara);
    virtual ~DinoEventExecFinishWatch();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecStartBehold : public MTransfer {
public:
    DinoEventExecStartBehold(DinoEventLoop *event, u32 chara, u32 target);
    virtual ~DinoEventExecStartBehold();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mTarget;        // 0x18
};

class DinoEventExecFinishBehold : public MTransfer {
public:
    DinoEventExecFinishBehold(DinoEventLoop *event, u32 chara);
    virtual ~DinoEventExecFinishBehold();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecChoice : public MProcess {
public:
    DinoEventExecChoice(DinoEventLoop *event, u32 a, u32 b, u32 c, u32 d);
    virtual ~DinoEventExecChoice();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mUnk78;             // 0x78
    u32 mUnk7C;             // 0x7C
    u32 mUnk80;             // 0x80
    u32 mStep;              // 0x84
    u32 mUnk88;             // 0x88
    u32 mUnk8C;             // 0x8C
};

class DinoEventExecChoice2 : public MProcess {
public:
    virtual ~DinoEventExecChoice2();
    virtual void Idle();
};

class DinoEventExecYesNo : public MProcess {
public:
    DinoEventExecYesNo(DinoEventLoop *event, u32 flag, u32 text);
    virtual ~DinoEventExecYesNo();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mFlag;          // 0x78: gets the answer
    u32 mText;          // 0x7C: question, then optionally the two answers, one per line
    u32 mStep;          // 0x80
    BOOL mResult;       // 0x84
};

class DinoEventExecShopChoice : public MProcess {
public:
    DinoEventExecShopChoice(DinoEventLoop *event, u32 a, DinoEventVar var);
    virtual ~DinoEventExecShopChoice();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mUnk78;             // 0x78
    DinoEventVar mVar;      // 0x7C
    u32 mStep;              // 0x84
    u32 mUnk88;             // 0x88
};

class DinoEventExecShopChoice2 : public MProcess {
public:
    DinoEventExecShopChoice2(DinoEventLoop *event, u32 a, DinoEventVar var, u32 b);
    virtual ~DinoEventExecShopChoice2();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mUnk78;             // 0x78
    DinoEventVar mVar;      // 0x7C
    u32 mUnk84;             // 0x84
    u32 mStep;              // 0x88
    u32 mUnk8C;             // 0x8C
};

class DinoEventExecFade : public MProcess {
public:
    DinoEventExecFade(DinoEventLoop *event, u32 a, u32 b, u8 wait);
    virtual ~DinoEventExecFade();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
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

// Sets a creature's revived part levels, and puts it in or takes it out of the teams.
class DinoEventExecReviverSet : public MTransfer {
public:
    DinoEventExecReviverSet(u32 creature, u32 head, u32 body, u32 arms, u32 legs, u8 addToTeam);
    virtual ~DinoEventExecReviverSet();
    virtual void Transfer();

    u32 mCreature;      // 0x10: from 1
    u32 mLevels[4];     // 0x14
    u8 mAddToTeam;      // 0x24
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
    DinoEventExecShopSet(u32 shop);
    virtual ~DinoEventExecShopSet();
    virtual void Transfer();

    u32 mShop;  // 0x10
};

// Shows a kaseki rock being got.
class DinoEventKasekiRockGet : public MProcess {
public:
    DinoEventKasekiRockGet(DinoEventLoop *event, u32 mode, const DinoBackupEntry *entry, u8 *result);
    virtual ~DinoEventKasekiRockGet();
    virtual void Idle();

    MTask *CreateMessageBox(u32 message);
    MTask *CreateYesNo(u32 message);
    void StateStart();
    void StateAskSwap();
    void StateAnswered();
    void StateChosen();
    void StateConfirmed();
    void StateSwap();
    void StateStore();
    void StateReplace();
    void StateEnd();

    void (DinoEventKasekiRockGet::*mState)(); // 0x74
    DinoEventLoop *mEvent;  // 0x7C
    u32 mMode;              // 0x80
    DinoBackupEntry mEntry; // 0x84: the rock got
    MResource<DinoMessageData> mMessages;   // 0x98
    std::string mName;      // 0xA8: the rock got's
    std::string mSwapName;  // 0xB4: the one it replaces
    int mAnswer;            // 0xC0: the yes/no dialog's
    s32 mSwapIndex;         // 0xC4: or -1
    u8 *mResult;            // 0xC8
};

class DinoEventKasekiRockGetContainerSelect : public DinoEventCaptureFadeUiTask {
public:
    virtual ~DinoEventKasekiRockGetContainerSelect();

    u32 mUnk84[2];                      // 0x84
    std::vector<std::string> mUnk8C;    // 0x8C
};

class DinoEventExecKasekiRockGet : public MProcess {
public:
    DinoEventExecKasekiRockGet(DinoEventLoop *event, u32 flag, u32 id, u32 a, u32 b, u8 c);
    virtual ~DinoEventExecKasekiRockGet();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
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
    DinoEventExecKasekiRockShot(DinoEventLoop *event, u32 id, u32 a, u32 b);
    virtual ~DinoEventExecKasekiRockShot();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mId;            // 0x78
    u32 mA;             // 0x7C
    u32 mB;             // 0x80
    u32 mStep;          // 0x84
};

class DinoEventExecCharaBodyModelIndex : public MTransfer {
public:
    DinoEventExecCharaBodyModelIndex(DinoEventLoop *event, u32 chara, u32 model);
    virtual ~DinoEventExecCharaBodyModelIndex();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mModel;         // 0x18
};

class DinoEventExecCharaHeadModelIndex : public MTransfer {
public:
    DinoEventExecCharaHeadModelIndex(DinoEventLoop *event, u32 chara, u32 model);
    virtual ~DinoEventExecCharaHeadModelIndex();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mModel;         // 0x18
};

class DinoEventExecCharaMotionSet : public MTransfer {
public:
    DinoEventExecCharaMotionSet(DinoEventLoop *event, u32 chara, u32 motion, u8 loop);
    virtual ~DinoEventExecCharaMotionSet();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mMotion;        // 0x18
    u8 mLoop;           // 0x1C
};

class DinoEventExecCharaMotionCache : public MTransfer {
public:
    DinoEventExecCharaMotionCache(DinoEventLoop *event, u32 chara, u32 motion);
    virtual ~DinoEventExecCharaMotionCache();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mMotion;        // 0x18
};

class DinoEventExecCharaMotionCacheClear : public MTransfer {
public:
    DinoEventExecCharaMotionCacheClear(DinoEventLoop *event, u32 chara);
    virtual ~DinoEventExecCharaMotionCacheClear();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecManpuShow : public MTransfer {
public:
    DinoEventExecManpuShow(DinoEventLoop *event, u32 chara, u32 manpu, MPoint offset);
    virtual ~DinoEventExecManpuShow();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mManpu;         // 0x18
    MPoint mOffset;     // 0x1C
};

class DinoEventExecManpuHide : public MTransfer {
public:
    DinoEventExecManpuHide(DinoEventLoop *event, u32 chara);
    virtual ~DinoEventExecManpuHide();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecPictShow : public MProcess {
public:
    DinoEventExecPictShow(DinoEventLoop *event, u32 pict);
    virtual ~DinoEventExecPictShow();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;          // 0x78
};

class DinoEventExecPictHide : public MProcess {
public:
    DinoEventExecPictHide(DinoEventLoop *event);
    virtual ~DinoEventExecPictHide();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
};

class DinoEventExecCmnMotion : public MTransfer {
public:
    DinoEventExecCmnMotion(DinoEventLoop *event, u32 chara, u32 motion);
    virtual ~DinoEventExecCmnMotion();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mMotion;        // 0x18
};

class DinoEventExecChrEffect : public MTransfer {
public:
    DinoEventExecChrEffect(DinoEventLoop *event, u32 chara, u32 effect);
    virtual ~DinoEventExecChrEffect();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
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
    DinoEventExecBgmPlay(DinoEventLoop *event, u32 bgm, u32 arg, u8 fade);
    virtual ~DinoEventExecBgmPlay();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mBgm;           // 0x14
    u32 mArg;           // 0x18
    u8 mFade;           // 0x1C
};

class DinoEventExecMapBgmPlay : public MProcess {
public:
    DinoEventExecMapBgmPlay(DinoEventLoop *event, u32 arg);
    virtual ~DinoEventExecMapBgmPlay();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mArg;           // 0x78
};

class DinoEventExecBgmStop : public MTransfer {
public:
    DinoEventExecBgmStop(DinoEventLoop *event, u32 fadeFrames);
    virtual ~DinoEventExecBgmStop();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mFadeFrames;    // 0x14
};

class DinoEventExecQuake : public MTransfer {
public:
    DinoEventExecQuake(DinoEventLoop *event, u32 a, u32 b, u32 c);
    virtual ~DinoEventExecQuake();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mA;             // 0x14
    u32 mB;             // 0x18
    u32 mC;             // 0x1C
};

class DinoEventExecDance : public MProcess {
public:
    DinoEventExecDance(DinoEventLoop *event, u32 a, u32 b, u32 c, u32 d, u32 e);
    virtual ~DinoEventExecDance();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mA;             // 0x78
    u32 mB;             // 0x7C
    u32 mC;             // 0x80
    u32 mD;             // 0x84
    u32 mE;             // 0x88
};

class DinoEventExecMapSelect : public MProcess {
public:
    DinoEventExecMapSelect(DinoEventLoop *event, u32 var);
    virtual ~DinoEventExecMapSelect();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
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
    DinoEventExecGameOver(DinoEventLoop *event);
    virtual ~DinoEventExecGameOver();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mStep;          // 0x78
};

class DinoEventExecExit : public MProcess {
public:
    DinoEventExecExit(DinoEventLoop *event);
    virtual ~DinoEventExecExit();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mStep;          // 0x78
};

class DinoEventExecTalk : public MProcess {
public:
    DinoEventExecTalk(DinoEventLoop *event, u32 id, u8 flag);
    virtual ~DinoEventExecTalk();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mId;            // 0x78
    u8 mFlag;           // 0x7C
    u32 mStep;          // 0x80
};

class MTextPicture;
class MPointAnime;

// Shows a map's name sliding across the bottom screen.
class DinoEventExecFieldName : public MProcess {
public:
    DinoEventExecFieldName(DinoEventLoop *event, u32 id);
    virtual ~DinoEventExecFieldName();
    virtual void Idle();

    DinoEventLoop *mEvent;          // 0x74
    u32 mId;                    // 0x78: from 1
    u32 mStep;                  // 0x7C
    MTextPicture *mPicture;     // 0x80
    MPointAnime *mSlide;        // 0x84
    MPointAnime *mEase;         // 0x88
    MParallelTaskSet *mAnimes;  // 0x8C
};

class DinoEventExecLowerPictFadeIn : public MProcess {
public:
    DinoEventExecLowerPictFadeIn(DinoEventLoop *event, u32 pict, MPoint pos, u32 frames, long unk, u8 wait);
    virtual ~DinoEventExecLowerPictFadeIn();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    MPoint mPos;
    u32 mFrames;
    long mUnk;
    u8 mWait;
};

class DinoEventExecLowerPictFadeOut : public MProcess {
public:
    DinoEventExecLowerPictFadeOut(DinoEventLoop *event, u32 pict, long frames, u8 wait);
    virtual ~DinoEventExecLowerPictFadeOut();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    long mFrames;
    u8 mWait;
};

class DinoEventExecLowerPictSlideIn : public MProcess {
public:
    DinoEventExecLowerPictSlideIn(DinoEventLoop *event, u32 pict, MPoint pos, u32 a, u32 b, long c, u8 wait);
    virtual ~DinoEventExecLowerPictSlideIn();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    MPoint mPos;
    u32 mA;
    u32 mB;
    long mC;
    u8 mWait;
};

class DinoEventExecLowerPictSlideOut : public MProcess {
public:
    DinoEventExecLowerPictSlideOut(DinoEventLoop *event, u32 pict, u32 a, long b, u8 wait);
    virtual ~DinoEventExecLowerPictSlideOut();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    u32 mA;
    long mB;
    u8 mWait;
};

class DinoEventExecLowerPictZoomIn : public MProcess {
public:
    DinoEventExecLowerPictZoomIn(DinoEventLoop *event, u32 pict, MPoint pos, MFx32 scale, u32 a, long b, u8 wait);
    virtual ~DinoEventExecLowerPictZoomIn();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    MPoint mPos;
    MFx32 mScale;
    u32 mA;
    long mB;
    u8 mWait;
};

class DinoEventExecLowerPictZoomOut : public MProcess {
public:
    DinoEventExecLowerPictZoomOut(DinoEventLoop *event, MFx32 scale, u32 a, long b, u8 wait);
    virtual ~DinoEventExecLowerPictZoomOut();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    MFx32 mScale;
    u32 mA;
    long mB;
    u8 mWait;
};

class DinoEventExecUpperPictFadeIn : public MProcess {
public:
    DinoEventExecUpperPictFadeIn(DinoEventLoop *event, u32 pict, MPoint pos, u32 frames, long unk, u8 wait);
    virtual ~DinoEventExecUpperPictFadeIn();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    MPoint mPos;
    u32 mFrames;
    long mUnk;
    u8 mWait;
};

class DinoEventExecUpperPictFadeOut : public MProcess {
public:
    DinoEventExecUpperPictFadeOut(DinoEventLoop *event, u32 pict, long frames, u8 wait);
    virtual ~DinoEventExecUpperPictFadeOut();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    long mFrames;
    u8 mWait;
};

class DinoEventExecUpperPictSlideIn : public MProcess {
public:
    DinoEventExecUpperPictSlideIn(DinoEventLoop *event, u32 pict, MPoint pos, u32 a, u32 b, long c, u8 wait);
    virtual ~DinoEventExecUpperPictSlideIn();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    MPoint mPos;
    u32 mA;
    u32 mB;
    long mC;
    u8 mWait;
};

class DinoEventExecUpperPictSlideOut : public MProcess {
public:
    DinoEventExecUpperPictSlideOut(DinoEventLoop *event, u32 pict, u32 a, long b, u8 wait);
    virtual ~DinoEventExecUpperPictSlideOut();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    u32 mA;
    long mB;
    u8 mWait;
};

class DinoEventExecUpperPictZoomIn : public MProcess {
public:
    DinoEventExecUpperPictZoomIn(DinoEventLoop *event, u32 pict, MPoint pos, MFx32 scale, u32 a, long b, u8 wait);
    virtual ~DinoEventExecUpperPictZoomIn();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mPict;
    MPoint mPos;
    MFx32 mScale;
    u32 mA;
    long mB;
    u8 mWait;
};

class DinoEventExecUpperPictZoomOut : public MProcess {
public:
    DinoEventExecUpperPictZoomOut(DinoEventLoop *event, MFx32 scale, u32 a, long b, u8 wait);
    virtual ~DinoEventExecUpperPictZoomOut();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    MFx32 mScale;
    u32 mA;
    long mB;
    u8 mWait;
};

class DinoEventExecRadarMode : public MProcess {
public:
    DinoEventExecRadarMode(DinoEventLoop *event, u32 mode, u32 a, u8 b);
    virtual ~DinoEventExecRadarMode();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mMode;          // 0x78
    u32 mA;             // 0x7C
    u8 mB;              // 0x80
};

class DinoEventExecTownMapMode : public MProcess {
public:
    DinoEventExecTownMapMode(DinoEventLoop *event, u32 mode, u32 a);
    virtual ~DinoEventExecTownMapMode();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mMode;          // 0x78
    u32 mA;             // 0x7C
};

class DinoEventExecDelAreaNamePanel : public MTask {
public:
    DinoEventExecDelAreaNamePanel(DinoEventLoop *event);
    virtual ~DinoEventExecDelAreaNamePanel();
    virtual void Execute();

    DinoEventLoop *mEvent;  // 0x10
};

class DinoEventExecHeightReset : public MTransfer {
public:
    DinoEventExecHeightReset(DinoEventLoop *event);
    virtual ~DinoEventExecHeightReset();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
};

class DinoEventExecCharaFootSeOff : public MTransfer {
public:
    DinoEventExecCharaFootSeOff(DinoEventLoop *event, u32 chara);
    virtual ~DinoEventExecCharaFootSeOff();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
};

class DinoEventExecCharaFootSePreserve : public MTransfer {
public:
    DinoEventExecCharaFootSePreserve(DinoEventLoop *event, u32 chara, u32 se);
    virtual ~DinoEventExecCharaFootSePreserve();
    virtual void Transfer();

    DinoEventLoop *mEvent;  // 0x10
    u32 mChara;         // 0x14
    u32 mSe;            // 0x18
};

class DinoEventExecMoneyShow : public MProcess {
public:
    DinoEventExecMoneyShow(DinoEventLoop *event);
    virtual ~DinoEventExecMoneyShow();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
};

class DinoEventExecMoneyHide : public MProcess {
public:
    DinoEventExecMoneyHide(DinoEventLoop *event);
    virtual ~DinoEventExecMoneyHide();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
};

class DinoEventExecDonateShow : public MProcess {
public:
    DinoEventExecDonateShow(DinoEventLoop *event);
    virtual ~DinoEventExecDonateShow();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
};

class DinoEventExecDonateHide : public MProcess {
public:
    DinoEventExecDonateHide(DinoEventLoop *event);
    virtual ~DinoEventExecDonateHide();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
};

class DinoEventExecProhibitNisReset : public MTransfer {
public:
    DinoEventExecProhibitNisReset();
    virtual ~DinoEventExecProhibitNisReset();
    virtual void Transfer();
};

class DinoEventExecBattle : public MProcess {
public:
    DinoEventExecBattle(DinoEventLoop *event, u32 unk80, u32 battle, u32 unk88, s32 demo);
    virtual ~DinoEventExecBattle();
    virtual void Idle();

    void StateStartEffect();
    void StateCloseField();
    void StateUnloadOverlays();
    void StateLoadOverlays();
    void StateReviveStatus();
    void StateResetDisplays();
    void StateOpenField();
    void StateFadeOut();

    void (DinoEventExecBattle::*mState)(); // 0x74
    DinoEventLoop *mEvent;          // 0x7C
    u32 mUnk80;                     // 0x80: 0 shows the battle start effect
    u32 mBattle;                    // 0x84
    u32 mUnk88;                     // 0x88
    u8 mUnk8C;                      // 0x8C
    u8 unk8D[3];
    u32 mUnk90[3];
    DinoEventReviveCreature mCreatures[5];  // 0x9C
    DinoEventReviveCreature mCreatures2[5]; // 0x114
    std::vector<void *> mUnk18C;    // 0x18C
    std::vector<void *> mUnk198;    // 0x198
    u32 mUnk1A4;                    // 0x1A4
    s32 mDemo;                      // 0x1A8: which demo battle, or -1
};

class DinoEventExecStartPatrol : public MTransfer {
public:
    DinoEventExecStartPatrol(DinoEventLoop *event, u32 chara, u32 a, u32 b, u32 c, const std::vector<u32> &route);
    virtual ~DinoEventExecStartPatrol();
    virtual void Transfer();

    DinoEventLoop *mEvent;          // 0x10
    u32 mChara;                     // 0x14
    u32 mA;                         // 0x18
    u32 mB;                         // 0x1C
    u32 mC;                         // 0x20
    std::vector<u32> mRoute;        // 0x24: the points it walks between
};

#endif
