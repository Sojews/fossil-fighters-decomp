// Loading and unloading overlays. An overlay's ID is the address of a linker-defined symbol, as in the SDK.
#ifndef M_OVERLAY_H
#define M_OVERLAY_H

#include "types.h"

// dsd's linker script names the ID symbols OVERLAY_<n>_ID and its delinker emits overlay_id relocations
// against them, so the game's ID constants and ours reference the same symbols.
#define M_OVERLAY_SYM_(n) OVERLAY_##n##_ID
#define M_OVERLAY_SYM(n) M_OVERLAY_SYM_(n)
#define M_OVERLAY_NUM_ov000 0
#define M_OVERLAY_NUM_ov001 1
#define M_OVERLAY_NUM_ov002 2
#define M_OVERLAY_NUM_ov003 3
#define M_OVERLAY_NUM_ov004 4
#define M_OVERLAY_NUM_ov005 5
#define M_OVERLAY_NUM_ov006 6
#define M_OVERLAY_NUM_ov007 7

#define M_EXTERN_OVERLAY(name) extern u32 M_OVERLAY_SYM(M_OVERLAY_NUM_##name)
#define M_OVERLAY_ID(name) ((u32)&M_OVERLAY_SYM(M_OVERLAY_NUM_##name))

BOOL MLoadOverlay(u32 target, u32 id);
BOOL MUnloadOverlay(u32 target, u32 id);

#endif
