#ifndef M_MENU_ITEM_H
#define M_MENU_ITEM_H

#include "m/task.h"

// One item of a menu.
class MMenuItem : public MProcess {
public:
    virtual ~MMenuItem() {}

    u32 mUnk74[(0x88 - 0x74) / 4];
};

#endif
