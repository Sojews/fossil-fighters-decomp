// DinoEventExec*: the event script commands.

#include "game/dino_event_exec.h"
#include "nitro/mi.h"
#include <algorithm>

extern "C" void func_0203b80c(void *event);

DinoEventExecVarValue::DinoEventExecVarValue(u32 op, DinoEventVar var, s32 value) {
    mOp = op;
    mVar = var;
    mValue = value;
}

void DinoEventExecVarValue::Transfer() {
    switch (mOp) {
    case 0x46:
        gDinoBackup->SetVar(mVar, mValue);
        break;
    case 0x47:
        gDinoBackup->SetVar(mVar, gDinoBackup->GetVar(mVar) + mValue);
        break;
    case 0x48:
        gDinoBackup->SetVar(mVar, gDinoBackup->GetVar(mVar) - mValue);
        break;
    case 0x49:
        gDinoBackup->SetVar(mVar, gDinoBackup->GetVar(mVar) * mValue);
        break;
    case 0x4a:
        gDinoBackup->SetVar(mVar, gDinoBackup->GetVar(mVar) / mValue);
        break;
    }
}

DinoEventExecVarVar::DinoEventExecVarVar(u32 op, DinoEventVar var, DinoEventVar source) {
    mOp = op;
    mVar = var;
    mSource = source;
}

void DinoEventExecVarVar::Transfer() {
    switch (mOp) {
    case 0x4b:
        gDinoBackup->SetVar(mVar, gDinoBackup->GetVar(mSource));
        break;
    case 0x4c:
        gDinoBackup->SetVar(mVar, gDinoBackup->GetVar(mVar) + gDinoBackup->GetVar(mSource));
        break;
    case 0x4d:
        gDinoBackup->SetVar(mVar, gDinoBackup->GetVar(mVar) - gDinoBackup->GetVar(mSource));
        break;
    case 0x4e:
        gDinoBackup->SetVar(mVar, gDinoBackup->GetVar(mVar) * gDinoBackup->GetVar(mSource));
        break;
    case 0x4f:
        gDinoBackup->SetVar(mVar, gDinoBackup->GetVar(mVar) / gDinoBackup->GetVar(mSource));
        break;
    }
}

DinoEventExecMapConnect::DinoEventExecMapConnect(u32 cmd, u32 from, u32 to) {
    mCmd = cmd;
    mFrom = from;
    mTo = to;
}

void DinoEventExecMapConnect::Execute() {
    gDinoBackup->ConnectRegion(mFrom, mTo);
    gDinoBackup->ConnectRegion(mTo, mFrom);
    Exit();
}

DinoEventExecMapConnectDirectional::DinoEventExecMapConnectDirectional(u32 cmd, u32 from, u32 to) {
    mCmd = cmd;
    mFrom = from;
    mTo = to;
}

void DinoEventExecMapConnectDirectional::Execute() {
    gDinoBackup->ConnectRegion(mFrom, mTo);
    Exit();
}

DinoEventExecMapDisconnect::DinoEventExecMapDisconnect(u32 cmd, u32 from, u32 to) {
    mCmd = cmd;
    mFrom = from;
    mTo = to;
}

void DinoEventExecMapDisconnect::Execute() {
    gDinoBackup->DisconnectRegion(mFrom, mTo);
    gDinoBackup->DisconnectRegion(mTo, mFrom);
    Exit();
}

DinoEventExecMapDisconnectDirectional::DinoEventExecMapDisconnectDirectional(u32 cmd, u32 from, u32 to) {
    mCmd = cmd;
    mFrom = from;
    mTo = to;
}

void DinoEventExecMapDisconnectDirectional::Execute() {
    gDinoBackup->DisconnectRegion(mFrom, mTo);
    Exit();
}

DinoEventExecFlagSet::DinoEventExecFlagSet(u32 cmd, DinoEventVar var, u8 value) {
    mCmd = cmd;
    mVar = var;
    mValue = value;
}

