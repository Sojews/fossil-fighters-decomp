// Moves the player character from the pad and touch screen.
#ifndef GAME_DINO_FIELD_PLAYER_CONTROL_H
#define GAME_DINO_FIELD_PLAYER_CONTROL_H

#include "m/resource.h"
#include "game/dino_resource.h"

#include "m/task.h"
#include "m/archive.h"
#include "nitro/fx.h"
#include "m/fx32.h"
#include "m/geometry.h"

class DinoField;
class MPolygonChar;
class MInput;
class MBitVector;

// Where the player control leaves the player when it ends.
struct DinoFieldPlayerControlResult {
    u16 unk00;
    MFx32 unk04;    // 0x04
    u8 unk08;       // 0x08
};

class DinoFieldPlayerControl : public MProcess {
public:
    DinoFieldPlayerControl(DinoField *field, u32 *arg1, DinoFieldPlayerControlResult *result, u32 arg3);
    virtual ~DinoFieldPlayerControl();
    virtual void Idle();

    void SetEffect(MFx32Point pos, MFx32 height, u16 angle, u32 effect);

    void StoreResult() {
        mResult->unk08 = 1;
        mResult->unk00 = mAngle;
        mResult->unk04 = mSpeed;
    }

    DinoField *mField;      // 0x74
    // 0x78: why the control ended (cleared at the start): {kind, arg, x, y}
    u32 *mUnk78;
    DinoFieldPlayerControlResult *mResult;  // 0x7C
    MInput *mInput;         // 0x80: the field's unk234
    u32 mArea;              // 0x84: the map area the player stands in
    u32 mMap;               // 0x88: the map that area is on
    MBitVector *mExitAreas; // 0x8C: areas that end the control when entered
    u16 mAngle;             // 0x90: the way the player faces
    MFx32 mSpeed;           // 0x94: the player's walking speed
    u8 mTouching;           // 0x98: a touch started on the player and may turn it
    u8 unk99[3];
    MPoint mTouchStart;     // 0x9C
    u32 mTouchFrames;       // 0xA4
    u16 mTouchAngle;        // 0xA8: from the player to where the touch started
    u8 unkAA[2];
    u32 mUnkAC;             // 0xAC
    MPolygonChar *mEffect;  // 0xB0
    MResource<DinoExcavateDefs> mExcavateDefs;  // 0xB4
};

#endif
