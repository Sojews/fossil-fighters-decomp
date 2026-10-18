// Moves the player character from the pad and touch screen.
#ifndef GAME_DINO_FIELD_PLAYER_CONTROL_H
#define GAME_DINO_FIELD_PLAYER_CONTROL_H

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
    u32 mUnk78;             // 0x78
    DinoFieldPlayerControlResult *mResult;  // 0x7C
    u32 mUnk80[4];          // 0x80
    u16 mUnk90;             // 0x90
    u32 mUnk94;             // 0x94
    u32 mUnk98[6];          // 0x98
    MPolygonChar *mEffect;  // 0xB0
    MArchivePtr mArchive;   // 0xB4
    MArchiveFile mFile;     // 0xB8
};

#endif