void DinoEventExecFlagSet::Execute() {
    gDinoBackup->SetVarFlag(mVar, mValue);
    Exit();
}

DinoEventExecEventClear::DinoEventExecEventClear(u32 cmd, u32 id, u8 value) {
    mCmd = cmd;
    mId = id;
    mValue = value;
}

void DinoEventExecEventClear::Execute() {
    gDinoBackup->SetEventClear(mId, mValue);
    Exit();
}

DinoEventExecSetEpisodeClear::DinoEventExecSetEpisodeClear(u32 cmd, u32 id, u8 value) {
    mCmd = cmd;
    mId = id;
    mValue = value;
}

void DinoEventExecSetEpisodeClear::Execute() {
    gDinoBackup->SetEpisodeClear(mId, mValue);
    Exit();
}

DinoEventExecProhibitCameraRelease::DinoEventExecProhibitCameraRelease(u8 *flag) {
    mFlag = flag;
}

void DinoEventExecProhibitCameraRelease::Transfer() {
    *mFlag = TRUE;
}

DinoEventExecFinish::DinoEventExecFinish(void *event) {
    mEvent = event;
}

void DinoEventExecFinish::Transfer() {
    func_0203b80c(mEvent);
}

DinoEventExecLicenseGet::DinoEventExecLicenseGet(u32 license) {
    mValue = license;
}

void DinoEventExecLicenseGet::Transfer() {
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, FALSE);
    player.license = mValue;
    gDinoBackup->SetPlayer(&player);
}

DinoEventExecHoriderLevel::DinoEventExecHoriderLevel(u32 level) {
    mValue = level;
}

void DinoEventExecHoriderLevel::Transfer() {
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, FALSE);
    player.horiderLevel = mValue;
    gDinoBackup->SetPlayer(&player);
}

DinoEventExecContainerNum::DinoEventExecContainerNum(u32 count) {
    mValue = count;
}

void DinoEventExecContainerNum::Transfer() {
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, FALSE);
    player.containerNum = mValue;
    gDinoBackup->SetPlayer(&player);
}

DinoEventExecSystemFlag::DinoEventExecSystemFlag(u32 id, u8 value) {
    mId = id;
    mValue = value;
}

void DinoEventExecSystemFlag::Transfer() {
    gDinoBackup->SetSystemFlag(mId, mValue);
}

DinoEventExecMedalChange::DinoEventExecMedalChange(u32 from, u32 to) {
    mFrom = from;
    mTo = to;
}

void DinoEventExecMedalChange::Transfer() {
    DinoCreatureState creature;
    gDinoBackup->GetCreature(mTo, &creature);
    if (creature.parts[0].level == 0) {
        return;
    }
    DinoTeam team;
    gDinoBackup->GetTeam(gDinoBackup->GetUnk34_26(), &team);
    for (u32 i = 0; i < 5; i++) {
        if (mFrom == team.slots[i]) {
            team.slots[i] = mTo;
            gDinoBackup->SetTeam(gDinoBackup->GetUnk34_26(), &team);
            return;
        }
    }
}

DinoEventExecBattlePointSet::DinoEventExecBattlePointSet(u32 creature, u32 points) {
    mCreature = creature;
    mPoints = points;
}

void DinoEventExecBattlePointSet::Transfer() {
    DinoCreatureState creature;
    gDinoBackup->GetCreature(mCreature, &creature);
    creature.unk0_8 = mPoints;
    gDinoBackup->SetCreature(mCreature, &creature);
}

DinoEventExecEraseAllExcrement::DinoEventExecEraseAllExcrement(u32 cmd) {
    mCmd = cmd;
}

void DinoEventExecEraseAllExcrement::Transfer() {
    DinoBackupEntry entry;
    for (u32 i = 0; i < 64; i++) {
        gDinoBackup->GetUnk98(i, &entry);
        if (entry.id != 0 && entry.active && entry.type == 2) {
            MI_CpuFill8(&entry, 0, sizeof(entry));
            gDinoBackup->SetUnk98(i, &entry);
        }
    }
    gDinoBackup->CompactUnk98();
}

