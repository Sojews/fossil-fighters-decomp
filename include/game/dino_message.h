// Messages passed between the game's processes (battle model, view and controller) and over wireless.
#ifndef GAME_DINO_MESSAGE_H
#define GAME_DINO_MESSAGE_H

#include "m/message.h"
#include "m/observer.h"
#include "m/geometry.h"
#include "game/dino_backup.h"

class DinoDummyMessage : public MMessage {
public:
    DinoDummyMessage() : MMessage(0x10001) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelGameBeginSignal : public MMessage {
public:
    DinoBattleModelGameBeginSignal() : MMessage(0x10002) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleControllerInitRequest : public MMessage {
public:
    DinoBattleControllerInitRequest() : MMessage(0x10003) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleControllerInitResult : public MMessage {
public:
    DinoBattleControllerInitResult() : MMessage(0x10004) {}
    DinoBattleControllerInitResult(u32 a) : MMessage(0x10004), mUnk0C(a) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
    }

    u32 mUnk0C;
};

class DinoBattleModelPlayerBeginSignal : public MMessage {
public:
    DinoBattleModelPlayerBeginSignal() : MMessage(0x10005) {}
    DinoBattleModelPlayerBeginSignal(u32 side, u32 seed, const char *name, u32 unk2C, u32 unk30)
        : MMessage(0x10005), mUnk0C(side), mUnk10(seed) {
        MI_CpuCopy8(name, mUnk14, sizeof(mUnk14));
        mUnk2C = unk2C;
        mUnk30 = unk30;
        mUnk34 = 0;
    }
    DinoBattleModelPlayerBeginSignal(u32 side, u32 seed, const char *name, u32 unk2C, u32 unk30, u8 unk34)
        : MMessage(0x10005), mUnk0C(side), mUnk10(seed) {
        MI_CpuCopy8(name, mUnk14, sizeof(mUnk14));
        mUnk2C = unk2C;
        mUnk30 = unk30;
        mUnk34 = unk34;
    }
    virtual u32 GetSize() { return 48; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(mUnk14, (u8 *)buf + 12, 24);
        MI_CpuCopy8(&mUnk2C, (u8 *)buf + 36, 4);
        MI_CpuCopy8(&mUnk30, (u8 *)buf + 40, 4);
        MI_CpuCopy8(&mUnk34, (u8 *)buf + 44, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, mUnk14, 24);
        MI_CpuCopy8((const u8 *)buf + 36, &mUnk2C, 4);
        MI_CpuCopy8((const u8 *)buf + 40, &mUnk30, 4);
        MI_CpuCopy8((const u8 *)buf + 44, &mUnk34, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14[0x18];
    u32 mUnk2C;
    u32 mUnk30;
    u8 mUnk34;
    u8 mPad35[0x3];
};

class DinoBattleModelCreatureBeginSignal : public MMessage {
public:
    DinoBattleModelCreatureBeginSignal() : MMessage(0x10006) {}
    // (Takes a side's 0x18-byte creature entry, DinoBattleSide::Creature.)
    // (unk30 is taken as given: the CPU controller passes a 4-byte struct, copied on the stack.)
    template <class C, class U>
    DinoBattleModelCreatureBeginSignal(u32 side, u32 slot, C creature, u32 unk2C, U unk30)
        : MMessage(0x10006), mUnk0C(side), mUnk10(slot) {
        *(C *)mUnk14 = creature;
        mUnk2C = unk2C;
        mUnk30 = unk30;
    }
    virtual u32 GetSize() { return 44; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(mUnk14, (u8 *)buf + 12, 24);
        MI_CpuCopy8(&mUnk2C, (u8 *)buf + 36, 4);
        MI_CpuCopy8(&mUnk30, (u8 *)buf + 40, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, mUnk14, 24);
        MI_CpuCopy8((const u8 *)buf + 36, &mUnk2C, 4);
        MI_CpuCopy8((const u8 *)buf + 40, &mUnk30, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14[6];      // 0x14: the side's creature entry
    u32 mUnk2C;
    u32 mUnk30;
};

class DinoBattleViewCreatureBeginSignal : public MMessage {
public:
    DinoBattleViewCreatureBeginSignal() : MMessage(0x10007) {}
    // (Takes the creature's 0x18-byte state, like DinoBattleModelCreatureBeginSignal.)
    template <class C>
    DinoBattleViewCreatureBeginSignal(u32 side, u32 id, u32 slot, C creature, u32 unk30)
        : MMessage(0x10007), mUnk0C(side), mUnk10(id), mUnk14(slot) {
        *(C *)mUnk18 = creature;
        mUnk30 = unk30;
    }
    virtual u32 GetSize() { return 44; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(mUnk18, (u8 *)buf + 16, 24);
        MI_CpuCopy8(&mUnk30, (u8 *)buf + 40, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, mUnk18, 24);
        MI_CpuCopy8((const u8 *)buf + 40, &mUnk30, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u8 mUnk18[0x18];
    u32 mUnk30;
};

class DinoBattleViewPlayerBeginSignal : public MMessage {
public:
    DinoBattleViewPlayerBeginSignal() : MMessage(0x10008) {}
    DinoBattleViewPlayerBeginSignal(u32 side, const void *name, u32 kasekiPoints, u32 unk34, u8 unk38)
        : MMessage(0x10008), mUnk0C(side) {
        MI_CpuCopy8(name, mUnk10, sizeof(mUnk10));
        mUnk30 = kasekiPoints;
        mUnk34 = unk34;
        mUnk38 = unk38;
    }
    virtual u32 GetSize() { return 52; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(mUnk10, (u8 *)buf + 8, 32);
        MI_CpuCopy8(&mUnk30, (u8 *)buf + 40, 4);
        MI_CpuCopy8(&mUnk34, (u8 *)buf + 44, 4);
        MI_CpuCopy8(&mUnk38, (u8 *)buf + 48, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, mUnk10, 32);
        MI_CpuCopy8((const u8 *)buf + 40, &mUnk30, 4);
        MI_CpuCopy8((const u8 *)buf + 44, &mUnk34, 4);
        MI_CpuCopy8((const u8 *)buf + 48, &mUnk38, 1);
    }

    u32 mUnk0C;
    u8 mUnk10[0x20];
    u32 mUnk30;
    u32 mUnk34;
    u8 mUnk38;
    u8 mPad39[0x3];
};

class DinoBattleViewEffectGameBeginRequest : public MMessage {
public:
    DinoBattleViewEffectGameBeginRequest() : MMessage(0x10009) {}
    DinoBattleViewEffectGameBeginRequest(u32 a) : MMessage(0x10009), mUnk0C(a) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
    }

    u32 mUnk0C;
};

class DinoBattleViewEffectGameBeginResult : public MMessage {
public:
    DinoBattleViewEffectGameBeginResult() : MMessage(0x1000A) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewEffectTurnBeginRequest : public MMessage {
public:
    DinoBattleViewEffectTurnBeginRequest() : MMessage(0x1000B) {}
    DinoBattleViewEffectTurnBeginRequest(u32 a, u32 b) : MMessage(0x1000B), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleViewEffectTurnBeginResult : public MMessage {
public:
    DinoBattleViewEffectTurnBeginResult() : MMessage(0x1000C) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleControllerWakeupSignal : public MMessage {
public:
    DinoBattleControllerWakeupSignal() : MMessage(0x1000D) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelPlayerTurnBeginRequst : public MMessage {
public:
    DinoBattleModelPlayerTurnBeginRequst() : MMessage(0x1000E) {}
    DinoBattleModelPlayerTurnBeginRequst(u32 a, u32 b) : MMessage(0x1000E), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleModelPlayerTurnBeginResult : public MMessage {
public:
    DinoBattleModelPlayerTurnBeginResult() : MMessage(0x1000F) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewUiRootMenuRequest : public MMessage {
public:
    DinoBattleViewUiRootMenuRequest() : MMessage(0x10010) {}
    DinoBattleViewUiRootMenuRequest(u32 reply, u32 side, u32 flags, u32 menu, u32 mask, u32 creatures)
        : MMessage(0x10010) {
        mUnk0C = reply;
        mUnk10 = side;
        mUnk14 = flags;
        mUnk18 = menu;
        mUnk1C = mask;
        mUnk20 = creatures;
    }
    virtual u32 GetSize() { return 28; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
};

class DinoBattleViewUiRootMenuResult : public MMessage {
public:
    DinoBattleViewUiRootMenuResult() : MMessage(0x10011) {}
    // (The choice comes last: the game loads it first.)
    DinoBattleViewUiRootMenuResult(u32 b, u32 c, u32 a) : MMessage(0x10011) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
    }
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleModelCreatureMoveRequest : public MMessage {
public:
    DinoBattleModelCreatureMoveRequest() : MMessage(0x10012) {}
    DinoBattleModelCreatureMoveRequest(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) : MMessage(0x10012) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
        mUnk18 = d;
        mUnk1C = e;
        mUnk20 = f;
        mUnk24 = g;
    }
    virtual u32 GetSize() { return 32; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
        mUnk24 = 0;
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
    u32 mUnk24;
};

class DinoBattleModelCreatureMoveResult : public MMessage {
public:
    DinoBattleModelCreatureMoveResult() : MMessage(0x10013) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelTurnChangeSignal : public MMessage {
public:
    DinoBattleModelTurnChangeSignal() : MMessage(0x10014) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelTurnEndRequest : public MMessage {
public:
    DinoBattleModelTurnEndRequest() : MMessage(0x10015) {}
    DinoBattleModelTurnEndRequest(u32 a, u32 b, u32 c) : MMessage(0x10015), mUnk0C(a), mUnk10(b), mUnk14(c) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        mUnk14 = 0;
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleModelTurnEndResult : public MMessage {
public:
    DinoBattleModelTurnEndResult() : MMessage(0x10016) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewUiChangeMenuRequest : public MMessage {
public:
    DinoBattleViewUiChangeMenuRequest() : MMessage(0x1001B) {}
    DinoBattleViewUiChangeMenuRequest(u32 reply, u32 side, u32 unk14, const u32 *ids, u32 count)
        : MMessage(0x1001B), mUnk0C(reply), mUnk10(side), mUnk14(unk14) {
        MI_CpuCopy8(ids, mUnk18, sizeof(mUnk18));
        mUnk24 = count;
    }
    virtual u32 GetSize() { return 32; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(mUnk18, (u8 *)buf + 16, 12);
        MI_CpuCopy8(&mUnk24, (u8 *)buf + 28, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, mUnk18, 12);
        MI_CpuCopy8((const u8 *)buf + 28, &mUnk24, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u8 mUnk18[0xC];
    u32 mUnk24;
};

class DinoBattleViewUiChangeMenuResult : public MMessage {
public:
    DinoBattleViewUiChangeMenuResult() : MMessage(0x1001C) {}
    DinoBattleViewUiChangeMenuResult(u8 cancelled, u32 arg, u32 id)
        : MMessage(0x1001C), mUnk0C(cancelled), mUnk10(arg), mUnk14(id) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 1);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 5, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 9, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 1);
        MI_CpuCopy8((const u8 *)buf + 5, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 9, &mUnk14, 4);
    }

    u8 mUnk0C;
    u8 mPad0D[0x3];
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleViewUiCreatureInfoMenuRequest : public MMessage {
public:
    DinoBattleViewUiCreatureInfoMenuRequest() : MMessage(0x1001D) {}
    DinoBattleViewUiCreatureInfoMenuRequest(u32 reply, u32 side) : MMessage(0x1001D), mUnk0C(reply), mUnk10(side) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleViewUiCreatureInfoMenuResult : public MMessage {
public:
    DinoBattleViewUiCreatureInfoMenuResult() : MMessage(0x1001E) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelGoEscapeAreaRequest : public MMessage {
public:
    DinoBattleModelGoEscapeAreaRequest() : MMessage(0x1001F) {}
    DinoBattleModelGoEscapeAreaRequest(u32 reply, u32 side, u32 unk14, u32 unk18)
        : MMessage(0x1001F), mUnk0C(reply), mUnk10(side), mUnk14(unk14), mUnk18(unk18) {}
    virtual u32 GetSize() { return 20; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
};

class DinoBattleModelGoEscapeAreaResult : public MMessage {
public:
    DinoBattleModelGoEscapeAreaResult() : MMessage(0x10020) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureMoveRequest : public MMessage {
public:
    DinoBattleViewCreatureMoveRequest() : MMessage(0x10021) {}
    DinoBattleViewCreatureMoveRequest(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f)
        : MMessage(0x10021), mUnk0C(a), mUnk10(b), mUnk14(c), mUnk18(d), mUnk1C(e), mUnk20(f) {}
    virtual u32 GetSize() { return 28; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
};

class DinoBattleViewCreatureMoveResult : public MMessage {
public:
    DinoBattleViewCreatureMoveResult() : MMessage(0x10022) {}
    DinoBattleViewCreatureMoveResult(u32 arg) : MMessage(0x10022), mUnk0C(arg) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
    }

    u32 mUnk0C;
};

class DinoBattleViewUiAttackerMenuRequest : public MMessage {
public:
    DinoBattleViewUiAttackerMenuRequest() : MMessage(0x10017) {}
    DinoBattleViewUiAttackerMenuRequest(u32 reply, u32 side, const u32 *ids, u32 count, u32 unk24, u32 unk28)
        : MMessage(0x10017), mUnk0C(reply), mUnk10(side) {
        MI_CpuCopy8(ids, mUnk14, sizeof(mUnk14));
        mUnk20 = count;
        mUnk24 = unk24;
        mUnk28 = unk28;
    }
    virtual u32 GetSize() { return 36; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(mUnk14, (u8 *)buf + 12, 12);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
        MI_CpuCopy8(&mUnk24, (u8 *)buf + 28, 4);
        MI_CpuCopy8(&mUnk28, (u8 *)buf + 32, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, mUnk14, 12);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
        MI_CpuCopy8((const u8 *)buf + 28, &mUnk24, 4);
        MI_CpuCopy8((const u8 *)buf + 32, &mUnk28, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14[0xC];
    u32 mUnk20;
    u32 mUnk24;
    u32 mUnk28;
};

class DinoBattleViewUiAttackerMenuResult : public MMessage {
public:
    DinoBattleViewUiAttackerMenuResult() : MMessage(0x10018) {}
    DinoBattleViewUiAttackerMenuResult(u32 id, u8 cancelled) : MMessage(0x10018), mUnk0C(id), mUnk10(cancelled) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 1);
    }

    u32 mUnk0C;
    u8 mUnk10;
    u8 mPad11[0x3];
};

class DinoBattleViewUiDefenderMenuRequest : public MMessage {
public:
    DinoBattleViewUiDefenderMenuRequest() : MMessage(0x10019) {}
    DinoBattleViewUiDefenderMenuRequest(u32 reply, u32 side, const u32 *ids, u32 count, u8 unk24)
        : MMessage(0x10019), mUnk0C(reply), mUnk10(side) {
        MI_CpuCopy8(ids, mUnk14, sizeof(mUnk14));
        mUnk20 = count;
        mUnk24 = unk24;
    }
    virtual u32 GetSize() { return 32; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(mUnk14, (u8 *)buf + 12, 12);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
        MI_CpuCopy8(&mUnk24, (u8 *)buf + 28, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, mUnk14, 12);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
        MI_CpuCopy8((const u8 *)buf + 28, &mUnk24, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14[0xC];
    u32 mUnk20;
    u8 mUnk24;
    u8 mPad25[0x3];
};

class DinoBattleViewUiDefenderMenuResult : public MMessage {
public:
    DinoBattleViewUiDefenderMenuResult() : MMessage(0x1001A) {}
    DinoBattleViewUiDefenderMenuResult(u32 a, u8 b) : MMessage(0x1001A) {
        mUnk0C = a;
        mUnk10 = b;
    }
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 1);
    }

    u32 mUnk0C;
    u8 mUnk10;
    u8 mPad11[0x3];
};

class DinoBattleViewUiAbilityMenuRequest : public MMessage {
public:
    DinoBattleViewUiAbilityMenuRequest() : MMessage(0x10023) {}
    // (Takes the creature's 0x18-byte stats, DinoBattleCreatureStats.)
    template <class S>
    DinoBattleViewUiAbilityMenuRequest(u32 reply, u32 side, u32 creature, u32 unk18, u32 unk1C, S stats, u8 unk38,
                                       u32 unk3C)
        : MMessage(0x10023), mUnk0C(reply), mUnk10(side), mUnk14(creature), mUnk18(unk18), mUnk1C(unk1C) {
        *(S *)mUnk20 = stats;
        mUnk38 = unk38;
        mUnk3C = unk3C;
    }
    virtual u32 GetSize() { return 56; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(mUnk20, (u8 *)buf + 24, 24);
        MI_CpuCopy8(&mUnk38, (u8 *)buf + 48, 1);
        MI_CpuCopy8(&mUnk3C, (u8 *)buf + 49, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, mUnk20, 24);
        MI_CpuCopy8((const u8 *)buf + 48, &mUnk38, 1);
        MI_CpuCopy8((const u8 *)buf + 49, &mUnk3C, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u8 mUnk20[0x18];
    u8 mUnk38;
    u8 mPad39[0x3];
    u32 mUnk3C;
};

class DinoBattleViewUiAbilityMenuResult : public MMessage {
public:
    DinoBattleViewUiAbilityMenuResult() : MMessage(0x10024) {}
    DinoBattleViewUiAbilityMenuResult(u32 a, u32 b, u8 c) : MMessage(0x10024) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
    }
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14;
    u8 mPad15[0x3];
};

class DinoBattleModelCreatureAbilityRequest : public MMessage {
public:
    DinoBattleModelCreatureAbilityRequest() : MMessage(0x10025) {}
    DinoBattleModelCreatureAbilityRequest(u32 reply, u32 side, u32 unk14, u32 unk18, u32 unk1C, u32 unk20)
        : MMessage(0x10025), mUnk0C(reply), mUnk10(side), mUnk14(unk14), mUnk18(unk18), mUnk1C(unk1C), mUnk20(unk20) {}
    virtual u32 GetSize() { return 28; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
};

class DinoBattleModelCreatureAbilityResult : public MMessage {
public:
    DinoBattleModelCreatureAbilityResult() : MMessage(0x10026) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewSetActionedSignal : public MMessage {
public:
    DinoBattleViewSetActionedSignal() : MMessage(0x10027) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureCpuArrowUpdateSignal : public MMessage {
public:
    DinoBattleViewCreatureCpuArrowUpdateSignal() : MMessage(0x10028) {}
    DinoBattleViewCreatureCpuArrowUpdateSignal(u32 a, u32 b) : MMessage(0x10028), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleViewCreatureAbilityRequest : public MMessage {
public:
    DinoBattleViewCreatureAbilityRequest() : MMessage(0x10029) {}
    DinoBattleViewCreatureAbilityRequest(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u8 g, u32 h, u8 i, u32 j, u32 k,
                                         u8 l, u8 m, u8 n, u32 o, u32 p, u32 q)
        : MMessage(0x10029) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
        mUnk18 = d;
        mUnk1C = e;
        mUnk20 = f;
        mUnk24 = g;
        mUnk28 = h;
        mUnk2C = i;
        mUnk30 = j;
        mUnk34 = k;
        mUnk38 = l;
        mUnk39 = m;
        mUnk3A = n;
        mUnk3C = o;
        mUnk40 = p;
        mUnk44 = q;
    }
    virtual u32 GetSize() { return 60; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
        MI_CpuCopy8(&mUnk24, (u8 *)buf + 28, 1);
        MI_CpuCopy8(&mUnk28, (u8 *)buf + 29, 4);
        MI_CpuCopy8(&mUnk2C, (u8 *)buf + 33, 1);
        MI_CpuCopy8(&mUnk30, (u8 *)buf + 34, 4);
        MI_CpuCopy8(&mUnk34, (u8 *)buf + 38, 4);
        MI_CpuCopy8(&mUnk38, (u8 *)buf + 42, 1);
        MI_CpuCopy8(&mUnk39, (u8 *)buf + 43, 1);
        MI_CpuCopy8(&mUnk3A, (u8 *)buf + 44, 1);
        MI_CpuCopy8(&mUnk3C, (u8 *)buf + 45, 4);
        MI_CpuCopy8(&mUnk40, (u8 *)buf + 49, 4);
        MI_CpuCopy8(&mUnk44, (u8 *)buf + 53, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
        MI_CpuCopy8((const u8 *)buf + 28, &mUnk24, 1);
        MI_CpuCopy8((const u8 *)buf + 29, &mUnk28, 4);
        MI_CpuCopy8((const u8 *)buf + 33, &mUnk2C, 1);
        MI_CpuCopy8((const u8 *)buf + 34, &mUnk30, 4);
        MI_CpuCopy8((const u8 *)buf + 38, &mUnk34, 4);
        MI_CpuCopy8((const u8 *)buf + 42, &mUnk38, 1);
        MI_CpuCopy8((const u8 *)buf + 43, &mUnk39, 1);
        MI_CpuCopy8((const u8 *)buf + 44, &mUnk3A, 1);
        MI_CpuCopy8((const u8 *)buf + 45, &mUnk3C, 4);
        MI_CpuCopy8((const u8 *)buf + 49, &mUnk40, 4);
        MI_CpuCopy8((const u8 *)buf + 53, &mUnk44, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
    u8 mUnk24;
    u8 mPad25[0x3];
    u32 mUnk28;
    u8 mUnk2C;
    u8 mPad2D[0x3];
    u32 mUnk30;
    u32 mUnk34;
    u8 mUnk38;
    u8 mUnk39;
    u8 mUnk3A;
    u8 mPad3B[0x1];
    u32 mUnk3C;
    u32 mUnk40;
    u32 mUnk44;
};

class DinoBattleViewCreatureAbilityResult : public MMessage {
public:
    DinoBattleViewCreatureAbilityResult() : MMessage(0x1002A) {}
    DinoBattleViewCreatureAbilityResult(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u8 h, u8 i, u32 j)
        : MMessage(0x1002A), mUnk0C(a), mUnk10(b), mUnk14(c), mUnk18(d), mUnk1C(e), mUnk20(f), mUnk24(g), mUnk28(h),
          mUnk29(i), mUnk2C(j) {}
    virtual u32 GetSize() { return 40; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
        MI_CpuCopy8(&mUnk24, (u8 *)buf + 28, 4);
        MI_CpuCopy8(&mUnk28, (u8 *)buf + 32, 1);
        MI_CpuCopy8(&mUnk29, (u8 *)buf + 33, 1);
        MI_CpuCopy8(&mUnk2C, (u8 *)buf + 34, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
        MI_CpuCopy8((const u8 *)buf + 28, &mUnk24, 4);
        MI_CpuCopy8((const u8 *)buf + 32, &mUnk28, 1);
        MI_CpuCopy8((const u8 *)buf + 33, &mUnk29, 1);
        MI_CpuCopy8((const u8 *)buf + 34, &mUnk2C, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
    u32 mUnk24;
    u8 mUnk28;
    u8 mUnk29;
    u8 mPad2A[0x2];
    u32 mUnk2C;
};

class DinoBattleView3DTeamAttackRequest : public MMessage {
public:
    DinoBattleView3DTeamAttackRequest() : MMessage(0x1002D) {}
    DinoBattleView3DTeamAttackRequest(u32 a, u32 side, u32 id, u32 index, u32 count, const void *targets,
                                      const void *kinds, const void *damages, const void *downs)
        : MMessage(0x1002D), mUnk0C(a), mUnk10(side), mUnk14(id), mUnk18(index), mUnk1C(count) {
        MI_CpuCopy8(targets, mUnk20, sizeof(mUnk20));
        MI_CpuCopy8(kinds, mUnk2C, sizeof(mUnk2C));
        MI_CpuCopy8(damages, mUnk38, sizeof(mUnk38));
        MI_CpuCopy8(downs, mUnk44, sizeof(mUnk44));
    }
    virtual u32 GetSize() { return 64; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(mUnk20, (u8 *)buf + 24, 12);
        MI_CpuCopy8(mUnk2C, (u8 *)buf + 36, 12);
        MI_CpuCopy8(mUnk38, (u8 *)buf + 48, 12);
        MI_CpuCopy8(mUnk44, (u8 *)buf + 60, 3);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, mUnk20, 12);
        MI_CpuCopy8((const u8 *)buf + 36, mUnk2C, 12);
        MI_CpuCopy8((const u8 *)buf + 48, mUnk38, 12);
        MI_CpuCopy8((const u8 *)buf + 60, mUnk44, 3);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20[3];      // the targets
    u32 mUnk2C[3];      // the kind of each hit
    u32 mUnk38[3];      // the damage of each
    u8 mUnk44[0x3];     // whether each goes down
    u8 mPad47[0x1];
};

class DinoBattleView3DTeamAttackResult : public MMessage {
public:
    DinoBattleView3DTeamAttackResult() : MMessage(0x1002E) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureUpdateRequest : public MMessage {
public:
    DinoBattleViewCreatureUpdateRequest() : MMessage(0x1002F) {}
    DinoBattleViewCreatureUpdateRequest(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h, u32 i, u32 j, u32 k,
                                        u32 l, u8 m)
        : MMessage(0x1002F) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
        mUnk18 = d;
        mUnk1C = e;
        mUnk20 = f;
        mUnk24 = g;
        mUnk28 = h;
        mUnk2C = i;
        mUnk30 = j;
        mUnk34 = k;
        mUnk38 = l;
        mUnk3C = m;
    }
    virtual u32 GetSize() { return 56; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
        MI_CpuCopy8(&mUnk24, (u8 *)buf + 28, 4);
        MI_CpuCopy8(&mUnk28, (u8 *)buf + 32, 4);
        MI_CpuCopy8(&mUnk2C, (u8 *)buf + 36, 4);
        MI_CpuCopy8(&mUnk30, (u8 *)buf + 40, 4);
        MI_CpuCopy8(&mUnk34, (u8 *)buf + 44, 4);
        MI_CpuCopy8(&mUnk38, (u8 *)buf + 48, 4);
        MI_CpuCopy8(&mUnk3C, (u8 *)buf + 52, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
        MI_CpuCopy8((const u8 *)buf + 28, &mUnk24, 4);
        MI_CpuCopy8((const u8 *)buf + 32, &mUnk28, 4);
        MI_CpuCopy8((const u8 *)buf + 36, &mUnk2C, 4);
        MI_CpuCopy8((const u8 *)buf + 40, &mUnk30, 4);
        MI_CpuCopy8((const u8 *)buf + 44, &mUnk34, 4);
        MI_CpuCopy8((const u8 *)buf + 48, &mUnk38, 4);
        MI_CpuCopy8((const u8 *)buf + 52, &mUnk3C, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
    u32 mUnk24;
    u32 mUnk28;
    u32 mUnk2C;
    u32 mUnk30;
    u32 mUnk34;
    u32 mUnk38;
    u8 mUnk3C;
    u8 mPad3D[0x3];
};

class DinoBattleViewCreatureAbility3DRequest : public MMessage {
public:
    DinoBattleViewCreatureAbility3DRequest() : MMessage(0x1002B) {}
    DinoBattleViewCreatureAbility3DRequest(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u8 g, u32 h, const int *damage, u8 i,
                                           u32 j, u32 k, u8 l, u8 m, u8 n, u32 o, u32 p, u32 q)
        : MMessage(0x1002B), mUnk0C(a), mUnk10(b), mUnk14(c), mUnk18(d), mUnk1C(e), mUnk20(f), mUnk24(g), mUnk28(h) {
        MI_CpuCopy8(damage, mUnk2C, sizeof(mUnk2C));
        mUnk38 = i;
        mUnk3C = j;
        mUnk40 = k;
        mUnk44 = l;
        mUnk45 = m;
        mUnk46 = n;
        mUnk48 = o;
        mUnk4C = p;
        mUnk50 = q;
    }
    virtual u32 GetSize() { return 72; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
        MI_CpuCopy8(&mUnk24, (u8 *)buf + 28, 1);
        MI_CpuCopy8(&mUnk28, (u8 *)buf + 29, 4);
        MI_CpuCopy8(mUnk2C, (u8 *)buf + 33, 12);
        MI_CpuCopy8(&mUnk38, (u8 *)buf + 45, 1);
        MI_CpuCopy8(&mUnk3C, (u8 *)buf + 46, 4);
        MI_CpuCopy8(&mUnk40, (u8 *)buf + 50, 4);
        MI_CpuCopy8(&mUnk44, (u8 *)buf + 54, 1);
        MI_CpuCopy8(&mUnk45, (u8 *)buf + 55, 1);
        MI_CpuCopy8(&mUnk46, (u8 *)buf + 56, 1);
        MI_CpuCopy8(&mUnk48, (u8 *)buf + 57, 4);
        MI_CpuCopy8(&mUnk4C, (u8 *)buf + 61, 4);
        MI_CpuCopy8(&mUnk50, (u8 *)buf + 65, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
        MI_CpuCopy8((const u8 *)buf + 28, &mUnk24, 1);
        MI_CpuCopy8((const u8 *)buf + 29, &mUnk28, 4);
        MI_CpuCopy8((const u8 *)buf + 33, mUnk2C, 12);
        MI_CpuCopy8((const u8 *)buf + 45, &mUnk38, 1);
        MI_CpuCopy8((const u8 *)buf + 46, &mUnk3C, 4);
        MI_CpuCopy8((const u8 *)buf + 50, &mUnk40, 4);
        MI_CpuCopy8((const u8 *)buf + 54, &mUnk44, 1);
        MI_CpuCopy8((const u8 *)buf + 55, &mUnk45, 1);
        MI_CpuCopy8((const u8 *)buf + 56, &mUnk46, 1);
        MI_CpuCopy8((const u8 *)buf + 57, &mUnk48, 4);
        MI_CpuCopy8((const u8 *)buf + 61, &mUnk4C, 4);
        MI_CpuCopy8((const u8 *)buf + 65, &mUnk50, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
    u8 mUnk24;
    u8 mPad25[0x3];
    u32 mUnk28;
    int mUnk2C[3];      // the damage of each hit
    u8 mUnk38;
    u8 mPad39[0x3];
    u32 mUnk3C;
    u32 mUnk40;
    u8 mUnk44;
    u8 mUnk45;
    u8 mUnk46;
    u8 mPad47[0x1];
    u32 mUnk48;
    u32 mUnk4C;
    u32 mUnk50;
};

class DinoBattleViewCreatureAbility3DResult : public MMessage {
public:
    DinoBattleViewCreatureAbility3DResult() : MMessage(0x1002C) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureUpdateResult : public MMessage {
public:
    DinoBattleViewCreatureUpdateResult() : MMessage(0x10030) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelCreatureEndRequest : public MMessage {
public:
    DinoBattleModelCreatureEndRequest() : MMessage(0x10031) {}
    DinoBattleModelCreatureEndRequest(u32 a, u32 b, u32 c) : MMessage(0x10031), mUnk0C(a), mUnk10(b), mUnk14(c) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleModelCreatureEndResult : public MMessage {
public:
    DinoBattleModelCreatureEndResult() : MMessage(0x10032) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureEndRequest : public MMessage {
public:
    DinoBattleViewCreatureEndRequest() : MMessage(0x10033) {}
    DinoBattleViewCreatureEndRequest(u32 reply, u32 side, u32 creature) : MMessage(0x10033) {
        mUnk0C = reply;
        mUnk10 = side;
        mUnk14 = creature;
    }
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleViewCreatureEndResult : public MMessage {
public:
    DinoBattleViewCreatureEndResult() : MMessage(0x10034) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelGameEndSignal : public MMessage {
public:
    DinoBattleModelGameEndSignal() : MMessage(0x10035) {}
    DinoBattleModelGameEndSignal(u32 winner, u8 a, u8 b) : MMessage(0x10035) {
        mUnk0C = winner;
        mUnk10 = a;
        mUnk11 = b;
    }
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 1);
        MI_CpuCopy8(&mUnk11, (u8 *)buf + 9, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 1);
        MI_CpuCopy8((const u8 *)buf + 9, &mUnk11, 1);
    }

    u32 mUnk0C;
    u8 mUnk10;
    u8 mUnk11;
    u8 mPad12[0x2];
};

class DinoBattleViewEffectGameEndRequest : public MMessage {
public:
    DinoBattleViewEffectGameEndRequest() : MMessage(0x10036) {}
    DinoBattleViewEffectGameEndRequest(u32 a, u32 b) : MMessage(0x10036), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleViewEffectGameEndResult : public MMessage {
public:
    DinoBattleViewEffectGameEndResult() : MMessage(0x10037) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewPlayerUpdateRequest : public MMessage {
public:
    DinoBattleViewPlayerUpdateRequest() : MMessage(0x10038) {}
    DinoBattleViewPlayerUpdateRequest(u32 a, u32 b, u32 c) : MMessage(0x10038) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
    }
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleViewPlayerUpdateResult : public MMessage {
public:
    DinoBattleViewPlayerUpdateResult() : MMessage(0x10039) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelCreatureTurnBeginRequest : public MMessage {
public:
    DinoBattleModelCreatureTurnBeginRequest() : MMessage(0x1003A) {}
    DinoBattleModelCreatureTurnBeginRequest(u32 a, u32 b) : MMessage(0x1003A), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleModelCreatureTurnBeginResult : public MMessage {
public:
    DinoBattleModelCreatureTurnBeginResult() : MMessage(0x1003B) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelPlayerTurnBeginProcessRequst : public MMessage {
public:
    DinoBattleModelPlayerTurnBeginProcessRequst() : MMessage(0x1003C) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleModelPlayerTurnBeginProcessResult : public MMessage {
public:
    DinoBattleModelPlayerTurnBeginProcessResult() : MMessage(0x1003D) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleModelCreatureSpecialLifeRecoveryRequest : public MMessage {
public:
    DinoBattleModelCreatureSpecialLifeRecoveryRequest() : MMessage(0x1003E) {}
    DinoBattleModelCreatureSpecialLifeRecoveryRequest(u32 a, u32 b) : MMessage(0x1003E), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleModelCreatureSpecialLifeRecoveryResult : public MMessage {
public:
    DinoBattleModelCreatureSpecialLifeRecoveryResult() : MMessage(0x1003F) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelEvolutionCreatureRequest : public MMessage {
public:
    DinoBattleModelEvolutionCreatureRequest() : MMessage(0x10040) {}
    virtual u32 GetSize() { return 20; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
};

class DinoBattleModelEvolutionCreatureResult : public MMessage {
public:
    DinoBattleModelEvolutionCreatureResult() : MMessage(0x10041) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewEvolutionCreatureRequest : public MMessage {
public:
    DinoBattleViewEvolutionCreatureRequest() : MMessage(0x10042) {}
    DinoBattleViewEvolutionCreatureRequest(u32 a, u32 b, u32 c) : MMessage(0x10042), mUnk0C(a), mUnk10(b), mUnk14(c) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleViewEvolutionCreatureResult : public MMessage {
public:
    DinoBattleViewEvolutionCreatureResult() : MMessage(0x10043) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureCounterDamageRequest : public MMessage {
public:
    DinoBattleViewCreatureCounterDamageRequest() : MMessage(0x10044) {}
    virtual u32 GetSize() { return 24; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u8 mUnk1C;
    u8 mPad1D[0x3];
};

class DinoBattleViewCreatureCounterDamageResult : public MMessage {
public:
    DinoBattleViewCreatureCounterDamageResult() : MMessage(0x10045) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewPlayerUpdateStateIconRequest : public MMessage {
public:
    DinoBattleViewPlayerUpdateStateIconRequest() : MMessage(0x10046) {}
    DinoBattleViewPlayerUpdateStateIconRequest(u32 reply, u32 side, u32 creature, u32 count, const u8 *icons)
        : MMessage(0x10046), mUnk0C(reply), mUnk10(side), mUnk14(creature), mUnk18(count) {
        MI_CpuCopy8(icons, mUnk1C, sizeof(mUnk1C));
    }
    virtual u32 GetSize() { return 32; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(mUnk1C, (u8 *)buf + 20, 12);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, mUnk1C, 12);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u8 mUnk1C[0xC];
};

class DinoBattleViewPlayerUpdateStateIconResult : public MMessage {
public:
    DinoBattleViewPlayerUpdateStateIconResult() : MMessage(0x10047) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewDeleteStatusEffectSignal : public MMessage {
public:
    DinoBattleViewDeleteStatusEffectSignal() : MMessage(0x10048) {}
    DinoBattleViewDeleteStatusEffectSignal(u32 a, u32 b) : MMessage(0x10048) {
        mUnk0C = a;
        mUnk10 = b;
    }
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleViewPlayerUpdateAbilityIconRequest : public MMessage {
public:
    DinoBattleViewPlayerUpdateAbilityIconRequest() : MMessage(0x10049) {}
    DinoBattleViewPlayerUpdateAbilityIconRequest(u32 a, u32 b, u8 c) : MMessage(0x10049) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
    }
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14;
    u8 mPad15[0x3];
};

class DinoBattleViewPlayerUpdateAbilityIconResult : public MMessage {
public:
    DinoBattleViewPlayerUpdateAbilityIconResult() : MMessage(0x1004A) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewUiTurnEndRequest : public MMessage {
public:
    DinoBattleViewUiTurnEndRequest() : MMessage(0x1004B) {}
    DinoBattleViewUiTurnEndRequest(u32 reply, u32 side, u8 arg) : MMessage(0x1004B) {
        mUnk0C = reply;
        mUnk10 = side;
        mUnk14 = arg;
    }
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14;
    u8 mPad15[0x3];
};

class DinoBattleViewUiTurnEndResult : public MMessage {
public:
    DinoBattleViewUiTurnEndResult() : MMessage(0x1004C) {}
    DinoBattleViewUiTurnEndResult(u8 a) : MMessage(0x1004C) {
        mUnk0C = a;
    }
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 1);
    }

    u8 mUnk0C;
    u8 mPad0D[0x3];
};

class DinoBattleModelPlayerConfusionExecRequest : public MMessage {
public:
    DinoBattleModelPlayerConfusionExecRequest() : MMessage(0x1004D) {}
    DinoBattleModelPlayerConfusionExecRequest(u32 a, u32 b) : MMessage(0x1004D), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleModelPlayerConfusionExecResult : public MMessage {
public:
    DinoBattleModelPlayerConfusionExecResult() : MMessage(0x1004E) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelCreatureSupportEffectRequest : public MMessage {
public:
    DinoBattleModelCreatureSupportEffectRequest() : MMessage(0x1004F) {}
    DinoBattleModelCreatureSupportEffectRequest(MMessageObserver *reply, u32 unk10, u32 unk14)
        : MMessage(0x1004F), mUnk0C((u32)reply), mUnk10(unk10), mUnk14(unk14) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleModelCreatureSupportEffectResult : public MMessage {
public:
    DinoBattleModelCreatureSupportEffectResult() : MMessage(0x10050) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureSupportEffectRequest : public MMessage {
public:
    DinoBattleViewCreatureSupportEffectRequest() : MMessage(0x10051) {}
    DinoBattleViewCreatureSupportEffectRequest(u32 a, u32 b, u32 c) : MMessage(0x10051), mUnk0C(a), mUnk10(b), mUnk14(c) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleViewCreatureSupportEffectResult : public MMessage {
public:
    DinoBattleViewCreatureSupportEffectResult() : MMessage(0x10052) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureMotionEffectRequest : public MMessage {
public:
    DinoBattleViewCreatureMotionEffectRequest() : MMessage(0x10053) {}
    DinoBattleViewCreatureMotionEffectRequest(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) : MMessage(0x10053) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
        mUnk18 = d;
        mUnk1C = e;
        mUnk20 = f;
    }
    virtual u32 GetSize() { return 28; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
};

class DinoBattleViewCreatureMotionEffectResult : public MMessage {
public:
    DinoBattleViewCreatureMotionEffectResult() : MMessage(0x10054) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewFieldEffectWaitRequest : public MMessage {
public:
    DinoBattleViewFieldEffectWaitRequest() : MMessage(0x10055) {}
    DinoBattleViewFieldEffectWaitRequest(MMessageObserver *reply) : MMessage(0x10055), mUnk0C((u32)reply) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
    }

    u32 mUnk0C;
};

class DinoBattleViewFieldEffectWaitResult : public MMessage {
public:
    DinoBattleViewFieldEffectWaitResult() : MMessage(0x10056) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreaturePlayMotionRequest : public MMessage {
public:
    DinoBattleViewCreaturePlayMotionRequest() : MMessage(0x10057) {}
    DinoBattleViewCreaturePlayMotionRequest(u32 a, u32 b, u32 c) : MMessage(0x10057), mUnk0C(a), mUnk10(b), mUnk14(c) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleViewCreaturePlayMotionResult : public MMessage {
public:
    DinoBattleViewCreaturePlayMotionResult() : MMessage(0x10058) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

// The end of the bottom screen a free-coordinates effect plays at.
enum DinoBattleFreeCoordSide { DINO_FREE_COORD_LEFT, DINO_FREE_COORD_RIGHT };

class DinoBattleViewFreeCoordEffectRequest : public MMessage {
public:
    DinoBattleViewFreeCoordEffectRequest() : MMessage(0x10059) {}
    DinoBattleViewFreeCoordEffectRequest(u32 a, u32 b) : MMessage(0x10059), mUnk0C(0), mUnk10(a), mUnk14(b) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleViewFreeCoordEffectResult : public MMessage {
public:
    DinoBattleViewFreeCoordEffectResult() : MMessage(0x1005A) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelCreatureReleaseStatusEffectRequest : public MMessage {
public:
    DinoBattleModelCreatureReleaseStatusEffectRequest() : MMessage(0x1005B) {}
    DinoBattleModelCreatureReleaseStatusEffectRequest(MMessageObserver *reply, u32 mask)
        : MMessage(0x1005B), mUnk0C((u32)reply), mUnk10(mask) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleModelCreatureReleaseStatusEffectResult : public MMessage {
public:
    DinoBattleModelCreatureReleaseStatusEffectResult() : MMessage(0x1005C) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureTurnBeginSignal : public MMessage {
public:
    DinoBattleViewCreatureTurnBeginSignal() : MMessage(0x1005D) {}
    DinoBattleViewCreatureTurnBeginSignal(u32 a, u8 b) : MMessage(0x1005D), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 1);
    }

    u32 mUnk0C;
    u8 mUnk10;
    u8 mPad11[0x3];
};

class DinoBattleViewCreatureTurnEndSignal : public MMessage {
public:
    DinoBattleViewCreatureTurnEndSignal() : MMessage(0x1005E) {}
    DinoBattleViewCreatureTurnEndSignal(u32 a) : MMessage(0x1005E), mUnk0C(a) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
    }

    u32 mUnk0C;
};

class DinoBattleViewCreatureDamagedUpdateRequest : public MMessage {
public:
    DinoBattleViewCreatureDamagedUpdateRequest() : MMessage(0x1005F) {}
    DinoBattleViewCreatureDamagedUpdateRequest(u32 a, u32 b, u32 c, u32 d, u32 e, u8 f, u8 g) : MMessage(0x1005F) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
        mUnk18 = d;
        mUnk1C = e;
        mUnk20 = f;
        mUnk21 = g;
    }
    virtual u32 GetSize() { return 28; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 1);
        MI_CpuCopy8(&mUnk21, (u8 *)buf + 25, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 1);
        MI_CpuCopy8((const u8 *)buf + 25, &mUnk21, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u8 mUnk20;
    u8 mUnk21;
    u8 mPad22[0x2];
};

class DinoBattleViewCreatureDamagedUpdateResult : public MMessage {
public:
    DinoBattleViewCreatureDamagedUpdateResult() : MMessage(0x10060) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewWaitUpdateSupportEffectAnimeRequest : public MMessage {
public:
    DinoBattleViewWaitUpdateSupportEffectAnimeRequest() : MMessage(0x10061) {}
    DinoBattleViewWaitUpdateSupportEffectAnimeRequest(MMessageObserver *reply)
        : MMessage(0x10061), mUnk0C((u32)reply) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
    }

    u32 mUnk0C;
};

class DinoBattleViewWaitUpdateSupportEffectAnimeResult : public MMessage {
public:
    DinoBattleViewWaitUpdateSupportEffectAnimeResult() : MMessage(0x10062) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewActionInfoBarUpdateRequest : public MMessage {
public:
    DinoBattleViewActionInfoBarUpdateRequest() : MMessage(0x10063) {}
    DinoBattleViewActionInfoBarUpdateRequest(u32 value) : MMessage(0x10063), mUnk0C(0), mUnk10(value) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleViewActionInfoBarUpdateResult : public MMessage {
public:
    DinoBattleViewActionInfoBarUpdateResult() : MMessage(0x10064) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewFocusLCDFrameRequest : public MMessage {
public:
    DinoBattleViewFocusLCDFrameRequest() : MMessage(0x10065) {}
    DinoBattleViewFocusLCDFrameRequest(u32 a, u8 b) : MMessage(0x10065), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 1);
    }

    u32 mUnk0C;
    u8 mUnk10;
    u8 mPad11[0x3];
};

class DinoBattleViewFocusLCDFrameResult : public MMessage {
public:
    DinoBattleViewFocusLCDFrameResult() : MMessage(0x10066) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreatureSetBlendRequest : public MMessage {
public:
    DinoBattleViewCreatureSetBlendRequest() : MMessage(0x10067) {}
    DinoBattleViewCreatureSetBlendRequest(u32 a, u32 count, const u32 *ids, u8 on) : MMessage(0x10067) {
        mUnk0C = a;
        mUnk10 = count;
        MI_CpuCopy8(ids, mUnk14, sizeof(mUnk14));
        mUnk2C = on;
    }
    virtual u32 GetSize() { return 40; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(mUnk14, (u8 *)buf + 12, 24);
        MI_CpuCopy8(&mUnk2C, (u8 *)buf + 36, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, mUnk14, 24);
        MI_CpuCopy8((const u8 *)buf + 36, &mUnk2C, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14[0x18];
    u8 mUnk2C;
    u8 mPad2D[0x3];
};

class DinoBattleViewCreatureSetBlendResult : public MMessage {
public:
    DinoBattleViewCreatureSetBlendResult() : MMessage(0x10068) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewLCDFadeRequest : public MMessage {
public:
    DinoBattleViewLCDFadeRequest() : MMessage(0x10069) {}
    DinoBattleViewLCDFadeRequest(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) : MMessage(0x10069) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
        mUnk18 = d;
        mUnk1C = e;
        mUnk20 = f;
        mUnk24 = g;
    }
    virtual u32 GetSize() { return 32; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
        MI_CpuCopy8(&mUnk1C, (u8 *)buf + 20, 4);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 4);
        MI_CpuCopy8(&mUnk24, (u8 *)buf + 28, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
        MI_CpuCopy8((const u8 *)buf + 20, &mUnk1C, 4);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 4);
        MI_CpuCopy8((const u8 *)buf + 28, &mUnk24, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
    u32 mUnk1C;
    u32 mUnk20;
    u32 mUnk24;
};

class DinoBattleViewLCDFadeResult : public MMessage {
public:
    DinoBattleViewLCDFadeResult() : MMessage(0x1006A) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewWaitActionInfoBarRequest : public MMessage {
public:
    DinoBattleViewWaitActionInfoBarRequest() : MMessage(0x1006B) {}
    DinoBattleViewWaitActionInfoBarRequest(u32 reply) : MMessage(0x1006B), mUnk0C(reply) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
    }

    u32 mUnk0C;
};

class DinoBattleViewWaitActionInfoBarResult : public MMessage {
public:
    DinoBattleViewWaitActionInfoBarResult() : MMessage(0x1006C) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewClearAttackBarRequest : public MMessage {
public:
    DinoBattleViewClearAttackBarRequest() : MMessage(0x1006D) {}
    DinoBattleViewClearAttackBarRequest(u32 a) : MMessage(0x1006D) {
        mUnk0C = a;
    }
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
    }

    u32 mUnk0C;
};

class DinoBattleViewClearAttackBarResult : public MMessage {
public:
    DinoBattleViewClearAttackBarResult() : MMessage(0x1006E) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewShowAttackNameBarRequest : public MMessage {
public:
    DinoBattleViewShowAttackNameBarRequest() : MMessage(0x1006F) {}
    DinoBattleViewShowAttackNameBarRequest(u32 a, u32 attack, u8 c, u8 d)
        : MMessage(0x1006F), mUnk0C(a), mUnk10(attack), mUnk14(c), mUnk15(d) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 1);
        MI_CpuCopy8(&mUnk15, (u8 *)buf + 13, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 1);
        MI_CpuCopy8((const u8 *)buf + 13, &mUnk15, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14;
    u8 mUnk15;
    u8 mPad16[0x2];
};

class DinoBattleViewMoveAttackNameBarRequest : public MMessage {
public:
    DinoBattleViewMoveAttackNameBarRequest() : MMessage(0x10070) {}
    DinoBattleViewMoveAttackNameBarRequest(u32 a) : MMessage(0x10070), mUnk0C(a) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
    }

    u32 mUnk0C;
};

class DinoBattleViewMoveAttackNameBarResult : public MMessage {
public:
    DinoBattleViewMoveAttackNameBarResult() : MMessage(0x10071) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewShowAttackNameBarResult : public MMessage {
public:
    DinoBattleViewShowAttackNameBarResult() : MMessage(0x10072) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleView3DSupportEffectRequest : public MMessage {
public:
    DinoBattleView3DSupportEffectRequest() : MMessage(0x10073) {}
    DinoBattleView3DSupportEffectRequest(u32 unk10, u32 unk14, u8 unk18)
        : MMessage(0x10073), mUnk0C(0), mUnk10(unk10), mUnk14(unk14), mUnk18(unk18) {}
    virtual u32 GetSize() { return 20; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u8 mUnk18;
    u8 mPad19[0x3];
};

class DinoBattleView3DSupportEffectResult : public MMessage {
public:
    DinoBattleView3DSupportEffectResult() : MMessage(0x10074) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleView3DWatchStatusRequest : public MMessage {
public:
    DinoBattleView3DWatchStatusRequest() : MMessage(0x10075) {}
    DinoBattleView3DWatchStatusRequest(u32 a, u32 b) : MMessage(0x10075) {
        mUnk0C = a;
        mUnk10 = b;
    }
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleView3DWatchStatusResult : public MMessage {
public:
    DinoBattleView3DWatchStatusResult() : MMessage(0x10076) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleView3DPoisonDamageRequest : public MMessage {
public:
    DinoBattleView3DPoisonDamageRequest() : MMessage(0x10077) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleView3DPoisonDamageResult : public MMessage {
public:
    DinoBattleView3DPoisonDamageResult() : MMessage(0x10078) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleView3DPreTurnEndRequest : public MMessage {
public:
    DinoBattleView3DPreTurnEndRequest() : MMessage(0x10079) {}
    DinoBattleView3DPreTurnEndRequest(u32 a, u32 b) : MMessage(0x10079), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleView3DPreTurnEndResult : public MMessage {
public:
    DinoBattleView3DPreTurnEndResult() : MMessage(0x1007A) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleView3DTurnEndRequest : public MMessage {
public:
    DinoBattleView3DTurnEndRequest() : MMessage(0x1007B) {}
    DinoBattleView3DTurnEndRequest(u32 a, u32 b) : MMessage(0x1007B), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleView3DTurnEndResult : public MMessage {
public:
    DinoBattleView3DTurnEndResult() : MMessage(0x1007C) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewSetCreaturePriorityRequest : public MMessage {
public:
    DinoBattleViewSetCreaturePriorityRequest() : MMessage(0x1007D) {}
    DinoBattleViewSetCreaturePriorityRequest(u32 a) : MMessage(0x1007D) {
        mUnk0C = a;
    }
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
    }

    u32 mUnk0C;
};

class DinoBattleViewSetCreaturePriorityResult : public MMessage {
public:
    DinoBattleViewSetCreaturePriorityResult() : MMessage(0x1007E) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCreaturePlayAttackMotionRequest : public MMessage {
public:
    DinoBattleViewCreaturePlayAttackMotionRequest() : MMessage(0x1007F) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleViewCreaturePlayAttackMotionResult : public MMessage {
public:
    DinoBattleViewCreaturePlayAttackMotionResult() : MMessage(0x10080) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewChangeStatusEffectIconRequest : public MMessage {
public:
    DinoBattleViewChangeStatusEffectIconRequest() : MMessage(0x10081) {}
    virtual u32 GetSize() { return 20; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(&mUnk18, (u8 *)buf + 16, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, &mUnk18, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u32 mUnk18;
};

class DinoBattleViewPopNumberRedIfZeroSignal : public MMessage {
public:
    DinoBattleViewPopNumberRedIfZeroSignal() : MMessage(0x10082) {}
    DinoBattleViewPopNumberRedIfZeroSignal(u8 a) : MMessage(0x10082), mUnk0C(a) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 1);
    }

    u8 mUnk0C;
    u8 mPad0D[0x3];
};

class DinoBattleViewChangeStatusEffectIconResult : public MMessage {
public:
    DinoBattleViewChangeStatusEffectIconResult() : MMessage(0x10083) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewStatusEffectIconBlendClearSignal : public MMessage {
public:
    DinoBattleViewStatusEffectIconBlendClearSignal() : MMessage(0x10084) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewTutorialWindowRequest : public MMessage {
public:
    DinoBattleViewTutorialWindowRequest() : MMessage(0x10085) {}
    DinoBattleViewTutorialWindowRequest(u32 unk10) : MMessage(0x10085), mUnk0C(0), mUnk10(unk10) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleViewTutorialWindowResult : public MMessage {
public:
    DinoBattleViewTutorialWindowResult() : MMessage(0x10086) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewTutorialSetArroeRequest : public MMessage {
public:
    DinoBattleViewTutorialSetArroeRequest() : MMessage(0x10087) {}
    DinoBattleViewTutorialSetArroeRequest(u32 unk10, u32 unk14, MPoint pos)
        : MMessage(0x10087), mUnk0C(0), mUnk10(unk10), mUnk14(unk14) {
        *(MPoint *)mUnk18 = pos;
    }
    virtual u32 GetSize() { return 24; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
        MI_CpuCopy8(mUnk18, (u8 *)buf + 16, 8);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
        MI_CpuCopy8((const u8 *)buf + 16, mUnk18, 8);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
    u8 mUnk18[0x8];
};

class DinoBattleViewTutorialSetArroeResult : public MMessage {
public:
    DinoBattleViewTutorialSetArroeResult() : MMessage(0x10088) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewTutorialSetRectRequest : public MMessage {
public:
    DinoBattleViewTutorialSetRectRequest() : MMessage(0x10089) {}
    DinoBattleViewTutorialSetRectRequest(MPoint a, MPoint b) : MMessage(0x10089), mUnk0C(0) {
        *(MPoint *)mUnk10 = a;
        *(MPoint *)mUnk18 = b;
    }
    virtual u32 GetSize() { return 24; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(mUnk10, (u8 *)buf + 8, 8);
        MI_CpuCopy8(mUnk18, (u8 *)buf + 16, 8);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, mUnk10, 8);
        MI_CpuCopy8((const u8 *)buf + 16, mUnk18, 8);
    }

    u32 mUnk0C;
    u8 mUnk10[0x8];
    u8 mUnk18[0x8];
};

class DinoBattleViewTutorialSetRectResult : public MMessage {
public:
    DinoBattleViewTutorialSetRectResult() : MMessage(0x1008A) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleView3DGameSetRequest : public MMessage {
public:
    DinoBattleView3DGameSetRequest() : MMessage(0x1008B) {}
    DinoBattleView3DGameSetRequest(u32 a, u32 b) : MMessage(0x1008B), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleView3DGameSetResult : public MMessage {
public:
    DinoBattleView3DGameSetResult() : MMessage(0x1008C) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewLiveMessageSignal : public MMessage {
public:
    DinoBattleViewLiveMessageSignal() : MMessage(0x1008D) {}
    DinoBattleViewLiveMessageSignal(u32 id) : MMessage(0x1008D) {
        mUnk0C = id;
    }
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
    }

    u32 mUnk0C;
};

class DinoBattleViewDecideFirstAttackTeamRequest : public MMessage {
public:
    DinoBattleViewDecideFirstAttackTeamRequest() : MMessage(0x1008E) {}
    DinoBattleViewDecideFirstAttackTeamRequest(u32 a, u32 b, const void *points) : MMessage(0x1008E), mUnk0C(a), mUnk10(b) {
        MI_CpuCopy8(points, mUnk14, sizeof(mUnk14));
    }
    virtual u32 GetSize() { return 20; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(mUnk14, (u8 *)buf + 12, 8);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, mUnk14, 8);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14[0x8];
};

class DinoBattleViewDecideFirstAttackTeamResult : public MMessage {
public:
    DinoBattleViewDecideFirstAttackTeamResult() : MMessage(0x1008F) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleView3DRevolutionSetRequest : public MMessage {
public:
    DinoBattleView3DRevolutionSetRequest() : MMessage(0x10090) {}
    DinoBattleView3DRevolutionSetRequest(u32 a, u32 b, u32 c) : MMessage(0x10090), mUnk0C(a), mUnk10(b), mUnk14(c) {}
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleView3DRevolutionSetResult : public MMessage {
public:
    DinoBattleView3DRevolutionSetResult() : MMessage(0x10091) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewPlayerWaitKPCountStopRequest : public MMessage {
public:
    DinoBattleViewPlayerWaitKPCountStopRequest() : MMessage(0x10092) {}
    DinoBattleViewPlayerWaitKPCountStopRequest(u32 a, u32 b) : MMessage(0x10092), mUnk0C(a), mUnk10(b) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleViewPlayerWaitKPCountStopResult : public MMessage {
public:
    DinoBattleViewPlayerWaitKPCountStopResult() : MMessage(0x10093) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleModelTutorialGameEndSignal : public MMessage {
public:
    DinoBattleModelTutorialGameEndSignal() : MMessage(0x10094) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewSetAttackBarUpperLCDRequest : public MMessage {
public:
    DinoBattleViewSetAttackBarUpperLCDRequest() : MMessage(0x10095) {}
    DinoBattleViewSetAttackBarUpperLCDRequest(u32 a, u32 b, u8 c) : MMessage(0x10095) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
    }
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 1);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u8 mUnk14;
    u8 mPad15[0x3];
};

class DinoBattleViewSetAttackBarUpperLCDResult : public MMessage {
public:
    DinoBattleViewSetAttackBarUpperLCDResult() : MMessage(0x10096) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewCountPlayerThinkTimeSignal : public MMessage {
public:
    DinoBattleViewCountPlayerThinkTimeSignal() : MMessage(0x10097) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
    }

    u32 mUnk0C;
};

class DinoBattleViewCreatureActionSignal : public MMessage {
public:
    DinoBattleViewCreatureActionSignal() : MMessage(0x10098) {}
    DinoBattleViewCreatureActionSignal(u32 a, u8 b) : MMessage(0x10098) {
        mUnk0C = a;
        mUnk10 = b;
    }
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 1);
    }

    u32 mUnk0C;
    u8 mUnk10;
    u8 mPad11[0x3];
};

class DinoBattleViewCreatureActionUpdateSignal : public MMessage {
public:
    DinoBattleViewCreatureActionUpdateSignal() : MMessage(0x10099) {}
    DinoBattleViewCreatureActionUpdateSignal(u32 creature) : MMessage(0x10099) {
        mUnk0C = creature;
    }
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
    }

    u32 mUnk0C;
};

class DinoBattleViewCreatureUpdateColorRequest : public MMessage {
public:
    DinoBattleViewCreatureUpdateColorRequest() : MMessage(0x1009A) {}
    DinoBattleViewCreatureUpdateColorRequest(u32 a, u32 b, u32 c) : MMessage(0x1009A) {
        mUnk0C = a;
        mUnk10 = b;
        mUnk14 = c;
    }
    virtual u32 GetSize() { return 16; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
        MI_CpuCopy8(&mUnk14, (u8 *)buf + 12, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
        MI_CpuCopy8((const u8 *)buf + 12, &mUnk14, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
    u32 mUnk14;
};

class DinoBattleViewCreatureUpdateColorResult : public MMessage {
public:
    DinoBattleViewCreatureUpdateColorResult() : MMessage(0x1009B) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattlePlayerInfoSignal : public MMessage {
public:
    DinoBattlePlayerInfoSignal() : MMessage(0x1009D) {}
    DinoBattlePlayerInfoSignal(u32 player, DinoPlayerData data, DinoTeam team, const DinoCreatureState *creatures)
        : MMessage(0x1009D) {
        mPlayer = player;
        mData = data;
        mTeam = team;
        MI_CpuCopy8(creatures, mCreatures, sizeof(mCreatures));
    }
    virtual u32 GetSize() { return 276; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mPlayer, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mData, (u8 *)buf + 8, 108);
        MI_CpuCopy8(&mTeam, (u8 *)buf + 116, 40);
        MI_CpuCopy8(mCreatures, (u8 *)buf + 156, 120);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mPlayer, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mData, 108);
        MI_CpuCopy8((const u8 *)buf + 116, &mTeam, 40);
        MI_CpuCopy8((const u8 *)buf + 156, mCreatures, 120);
    }

    u32 mPlayer;                    // 0x0C
    DinoPlayerData mData;           // 0x10
    DinoTeam mTeam;                 // 0x7C
    DinoCreatureState mCreatures[5];    // 0xA4
};

class DinoBattlePlayerLevelSignal : public MMessage {
public:
    DinoBattlePlayerLevelSignal() : MMessage(0x1009C) {}
    DinoBattlePlayerLevelSignal(u32 player, u32 level) : MMessage(0x1009C) {
        mUnk0C = player;
        mUnk10 = level;
    }
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 4);
    }

    u32 mUnk0C;
    u32 mUnk10;
};

class DinoBattleReadyToBeginBattleSignal : public MMessage {
public:
    DinoBattleReadyToBeginBattleSignal() : MMessage(0x1009E) {}
    DinoBattleReadyToBeginBattleSignal(u32 player) : MMessage(0x1009E) { mUnk0C = player; }
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
    }

    u32 mUnk0C;
};

class DinoBattleReadyToContinueBattleSignal : public MMessage {
public:
    DinoBattleReadyToContinueBattleSignal() : MMessage(0x100A4) {}
    DinoBattleReadyToContinueBattleSignal(u32 player, u8 cont) : MMessage(0x100A4) {
        mUnk0C = player;
        mUnk10 = cont;
    }
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 1);
    }

    u32 mUnk0C;
    u8 mUnk10;
    u8 mPad11[0x3];
};

class DinoBattleModelLiveEventSignal : public MMessage {
public:
    DinoBattleModelLiveEventSignal() : MMessage(0x1009F) {}
    DinoBattleModelLiveEventSignal(u8 arg) : MMessage(0x1009F) {
        mUnk0C = arg;
    }
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 1);
    }

    u8 mUnk0C;
    u8 mPad0D[0x3];
};

class DinoBattleViewCreatureViewInfoMoveRequest : public MMessage {
public:
    DinoBattleViewCreatureViewInfoMoveRequest() : MMessage(0x100A0) {}
    DinoBattleViewCreatureViewInfoMoveRequest(MMessageObserver *reply, u8 arg)
        : MMessage(0x100A0), mUnk0C((u32)reply), mUnk10(arg) {}
    virtual u32 GetSize() { return 12; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk10, (u8 *)buf + 8, 1);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
        MI_CpuCopy8((const u8 *)buf + 8, &mUnk10, 1);
    }

    u32 mUnk0C;
    u8 mUnk10;
    u8 mPad11[0x3];
};

class DinoBattleViewCreatureViewInfoMoveResult : public MMessage {
public:
    DinoBattleViewCreatureViewInfoMoveResult() : MMessage(0x100A1) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleView3DGameSetMotionCancelRequest : public MMessage {
public:
    DinoBattleView3DGameSetMotionCancelRequest() : MMessage(0x100A2) {}
    DinoBattleView3DGameSetMotionCancelRequest(u32 unk0C) : MMessage(0x100A2), mUnk0C(unk0C) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        mUnk0C = 0;
    }

    u32 mUnk0C;
};

class DinoBattleView3DGameSetMotionCancelResult : public MMessage {
public:
    DinoBattleView3DGameSetMotionCancelResult() : MMessage(0x100A3) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoWirelessCannonKasekiRockSignal : public MMessage {
public:
    DinoWirelessCannonKasekiRockSignal() : MMessage(0x100A5) {}
    DinoWirelessCannonKasekiRockSignal(DinoBackupEntry entry) : MMessage(0x100A5) {
        *(DinoBackupEntry *)mUnk0C = entry;
        mUnk20 = 0;
        mUnk21 = 0;
        mUnk24 = 0;
    }
    virtual u32 GetSize() { return 32; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(mUnk0C, (u8 *)buf + 4, 20);
        MI_CpuCopy8(&mUnk20, (u8 *)buf + 24, 1);
        MI_CpuCopy8(&mUnk21, (u8 *)buf + 25, 1);
        MI_CpuCopy8(&mUnk24, (u8 *)buf + 26, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, mUnk0C, 20);
        MI_CpuCopy8((const u8 *)buf + 24, &mUnk20, 1);
        MI_CpuCopy8((const u8 *)buf + 25, &mUnk21, 1);
        MI_CpuCopy8((const u8 *)buf + 26, &mUnk24, 4);
    }

    u8 mUnk0C[0x14];
    u8 mUnk20;
    u8 mUnk21;
    u8 mPad22[0x2];
    u32 mUnk24;
};

class DinoWirelessCannonPingSignal : public MMessage {
public:
    DinoWirelessCannonPingSignal() : MMessage(0x100A6) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoWirelessCannonPongSignal : public MMessage {
public:
    DinoWirelessCannonPongSignal() : MMessage(0x100A7) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoExchangeKasekiRockSignal : public MMessage {
public:
    DinoExchangeKasekiRockSignal() : MMessage(0x100A8) {}
    DinoExchangeKasekiRockSignal(DinoBackupEntry entry) : MMessage(0x100A8) {
        *(DinoBackupEntry *)mUnk0C = entry;
    }
    virtual u32 GetSize() { return 24; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(mUnk0C, (u8 *)buf + 4, 20);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, mUnk0C, 20);
    }

    u8 mUnk0C[0x14];
};

class DinoExchangeDoneSignal : public MMessage {
public:
    DinoExchangeDoneSignal() : MMessage(0x100AA) {}
    DinoExchangeDoneSignal(u32 index) : MMessage(0x100AA) { mUnk0C = index; }
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
    }

    u32 mUnk0C;
};

class DinoExchangeSenryuSignal : public MMessage {
public:
    DinoExchangeSenryuSignal() : MMessage(0x100A9) {}
    DinoExchangeSenryuSignal(DinoPlayerProfile profile) : MMessage(0x100A9) {
        *(DinoPlayerProfile *)mUnk0C = profile;
    }
    virtual u32 GetSize() { return 136; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(mUnk0C, (u8 *)buf + 4, 132);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, mUnk0C, 132);
    }

    u8 mUnk0C[0x84];
};

// A task that waits for a number of signals of one type, then posts a message to an observer and exits.
class DinoSignalWait : public MTask, public MMessageObserver {
public:
    DinoSignalWait(u32 id, MMessageObserver *target, MMessage *msg, int count);
    virtual ~DinoSignalWait() {
        if (mMsg != NULL) {
            mMsg->Release();
        }
    }
    virtual void Execute() { Dispatch(); }
    virtual void OnMessage(MMessage *msg) {
        if (msg->mId == mId && --mCount <= 0) {
            if (mTarget != NULL) {
                mTarget->Post(mMsg);
            }
            Exit();
        }
    }

    u32 mId;                    // 0x44: the signal waited for
    int mCount;                 // 0x48: signals still to come
    MMessageObserver *mTarget;  // 0x4C
    MMessage *mMsg;             // 0x50: posted to the target at the end
    u32 mSerial;                // 0x54
};

// Posts the request, then waits for the signal that answers it (the next message id), which goes to the waiter
// the request names.
template <class T> MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, T *msg) {
    MParallelTaskSet *tasks = new MParallelTaskSet(TRUE);
    DinoSignalWait *wait = new DinoSignalWait(0, NULL, NULL, 1);
    MTask *post = target->CreatePostTask(msg);
    tasks->AddTail(wait);
    tasks->AddTail(post);
    msg->mUnk0C = (u32)(MMessageObserver *)wait;
    wait->mId = msg->mId + 1;
    return tasks;
}

// The same, posting to an observer given as its derived class.
template <class O, class T> MTask *DinoCreatePostAndWaitTask(O *target, T *msg) {
    MParallelTaskSet *tasks = new MParallelTaskSet(TRUE);
    DinoSignalWait *wait = new DinoSignalWait(0, NULL, NULL, 1);
    MTask *post = target->CreatePostTask(msg);
    tasks->AddTail(wait);
    tasks->AddTail(post);
    msg->mUnk0C = (u32)(MMessageObserver *)wait;
    wait->mId = msg->mId + 1;
    return tasks;
}

// Registers the game's message types.
void RegisterDinoMessages();

#endif
