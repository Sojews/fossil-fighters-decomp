// Moves the player character from the pad and touch screen.
#ifndef GAME_DINO_FIELD_PLAYER_CONTROL_H
#define GAME_DINO_FIELD_PLAYER_CONTROL_H

#include "m/resource.h"
#include "game/dino_resource.h"

#include "m/task.h"
#include "m/archive.h"
#include "nitro/fx.h"

class DinoField;
class MPolygonChar;

// Where the player control leaves the player when it ends.
struct DinoFieldPlayerControlResult {
    u16 unk00;
    u32 unk04;  // 0x04
    u8 unk08;   // 0x08
};

class DinoFieldPlayerControl : public MProcess {
public:
    DinoFieldPlayerControl(DinoField *field, u32 *arg1, DinoFieldPlayerControlResult *result, u32 arg3);
    virtual ~DinoFieldPlayerControl();
    virtual void Idle();

    // pos: the field position (x, y) and height (z).
    void SetEffect(VecFx32 pos, u16 angle, u32 effect);

    void StoreResult() {
        mResult->unk08 = 1;
        mResult->unk00 = mUnk90;
        mResult->unk04 = mUnk94;
    }

    DinoField *mField;      // 0x74
    u32 *mUnk78;            // 0x78: cleared at the start
    DinoFieldPlayerControlResult *mResult;  // 0x7C
    u32 mUnk80;             // 0x80: the field's unk234
    u32 mUnk84;             // 0x84
    u32 mUnk88;             // 0x88
    u32 mUnk8C;             // 0x8C
    u16 mUnk90;             // 0x90
    u32 mUnk94;             // 0x94
    u8 mUnk98;              // 0x98
    u8 unk99[0xAC - 0x99];
    u32 mUnkAC;             // 0xAC
    MPolygonChar *mEffect;  // 0xB0
    MResource<DinoExcavateDefs> mExcavateDefs;  // 0xB4
};

#endif