DinoEventExecEraseExcrement::DinoEventExecEraseExcrement(u32 cmd, DinoEventVar count) {
    mCmd = cmd;
    mCount = count;
}

void DinoEventExecEraseExcrement::Transfer() {
    DinoBackupEntry entry;
    for (s32 n = std::min(gDinoBackup->GetVar(mCount), gDinoBackup->GetSystemVar(12)); n > 0; n--) {
        for (u32 i = 0; i < 64; i++) {
            gDinoBackup->GetUnk98(i, &entry);
            if (entry.id != 0 && entry.active && entry.type == 2) {
                MI_CpuFill8(&entry, 0, sizeof(entry));
                gDinoBackup->SetUnk98(i, &entry);
                break;
            }
        }
    }
    gDinoBackup->CompactUnk98();
}

static void ProhibitNisReset() {
}

DinoEventExecProhibitNisReset::DinoEventExecProhibitNisReset() {
}

void DinoEventExecProhibitNisReset::Transfer() {
    ProhibitNisReset();
}

DinoEventExecStartBehold::DinoEventExecStartBehold(DinoEvent *event, u32 chara, u32 target) {
    mEvent = event;
    mChara = chara;
    mTarget = target;
}

void DinoEventExecStartBehold::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->StartBehold(mTarget);
    }
}

DinoEventExecFinishBehold::DinoEventExecFinishBehold(DinoEvent *event, u32 chara) {
    mEvent = event;
    mChara = chara;
}

void DinoEventExecFinishBehold::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->FinishBehold();
    }
}

DinoEventExecCharaMotionSet::DinoEventExecCharaMotionSet(DinoEvent *event, u32 chara, u32 motion, u8 loop) {
    mEvent = event;
    mChara = chara;
    mMotion = motion;
    mLoop = loop;
}

void DinoEventExecCharaMotionSet::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->SetMotion(mMotion, mLoop, MFx32(1));
        chara->ApplyMotion();
    }
}

DinoEventExecCharaMotionCache::DinoEventExecCharaMotionCache(DinoEvent *event, u32 chara, u32 motion) {
    mEvent = event;
    mChara = chara;
    mMotion = motion;
}

void DinoEventExecCharaMotionCache::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->CacheMotion(mMotion);
    }
}

DinoEventExecCharaMotionCacheClear::DinoEventExecCharaMotionCacheClear(DinoEvent *event, u32 chara) {
    mEvent = event;
    mChara = chara;
}

void DinoEventExecCharaMotionCacheClear::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->ClearMotionCache();
    }
}

DinoEventExecManpuShow::DinoEventExecManpuShow(DinoEvent *event, u32 chara, u32 manpu, MPoint offset) {
    mEvent = event;
    mChara = chara;
    mManpu = manpu;
    mOffset = offset;
}

void DinoEventExecManpuShow::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->ShowManpu(mManpu, mOffset);
    }
}

DinoEventExecManpuHide::DinoEventExecManpuHide(DinoEvent *event, u32 chara) {
    mEvent = event;
    mChara = chara;
}

void DinoEventExecManpuHide::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->HideManpu();
    }
}

DinoEventExecCmnMotion::DinoEventExecCmnMotion(DinoEvent *event, u32 chara, u32 motion) {
    mEvent = event;
    mChara = chara;
    mMotion = motion;
}

void DinoEventExecCmnMotion::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->SetCommonMotion(mMotion);
    }
}

DinoEventExecChrEffect::DinoEventExecChrEffect(DinoEvent *event, u32 chara, u32 effect) {
    mEvent = event;
    mChara = chara;
    mEffect = effect;
}

void DinoEventExecChrEffect::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->SetEffect(mEffect);
    }
}

DinoEventExecBgmPlay::DinoEventExecBgmPlay(DinoEvent *event, u32 bgm, u32 arg, u8 fade) {
    mEvent = event;
    mBgm = bgm;
    mArg = arg;
    mFade = fade;
}

