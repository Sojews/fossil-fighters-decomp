// Picks an index at random, each with its own weight.
#ifndef GAME_DINO_WEIGHTED_RANDOM_H
#define GAME_DINO_WEIGHTED_RANDOM_H

#include <vector>
#include "types.h"

class MRandom;

class DinoWeightedRandom {
public:
    DinoWeightedRandom(MRandom *random);
    void Add(int weight);
    u32 Pick();

    MRandom *mRandom;               // 0x00
    int mTotal;                     // 0x04: the sum of the weights
    std::vector<void *> mBounds;    // 0x08: each index's running total
};

#endif
