// Globals no code uses (between the text codec and the file code): the game still constructs them at startup and
// destroys them at exit.
// NONMATCHING: the static initializer (the globals' and their destructor records' places in the pool depend on
// names we don't know).

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

static u32 sUnused0;
static std::vector<MUnusedEntry10> sUnused10;
static std::vector<MUnusedEntry34> sUnused34;
static std::vector<u32> sUnused4;
static std::string sUnusedText;

// (The vectors' members, in the order the game emits them.)
template std::__vec_deleter<u32, std::allocator<u32> >::~__vec_deleter();
template std::vector<MUnusedEntry10, std::allocator<MUnusedEntry10> >::~vector();
template std::vector<MUnusedEntry34, std::allocator<MUnusedEntry34> >::~vector();
template std::vector<u32, std::allocator<u32> >::~vector();
template std::__vec_deleter<MUnusedEntry34, std::allocator<MUnusedEntry34> >::~__vec_deleter();
template std::__vec_deleter<MUnusedEntry10, std::allocator<MUnusedEntry10> >::~__vec_deleter();
template void std::__vec_deleter<MUnusedEntry34, std::allocator<MUnusedEntry34> >::erase_at_end(size_t n, Metrowerks::int2type<false>);
template void std::__vec_deleter<MUnusedEntry10, std::allocator<MUnusedEntry10> >::erase_at_end(size_t n, Metrowerks::int2type<false>);