void DinoEventExecBgmPlay::Transfer() {
    u32 bgm = mBgm;
    if (bgm == 0) {
        return;
    }
    if (mFade) {
        mEvent->GetField()->PlayBgmFade(bgm, mArg);
    } else {
        mEvent->GetField()->PlayBgm(bgm, mArg);
    }
}

DinoEventExecBgmStop::DinoEventExecBgmStop(DinoEvent *event, u32 fadeFrames) {
    mEvent = event;
    mFadeFrames = fadeFrames;
}

void DinoEventExecBgmStop::Transfer() {
    mEvent->GetField()->StopBgm(mFadeFrames);
}

DinoEventExecQuake::DinoEventExecQuake(DinoEvent *event, u32 a, u32 b, u32 c) {
    mEvent = event;
    mA = a;
    mB = b;
    mC = c;
}

void DinoEventExecQuake::Transfer() {
    mEvent->GetField()->Quake(mA, mB, mC);
}

DinoEventExecDelAreaNamePanel::DinoEventExecDelAreaNamePanel(DinoEvent *event) {
    mEvent = event;
}

void DinoEventExecDelAreaNamePanel::Execute() {
    mEvent->GetField()->GetCamera()->mNamePanel->SetMode(0, 1);
    Exit();
}

DinoEventExecHeightReset::DinoEventExecHeightReset(DinoEvent *event) {
    mEvent = event;
}

void DinoEventExecHeightReset::Transfer() {
    mEvent->GetField()->GetCamera()->ResetHeight();
}

DinoEventExecCharaFootSeOff::DinoEventExecCharaFootSeOff(DinoEvent *event, u32 chara) {
    mEvent = event;
    mChara = chara;
}

void DinoEventExecCharaFootSeOff::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->SetFootSe(0);
    }
}

DinoEventExecCharaFootSePreserve::DinoEventExecCharaFootSePreserve(DinoEvent *event, u32 chara, u32 se) {
    mEvent = event;
    mChara = chara;
    mSe = se;
}

void DinoEventExecCharaFootSePreserve::Transfer() {
    DinoFieldChara *chara = mEvent->GetField()->GetChara(mChara);
    if (chara != NULL) {
        chara->SetFootSe(mSe);
    }
}

DinoEventExecMoneyShow::DinoEventExecMoneyShow(DinoEvent *event) : MProcess(NULL) {
    mEvent = event;
}

void DinoEventExecMoneyShow::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = mEvent->GetField()->CreateMoneyShowTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(exit);
}

DinoEventExecMoneyHide::DinoEventExecMoneyHide(DinoEvent *event) : MProcess(NULL) {
    mEvent = event;
}

void DinoEventExecMoneyHide::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = mEvent->GetField()->CreateMoneyHideTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(exit);
}

DinoEventExecDonateShow::DinoEventExecDonateShow(DinoEvent *event) : MProcess(NULL) {
    mEvent = event;
}

void DinoEventExecDonateShow::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = mEvent->GetField()->CreateDonateShowTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(exit);
}

DinoEventExecDonateHide::DinoEventExecDonateHide(DinoEvent *event) : MProcess(NULL) {
    mEvent = event;
}

void DinoEventExecDonateHide::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = mEvent->GetField()->CreateDonateHideTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(exit);
}

DinoEventExecVarValue::~DinoEventExecVarValue() {
}

DinoEventExecVarVar::~DinoEventExecVarVar() {
}

DinoEventExecMapConnect::~DinoEventExecMapConnect() {
}

DinoEventExecMapConnectDirectional::~DinoEventExecMapConnectDirectional() {
}

DinoEventExecMapDisconnect::~DinoEventExecMapDisconnect() {
}

DinoEventExecMapDisconnectDirectional::~DinoEventExecMapDisconnectDirectional() {
}

DinoEventExecFlagSet::~DinoEventExecFlagSet() {
}

