#ifndef GAME_DINO_BATTLE_VIEW_KASEKI_POINT_MENU_H
#define GAME_DINO_BATTLE_VIEW_KASEKI_POINT_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"

class MMotion;
class DinoBattleView;
class DinoBattleViewCreature;
class DinoBattleViewKasekiPoint;

class DinoBattleViewPlayer : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleViewPlayer();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    DinoBattleViewKasekiPoint *GetKasekiPoint();
    void AddTimedTask(MTask *task, MTask *old);
    u32 GetUnk108();
    u32 GetUnkAC();
    std::string GetUnkC0();
    void SetMotions(MMotion *motion, MTask *task);
    void TakeMotions(MMotion **motion, MTask **task);
    int GetCreatureCount();
    DinoBattleViewCreature *GetCreature(int index);
    DinoBattleViewCreature *FindCreatureByB4(u32 id);
    DinoBattleViewCreature *FindCreatureByAC(u32 id);
    DinoBattleViewPlayer *GetOpponent();
    void UpdateUnk108();
    void AccumulateCreature(u32 *mask, int *sums, DinoBattleViewCreature *creature, bool opponent);
    BOOL HasCommonCreatureFlags();

    u32 mUnkA8;                 // 0xA8: player index
    u32 mUnkAC;                 // 0xAC
    u32 mUnkB0;                 // 0xB0
    DinoBattleView *mView;      // 0xB4
    u32 mUnkB8;                 // 0xB8
    DinoBattleViewPlayer *mOpponent; // 0xBC
    std::string mUnkC0;         // 0xC0
    u32 mUnkCC;                 // 0xCC
    MMessageSubject mSubject;   // 0xD0
    std::vector<DinoBattleViewCreature *> mCreatures; // 0xF8
    DinoBattleViewKasekiPoint *mKasekiPoint; // 0x104
    u32 mUnk108;                // 0x108
    MMotion *mMotion;           // 0x10C
    MTask *mMotionTask;         // 0x110
};

#endif
