// MSerializer.

#include "m/serializer.h"

MSerializer::MSerializer() : mClosed(FALSE), mSize(0) {
}

void MSerializer::Add(int type, void *ptr, u32 size, u32 count, u32 align) {
    mSize = (mSize + align - 1) / align * align;
    Entry entry;
    entry.type = type;
    entry.ptr = ptr;
    entry.size = size;
    entry.count = count;
    entry.offset = mSize;
    mEntries.push_back(entry);
    mSize += size * count;
}

void MSerializer::Add(u8 *ptr, u32 count) {
    Add(TYPE_DATA, ptr, 1, count, 1);
}

void MSerializer::Add(u16 *ptr, u32 count) {
    Add(TYPE_DATA, ptr, 2, count, 2);
}

void MSerializer::Add(u32 *ptr, u32 count) {
    Add(TYPE_DATA, ptr, 4, count, 4);
}

void MSerializer::Add(s32 *ptr, u32 count) {
    Add(TYPE_DATA, ptr, 4, count, 4);
}

void MSerializer::Add(u64 *ptr, u32 count) {
    Add(TYPE_DATA, ptr, 8, count, 4);
}

void MSerializer::Add(MSerializable *obj) {
    Add(TYPE_SERIALIZABLE, obj, obj->GetSize(), 1, 4);
}

void MSerializer::Close() {
    mSize = (mSize + 3) & ~3;
    mClosed = TRUE;
}

int MSerializer::Unk14() {
    return mClosed;
}

u32 MSerializer::GetSize() {
    return mSize;
}

void MSerializer::Write(void *buf) {
    for (u32 i = 0; i < mEntries.size(); i++) {
        Entry entry = mEntries[i];
        switch (entry.type) {
        case TYPE_DATA:
            MI_CpuCopy8(entry.ptr, (u8 *)buf + entry.offset, entry.size * entry.count);
            break;
        case TYPE_SERIALIZABLE:
            ((MSerializable *)entry.ptr)->Write((u8 *)buf + entry.offset);
            break;
        }
    }
}

void MSerializer::Read(const void *buf) {
    for (u32 i = 0; i < mEntries.size(); i++) {
        Entry entry = mEntries[i];
        switch (entry.type) {
        case TYPE_DATA:
            MI_CpuCopy8((const u8 *)buf + entry.offset, entry.ptr, entry.size * entry.count);
            break;
        case TYPE_SERIALIZABLE:
            ((MSerializable *)entry.ptr)->Read((const u8 *)buf + entry.offset);
            break;
        }
    }
}

// (The reallocation helper's deleter destructor goes ahead of the helper's, and the entries' deleter ahead of the
// serializer's destructor, as in the game.)
template std::__vec_deleter<MSerializer::Entry, std::allocator<MSerializer::Entry> &>::~__vec_deleter();
template std::__vec_deleter<MSerializer::Entry, std::allocator<MSerializer::Entry> >::~__vec_deleter();
