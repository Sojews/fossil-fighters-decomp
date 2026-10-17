// SeedGenerator and MRandom.

#include "m/seed_generator.h"
#include "m/input.h"
#include "m/irq.h"
#include "nitro/os.h"

static SeedGenerator *sSeedGenerator;
static u32 sSeedCount;

static const u32 sMag01[2] = {0, 0x9908B0DF};

SeedGenerator::SeedGenerator() {
    *(u16 *)&mShift = 0;  // (the game does this too)
    OS_InitTick();
    OS_GetLowEntropyData((u32 *)mEntropy);
    mPos = 0;
    mShift = 0;
}

void SeedGenerator::Execute() {
    u16 x = 0;
    x ^= ~MInputHub::GetButtons();
    x ^= MInputHub::GetTouchPos().x;
    x ^= MInputHub::GetTouchPos().y << 8;
    if (mShift != 0) {
        x = (x >> mShift) | (x << (16 - mShift));
    }
    mEntropy[mPos++] ^= x;
    if (mPos >= 0x4E0) {
        mPos = 0;
        mShift = (mShift + 1) & 0xF;
    }
}

void SeedGenerator::Create() {
    sSeedGenerator = new SeedGenerator();
    MAddIntrTask(0, sSeedGenerator);
}

u32 SeedGenerator::GenerateSeed() {
    int i;
    u32 seed;
    const u32 *p;
    p = (const u32 *)sSeedGenerator->mEntropy;
    seed = 0;
    for (i = 0; i < MRandom::N; i++) {
        seed = ((seed << 1) | (seed >> 31)) ^ *p++;
    }
    return seed + sSeedCount++;
}

MRandom::MRandom() {
    const u32 *key = (const u32 *)sSeedGenerator->mEntropy;
    Init(19650218);
    u32 i = 1;
    u32 j = 0;
    for (u32 k = N; k != 0; k--) {
        mState[i] = (mState[i] ^ ((mState[i - 1] ^ (mState[i - 1] >> 30)) * 1664525)) + key[j] + j;
        i++;
        j++;
        if (i >= N) {
            mState[0] = mState[N - 1];
            i = 1;
        }
        if (j >= N) {
            j = 0;
        }
    }
    for (u32 k = N - 1; k != 0; k--) {
        mState[i] = (mState[i] ^ ((mState[i - 1] ^ (mState[i - 1] >> 30)) * 1566083941)) - i;
        i++;
        if (i >= N) {
            mState[0] = mState[N - 1];
            i = 1;
        }
    }
    mState[0] = 0x80000000;
}

MRandom::~MRandom() {
    delete[] mState;
}

void MRandom::Init(u32 seed) {
    mState = new u32[N];
    mState[0] = seed;
    for (mIndex = 1; mIndex < N; mIndex++) {
        mState[mIndex] = 1812433253 * (mState[mIndex - 1] ^ (mState[mIndex - 1] >> 30)) + mIndex;
    }
    mIndex = 0;
}

u32 MRandom::Next() {
    u32 y;
    if (mIndex < N - 397) {
        y = (mState[mIndex] & 0x80000000) | (mState[mIndex + 1] & 0x7FFFFFFF);
        mState[mIndex] = mState[mIndex + 397] ^ (y >> 1) ^ sMag01[y & 1];
    } else if (mIndex < N - 1) {
        y = (mState[mIndex] & 0x80000000) | (mState[mIndex + 1] & 0x7FFFFFFF);
        mState[mIndex] = mState[mIndex + (397 - N)] ^ (y >> 1) ^ sMag01[y & 1];
    } else {
        y = (mState[N - 1] & 0x80000000) | (mState[0] & 0x7FFFFFFF);
        mState[N - 1] = mState[396] ^ (y >> 1) ^ sMag01[y & 1];
    }
    y = mState[mIndex++];
    if (mIndex >= N) {
        mIndex = 0;
    }
    y ^= y >> 11;
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    y ^= y >> 18;
    return y;
}

u32 MRandom::Next(u32 n) {
    return (Next() & 0x7FFFFFFF) % n;
}

MFx32 MRandom::NextFraction() {
    MFx32 r;
    r.v = Next() & 0xFFF;
    return r;
}

MRandomRef::MRandomRef(MRandom *random) {
    mRandom = random != NULL ? random : new MRandom();
    mOwned = random == NULL;
    mCount = 0;
}

MRandomRef::~MRandomRef() {
    if (mOwned) {
        delete mRandom;
    }
}

BOOL MRandomRef::Roll() {
    mCount++;
    if ((u16)mRandom->Next() * mCount < 0x10000) {
        return TRUE;
    }
    return FALSE;
}
