// Message types used by the battle system and wireless play, and the factory that creates them from received data.

#include "game/dino_message.h"

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

DinoDummyMessage::DinoDummyMessage() : MMessage(0x10001) {
}

DinoBattleModelGameBeginSignal::DinoBattleModelGameBeginSignal() : MMessage(0x10002) {
}

DinoBattleControllerInitRequest::DinoBattleControllerInitRequest() : MMessage(0x10003) {
}

DinoBattleControllerInitResult::DinoBattleControllerInitResult() : MMessage(0x10004) {
}

DinoBattleModelPlayerBeginSignal::DinoBattleModelPlayerBeginSignal() : MMessage(0x10005) {
}

DinoBattleModelCreatureBeginSignal::DinoBattleModelCreatureBeginSignal() : MMessage(0x10006) {
}

DinoBattleViewCreatureBeginSignal::DinoBattleViewCreatureBeginSignal() : MMessage(0x10007) {
}

DinoBattleViewPlayerBeginSignal::DinoBattleViewPlayerBeginSignal() : MMessage(0x10008) {
}

DinoBattleViewEffectGameBeginRequest::DinoBattleViewEffectGameBeginRequest() : MMessage(0x10009) {
}

DinoBattleViewEffectGameBeginResult::DinoBattleViewEffectGameBeginResult() : MMessage(0x1000A) {
}

DinoBattleViewEffectTurnBeginRequest::DinoBattleViewEffectTurnBeginRequest() : MMessage(0x1000B) {
}

DinoBattleViewEffectTurnBeginResult::DinoBattleViewEffectTurnBeginResult() : MMessage(0x1000C) {
}

DinoBattleControllerWakeupSignal::DinoBattleControllerWakeupSignal() : MMessage(0x1000D) {
}

DinoBattleModelPlayerTurnBeginRequst::DinoBattleModelPlayerTurnBeginRequst() : MMessage(0x1000E) {
}

DinoBattleModelPlayerTurnBeginResult::DinoBattleModelPlayerTurnBeginResult() : MMessage(0x1000F) {
}

DinoBattleViewUiRootMenuRequest::DinoBattleViewUiRootMenuRequest() : MMessage(0x10010) {
}

DinoBattleViewUiRootMenuResult::DinoBattleViewUiRootMenuResult() : MMessage(0x10011) {
}

DinoBattleModelCreatureMoveRequest::DinoBattleModelCreatureMoveRequest() : MMessage(0x10012) {
}

DinoBattleModelCreatureMoveResult::DinoBattleModelCreatureMoveResult() : MMessage(0x10013) {
}

DinoBattleModelTurnChangeSignal::DinoBattleModelTurnChangeSignal() : MMessage(0x10014) {
}

DinoBattleModelTurnEndRequest::DinoBattleModelTurnEndRequest() : MMessage(0x10015) {
}

DinoBattleModelTurnEndResult::DinoBattleModelTurnEndResult() : MMessage(0x10016) {
}

DinoBattleViewUiChangeMenuRequest::DinoBattleViewUiChangeMenuRequest() : MMessage(0x1001B) {
}

DinoBattleViewUiChangeMenuResult::DinoBattleViewUiChangeMenuResult() : MMessage(0x1001C) {
}

DinoBattleViewUiCreatureInfoMenuRequest::DinoBattleViewUiCreatureInfoMenuRequest() : MMessage(0x1001D) {
}

DinoBattleViewUiCreatureInfoMenuResult::DinoBattleViewUiCreatureInfoMenuResult() : MMessage(0x1001E) {
}

DinoBattleModelGoEscapeAreaRequest::DinoBattleModelGoEscapeAreaRequest() : MMessage(0x1001F) {
}

DinoBattleModelGoEscapeAreaResult::DinoBattleModelGoEscapeAreaResult() : MMessage(0x10020) {
}

DinoBattleViewCreatureMoveRequest::DinoBattleViewCreatureMoveRequest() : MMessage(0x10021) {
}

DinoBattleViewCreatureMoveResult::DinoBattleViewCreatureMoveResult() : MMessage(0x10022) {
}