DinoEventExecEventClear::~DinoEventExecEventClear() {
}

DinoEventExecSetEpisodeClear::~DinoEventExecSetEpisodeClear() {
}

DinoEventExecAppendCharaRegion::~DinoEventExecAppendCharaRegion() {
}

DinoEventExecAppendCharaChara::~DinoEventExecAppendCharaChara() {
}

DinoEventExecRemoveChara::~DinoEventExecRemoveChara() {
}

DinoEventExecCharaAngle::~DinoEventExecCharaAngle() {
}

DinoEventExecCharaAngleToChara::~DinoEventExecCharaAngleToChara() {
}

DinoEventExecCharaAngleOffset::~DinoEventExecCharaAngleOffset() {
}

DinoEventExecCharaAngleOffsetToChara::~DinoEventExecCharaAngleOffsetToChara() {
}

DinoEventExecCharaMoveToChara::~DinoEventExecCharaMoveToChara() {
}

DinoEventExecCharaMoveToRegion::~DinoEventExecCharaMoveToRegion() {
}

DinoEventExecCharaMoveToCoord::~DinoEventExecCharaMoveToCoord() {
}

DinoEventExecCharaMoveOffset::~DinoEventExecCharaMoveOffset() {
}

DinoEventExecCharaMoveVector::~DinoEventExecCharaMoveVector() {
}

DinoEventExecCharaWaitMove::~DinoEventExecCharaWaitMove() {
}

DinoEventExecCharaFocus::~DinoEventExecCharaFocus() {
}

DinoEventExecCleaning::~DinoEventExecCleaning() {
}

DinoEventExecAutoWarp::~DinoEventExecAutoWarp() {
}

DinoEventExecCamera::~DinoEventExecCamera() {
}

DinoEventExecProhibitCameraRelease::~DinoEventExecProhibitCameraRelease() {
}

DinoEventExecFinish::~DinoEventExecFinish() {
}

DinoEventExecStartFellow::~DinoEventExecStartFellow() {
}

DinoEventExecFinishFellow::~DinoEventExecFinishFellow() {
}

DinoEventExecStartWander::~DinoEventExecStartWander() {
}

DinoEventExecStartWatch::~DinoEventExecStartWatch() {
}

DinoEventExecFinishWatch::~DinoEventExecFinishWatch() {
}

DinoEventExecStartBehold::~DinoEventExecStartBehold() {
}

DinoEventExecFinishBehold::~DinoEventExecFinishBehold() {
}

DinoEventExecChoice::~DinoEventExecChoice() {
}

DinoEventExecChoice2::~DinoEventExecChoice2() {
}

DinoEventExecYesNo::~DinoEventExecYesNo() {
}

DinoEventExecShopChoice::~DinoEventExecShopChoice() {
}

DinoEventExecShopChoice2::~DinoEventExecShopChoice2() {
}

DinoEventExecFade::~DinoEventExecFade() {
}

DinoEventExecLicenseGet::~DinoEventExecLicenseGet() {
}

DinoEventExecHoriderLevel::~DinoEventExecHoriderLevel() {
}

DinoEventExecContainerNum::~DinoEventExecContainerNum() {
}

DinoEventExecSystemFlag::~DinoEventExecSystemFlag() {
}

DinoEventExecReviverSet::~DinoEventExecReviverSet() {
}

DinoEventExecMedalChange::~DinoEventExecMedalChange() {
}

DinoEventExecBattlePointSet::~DinoEventExecBattlePointSet() {
}

DinoEventExecShopSet::~DinoEventExecShopSet() {
}

DinoEventExecKasekiRockGet::~DinoEventExecKasekiRockGet() {
}

DinoEventExecKasekiRockShot::~DinoEventExecKasekiRockShot() {
}

DinoEventExecCharaBodyModelIndex::~DinoEventExecCharaBodyModelIndex() {
}

DinoEventExecCharaHeadModelIndex::~DinoEventExecCharaHeadModelIndex() {
}

