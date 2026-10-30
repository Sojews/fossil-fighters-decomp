// Ov00602178b80: the start of DinoUiRvStatusMenu's file (its constructors and the creature sorts).

#pragma ipa file

#include <vector>
#include "game/dino_creature_sort.h"
#include "game/dino_ui_rv_status_menu_item_menu.h"
#include "m/lcd.h"

// The sorts' buffer: the pair's zero is the long at +0xC of the file's pooled .data object (the rest of it isn't
// decompiled), so the instance is spelled out here with that zero.
extern "C" struct {
    u8 unk00[0xC];
    long zero;  // 0x0C
} data_ov006_021939d4;
namespace std {
template <> pair<u32 *, ptrdiff_t> get_temporary_buffer<u32>(ptrdiff_t len) {
    pair<u32 *, ptrdiff_t> result(0, data_ov006_021939d4.zero);
    while (len > 0) {
        result.first = (u32 *)::operator new[](len * sizeof(u32), nothrow);
        if (result.first != 0) {
            result.second = len;
            return result;
        }
        len /= 2;
    }
    return result;
}
} // namespace std

DinoUiRvStatusMenu::DinoUiRvStatusMenu(int page, const DinoTeam *team)
    : MMenu(gLcd->mMain, NULL, 0), mResult(NULL), mPage(page), mLastPage(0), mTab(0), mUnk109(0xFF), mUnk10A(0) {
    mCreatures.clear();
    int count = 0;
    for (int i = 0; i < 5; i++) {
        if (i == mPage) {
            mPage = count;
        }
        u8 slot = team->slots[i];
        if (slot != 0) {
            u32 id = slot;
            mCreatures.push_back(id);
            count++;
        }
    }
    mLastPage = count - 1;
}

DinoUiRvStatusMenu::DinoUiRvStatusMenu(int *page)
    : MMenu(gLcd->mMain, NULL, 0), mResult(page), mPage(*page), mLastPage(0), mTab(0), mUnk109(0xFF), mUnk10A(0) {
    DinoTeam team;
    gDinoBackup->GetTeam(gDinoBackup->GetUnk34_26(), &team);
    mCreatures.clear();
    int count = 0;
    for (int i = 0; i < 5; i++) {
        if (i == mPage) {
            mPage = count;
        }
        u8 slot = team.slots[i];
        if (slot != 0) {
            u32 id = slot;
            mCreatures.push_back(id);
            count++;
        }
    }
    mLastPage = count - 1;
}

DinoUiRvStatusMenu::DinoUiRvStatusMenu(int *page, u8 sort)
    : MMenu(gLcd->mMain, NULL, 0), mResult(page), mPage(*page), mLastPage(0), mTab(0), mUnk109(0xFF), mUnk10A(1) {
    mCreatures.clear();
    for (u32 id = 1; id <= gDinoResource->GetCreatureCount() - 1; id++) {
        DinoCreatureState creature;
        gDinoBackup->GetCreature(id, &creature);
        if (creature.parts[0].level != 0 ||
            creature.parts[1].level + creature.parts[2].level + creature.parts[3].level != 0) {
            u32 index = creature.index;
            mCreatures.push_back(index);
        }
    }
    mLastPage = mCreatures.size() - 1;
    DinoCreatureByVoice byVoice;
    std::stable_sort(mCreatures.begin(), mCreatures.end(), byVoice);
    switch (sort) {
    case 1: {
        DinoCreatureByUnk88 byUnk88;
        std::stable_sort(mCreatures.begin(), mCreatures.end(), byUnk88);
        break;
    }
    case 2: {
        DinoCreatureByLevel byLevel;
        std::stable_sort(mCreatures.begin(), mCreatures.end(), byLevel);
        break;
    }
    case 3: {
        DinoCreatureByUnk02031104 byUnk02031104;
        std::stable_sort(mCreatures.begin(), mCreatures.end(), byUnk02031104);
        break;
    }
    case 4: {
        DinoCreatureByUnk0D byUnk0D;
        std::stable_sort(mCreatures.begin(), mCreatures.end(), byUnk0D);
        break;
    }
    }
}

// The list's sorts (the game emits the whole stable_sort machinery here, the medal menu and the museum use it too).
// NONMATCHING: std::__inplace_merge (stack layout, and the game elides one argument copy), std::__stable_sort_move
// and std::__insertion_sort_move (register allocation), as in the museum.
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByUnk0D);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByUnk02031104);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByLevel);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByUnk88);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByVoice);