DinoBattleViewUiAttackerMenuRequest::DinoBattleViewUiAttackerMenuRequest() : MMessage(0x10017) {
}

DinoBattleViewUiAttackerMenuResult::DinoBattleViewUiAttackerMenuResult() : MMessage(0x10018) {
}

DinoBattleViewUiDefenderMenuRequest::DinoBattleViewUiDefenderMenuRequest() : MMessage(0x10019) {
}

DinoBattleViewUiDefenderMenuResult::DinoBattleViewUiDefenderMenuResult() : MMessage(0x1001A) {
}

DinoBattleViewUiAbilityMenuRequest::DinoBattleViewUiAbilityMenuRequest() : MMessage(0x10023) {
}

DinoBattleViewUiAbilityMenuResult::DinoBattleViewUiAbilityMenuResult() : MMessage(0x10024) {
}

DinoBattleModelCreatureAbilityRequest::DinoBattleModelCreatureAbilityRequest() : MMessage(0x10025) {
}

DinoBattleModelCreatureAbilityResult::DinoBattleModelCreatureAbilityResult() : MMessage(0x10026) {
}

DinoBattleViewSetActionedSignal::DinoBattleViewSetActionedSignal() : MMessage(0x10027) {
}

DinoBattleViewCreatureCpuArrowUpdateSignal::DinoBattleViewCreatureCpuArrowUpdateSignal() : MMessage(0x10028) {
}

DinoBattleViewCreatureAbilityRequest::DinoBattleViewCreatureAbilityRequest() : MMessage(0x10029) {
}

DinoBattleViewCreatureAbilityResult::DinoBattleViewCreatureAbilityResult() : MMessage(0x1002A) {
}

DinoBattleView3DTeamAttackRequest::DinoBattleView3DTeamAttackRequest() : MMessage(0x1002D) {
}

DinoBattleView3DTeamAttackResult::DinoBattleView3DTeamAttackResult() : MMessage(0x1002E) {
}

DinoBattleViewCreatureUpdateRequest::DinoBattleViewCreatureUpdateRequest() : MMessage(0x1002F) {
}

DinoBattleViewCreatureAbility3DRequest::DinoBattleViewCreatureAbility3DRequest() : MMessage(0x1002B) {
}

DinoBattleViewCreatureAbility3DResult::DinoBattleViewCreatureAbility3DResult() : MMessage(0x1002C) {
}

DinoBattleViewCreatureUpdateResult::DinoBattleViewCreatureUpdateResult() : MMessage(0x10030) {
}

DinoBattleModelCreatureEndRequest::DinoBattleModelCreatureEndRequest() : MMessage(0x10031) {
}

DinoBattleModelCreatureEndResult::DinoBattleModelCreatureEndResult() : MMessage(0x10032) {
}

DinoBattleViewCreatureEndRequest::DinoBattleViewCreatureEndRequest() : MMessage(0x10033) {
}

DinoBattleViewCreatureEndResult::DinoBattleViewCreatureEndResult() : MMessage(0x10034) {
}

DinoBattleModelGameEndSignal::DinoBattleModelGameEndSignal() : MMessage(0x10035) {
}

DinoBattleViewEffectGameEndRequest::DinoBattleViewEffectGameEndRequest() : MMessage(0x10036) {
}

DinoBattleViewEffectGameEndResult::DinoBattleViewEffectGameEndResult() : MMessage(0x10037) {
}

DinoBattleViewPlayerUpdateRequest::DinoBattleViewPlayerUpdateRequest() : MMessage(0x10038) {
}

DinoBattleViewPlayerUpdateResult::DinoBattleViewPlayerUpdateResult() : MMessage(0x10039) {
}

DinoBattleModelCreatureTurnBeginRequest::DinoBattleModelCreatureTurnBeginRequest() : MMessage(0x1003A) {
}

DinoBattleModelCreatureTurnBeginResult::DinoBattleModelCreatureTurnBeginResult() : MMessage(0x1003B) {
}

