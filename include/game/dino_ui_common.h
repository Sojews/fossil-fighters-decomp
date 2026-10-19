// Helpers several overlays keep their own copies of (ov003_0215b008.cpp, ov005_02155f94.cpp,
// dino_ui2_parts_rock_name.cpp).
#ifndef GAME_DINO_UI_COMMON_H
#define GAME_DINO_UI_COMMON_H

#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "game/dino_ui_parts_kaseki_button.h"
#include "m/resource.h"

// What the container entry's button shows.
DinoKasekiButtonInfo DinoGetKasekiButtonInfo(const DinoBackupEntry *entry, MResource<DinoKasekiDefs> defs);
DinoKasekiButtonInfo DinoGetKasekiButtonInfo(const DinoBackupEntry *entry);
// Dims the screen behind a menu.
void DinoDimScreen();

#endif
