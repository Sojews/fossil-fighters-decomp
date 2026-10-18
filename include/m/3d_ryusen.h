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
    virtual void Draw();

    void SetQuad(const VecFx32 *origin, const VecFx32 *a, const VecFx32 *b, const VecFx32 *c);

    VecFx32 mOrigin;    // 0x32C
    VecFx32 mUnk338;    // 0x338
    VecFx32 mEdges[3];  // 0x344: the other corners, relative to the origin
};

#endif