DinoBattleModelPlayerTurnBeginProcessRequst::DinoBattleModelPlayerTurnBeginProcessRequst() : MMessage(0x1003C) {
}

DinoBattleModelPlayerTurnBeginProcessResult::DinoBattleModelPlayerTurnBeginProcessResult() : MMessage(0x1003D) {
}

DinoBattleModelCreatureSpecialLifeRecoveryRequest::DinoBattleModelCreatureSpecialLifeRecoveryRequest() : MMessage(0x1003E) {
}

DinoBattleModelCreatureSpecialLifeRecoveryResult::DinoBattleModelCreatureSpecialLifeRecoveryResult() : MMessage(0x1003F) {
}

DinoBattleModelEvolutionCreatureRequest::DinoBattleModelEvolutionCreatureRequest() : MMessage(0x10040) {
}

DinoBattleModelEvolutionCreatureResult::DinoBattleModelEvolutionCreatureResult() : MMessage(0x10041) {
}

DinoBattleViewEvolutionCreatureRequest::DinoBattleViewEvolutionCreatureRequest() : MMessage(0x10042) {
}

DinoBattleViewEvolutionCreatureResult::DinoBattleViewEvolutionCreatureResult() : MMessage(0x10043) {
}

DinoBattleViewCreatureCounterDamageRequest::DinoBattleViewCreatureCounterDamageRequest() : MMessage(0x10044) {
}

DinoBattleViewCreatureCounterDamageResult::DinoBattleViewCreatureCounterDamageResult() : MMessage(0x10045) {
}

DinoBattleViewPlayerUpdateStateIconRequest::DinoBattleViewPlayerUpdateStateIconRequest() : MMessage(0x10046) {
}

DinoBattleViewPlayerUpdateStateIconResult::DinoBattleViewPlayerUpdateStateIconResult() : MMessage(0x10047) {
}

DinoBattleViewDeleteStatusEffectSignal::DinoBattleViewDeleteStatusEffectSignal() : MMessage(0x10048) {
}

DinoBattleViewPlayerUpdateAbilityIconRequest::DinoBattleViewPlayerUpdateAbilityIconRequest() : MMessage(0x10049) {
}

DinoBattleViewPlayerUpdateAbilityIconResult::DinoBattleViewPlayerUpdateAbilityIconResult() : MMessage(0x1004A) {
}

DinoBattleViewUiTurnEndRequest::DinoBattleViewUiTurnEndRequest() : MMessage(0x1004B) {
}

DinoBattleViewUiTurnEndResult::DinoBattleViewUiTurnEndResult() : MMessage(0x1004C) {
}

DinoBattleModelPlayerConfusionExecRequest::DinoBattleModelPlayerConfusionExecRequest() : MMessage(0x1004D) {
}

DinoBattleModelPlayerConfusionExecResult::DinoBattleModelPlayerConfusionExecResult() : MMessage(0x1004E) {
}

DinoBattleModelCreatureSupportEffectRequest::DinoBattleModelCreatureSupportEffectRequest() : MMessage(0x1004F) {
}

DinoBattleModelCreatureSupportEffectResult::DinoBattleModelCreatureSupportEffectResult() : MMessage(0x10050) {
}

DinoBattleViewCreatureSupportEffectRequest::DinoBattleViewCreatureSupportEffectRequest() : MMessage(0x10051) {
}

DinoBattleViewCreatureSupportEffectResult::DinoBattleViewCreatureSupportEffectResult() : MMessage(0x10052) {
}

DinoBattleViewCreatureMotionEffectRequest::DinoBattleViewCreatureMotionEffectRequest() : MMessage(0x10053) {
}

DinoBattleViewCreatureMotionEffectResult::DinoBattleViewCreatureMotionEffectResult() : MMessage(0x10054) {
}

DinoBattleViewFieldEffectWaitRequest::DinoBattleViewFieldEffectWaitRequest() : MMessage(0x10055) {
}

DinoBattleViewFieldEffectWaitResult::DinoBattleViewFieldEffectWaitResult() : MMessage(0x10056) {
}

