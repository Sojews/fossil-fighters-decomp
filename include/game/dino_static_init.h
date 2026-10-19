#ifndef GAME_DINO_STATIC_INIT_H
#define GAME_DINO_STATIC_INIT_H

#include "types.h"

// A file's startup initialization of its pooled statics.
// The game's files define static objects (a common header's three points, the file's layout tables) that mwcc
// places in the file's size-sorted .data pool and constructs in __sinit. The pools aren't reproduced here (code
// reads them through extern structs named after the game's anchors), so a file's __sinit is rebuilt as one static
// object whose constructor writes the same words, in the same order, through those anchors.
#define DINO_POOL_WORD(sym, off) (((volatile s32 *)&(sym))[(off) / 4])
#define DINO_POOL_WORD_16(sym, off) (((volatile s16 *)&(sym))[(off) / 2])
#define DINO_POOL_WORD_8(sym, off) (((volatile s8 *)&(sym))[(off)])

#endif
