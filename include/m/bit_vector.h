// MBitVector: a serializable array of bits (game flags and the like).
#ifndef M_BIT_VECTOR_H
#define M_BIT_VECTOR_H

#include "m/message.h"

class MBitVector : public MSerializable {
public:
    MBitVector(u32 bits) {
        mBits = bits;
        mBytes = new u8[(mBits + 7) / 8];
    }
    // Inline in the game (inlined into owners' destructors); src/m/bit_vector.cpp defines MBIT_VECTOR_OWNER and
    // emits the out-of-line copy.
#ifdef MBIT_VECTOR_OWNER
    virtual ~MBitVector();
#else
    virtual ~MBitVector() { delete[] mBytes; }
#endif
    virtual u32 GetSize();
    virtual void Write(void *buf);
    virtual void Read(const void *buf);
    virtual int Unk14();

    // One bit, as something to assign to.
    class Reference {
    public:
        Reference(MBitVector *vector, u32 index) : mVector(vector), mIndex(index) {}
        Reference &operator=(BOOL value) {
            if (value) {
                mVector->mBytes[mIndex >> 3] |= 1 << (mIndex & 7);
            } else {
                mVector->mBytes[mIndex >> 3] &= ~(1 << (mIndex & 7));
            }
            return *this;
        }

        MBitVector *mVector;
        u32 mIndex;
    };

    Reference operator[](u32 index) { return Reference(this, index); }
    void Set(u32 index) { mBytes[index >> 3] |= 1 << (index & 7); }
    BOOL Test(u32 index) { return (mBytes[index >> 3] & (1 << (index & 7))) ? TRUE : FALSE; }

    u32 mBits;      // 0x04
    u8 *mBytes;     // 0x08
};

#endif