DinoBattleViewCreaturePlayMotionRequest::DinoBattleViewCreaturePlayMotionRequest() : MMessage(0x10057) {
}

DinoBattleViewCreaturePlayMotionResult::DinoBattleViewCreaturePlayMotionResult() : MMessage(0x10058) {
}

DinoBattleViewFreeCoordEffectRequest::DinoBattleViewFreeCoordEffectRequest() : MMessage(0x10059) {
}

DinoBattleViewFreeCoordEffectResult::DinoBattleViewFreeCoordEffectResult() : MMessage(0x1005A) {
}

DinoBattleModelCreatureReleaseStatusEffectRequest::DinoBattleModelCreatureReleaseStatusEffectRequest() : MMessage(0x1005B) {
}

DinoBattleModelCreatureReleaseStatusEffectResult::DinoBattleModelCreatureReleaseStatusEffectResult() : MMessage(0x1005C) {
}

DinoBattleViewCreatureTurnBeginSignal::DinoBattleViewCreatureTurnBeginSignal() : MMessage(0x1005D) {
}

DinoBattleViewCreatureTurnEndSignal::DinoBattleViewCreatureTurnEndSignal() : MMessage(0x1005E) {
}

DinoBattleViewCreatureDamagedUpdateRequest::DinoBattleViewCreatureDamagedUpdateRequest() : MMessage(0x1005F) {
}

DinoBattleViewCreatureDamagedUpdateResult::DinoBattleViewCreatureDamagedUpdateResult() : MMessage(0x10060) {
}

DinoBattleViewWaitUpdateSupportEffectAnimeRequest::DinoBattleViewWaitUpdateSupportEffectAnimeRequest() : MMessage(0x10061) {
}

DinoBattleViewWaitUpdateSupportEffectAnimeResult::DinoBattleViewWaitUpdateSupportEffectAnimeResult() : MMessage(0x10062) {
}

DinoBattleViewActionInfoBarUpdateRequest::DinoBattleViewActionInfoBarUpdateRequest() : MMessage(0x10063) {
}

DinoBattleViewActionInfoBarUpdateResult::DinoBattleViewActionInfoBarUpdateResult() : MMessage(0x10064) {
}

DinoBattleViewFocusLCDFrameRequest::DinoBattleViewFocusLCDFrameRequest() : MMessage(0x10065) {
}

DinoBattleViewFocusLCDFrameResult::DinoBattleViewFocusLCDFrameResult() : MMessage(0x10066) {
}

DinoBattleViewCreatureSetBlendRequest::DinoBattleViewCreatureSetBlendRequest() : MMessage(0x10067) {
}

DinoBattleViewCreatureSetBlendResult::DinoBattleViewCreatureSetBlendResult() : MMessage(0x10068) {
}

DinoBattleViewLCDFadeRequest::DinoBattleViewLCDFadeRequest() : MMessage(0x10069) {
}

DinoBattleViewLCDFadeResult::DinoBattleViewLCDFadeResult() : MMessage(0x1006A) {
}

DinoBattleViewWaitActionInfoBarRequest::DinoBattleViewWaitActionInfoBarRequest() : MMessage(0x1006B) {
}

DinoBattleViewWaitActionInfoBarResult::DinoBattleViewWaitActionInfoBarResult() : MMessage(0x1006C) {
}

DinoBattleViewClearAttackBarRequest::DinoBattleViewClearAttackBarRequest() : MMessage(0x1006D) {
}

DinoBattleViewClearAttackBarResult::DinoBattleViewClearAttackBarResult() : MMessage(0x1006E) {
}

DinoBattleViewShowAttackNameBarRequest::DinoBattleViewShowAttackNameBarRequest() : MMessage(0x1006F) {
}

DinoBattleViewMoveAttackNameBarRequest::DinoBattleViewMoveAttackNameBarRequest() : MMessage(0x10070) {
}

DinoBattleViewMoveAttackNameBarResult::DinoBattleViewMoveAttackNameBarResult() : MMessage(0x10071) {
}

