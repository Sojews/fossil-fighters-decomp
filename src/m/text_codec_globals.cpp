// Globals no code uses (between the text codec and the file code): the game still constructs them at startup and
// destroys them at exit.
// mwcc lays a file's .bss out by heap-sorting its objects by size, starting from the reverse of their definition order
// (each destructor record is defined just before its object). The game's layout doesn't come out of these four
// objects alone, so the unused arrays (sizes over 12 bytes, so they land after everything here) steer the sort.

#include "types.h"
#include <string>
#include <vector>

// (The game calls dino_se_motion.cpp's copy.)
template <> std::__vec_deleter<void *, std::allocator<void *> >::~__vec_deleter();

// (The element types' names are made up; nothing reads them.)
struct MUnusedEntry10 {
    u32 unk0;
    std::string text;   // 0x04
};

struct MUnusedEntry34 {
    std::string text0;          // 0x00
    std::string text1;          // 0x0C
    u32 unk18;                  // 0x18
    std::vector<void *> list0;  // 0x1C
    std::vector<void *> list1;  // 0x28
};

// (The objects carry the game's address names, as objdiff compares references to them by name.)
static u32 sPad0[8];
static u32 sPad1[5];
static std::vector<MUnusedEntry10> data_0214d254;
static u32 sPad2[4];
static u32 sPad3[8];
static u32 sPad4[7];
static u32 sUnused0;
static std::vector<MUnusedEntry34> data_0214d224;
static u32 sPad5[6];
static std::vector<u32> data_0214d218;
static std::string data_0214d248;
static u32 sPad6[5];

// (The vectors' members, in the order the game emits them.)
template std::__vec_deleter<u32, std::allocator<u32> >::~__vec_deleter();
template std::vector<MUnusedEntry10, std::allocator<MUnusedEntry10> >::~vector();
template std::vector<MUnusedEntry34, std::allocator<MUnusedEntry34> >::~vector();
template std::vector<u32, std::allocator<u32> >::~vector();
template std::__vec_deleter<MUnusedEntry34, std::allocator<MUnusedEntry34> >::~__vec_deleter();
template std::__vec_deleter<MUnusedEntry10, std::allocator<MUnusedEntry10> >::~__vec_deleter();
template void std::__vec_deleter<MUnusedEntry34, std::allocator<MUnusedEntry34> >::erase_at_end(size_t n, Metrowerks::int2type<false>);
template void std::__vec_deleter<MUnusedEntry10, std::allocator<MUnusedEntry10> >::erase_at_end(size_t n, Metrowerks::int2type<false>);
