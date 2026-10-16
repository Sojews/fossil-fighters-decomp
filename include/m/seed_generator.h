// Random numbers: SeedGenerator collects entropy (the low entropy data, then the buttons and touch screen every
// frame), and MRandom is a Mersenne Twister (MT19937) seeded from it.
#ifndef M_SEED_GENERATOR_H
#define M_SEED_GENERATOR_H

#include "m/task.h"
#include "m/fx32.h"

class SeedGenerator : public MTask {
public:
    SeedGenerator();
    virtual ~SeedGenerator() {}
    virtual void Execute();

    static void Create();
    // Folds the entropy into a word, made different on each call.
    static u32 GenerateSeed();

    u16 mEntropy[0x4E0];    // 0x10: 624 words
    int mShift;             // 0x9D0: rotation of the next inputs
    int mPos;               // 0x9D4
};

class MRandom {
public:
    enum { N = 624 };

    MRandom();
    ~MRandom();
    void Init(u32 seed);
    u32 Next();
    u32 Next(u32 n);
    MFx32 NextFraction();  // in [0, 1)

    u32 *mState;    // 0x00
    u16 mIndex;     // 0x04
};

// Shares an MRandom (or makes its own).
class MRandomRef {
public:
    MRandomRef(MRandom *random);
    ~MRandomRef();
    // TRUE with a probability that falls with every call.
    BOOL Roll();

    MRandom *mRandom;   // 0x00
    u8 mOwned;          // 0x04
    u32 mCount;         // 0x08
};

#endif
