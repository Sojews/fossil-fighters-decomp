#ifndef GAME_DINO_UI3_PARTS_MONEY_H
#define GAME_DINO_UI3_PARTS_MONEY_H

#include "m/task.h"
#include "m/motion.h"
#include "m/indicator.h"

class DinoUi3PartsMoney : public MProcess {
public:
    virtual ~DinoUi3PartsMoney();
    virtual void Idle();
    void SetPrice(int price);
    void RefreshMoney();
    void SetChange(int change);

    MIndicator *mPrice;     // 0x74: blank at 0
    MIndicator *mMoney;     // 0x78
    MIndicator *mChange;    // 0x7C
};

#endif
