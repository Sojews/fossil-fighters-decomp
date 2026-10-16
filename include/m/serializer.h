// MSerializer: an MSerializable made of a list of variables and objects, laid out one after another.
#ifndef M_SERIALIZER_H
#define M_SERIALIZER_H

#include <vector>
#include "m/message.h"

class MSerializer : public MSerializable {
public:
    enum {
        TYPE_DATA,          // copied as bytes
        TYPE_SERIALIZABLE,  // an MSerializable, written and read by its own functions
    };

    struct Entry {
        int type;
        void *ptr;
        u32 offset;     // in the serialized form
        u32 size;       // of one element
        u32 count;
    };

    MSerializer();
    virtual u32 GetSize();
    virtual void Write(void *buf);
    virtual void Read(const void *buf);
    virtual int Unk14();

    // Adds an entry, aligning its offset to align bytes.
    void Add(int type, void *ptr, u32 size, u32 count, u32 align);
    void Add(u8 *ptr, u32 count);
    void Add(u16 *ptr, u32 count);
    void Add(u32 *ptr, u32 count);
    void Add(s32 *ptr, u32 count);
    void Add(u64 *ptr, u32 count);
    void Add(MSerializable *obj);
    // Pads the size to a multiple of 4; no more entries are added after this.
    void Close();

    u8 mClosed;                     // 0x04
    u32 mSize;                      // 0x08
    std::vector<Entry> mEntries;    // 0x0C
};

#endif
