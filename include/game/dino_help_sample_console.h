#ifndef GAME_DINO_HELP_SAMPLE_CONSOLE_H
#define GAME_DINO_HELP_SAMPLE_CONSOLE_H

#include "m/task.h"
#include "game/help.h"

class DinoHelpSampleConsole : public MProcess, public DinoHelpItem {
public:
    virtual ~DinoHelpSampleConsole();
};

#endif
