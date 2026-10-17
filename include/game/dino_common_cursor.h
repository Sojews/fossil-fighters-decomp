// The common menu cursor.
#ifndef GAME_DINO_COMMON_CURSOR_H
#define GAME_DINO_COMMON_CURSOR_H

#include "m/view.h"
#include "m/task.h"

class DinoCommonCursor : public MView {
public:
    virtual ~DinoCommonCursor();
    virtual void Execute();

    MSerialTaskSet mTasks;  // 0x10
};

#endif