DinoBattleViewShowAttackNameBarResult::DinoBattleViewShowAttackNameBarResult() : MMessage(0x10072) {
}

DinoBattleView3DSupportEffectRequest::DinoBattleView3DSupportEffectRequest() : MMessage(0x10073) {
}

DinoBattleView3DSupportEffectResult::DinoBattleView3DSupportEffectResult() : MMessage(0x10074) {
}

DinoBattleView3DWatchStatusRequest::DinoBattleView3DWatchStatusRequest() : MMessage(0x10075) {
}

DinoBattleView3DWatchStatusResult::DinoBattleView3DWatchStatusResult() : MMessage(0x10076) {
}

DinoBattleView3DPoisonDamageRequest::DinoBattleView3DPoisonDamageRequest() : MMessage(0x10077) {
}

DinoBattleView3DPoisonDamageResult::DinoBattleView3DPoisonDamageResult() : MMessage(0x10078) {
}

DinoBattleView3DPreTurnEndRequest::DinoBattleView3DPreTurnEndRequest() : MMessage(0x10079) {
}

DinoBattleView3DPreTurnEndResult::DinoBattleView3DPreTurnEndResult() : MMessage(0x1007A) {
}

DinoBattleView3DTurnEndRequest::DinoBattleView3DTurnEndRequest() : MMessage(0x1007B) {
}

DinoBattleView3DTurnEndResult::DinoBattleView3DTurnEndResult() : MMessage(0x1007C) {
}

DinoBattleViewSetCreaturePriorityRequest::DinoBattleViewSetCreaturePriorityRequest() : MMessage(0x1007D) {
}

DinoBattleViewSetCreaturePriorityResult::DinoBattleViewSetCreaturePriorityResult() : MMessage(0x1007E) {
}

DinoBattleViewCreaturePlayAttackMotionRequest::DinoBattleViewCreaturePlayAttackMotionRequest() : MMessage(0x1007F) {
}

DinoBattleViewCreaturePlayAttackMotionResult::DinoBattleViewCreaturePlayAttackMotionResult() : MMessage(0x10080) {
}

DinoBattleViewChangeStatusEffectIconRequest::DinoBattleViewChangeStatusEffectIconRequest() : MMessage(0x10081) {
}

DinoBattleViewPopNumberRedIfZeroSignal::DinoBattleViewPopNumberRedIfZeroSignal() : MMessage(0x10082) {
}

DinoBattleViewChangeStatusEffectIconResult::DinoBattleViewChangeStatusEffectIconResult() : MMessage(0x10083) {
}

DinoBattleViewStatusEffectIconBlendClearSignal::DinoBattleViewStatusEffectIconBlendClearSignal() : MMessage(0x10084) {
}

DinoBattleViewTutorialWindowRequest::DinoBattleViewTutorialWindowRequest() : MMessage(0x10085) {
}

DinoBattleViewTutorialWindowResult::DinoBattleViewTutorialWindowResult() : MMessage(0x10086) {
}

DinoBattleViewTutorialSetArroeRequest::DinoBattleViewTutorialSetArroeRequest() : MMessage(0x10087) {
}

DinoBattleViewTutorialSetArroeResult::DinoBattleViewTutorialSetArroeResult() : MMessage(0x10088) {
}

DinoBattleViewTutorialSetRectRequest::DinoBattleViewTutorialSetRectRequest() : MMessage(0x10089) {
}

DinoBattleViewTutorialSetRectResult::DinoBattleViewTutorialSetRectResult() : MMessage(0x1008A) {
}

DinoBattleView3DGameSetRequest::DinoBattleView3DGameSetRequest() : MMessage(0x1008B) {
}

DinoBattleView3DGameSetResult::DinoBattleView3DGameSetResult() : MMessage(0x1008C) {
}

DinoBattleViewLiveMessageSignal::DinoBattleViewLiveMessageSignal() : MMessage(0x1008D) {
}

DinoBattleViewDecideFirstAttackTeamRequest::DinoBattleViewDecideFirstAttackTeamRequest() : MMessage(0x1008E) {
}

