#ifndef GAME_DINO_UI3_PARTS_ROCK_NAME_H
#define GAME_DINO_UI3_PARTS_ROCK_NAME_H

#include "m/task.h"
#include "m/motion.h"
#include "m/indicator.h"

struct DinoBackupEntry;
class MFont;

class DinoUi3PartsRockName : public MProcess {
public:
    DinoUi3PartsRockName(MPoint pos);
    virtual ~DinoUi3PartsRockName();
    virtual void Idle();
    void SetEntry(const DinoBackupEntry *entry, BOOL selected);
    void Clear();

    MFont *mFont;           // 0x74
    MIndicator *mText;      // 0x78
};

#endif
