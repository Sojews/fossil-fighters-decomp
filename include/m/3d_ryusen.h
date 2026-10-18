// A model with a ryusen (streamline) effect.
#ifndef M_3D_RYUSEN_H
#define M_3D_RYUSEN_H

#include "m/3d_model.h"

class M3DRyusen : public M3DModel {
public:
    M3DRyusen() {}
    virtual ~M3DRyusen();
    virtual int GetFrameCount();
    virtual BOOL IsAnimDone();

    u8 unk32C[0x368 - 0x32C];
};

#endif