DinoBattleViewDecideFirstAttackTeamResult::DinoBattleViewDecideFirstAttackTeamResult() : MMessage(0x1008F) {
}

DinoBattleView3DRevolutionSetRequest::DinoBattleView3DRevolutionSetRequest() : MMessage(0x10090) {
}

DinoBattleView3DRevolutionSetResult::DinoBattleView3DRevolutionSetResult() : MMessage(0x10091) {
}

DinoBattleViewPlayerWaitKPCountStopRequest::DinoBattleViewPlayerWaitKPCountStopRequest() : MMessage(0x10092) {
}

DinoBattleViewPlayerWaitKPCountStopResult::DinoBattleViewPlayerWaitKPCountStopResult() : MMessage(0x10093) {
}

DinoBattleModelTutorialGameEndSignal::DinoBattleModelTutorialGameEndSignal() : MMessage(0x10094) {
}

DinoBattleViewSetAttackBarUpperLCDRequest::DinoBattleViewSetAttackBarUpperLCDRequest() : MMessage(0x10095) {
}

DinoBattleViewSetAttackBarUpperLCDResult::DinoBattleViewSetAttackBarUpperLCDResult() : MMessage(0x10096) {
}

DinoBattleViewCountPlayerThinkTimeSignal::DinoBattleViewCountPlayerThinkTimeSignal() : MMessage(0x10097) {
}

DinoBattleViewCreatureActionSignal::DinoBattleViewCreatureActionSignal() : MMessage(0x10098) {
}

DinoBattleViewCreatureActionUpdateSignal::DinoBattleViewCreatureActionUpdateSignal() : MMessage(0x10099) {
}

DinoBattleViewCreatureUpdateColorRequest::DinoBattleViewCreatureUpdateColorRequest() : MMessage(0x1009A) {
}

DinoBattleViewCreatureUpdateColorResult::DinoBattleViewCreatureUpdateColorResult() : MMessage(0x1009B) {
}

DinoBattlePlayerInfoSignal::DinoBattlePlayerInfoSignal() : MMessage(0x1009D) {
}

DinoBattlePlayerLevelSignal::DinoBattlePlayerLevelSignal() : MMessage(0x1009C) {
}

DinoBattleReadyToBeginBattleSignal::DinoBattleReadyToBeginBattleSignal() : MMessage(0x1009E) {
}

DinoBattleReadyToContinueBattleSignal::DinoBattleReadyToContinueBattleSignal() : MMessage(0x100A4) {
}

DinoBattleModelLiveEventSignal::DinoBattleModelLiveEventSignal() : MMessage(0x1009F) {
}

DinoBattleViewCreatureViewInfoMoveRequest::DinoBattleViewCreatureViewInfoMoveRequest() : MMessage(0x100A0) {
}

DinoBattleViewCreatureViewInfoMoveResult::DinoBattleViewCreatureViewInfoMoveResult() : MMessage(0x100A1) {
}

DinoBattleView3DGameSetMotionCancelRequest::DinoBattleView3DGameSetMotionCancelRequest() : MMessage(0x100A2) {
}

DinoBattleView3DGameSetMotionCancelResult::DinoBattleView3DGameSetMotionCancelResult() : MMessage(0x100A3) {
}

DinoWirelessCannonKasekiRockSignal::DinoWirelessCannonKasekiRockSignal() : MMessage(0x100A5) {
}

DinoWirelessCannonPingSignal::DinoWirelessCannonPingSignal() : MMessage(0x100A6) {
}

DinoWirelessCannonPongSignal::DinoWirelessCannonPongSignal() : MMessage(0x100A7) {
}

DinoExchangeKasekiRockSignal::DinoExchangeKasekiRockSignal() : MMessage(0x100A8) {
}

DinoExchangeDoneSignal::DinoExchangeDoneSignal() : MMessage(0x100AA) {
}

DinoExchangeSenryuSignal::DinoExchangeSenryuSignal() : MMessage(0x100A9) {
}

void RegisterDinoMessages() {
    AddMessageFactory(CreateDinoMessage);
}
