// DinoUiWirelessCancelItem.
// Only the destructors are decompiled so far.

#include "game/dino_ui_wireless_cancel_item.h"
#include "game/dino_ui_child_scanner_cancel_item.h"
#include "game/dino_ui_child_scanner_start_battle_parent_item.h"
#include "game/dino_ui_child_scanner_start_cannon_parent_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/observer.h"

DinoUiWirelessCancelItem::~DinoUiWirelessCancelItem() {
}

DinoUiChildScannerCancelItem::~DinoUiChildScannerCancelItem() {
}

DinoUiChildScannerStartBattleParentItem::~DinoUiChildScannerStartBattleParentItem() {
}

DinoUiChildScannerStartCannonParentItem::~DinoUiChildScannerStartCannonParentItem() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiBattleParentScanner : public MMessageObserver {};
template class MCallTask0<DinoUiBattleParentScanner, MMessageObserver>;
template MTask *MCreateCallTask(DinoUiBattleParentScanner *obj, void (MMessageObserver::*func)());
