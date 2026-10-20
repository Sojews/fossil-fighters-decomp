// M3DRyusen.

#include "m/3d_ryusen.h"
#include "nitro/g3.h"

M3DRyusen::~M3DRyusen() {
}

void M3DRyusen::SetQuad(const VecFx32 *origin, const VecFx32 *a, const VecFx32 *b, const VecFx32 *c) {
    mOrigin.x = origin->x;
    mOrigin.y = origin->y;
    mOrigin.z = origin->z;
    mUnk338.x = 0;
    mUnk338.y = 0;
    mUnk338.z = 0;
    mEdges[0].x = a->x - origin->x;
    mEdges[0].y = a->y - origin->y;
    mEdges[0].z = a->z - origin->z;
    mEdges[1].x = b->x - origin->x;
    mEdges[1].y = b->y - origin->y;
    mEdges[1].z = b->z - origin->z;
    mEdges[2].x = c->x - origin->x;
    mEdges[2].y = c->y - origin->y;
    mEdges[2].z = c->z - origin->z;
}

// A translucent quad from the origin.
void M3DRyusen::Draw(u32 lights) {
    int id = mPolygonId & 0x3F;
    if (mUnk1C == 0) {
        return;
    }
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_PushMtx();
    if (mUnkC4 != NULL) {
        G3_MultMtx44(mUnkC4);
    }
    G3_Translate(mOrigin.x, mOrigin.y, mOrigin.z);
    G3_PolygonAttr(0, 0, GX_CULL_NONE, id, mUnk1C, 0x3000);
    G3_TexImageParam(0x1600);
    G3_Color(mUnk2F4);
    G3_Begin(GX_BEGIN_TRIANGLE_STRIP);
    G3_Vtx(mUnk338.x, mUnk338.y, mUnk338.z);
    G3_Vtx(mEdges[0].x, mEdges[0].y, mEdges[0].z);
    G3_Vtx(mEdges[1].x, mEdges[1].y, mEdges[1].z);
    G3_Vtx(mEdges[2].x, mEdges[2].y, mEdges[2].z);
    G3_End();
    G3_PopMtx(1);
}
