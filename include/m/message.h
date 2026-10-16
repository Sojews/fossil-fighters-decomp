// Messages: serializable objects that processes send each other, locally or over wireless.
#ifndef M_MESSAGE_H
#define M_MESSAGE_H

#include "types.h"
#include "nitro/mi.h"

// An object that can be written to and read from a byte buffer.
class MSerializable {
public:
    virtual ~MSerializable() {}
    virtual u32 GetSize() = 0;              // bytes Write() writes
    virtual void Write(void *buf) = 0;
    virtual void Read(const void *buf) = 0;
    virtual int Unk14() = 0;
};

// A message. Its serialized form starts with its type ID.
class MMessage : public MSerializable {
public:
    MMessage(u32 id) {
        mUnk4 = 0;
        mId = id;
    }
    virtual u32 GetSize() { return 0; }
    virtual void Write(void *buf) {}
    virtual void Read(const void *buf) {}
    virtual int Unk14() { return 0; }

    u32 mUnk4;
    u32 mId;        // 0x08
};

#endif
