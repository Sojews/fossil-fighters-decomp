// A yes/no question window.
#ifndef GAME_DINO_COMMON_WINDOW_YES_NO_DIALOG_H
#define GAME_DINO_COMMON_WINDOW_YES_NO_DIALOG_H

#include <string>
#include "m/task.h"

class DinoCommonWindowYesNoDialog : public MProcess {
public:
    DinoCommonWindowYesNoDialog(MDisplay *display, std::string text, BOOL *result);
    virtual ~DinoCommonWindowYesNoDialog();
    virtual void Idle();
    void SetLabels(std::string yes, std::string no);

    std::string mUnk74; // 0x74
    std::string mUnk80; // 0x80
    std::string mUnk8C; // 0x8C
    std::string mUnk98; // 0x98
    std::string mUnkA4; // 0xA4
    u32 mUnkB0[4];      // 0xB0
};

#endif
