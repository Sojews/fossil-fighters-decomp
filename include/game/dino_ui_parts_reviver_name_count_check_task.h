#ifndef GAME_DINO_UI_PARTS_REVIVER_NAME_COUNT_CHECK_TASK_H
#define GAME_DINO_UI_PARTS_REVIVER_NAME_COUNT_CHECK_TASK_H

#include "m/task.h"

class DinoUiPartsReviverName;

class DinoUiPartsReviverNameCountCheckTask : public MTask {
public:
    virtual ~DinoUiPartsReviverNameCountCheckTask();
    virtual void Execute();

    DinoUiPartsReviverName *mName;  // 0x10: exits once its letters are all out
};

#endif
