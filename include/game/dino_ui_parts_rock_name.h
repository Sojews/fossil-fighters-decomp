#ifndef GAME_DINO_UI_PARTS_ROCK_NAME_H
#define GAME_DINO_UI_PARTS_ROCK_NAME_H

#include "m/task.h"
#include "m/motion.h"
#include "m/indicator.h"
#include "game/dino_backup.h"

// A kaseki rock's name.
class DinoUiPartsRockName : public MProcess {
public:
    DinoUiPartsRockName(MPoint pos);
    virtual ~DinoUiPartsRockName();
    virtual void Idle();
    void SetEntry(const DinoBackupEntry *entry, BOOL selected);
    void Clear();

    MFont *mFont;           // 0x74
    MIndicator *mText;      // 0x78
};

#endif
