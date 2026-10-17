#ifndef GAME_DINO_CLEANING_TOOL_H
#define GAME_DINO_CLEANING_TOOL_H

#include "m/task.h"

class DinoCleaningTool : public MProcess {
public:
    // The game's destructor is inline. Its vtable's owner (dino_cleaning_tool.cpp) defines DINO_CLEANING_TOOL_OWNER and emits it out of line
    // until the key function there is decompiled.
#ifdef DINO_CLEANING_TOOL_OWNER
    virtual ~DinoCleaningTool();
#else
    virtual ~DinoCleaningTool() {}
#endif
    virtual void Idle();
};

#endif
