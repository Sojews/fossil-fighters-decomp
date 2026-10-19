// ov005's copies of the kaseki button helpers (see ov003_0215b008.cpp and dino_ui2_parts_rock_name.cpp).

#pragma auto_inline off

#include <vector>
#include "game/dino_ui_common.h"
#include "game/dino_ui2_parts_kaseki_button.h"
#include "m/blend.h"
#include "m/display.h"
#include "m/menu.h"

// What the container entry's button shows: a kaseki rock, a fossil (cleaned or not, and whether the creature is
// revived), or one of the two special kinds of rock.
DinoKasekiButtonInfo DinoGetKasekiButtonInfo(const DinoBackupEntry *entry, MResource<DinoKasekiDefs> &defs) {
    DinoKasekiButtonInfo info;
    info.type = 0;
    info.level = 0;
    u32 id = entry->id;
    if (id >= 501 && id <= 530 && entry->active == 1) {
        info.icon = (id - 501) / 3 + 19;
        info.color = 0;
        info.type = 5;
        info.level = entry->score;
    } else if (id >= 601 && id <= 630 && entry->active == 1) {
        info.icon = 29;
        info.color = 0;
    } else {
        info.icon = defs.Get()->GetDef(id)->icon;
        info.color = entry->unk10_0;
        DinoKasekiRockState rock;
        gDinoBackup->GetKasekiRock(entry->id, (u32 *)&rock);
        if (rock.unk10) {
            DinoCreatureState creature;
            gDinoBackup->GetCreature((entry->id - 1) / 4 + 1, &creature);
            if (creature.parts[0].level != 0) {
                info.type = 1;
            } else {
                info.type = 2;
            }
            info.level = rock.score;
        } else {
            info.type = 3;
        }
        if (entry->unk10_0 == 1) {
            info.type = 4;
        }
    }
    return info;
}

DinoKasekiButtonInfo DinoGetKasekiButtonInfo(const DinoBackupEntry *entry) {
    MResource<DinoKasekiDefs> defs = gDinoResource->LoadKasekiDefs();
    return DinoGetKasekiButtonInfo(entry, defs);
}

DinoKasekiButtonInfo DinoUi2GetEmptyKasekiInfo() {
    DinoKasekiButtonInfo info;
    info.icon = 1;
    info.color = 0;
    info.type = 0;
    info.level = 0;
    return info;
}

// Dims the screen behind a menu.
void DinoDimScreen() {
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->Reset();
    blend->SetMode(3);
    blend->SetBrightness(8);
    blend->SetAlpha(16, 0);
    blend->SetTarget1(0x10);
    blend->SetTarget2(0x3F);
}

// NONMATCHING: the stack slots of the items' begin temporaries (the game has one fewer).
void DinoUi2LinkKasekiGrid(MMenu *menu, std::vector<MMenuItem *> &items, MMenuItem *below) {
    for (int i = 0; i < items.size(); i++) {
        if (i + 4 < items.size()) {
            menu->AddLink(items[i], 2, items[i + 4]);
        } else {
            menu->AddLink(items[i], 2, below);
        }
        int col = i % 4;
        if (col != 0) {
            menu->AddLink(items[i], 3, items[i - 1]);
        } else if (i + 4 < items.size()) {
            menu->AddLink(items[i], 3, items[i + 3]);
        } else {
            menu->AddLink(items[i], 3, *(items.end() - 1));
        }
        if (col == 3 || i == items.size() - 1) {
            menu->AddLink(items[i], 1, items[i / 4 * 4]);
        } else {
            menu->AddLink(items[i], 1, items[i + 1]);
        }
    }
}
