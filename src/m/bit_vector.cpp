// MBitVector. The game has these functions in reverse order among its save data code, as if they were
// inline functions emitted with the class's vtable.

#define MBIT_VECTOR_OWNER
#include "m/bit_vector.h"

MBitVector::~MBitVector() {
    delete[] mBytes;
}

int MBitVector::Unk14() {
    return TRUE;
}

void MBitVector::Read(const void *buf) {
    MI_CpuCopy8(buf, mBytes, (mBits + 7) / 8);
}

void MBitVector::Write(void *buf) {
    MI_CpuCopy8(mBytes, buf, (mBits + 7) / 8);
}

u32 MBitVector::GetSize() {
    return ((mBits + 7) / 8 + 3) & ~3;
}
