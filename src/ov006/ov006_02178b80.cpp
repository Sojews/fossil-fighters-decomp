// Ov00602178b80: the start of DinoUiRvStatusMenu's file.
// Only the creature sorts are decompiled so far.

#pragma ipa file

#include <vector>
#include "game/dino_creature_sort.h"

// The list's sorts (the game emits the whole stable_sort machinery here, the medal menu and the museum use it too).
// NONMATCHING: std::__inplace_merge (stack layout, and the game elides one argument copy), std::__stable_sort_move
// and std::__insertion_sort_move (register allocation), as in the museum; std::get_temporary_buffer (the game loads
// the pair's zero from the TU's pooled .data object at data_ov006_021939d4, which isn't decompiled).
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByUnk0D);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByUnk02031104);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByLevel);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByUnk88);
template void std::stable_sort(std::vector<u32>::iterator, std::vector<u32>::iterator, DinoCreatureByVoice);
