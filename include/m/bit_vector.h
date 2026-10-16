// MBitVector: a serializable array of bits (game flags and the like).
#ifndef M_BIT_VECTOR_H
#define M_BIT_VECTOR_H

#include "m/message.h"

class MBitVector : public MSerializable {
public:
    MBitVector(u32 bits) {
        mBits = bits;
        mBytes = new u8[(bits + 7) / 8];
    }
    virtual ~MBitVector();
    virtual u32 GetSize();
    virtual void Write(void *buf);
    virtual void Read(const void *buf);
    virtual int Unk14();

    u32 mBits;      // 0x04
    u8 *mBytes;     // 0x08
};

#endif
