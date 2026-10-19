// Globals no code uses (between the text codec and the file code): the game still constructs them at startup and
// destroys them at exit.
// NONMATCHING: the static initializer (the globals' and their destructor records' places in the pool depend on
// names we don't know).

#include "types.h"
#include <string>
#include <vector>

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