DinoEventExecCharaMotionSet::~DinoEventExecCharaMotionSet() {
}

DinoEventExecCharaMotionCache::~DinoEventExecCharaMotionCache() {
}

DinoEventExecCharaMotionCacheClear::~DinoEventExecCharaMotionCacheClear() {
}

DinoEventExecManpuShow::~DinoEventExecManpuShow() {
}

DinoEventExecManpuHide::~DinoEventExecManpuHide() {
}

DinoEventExecPictShow::~DinoEventExecPictShow() {
}

DinoEventExecPictHide::~DinoEventExecPictHide() {
}

DinoEventExecCmnMotion::~DinoEventExecCmnMotion() {
}

DinoEventExecChrEffect::~DinoEventExecChrEffect() {
}

DinoEventExecKeyWait::~DinoEventExecKeyWait() {
}

DinoEventExecBgmPlay::~DinoEventExecBgmPlay() {
}

DinoEventExecMapBgmPlay::~DinoEventExecMapBgmPlay() {
}

DinoEventExecBgmStop::~DinoEventExecBgmStop() {
}

DinoEventExecQuake::~DinoEventExecQuake() {
}

DinoEventExecDance::~DinoEventExecDance() {
}

DinoEventExecMapSelect::~DinoEventExecMapSelect() {
}

DinoEventExecEraseAllExcrement::~DinoEventExecEraseAllExcrement() {
}

DinoEventExecEraseExcrement::~DinoEventExecEraseExcrement() {
}

DinoEventExecGameOver::~DinoEventExecGameOver() {
}

DinoEventExecExit::~DinoEventExecExit() {
}

DinoEventExecTalk::~DinoEventExecTalk() {
}

DinoEventExecFieldName::~DinoEventExecFieldName() {
}

DinoEventExecLowerPictFadeIn::~DinoEventExecLowerPictFadeIn() {
}

DinoEventExecLowerPictFadeOut::~DinoEventExecLowerPictFadeOut() {
}

DinoEventExecLowerPictSlideIn::~DinoEventExecLowerPictSlideIn() {
}

DinoEventExecLowerPictSlideOut::~DinoEventExecLowerPictSlideOut() {
}

DinoEventExecLowerPictZoomIn::~DinoEventExecLowerPictZoomIn() {
}

DinoEventExecLowerPictZoomOut::~DinoEventExecLowerPictZoomOut() {
}

DinoEventExecUpperPictFadeIn::~DinoEventExecUpperPictFadeIn() {
}

DinoEventExecUpperPictFadeOut::~DinoEventExecUpperPictFadeOut() {
}

DinoEventExecUpperPictSlideIn::~DinoEventExecUpperPictSlideIn() {
}

DinoEventExecUpperPictSlideOut::~DinoEventExecUpperPictSlideOut() {
}

DinoEventExecUpperPictZoomIn::~DinoEventExecUpperPictZoomIn() {
}

DinoEventExecUpperPictZoomOut::~DinoEventExecUpperPictZoomOut() {
}

DinoEventExecRadarMode::~DinoEventExecRadarMode() {
}

DinoEventExecTownMapMode::~DinoEventExecTownMapMode() {
}

DinoEventExecDelAreaNamePanel::~DinoEventExecDelAreaNamePanel() {
}

DinoEventExecHeightReset::~DinoEventExecHeightReset() {
}

DinoEventExecCharaFootSeOff::~DinoEventExecCharaFootSeOff() {
}

DinoEventExecCharaFootSePreserve::~DinoEventExecCharaFootSePreserve() {
}

DinoEventExecMoneyShow::~DinoEventExecMoneyShow() {
}

DinoEventExecMoneyHide::~DinoEventExecMoneyHide() {
}

DinoEventExecDonateShow::~DinoEventExecDonateShow() {
}

DinoEventExecDonateHide::~DinoEventExecDonateHide() {
}

DinoEventExecProhibitNisReset::~DinoEventExecProhibitNisReset() {
}
