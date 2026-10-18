// Message types used by the battle system and wireless play, and the factory that creates them from received data.

#include "game/dino_message.h"

// (The factory was built without inlining, so it calls the messages' inline constructors.)
#pragma dont_inline on
// Creates a message from its serialized form.
MMessage *CreateDinoMessage(const void *data) {
    MMessage *msg;

    switch (*(const u32 *)data) {
    default:
        return NULL;
    case 0x10001:
        msg = new DinoDummyMessage();
        break;
    case 0x10002:
        msg = new DinoBattleModelGameBeginSignal();
        break;
    case 0x10003:
        msg = new DinoBattleControllerInitRequest();
        break;
    case 0x10004:
        msg = new DinoBattleControllerInitResult();
        break;
    case 0x10005:
        msg = new DinoBattleModelPlayerBeginSignal();
        break;
    case 0x10006:
        msg = new DinoBattleModelCreatureBeginSignal();
        break;
    case 0x10007:
        msg = new DinoBattleViewCreatureBeginSignal();
        break;
    case 0x10008:
        msg = new DinoBattleViewPlayerBeginSignal();
        break;
    case 0x10009:
        msg = new DinoBattleViewEffectGameBeginRequest();
        break;
    case 0x1000A:
        msg = new DinoBattleViewEffectGameBeginResult();
        break;
    case 0x1000B:
        msg = new DinoBattleViewEffectTurnBeginRequest();
        break;
    case 0x1000C:
        msg = new DinoBattleViewEffectTurnBeginResult();
        break;
    case 0x1000D:
        msg = new DinoBattleControllerWakeupSignal();
        break;
    case 0x1000E:
        msg = new DinoBattleModelPlayerTurnBeginRequst();
        break;
    case 0x1000F:
        msg = new DinoBattleModelPlayerTurnBeginResult();
        break;
    case 0x10010:
        msg = new DinoBattleViewUiRootMenuRequest();
        break;
    case 0x10011:
        msg = new DinoBattleViewUiRootMenuResult();
        break;
    case 0x10012:
        msg = new DinoBattleModelCreatureMoveRequest();
        break;
    case 0x10013:
        msg = new DinoBattleModelCreatureMoveResult();
        break;
    case 0x10014:
        msg = new DinoBattleModelTurnChangeSignal();
        break;
    case 0x10015:
        msg = new DinoBattleModelTurnEndRequest();
        break;
    case 0x10016:
        msg = new DinoBattleModelTurnEndResult();
        break;
    case 0x1001B:
        msg = new DinoBattleViewUiChangeMenuRequest();
        break;
    case 0x1001C:
        msg = new DinoBattleViewUiChangeMenuResult();
        break;
    case 0x1001D:
        msg = new DinoBattleViewUiCreatureInfoMenuRequest();
        break;
    case 0x1001E:
        msg = new DinoBattleViewUiCreatureInfoMenuResult();
        break;
    case 0x1001F:
        msg = new DinoBattleModelGoEscapeAreaRequest();
        break;
    case 0x10020:
        msg = new DinoBattleModelGoEscapeAreaResult();
        break;
    case 0x10021:
        msg = new DinoBattleViewCreatureMoveRequest();
        break;
    case 0x10022:
        msg = new DinoBattleViewCreatureMoveResult();
        break;
    case 0x10017:
        msg = new DinoBattleViewUiAttackerMenuRequest();
        break;
    case 0x10018:
        msg = new DinoBattleViewUiAttackerMenuResult();
        break;
    case 0x10019:
        msg = new DinoBattleViewUiDefenderMenuRequest();
        break;
    case 0x1001A:
        msg = new DinoBattleViewUiDefenderMenuResult();
        break;
    case 0x10023:
        msg = new DinoBattleViewUiAbilityMenuRequest();
        break;
    case 0x10024:
        msg = new DinoBattleViewUiAbilityMenuResult();
        break;
    case 0x10025:
        msg = new DinoBattleModelCreatureAbilityRequest();
        break;
    case 0x10026:
        msg = new DinoBattleModelCreatureAbilityResult();
        break;
    case 0x10027:
        msg = new DinoBattleViewSetActionedSignal();
        break;
    case 0x10028:
        msg = new DinoBattleViewCreatureCpuArrowUpdateSignal();
        break;
    case 0x10029:
        msg = new DinoBattleViewCreatureAbilityRequest();
        break;
    case 0x1002A:
        msg = new DinoBattleViewCreatureAbilityResult();
        break;
    case 0x1002D:
        msg = new DinoBattleView3DTeamAttackRequest();
        break;
    case 0x1002E:
        msg = new DinoBattleView3DTeamAttackResult();
        break;
    case 0x1002F:
        msg = new DinoBattleViewCreatureUpdateRequest();
        break;
    case 0x1002B:
        msg = new DinoBattleViewCreatureAbility3DRequest();
        break;
    case 0x1002C:
        msg = new DinoBattleViewCreatureAbility3DResult();
        break;
    case 0x10030:
        msg = new DinoBattleViewCreatureUpdateResult();
        break;
    case 0x10031:
        msg = new DinoBattleModelCreatureEndRequest();
        break;
    case 0x10032:
        msg = new DinoBattleModelCreatureEndResult();
        break;
    case 0x10033:
        msg = new DinoBattleViewCreatureEndRequest();
        break;
    case 0x10034:
        msg = new DinoBattleViewCreatureEndResult();
        break;
    case 0x10035:
        msg = new DinoBattleModelGameEndSignal();
        break;
    case 0x10036:
        msg = new DinoBattleViewEffectGameEndRequest();
        break;
    case 0x10037:
        msg = new DinoBattleViewEffectGameEndResult();
        break;
    case 0x10038:
        msg = new DinoBattleViewPlayerUpdateRequest();
        break;
    case 0x10039:
        msg = new DinoBattleViewPlayerUpdateResult();
        break;
    case 0x1003A:
        msg = new DinoBattleModelCreatureTurnBeginRequest();
        break;
    case 0x1003B:
        msg = new DinoBattleModelCreatureTurnBeginResult();
        break;
    case 0x1003C:
        msg = new DinoBattleModelPlayerTurnBeginProcessRequst();
        break;
    case 0x1003D:
        msg = new DinoBattleModelPlayerTurnBeginProcessResult();
        break;
    case 0x1003E:
        msg = new DinoBattleModelCreatureSpecialLifeRecoveryRequest();
        break;
    case 0x1003F:
        msg = new DinoBattleModelCreatureSpecialLifeRecoveryResult();
        break;
    case 0x10040:
        msg = new DinoBattleModelEvolutionCreatureRequest();
        break;
    case 0x10041:
        msg = new DinoBattleModelEvolutionCreatureResult();
        break;
    case 0x10042:
        msg = new DinoBattleViewEvolutionCreatureRequest();
        break;
    case 0x10043:
        msg = new DinoBattleViewEvolutionCreatureResult();
        break;
    case 0x10044:
        msg = new DinoBattleViewCreatureCounterDamageRequest();
        break;
    case 0x10045:
        msg = new DinoBattleViewCreatureCounterDamageResult();
        break;
    case 0x10046:
        msg = new DinoBattleViewPlayerUpdateStateIconRequest();
        break;
    case 0x10047:
        msg = new DinoBattleViewPlayerUpdateStateIconResult();
        break;
    case 0x10048:
        msg = new DinoBattleViewDeleteStatusEffectSignal();
        break;
    case 0x10049:
        msg = new DinoBattleViewPlayerUpdateAbilityIconRequest();
        break;
    case 0x1004A:
        msg = new DinoBattleViewPlayerUpdateAbilityIconResult();
        break;
    case 0x1004B:
        msg = new DinoBattleViewUiTurnEndRequest();
        break;
    case 0x1004C:
        msg = new DinoBattleViewUiTurnEndResult();
        break;
    case 0x1004D:
        msg = new DinoBattleModelPlayerConfusionExecRequest();
        break;
    case 0x1004E:
        msg = new DinoBattleModelPlayerConfusionExecResult();
        break;
    case 0x1004F:
        msg = new DinoBattleModelCreatureSupportEffectRequest();
        break;
    case 0x10050:
        msg = new DinoBattleModelCreatureSupportEffectResult();
        break;
    case 0x10051:
        msg = new DinoBattleViewCreatureSupportEffectRequest();
        break;
    case 0x10052:
        msg = new DinoBattleViewCreatureSupportEffectResult();
        break;
    case 0x10053:
        msg = new DinoBattleViewCreatureMotionEffectRequest();
        break;
    case 0x10054:
        msg = new DinoBattleViewCreatureMotionEffectResult();
        break;
    case 0x10055:
        msg = new DinoBattleViewFieldEffectWaitRequest();
        break;
    case 0x10056:
        msg = new DinoBattleViewFieldEffectWaitResult();
        break;
    case 0x10057:
        msg = new DinoBattleViewCreaturePlayMotionRequest();
        break;
    case 0x10058:
        msg = new DinoBattleViewCreaturePlayMotionResult();
        break;
    case 0x10059:
        msg = new DinoBattleViewFreeCoordEffectRequest();
        break;
    case 0x1005A:
        msg = new DinoBattleViewFreeCoordEffectResult();
        break;
    case 0x1005B:
        msg = new DinoBattleModelCreatureReleaseStatusEffectRequest();
        break;
    case 0x1005C:
        msg = new DinoBattleModelCreatureReleaseStatusEffectResult();
        break;
    case 0x1005D:
        msg = new DinoBattleViewCreatureTurnBeginSignal();
        break;
    case 0x1005E:
        msg = new DinoBattleViewCreatureTurnEndSignal();
        break;
    case 0x1005F:
        msg = new DinoBattleViewCreatureDamagedUpdateRequest();
        break;
    case 0x10060:
        msg = new DinoBattleViewCreatureDamagedUpdateResult();
        break;
    case 0x10061:
        msg = new DinoBattleViewWaitUpdateSupportEffectAnimeRequest();
        break;
    case 0x10062:
        msg = new DinoBattleViewWaitUpdateSupportEffectAnimeResult();
        break;
    case 0x10063:
        msg = new DinoBattleViewActionInfoBarUpdateRequest();
        break;
    case 0x10064:
        msg = new DinoBattleViewActionInfoBarUpdateResult();
        break;
    case 0x10065:
        msg = new DinoBattleViewFocusLCDFrameRequest();
        break;
    case 0x10066:
        msg = new DinoBattleViewFocusLCDFrameResult();
        break;
    case 0x10067:
        msg = new DinoBattleViewCreatureSetBlendRequest();
        break;
    case 0x10068:
        msg = new DinoBattleViewCreatureSetBlendResult();
        break;
    case 0x10069:
        msg = new DinoBattleViewLCDFadeRequest();
        break;
    case 0x1006A:
        msg = new DinoBattleViewLCDFadeResult();
        break;
    case 0x1006B:
        msg = new DinoBattleViewWaitActionInfoBarRequest();
        break;
    case 0x1006C:
        msg = new DinoBattleViewWaitActionInfoBarResult();
        break;
    case 0x1006D:
        msg = new DinoBattleViewClearAttackBarRequest();
        break;
    case 0x1006E:
        msg = new DinoBattleViewClearAttackBarResult();
        break;
    case 0x1006F:
        msg = new DinoBattleViewShowAttackNameBarRequest();
        break;
    case 0x10070:
        msg = new DinoBattleViewMoveAttackNameBarRequest();
        break;
    case 0x10071:
        msg = new DinoBattleViewMoveAttackNameBarResult();
        break;
    case 0x10072:
        msg = new DinoBattleViewShowAttackNameBarResult();
        break;
    case 0x10073:
        msg = new DinoBattleView3DSupportEffectRequest();
        break;
    case 0x10074:
        msg = new DinoBattleView3DSupportEffectResult();
        break;
    case 0x10075:
        msg = new DinoBattleView3DWatchStatusRequest();
        break;
    case 0x10076:
        msg = new DinoBattleView3DWatchStatusResult();
        break;
    case 0x10077:
        msg = new DinoBattleView3DPoisonDamageRequest();
        break;
    case 0x10078:
        msg = new DinoBattleView3DPoisonDamageResult();
        break;
    case 0x10079:
        msg = new DinoBattleView3DPreTurnEndRequest();
        break;
    case 0x1007A:
        msg = new DinoBattleView3DPreTurnEndResult();
        break;
    case 0x1007B:
        msg = new DinoBattleView3DTurnEndRequest();
        break;
    case 0x1007C:
        msg = new DinoBattleView3DTurnEndResult();
        break;
    case 0x1007D:
        msg = new DinoBattleViewSetCreaturePriorityRequest();
        break;
    case 0x1007E:
        msg = new DinoBattleViewSetCreaturePriorityResult();
        break;
    case 0x1007F:
        msg = new DinoBattleViewCreaturePlayAttackMotionRequest();
        break;
    case 0x10080:
        msg = new DinoBattleViewCreaturePlayAttackMotionResult();
        break;
    case 0x10081:
        msg = new DinoBattleViewChangeStatusEffectIconRequest();
        break;
    case 0x10082:
        msg = new DinoBattleViewPopNumberRedIfZeroSignal();
        break;
    case 0x10083:
        msg = new DinoBattleViewChangeStatusEffectIconResult();
        break;
    case 0x10084:
        msg = new DinoBattleViewStatusEffectIconBlendClearSignal();
        break;
    case 0x10085:
        msg = new DinoBattleViewTutorialWindowRequest();
        break;
    case 0x10086:
        msg = new DinoBattleViewTutorialWindowResult();
        break;
    case 0x10087:
        msg = new DinoBattleViewTutorialSetArroeRequest();
        break;
    case 0x10088:
        msg = new DinoBattleViewTutorialSetArroeResult();
        break;
    case 0x10089:
        msg = new DinoBattleViewTutorialSetRectRequest();
        break;
    case 0x1008A:
        msg = new DinoBattleViewTutorialSetRectResult();
        break;
    case 0x1008B:
        msg = new DinoBattleView3DGameSetRequest();
        break;
    case 0x1008C:
        msg = new DinoBattleView3DGameSetResult();
        break;
    case 0x1008D:
        msg = new DinoBattleViewLiveMessageSignal();
        break;
    case 0x1008E:
        msg = new DinoBattleViewDecideFirstAttackTeamRequest();
        break;
    case 0x1008F:
        msg = new DinoBattleViewDecideFirstAttackTeamResult();
        break;
    case 0x10090:
        msg = new DinoBattleView3DRevolutionSetRequest();
        break;
    case 0x10091:
        msg = new DinoBattleView3DRevolutionSetResult();
        break;
    case 0x10092:
        msg = new DinoBattleViewPlayerWaitKPCountStopRequest();
        break;
    case 0x10093:
        msg = new DinoBattleViewPlayerWaitKPCountStopResult();
        break;
    case 0x10094:
        msg = new DinoBattleModelTutorialGameEndSignal();
        break;
    case 0x10095:
        msg = new DinoBattleViewSetAttackBarUpperLCDRequest();
        break;
    case 0x10096:
        msg = new DinoBattleViewSetAttackBarUpperLCDResult();
        break;
    case 0x10097:
        msg = new DinoBattleViewCountPlayerThinkTimeSignal();
        break;
    case 0x10098:
        msg = new DinoBattleViewCreatureActionSignal();
        break;
    case 0x10099:
        msg = new DinoBattleViewCreatureActionUpdateSignal();
        break;
    case 0x1009A:
        msg = new DinoBattleViewCreatureUpdateColorRequest();
        break;
    case 0x1009B:
        msg = new DinoBattleViewCreatureUpdateColorResult();
        break;
    case 0x1009D:
        msg = new DinoBattlePlayerInfoSignal();
        break;
    case 0x1009C:
        msg = new DinoBattlePlayerLevelSignal();
        break;
    case 0x1009E:
        msg = new DinoBattleReadyToBeginBattleSignal();
        break;
    case 0x100A4:
        msg = new DinoBattleReadyToContinueBattleSignal();
        break;
    case 0x1009F:
        msg = new DinoBattleModelLiveEventSignal();
        break;
    case 0x100A0:
        msg = new DinoBattleViewCreatureViewInfoMoveRequest();
        break;
    case 0x100A1:
        msg = new DinoBattleViewCreatureViewInfoMoveResult();
        break;
    case 0x100A2:
        msg = new DinoBattleView3DGameSetMotionCancelRequest();
        break;
    case 0x100A3:
        msg = new DinoBattleView3DGameSetMotionCancelResult();
        break;
    case 0x100A5:
        msg = new DinoWirelessCannonKasekiRockSignal();
        break;
    case 0x100A6:
        msg = new DinoWirelessCannonPingSignal();
        break;
    case 0x100A7:
        msg = new DinoWirelessCannonPongSignal();
        break;
    case 0x100A8:
        msg = new DinoExchangeKasekiRockSignal();
        break;
    case 0x100AA:
        msg = new DinoExchangeDoneSignal();
        break;
    case 0x100A9:
        msg = new DinoExchangeSenryuSignal();
        break;
    }
    msg->Read(data);
    return msg;
}
#pragma dont_inline reset

void RegisterDinoMessages() {
    AddMessageFactory(CreateDinoMessage);
}

static u32 sNextSignalWaitSerial;

DinoSignalWait::DinoSignalWait(u32 id, MMessageObserver *target, MMessage *msg, int count) {
    mId = id;
    mCount = count;
    mTarget = target;
    mMsg = msg;
    mSerial = sNextSignalWaitSerial;
    sNextSignalWaitSerial++;
    if (mMsg != NULL) {
        mMsg->AddRef();
    }
}
