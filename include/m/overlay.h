// Loading and unloading overlays. An overlay's ID is the address of a linker-defined symbol, as in the SDK.
#ifndef M_OVERLAY_H
#define M_OVERLAY_H

#include "types.h"

#define M_EXTERN_OVERLAY(name) extern u32 SDK_OVERLAY_##name##_ID
#define M_OVERLAY_ID(name) ((u32)&SDK_OVERLAY_##name##_ID)

BOOL MLoadOverlay(u32 target, u32 id);
BOOL MUnloadOverlay(u32 target, u32 id);

#endif
