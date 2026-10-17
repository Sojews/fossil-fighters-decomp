// NitroSDK G3: geometry commands that send a matrix or table through the FIFO.

#include "nitro/fx.h"

#define REG_G3X_GXFIFO (*(volatile u32 *) 0x04000400)

void GX_SendFifo48B(const void *src);
void GX_SendFifo64B(const void *src);
void GX_SendFifo128B(const void *src);

void G3_LoadMtx44(const MtxFx44 *m) {
    REG_G3X_GXFIFO = 0x16;
    GX_SendFifo64B(m);
}

void G3_LoadMtx43(const void *m) {
    REG_G3X_GXFIFO = 0x17;
    GX_SendFifo48B(m);
}

void G3_MultMtx44(const MtxFx44 *m) {
    REG_G3X_GXFIFO = 0x18;
    GX_SendFifo64B(m);
}

void G3_MultMtx43(const void *m) {
    REG_G3X_GXFIFO = 0x19;
    GX_SendFifo48B(m);
}

void G3_Shininess(const u32 *table) {
    REG_G3X_GXFIFO = 0x34;
    GX_SendFifo128B(table);
}
