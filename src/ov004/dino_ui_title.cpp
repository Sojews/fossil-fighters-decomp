// DinoUiTitle, DinoUiTitleSkip, DinoUiOpeningCloud, DinoUiOpeningChara, DinoUiTitleMenuGameItem.
// Only the destructors are decompiled so far.

#include "game/dino_ui_title.h"
#include "game/dino_ui_title_skip.h"
#include "game/dino_ui_opening_cloud.h"
#include "game/dino_ui_opening_chara.h"
#include "game/dino_ui_title_menu_game_item.h"
#include "m/bg.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiTitle::~DinoUiTitle() {
}

DinoUiTitleSkip::~DinoUiTitleSkip() {
}

DinoUiOpeningCloud::~DinoUiOpeningCloud() {
}

DinoUiOpeningChara::~DinoUiOpeningChara() {
}

DinoUiTitleMenuGameItem::~DinoUiTitleMenuGameItem() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiOpening {};
template class MBgCallTask0<DinoUiTitle>;
template MTask *MCreateBgCallTask(DinoUiTitle *obj, void (DinoUiTitle::*func)(), int thread);
template class MCallTask1<MBg, MBg, bg_mode>;
template MTask *MCreateCallTask(MBg *obj, void (MBg::*func)(bg_mode), bg_mode a0);
template class MBgCallTask0<DinoUiOpening>;
template MTask *MCreateBgCallTask(DinoUiOpening *obj, void (DinoUiOpening::*func)(), int thread);
