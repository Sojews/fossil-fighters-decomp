// The battle's creature info panel (top screen).
#ifndef GAME_DINO_BATTLE_VIEW_CREATURE_INFO_H
#define GAME_DINO_BATTLE_VIEW_CREATURE_INFO_H

#include "m/task.h"
#include "m/geometry.h"

class MSmartOam;
class MMotion;
class MIndicator;
class MConsole;
class MPictureFile;
class InfoPanelWindow;
class DinoBattleViewCreature;

class DinoBattleView;

class DinoBattleViewCreatureInfo : public MProcess {
public:
    DinoBattleViewCreatureInfo(DinoBattleView *view, MPoint pos);
    virtual ~DinoBattleViewCreatureInfo();
    virtual void Idle();

    MMotion *GetMotion(int index);
    void SetPosition(MPoint pos);
    void Show();
    void PlayMotion(int index);
    void Update(BOOL show, DinoBattleViewCreature *creature, u32 kind, u8 arg3, u32 count);

    MPoint mPos;                    // 0x74
    MSmartOam *mOam;                // 0x7C
    u32 mUnk80;                     // 0x80: the DinoBattleView
    MIndicator *mUnk84;             // 0x84
    MIndicator *mUnk88[6];          // 0x88
    MIndicator *mUnkA0[5];          // 0xA0
    MMotion *mUnkB4[2];             // 0xB4
    MMotion *mUnkBC[2];             // 0xBC
    MConsole *mUnkC4;               // 0xC4
    u32 unkC8[8];
    u32 mUnkE8;                     // 0xE8: what is shown
    u8 mUnkEC;                      // 0xEC
    DinoBattleViewCreature *mCreature;  // 0xF0: shown
    MParallelTaskSet *mTasks;       // 0xF4
    InfoPanelWindow *mWindow;       // 0xF8
    MMotion *mUnkFC;                // 0xFC
    u32 unk100[4];
    MMotion *mMotions[2];           // 0x110
    u32 unk118[3];
    MPictureFile *mPicture;         // 0x124
    u32 unk128[2];
};

#endif
