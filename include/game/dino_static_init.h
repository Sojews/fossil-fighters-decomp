#ifndef GAME_DINO_STATIC_INIT_H
#define GAME_DINO_STATIC_INIT_H

#include "types.h"
#include "m/geometry.h"

// A file's startup initialization of its pooled statics.
// The game's files define static objects (a common header's three points, the file's layout tables) that mwcc
// places in the file's size-sorted .data pool and constructs in __sinit. The pools aren't reproduced here (code
// reads them through extern structs named after the game's anchors), so a file's __sinit is rebuilt as one static
// object whose constructor writes the same words, in the same order, through those anchors.
#define DINO_POOL_WORD(sym, off) (((volatile s32 *)&(sym))[(off) / 4])
#define DINO_POOL_WORD_16(sym, off) (((volatile s16 *)&(sym))[(off) / 2])
#define DINO_POOL_WORD_8(sym, off) (((volatile s8 *)&(sym))[(off)])
// A pooled object of type T at the anchor plus off.
#define DINO_POOL(T, sym, off) (*(T *)&DINO_POOL_WORD(sym, off))
// The same without volatile, for files whose stores the game schedules freely across several anchors.
#define DINO_POOL_NV(T, sym, off) (*(T *)((u8 *)&(sym) + (off)))

// A common header's layout constants (the cleaning and revive files): the screen size and rect, an icon margin, the
// icon size and rect, and the top bar. Each rect is built as `static const MRect kRect = DinoSizeRect(kSize);`; the
// size argument's copy and the origin are the stack temporaries the game's __sinit still stores. Files that use them
// turn on `#pragma always_inline` before their static-init object and leave it on to the end of the file (__sinit is
// generated there), so mwcc inlines these into __sinit as the game's (-ipa) build did.
static inline MRect DinoSizeRect(MPoint size) {
    return MRect(MPoint(0, 0), size);
}
#define DINO_COMMON_LAYOUT_INIT(sym, screenSize, screenRect, iconMargin, iconSize, iconRect, barPos, barSize, barRect) \
    DINO_POOL(MPoint, sym, screenSize) = MPoint(256, 192);                                                            \
    DINO_POOL(MRect, sym, screenRect) = DinoSizeRect(DINO_POOL(MPoint, sym, screenSize));                             \
    DINO_POOL(MPoint, sym, iconMargin) = MPoint(2, 2);                                                                \
    DINO_POOL(MPoint, sym, iconSize) = MPoint(36, 28);                                                                \
    DINO_POOL(MRect, sym, iconRect) = DinoSizeRect(DINO_POOL(MPoint, sym, iconSize));                                 \
    DINO_POOL(MPoint, sym, barPos) = MPoint(0, 24);                                                                   \
    DINO_POOL(MPoint, sym, barSize) = MPoint(256, 192);                                                               \
    DINO_POOL(MRect, sym, barRect) = MRect(0, 0, 256, 24)

// The same through the game's two pool anchors: the pool's first 0x80 bytes through sym, the rest through sym80 (the
// symbol at sym + 0x80). The game reached a pool's words past 0x7c through a separate anchor, so its scheduler saw the
// screen rect and the screen size as different objects and hoisted the rect's zero half above the size's stores.
#define DINO_POOL_AT(T, sym, sym80, off) \
    (*(T *)((off) < 0x80 ? (u8 *)&(sym) + (off) : (u8 *)&(sym80) + ((off) - 0x80)))
#define DINO_COMMON_LAYOUT_INIT2(sym, sym80, screenSize, screenRect, iconMargin, iconSize, iconRect, barPos, barSize,   \
                                 barRect)                                                                              \
    DINO_POOL_AT(MPoint, sym, sym80, screenSize) = MPoint(256, 192);                                                    \
    DINO_POOL_AT(MRect, sym, sym80, screenRect) = DinoSizeRect(DINO_POOL_AT(MPoint, sym, sym80, screenSize));           \
    DINO_POOL_AT(MPoint, sym, sym80, iconMargin) = MPoint(2, 2);                                                        \
    DINO_POOL_AT(MPoint, sym, sym80, iconSize) = MPoint(36, 28);                                                        \
    DINO_POOL_AT(MRect, sym, sym80, iconRect) = DinoSizeRect(DINO_POOL_AT(MPoint, sym, sym80, iconSize));               \
    DINO_POOL_AT(MPoint, sym, sym80, barPos) = MPoint(0, 24);                                                           \
    DINO_POOL_AT(MPoint, sym, sym80, barSize) = MPoint(256, 192);                                                       \
    DINO_POOL_AT(MRect, sym, sym80, barRect) = MRect(0, 0, 256, 24)

#endif
