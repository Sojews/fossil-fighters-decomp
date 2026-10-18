// A yes/no question window.
#ifndef GAME_DINO_COMMON_WINDOW_YES_NO_DIALOG_H
#define GAME_DINO_COMMON_WINDOW_YES_NO_DIALOG_H

#include <string>
#include "m/task.h"
#include "m/geometry.h"

class DinoCommonWindowYesNoDialog : public MProcess {
public:
    void SetUnkBC(u8 value);
    DinoCommonWindowYesNoDialog(std::string text, BOOL *result);
    DinoCommonWindowYesNoDialog(MDisplay *display, std::string text, BOOL *result);
    virtual ~DinoCommonWindowYesNoDialog();
    virtual void Idle();
    void SetLabels(std::string yes, std::string no);
    void SetLabels2(std::string yes, std::string no);   // (a second pair of labels)

    std::string mUnk74; // 0x74: the question
    std::string mUnk80; // 0x80: "no"
    std::string mUnk8C; // 0x8C: "yes"
    std::string mUnk98; // 0x98
    std::string mUnkA4; // 0xA4
    BOOL *mResult;      // 0xB0
    MPoint mPos;        // 0xB4
    u8 mUnkBC;          // 0xBC
};

#endif
