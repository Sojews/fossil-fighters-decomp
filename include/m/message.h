// Messages: serializable objects that processes send each other, locally or over wireless.
#ifndef M_MESSAGE_H
#define M_MESSAGE_H

#include "types.h"
#include "nitro/mi.h"
#include "m/irq.h"

// An object that can be written to and read from a byte buffer.
class MSerializable {
public:
    virtual ~MSerializable() {}
    virtual u32 GetSize() = 0;              // bytes Write() writes
    virtual void Write(void *buf) = 0;
    virtual void Read(const void *buf) = 0;
    virtual int Unk14() = 0;
};

// A message. Its serialized form starts with its type ID. Messages are reference counted, since one can be
// delivered to several observers.
class MMessage : public MSerializable {
public:
    MMessage(u32 id) {
        mRefCount = 0;
        mId = id;
    }
    virtual u32 GetSize() { return 0; }
    virtual void Write(void *buf) {}
    virtual void Read(const void *buf) {}
    virtual int Unk14() { return 0; }

    void AddRef();
    void Release();

    int mRefCount;  // 0x04
    u32 mId;        // 0x08
};

// Creates a message from its serialized form, or returns NULL if the type is unknown.
typedef MMessage *(*MMessageFactory)(const void *data);

void AddMessageFactory(MMessageFactory factory);
MMessage *CreateMessage(const void *data);

#endif
