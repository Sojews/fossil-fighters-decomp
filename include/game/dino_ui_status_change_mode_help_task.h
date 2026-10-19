#ifndef GAME_DINO_UI_STATUS_CHANGE_MODE_HELP_TASK_H
#define GAME_DINO_UI_STATUS_CHANGE_MODE_HELP_TASK_H

#include "game/help.h"
#include "m/task.h"
#include "game/dino_resource.h"

extern const u32 data_0208a870, data_0208a874;

// The help while a replacement is picked: one entry for the members' row, one for what's left of the screen.
class DinoUiStatusChangeModeHelpTask : public DinoHelpBase, public MProcess {
public:
    DinoUiStatusChangeModeHelpTask(MRect rect) : MProcess(NULL) {
        DinoHelpDummyItem *members;
        mParallel.AddTail(members = new DinoHelpDummyItem(GetDisplay(), MRect(0x15, 0x83, 0xEA, 0xAB),
                                                          std::string(gDinoResource->GetText(data_0208a874))));
        DinoHelpDummyItem *back;
        mParallel.AddTail(
            back = new DinoHelpDummyItem(GetDisplay(), rect, std::string(gDinoResource->GetText(data_0208a870))));
        AddEntry((u32)(DinoHelpItem *)members, 1, 0);
        AddEntry((u32)(DinoHelpItem *)back, 4, (u32)(DinoHelpItem *)members);
        AddEntry((u32)(DinoHelpItem *)back, 3, 0);
    }
    virtual ~DinoUiStatusChangeModeHelpTask();
    virtual void Idle();
};

#endif
