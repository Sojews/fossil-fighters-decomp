// SetEpisodeTask, DinoEventFieldMenu, DinoEventMedalList, DinoEventDebugMenu, DinoEventSaveOption, DinoEventSave, DinoEventATM, DinoEventNameEntry, DinoEventSenryuEntry, DinoEventSenryuListEntry, DinoEventShopBuy, DinoEventShopSell, DinoEventShopMask, DinoEventMaskEquip, DinoEventLicenseCard, DinoEventEnding, DinoEventMuseum, DinoEventReviveEffectCaller.
// Only the destructors are decompiled so far.

#include "game/set_episode_task.h"
#include "game/dino_event_field_menu.h"
#include "game/dino_event_medal_list.h"
#include "game/dino_event_debug_menu.h"
#include "game/dino_event_save_option.h"
#include "game/dino_event_save.h"
#include "game/dino_event_atm.h"
#include "game/dino_event_name_entry.h"
#include "game/dino_event_senryu_entry.h"
#include "game/dino_event_senryu_list_entry.h"
#include "game/dino_event_shop_buy.h"
#include "game/dino_event_canon_putin.h"
#include "game/dino_event_canon_takeout.h"
#include "game/dino_backup.h"
#include "game/dino_event_shop_sell.h"
#include "game/dino_event_shop_mask.h"
#include "game/dino_event_mask_equip.h"
#include "game/dino_event_license_card.h"
#include "game/dino_event_ending.h"
#include "game/dino_event_museum.h"
#include "game/dino_event_revive_effect_caller.h"
#include "game/dino_event_capture_fade_ui_task.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

SetEpisodeTask::~SetEpisodeTask() {
}

DinoEventFieldMenu::~DinoEventFieldMenu() {
}

DinoEventMedalList::~DinoEventMedalList() {
}

DinoEventDebugMenu::~DinoEventDebugMenu() {
}

DinoEventSaveOption::~DinoEventSaveOption() {
}

DinoEventSave::~DinoEventSave() {
}

DinoEventATM::~DinoEventATM() {
}

DinoEventNameEntry::~DinoEventNameEntry() {
}

DinoEventSenryuEntry::~DinoEventSenryuEntry() {
}

DinoEventSenryuListEntry::~DinoEventSenryuListEntry() {
}

DinoEventShopBuy::~DinoEventShopBuy() {
}

DinoEventShopSell::~DinoEventShopSell() {
}

DinoEventShopMask::~DinoEventShopMask() {
}

DinoEventMaskEquip::~DinoEventMaskEquip() {
}

DinoEventLicenseCard::~DinoEventLicenseCard() {
}

DinoEventEnding::~DinoEventEnding() {
}

DinoEventMuseum::~DinoEventMuseum() {
}

DinoEventReviveEffectCaller::~DinoEventReviveEffectCaller() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoEventLoop {};
template class MBgCallTask0<DinoEventSaveOption>;
template MTask *MCreateBgCallTask(DinoEventSaveOption *obj, void (DinoEventSaveOption::*func)(), int thread);
template class MBgCallTask1<DinoEventLoop, u32>;
template MTask *MCreateBgCallTask(DinoEventLoop *obj, void (DinoEventLoop::*func)(u32), u32 a0, int thread);
template class MBgCallTask0<DinoEventCaptureFadeUiTask>;
template MTask *MCreateBgCallTask(DinoEventCaptureFadeUiTask *obj, void (DinoEventCaptureFadeUiTask::*func)(), int thread);

DinoEventCanonTakeout::~DinoEventCanonTakeout() {
    gDinoBackup->SetSystemFlag(0x26, mCount != gDinoBackup->CountUsedUnk14C());
}

DinoEventCanonPutin::~DinoEventCanonPutin() {
    gDinoBackup->SetSystemFlag(0x25, mCount != gDinoBackup->CountUsedUnk14C());
}
