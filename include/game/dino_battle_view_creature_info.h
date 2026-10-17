// The battle's creature info panel (top screen).
#ifndef GAME_DINO_BATTLE_VIEW_CREATURE_INFO_H
#define GAME_DINO_BATTLE_VIEW_CREATURE_INFO_H

#include "m/task.h"
#include "m/geometry.h"

class MSmartOam;
class MMotion;
class MPictureFile;
class InfoPanelWindow;

class DinoBattleViewCreatureInfo : public MProcess {
public:
    virtual ~DinoBattleViewCreatureInfo();
    virtual void Idle();

    MMotion *GetMotion(int index);
    void SetPosition(MPoint pos);
    void Show();
    void PlayMotion(int index);

    MPoint mPos;                    // 0x74
    MSmartOam *mOam;                // 0x7C
    u32 mUnk80;                     // 0x80
    u32 unk84[25];
    u32 mUnkE8;                     // 0xE8
    u32 unkEC;
    u32 mUnkF0;                     // 0xF0
    MParallelTaskSet *mTasks;       // 0xF4
    InfoPanelWindow *mWindow;       // 0xF8
    u32 unkFC[5];
    MMotion *mMotions[2];           // 0x110
    u32 unk118[3];
    MPictureFile *mPicture;         // 0x124
};

#endif
