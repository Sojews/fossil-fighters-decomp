#ifndef GAME_DINO_CLEANING_TOOL_H
#define GAME_DINO_CLEANING_TOOL_H

#include "m/task.h"

class DinoCleaning;

// A cleaning tool (hammer, drill, X-ray...) working on the fossil rock.
class DinoCleaningTool : public MProcess {
public:
    DinoCleaningTool(DinoCleaning *cleaning);
    // The game's destructor is inline. Its vtable's owner (dino_cleaning_tool.cpp) defines DINO_CLEANING_TOOL_OWNER and emits it out of line
    // until the key function there is decompiled.
#ifdef DINO_CLEANING_TOOL_OWNER
    virtual ~DinoCleaningTool();
#else
    virtual ~DinoCleaningTool() {}
#endif
    virtual void Idle();
    virtual void Unk18();
    virtual void Unk1C();
    virtual void Unk20();

    DinoCleaning *GetCleaning();

    DinoCleaning *mCleaning;  // 0x74
};

#endif
