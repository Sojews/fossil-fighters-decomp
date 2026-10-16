// Messages passed between the game's processes (battle model, view and controller) and over wireless.
#ifndef GAME_DINO_MESSAGE_H
#define GAME_DINO_MESSAGE_H

#include "m/message.h"

class DinoDummyMessage : public MMessage {
public:
    DinoDummyMessage();
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
    DinoBattleModelGameBeginSignal();
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
    DinoBattleControllerInitRequest();
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
    DinoBattleControllerInitResult();
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
    DinoBattleModelPlayerBeginSignal();
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
    DinoBattleModelCreatureBeginSignal();
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
    u8 mUnk14[0x18];
    u32 mUnk2C;
    u32 mUnk30;
};

class DinoBattleViewCreatureBeginSignal : public MMessage {
public:
    DinoBattleViewCreatureBeginSignal();
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
    DinoBattleViewPlayerBeginSignal();
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
    DinoBattleViewEffectGameBeginRequest();
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
    DinoBattleViewEffectGameBeginResult();
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
    DinoBattleViewEffectTurnBeginRequest();
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
    DinoBattleViewEffectTurnBeginResult();
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
    DinoBattleControllerWakeupSignal();
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
    DinoBattleModelPlayerTurnBeginRequst();
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
    DinoBattleModelPlayerTurnBeginResult();
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
    DinoBattleViewUiRootMenuRequest();
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
    DinoBattleViewUiRootMenuResult();
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
    DinoBattleModelCreatureMoveRequest();
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
    DinoBattleModelCreatureMoveResult();
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
    DinoBattleModelTurnChangeSignal();
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
    DinoBattleModelTurnEndRequest();
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
    DinoBattleModelTurnEndResult();
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
    DinoBattleViewUiChangeMenuRequest();
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
    DinoBattleViewUiChangeMenuResult();
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
    DinoBattleViewUiCreatureInfoMenuRequest();
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
    DinoBattleViewUiCreatureInfoMenuResult();
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
    DinoBattleModelGoEscapeAreaRequest();
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
    DinoBattleModelGoEscapeAreaResult();
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
    DinoBattleViewCreatureMoveRequest();
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
    DinoBattleViewCreatureMoveResult();
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
    DinoBattleViewUiAttackerMenuRequest();
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
    DinoBattleViewUiAttackerMenuResult();
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
    DinoBattleViewUiDefenderMenuRequest();
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
    DinoBattleViewUiDefenderMenuResult();
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
    DinoBattleViewUiAbilityMenuRequest();
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
    DinoBattleViewUiAbilityMenuResult();
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
    DinoBattleModelCreatureAbilityRequest();
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
    DinoBattleModelCreatureAbilityResult();
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
    DinoBattleViewSetActionedSignal();
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
    DinoBattleViewCreatureCpuArrowUpdateSignal();
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
    DinoBattleViewCreatureAbilityRequest();
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
    DinoBattleViewCreatureAbilityResult();
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
    DinoBattleView3DTeamAttackRequest();
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
    u8 mUnk20[0xC];
    u8 mUnk2C[0xC];
    u8 mUnk38[0xC];
    u8 mUnk44[0x3];
    u8 mPad47[0x1];
};

class DinoBattleView3DTeamAttackResult : public MMessage {
public:
    DinoBattleView3DTeamAttackResult();
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
    DinoBattleViewCreatureUpdateRequest();
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
    DinoBattleViewCreatureAbility3DRequest();
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
    u8 mUnk2C[0xC];
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
    DinoBattleViewCreatureAbility3DResult();
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
    DinoBattleViewCreatureUpdateResult();
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
    DinoBattleModelCreatureEndRequest();
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
    DinoBattleModelCreatureEndResult();
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
    DinoBattleViewCreatureEndRequest();
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
    DinoBattleViewCreatureEndResult();
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
    DinoBattleModelGameEndSignal();
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
    DinoBattleViewEffectGameEndRequest();
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
    DinoBattleViewEffectGameEndResult();
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
    DinoBattleViewPlayerUpdateRequest();
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
    DinoBattleViewPlayerUpdateResult();
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
    DinoBattleModelCreatureTurnBeginRequest();
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
    DinoBattleModelCreatureTurnBeginResult();
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
    DinoBattleModelPlayerTurnBeginProcessRequst();
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
    DinoBattleModelPlayerTurnBeginProcessResult();
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
    DinoBattleModelCreatureSpecialLifeRecoveryRequest();
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
    DinoBattleModelCreatureSpecialLifeRecoveryResult();
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
    DinoBattleModelEvolutionCreatureRequest();
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
    DinoBattleModelEvolutionCreatureResult();
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
    DinoBattleViewEvolutionCreatureRequest();
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
    DinoBattleViewEvolutionCreatureResult();
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
    DinoBattleViewCreatureCounterDamageRequest();
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
    DinoBattleViewCreatureCounterDamageResult();
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
    DinoBattleViewPlayerUpdateStateIconRequest();
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
    DinoBattleViewPlayerUpdateStateIconResult();
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
    DinoBattleViewDeleteStatusEffectSignal();
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
    DinoBattleViewPlayerUpdateAbilityIconRequest();
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
    DinoBattleViewPlayerUpdateAbilityIconResult();
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
    DinoBattleViewUiTurnEndRequest();
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
    DinoBattleViewUiTurnEndResult();
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
    DinoBattleModelPlayerConfusionExecRequest();
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
    DinoBattleModelPlayerConfusionExecResult();
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
    DinoBattleModelCreatureSupportEffectRequest();
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
    DinoBattleModelCreatureSupportEffectResult();
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
    DinoBattleViewCreatureSupportEffectRequest();
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
    DinoBattleViewCreatureSupportEffectResult();
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
    DinoBattleViewCreatureMotionEffectRequest();
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
    DinoBattleViewCreatureMotionEffectResult();
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
    DinoBattleViewFieldEffectWaitRequest();
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
    DinoBattleViewFieldEffectWaitResult();
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
    DinoBattleViewCreaturePlayMotionRequest();
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
    DinoBattleViewCreaturePlayMotionResult();
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class DinoBattleViewFreeCoordEffectRequest : public MMessage {
public:
    DinoBattleViewFreeCoordEffectRequest();
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
    DinoBattleViewFreeCoordEffectResult();
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
    DinoBattleModelCreatureReleaseStatusEffectRequest();
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
    DinoBattleModelCreatureReleaseStatusEffectResult();
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
    DinoBattleViewCreatureTurnBeginSignal();
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
    DinoBattleViewCreatureTurnEndSignal();
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
    DinoBattleViewCreatureDamagedUpdateRequest();
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
    DinoBattleViewCreatureDamagedUpdateResult();
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
    DinoBattleViewWaitUpdateSupportEffectAnimeRequest();
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
    DinoBattleViewWaitUpdateSupportEffectAnimeResult();
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
    DinoBattleViewActionInfoBarUpdateRequest();
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
    DinoBattleViewActionInfoBarUpdateResult();
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
    DinoBattleViewFocusLCDFrameRequest();
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
    DinoBattleViewFocusLCDFrameResult();
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
    DinoBattleViewCreatureSetBlendRequest();
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
    DinoBattleViewCreatureSetBlendResult();
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
    DinoBattleViewLCDFadeRequest();
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
    DinoBattleViewLCDFadeResult();
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
    DinoBattleViewWaitActionInfoBarRequest();
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
    DinoBattleViewWaitActionInfoBarResult();
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
    DinoBattleViewClearAttackBarRequest();
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
    DinoBattleViewClearAttackBarResult();
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
    DinoBattleViewShowAttackNameBarRequest();
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
    DinoBattleViewMoveAttackNameBarRequest();
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
    DinoBattleViewMoveAttackNameBarResult();
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
    DinoBattleViewShowAttackNameBarResult();
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
    DinoBattleView3DSupportEffectRequest();
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
    DinoBattleView3DSupportEffectResult();
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
    DinoBattleView3DWatchStatusRequest();
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
    DinoBattleView3DWatchStatusResult();
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
    DinoBattleView3DPoisonDamageRequest();
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
    DinoBattleView3DPoisonDamageResult();
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
    DinoBattleView3DPreTurnEndRequest();
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
    DinoBattleView3DPreTurnEndResult();
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
    DinoBattleView3DTurnEndRequest();
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
    DinoBattleView3DTurnEndResult();
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
    DinoBattleViewSetCreaturePriorityRequest();
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
    DinoBattleViewSetCreaturePriorityResult();
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
    DinoBattleViewCreaturePlayAttackMotionRequest();
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
    DinoBattleViewCreaturePlayAttackMotionResult();
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
    DinoBattleViewChangeStatusEffectIconRequest();
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
    DinoBattleViewPopNumberRedIfZeroSignal();
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
    DinoBattleViewChangeStatusEffectIconResult();
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
    DinoBattleViewStatusEffectIconBlendClearSignal();
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
    DinoBattleViewTutorialWindowRequest();
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
    DinoBattleViewTutorialWindowResult();
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
    DinoBattleViewTutorialSetArroeRequest();
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
    DinoBattleViewTutorialSetArroeResult();
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
    DinoBattleViewTutorialSetRectRequest();
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
    DinoBattleViewTutorialSetRectResult();
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
    DinoBattleView3DGameSetRequest();
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
    DinoBattleView3DGameSetResult();
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
    DinoBattleViewLiveMessageSignal();
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
    DinoBattleViewDecideFirstAttackTeamRequest();
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
    DinoBattleViewDecideFirstAttackTeamResult();
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
    DinoBattleView3DRevolutionSetRequest();
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
    DinoBattleView3DRevolutionSetResult();
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
    DinoBattleViewPlayerWaitKPCountStopRequest();
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
    DinoBattleViewPlayerWaitKPCountStopResult();
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
    DinoBattleModelTutorialGameEndSignal();
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
    DinoBattleViewSetAttackBarUpperLCDRequest();
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
    DinoBattleViewSetAttackBarUpperLCDResult();
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
    DinoBattleViewCountPlayerThinkTimeSignal();
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
    DinoBattleViewCreatureActionSignal();
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
    DinoBattleViewCreatureActionUpdateSignal();
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
    DinoBattleViewCreatureUpdateColorRequest();
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
    DinoBattleViewCreatureUpdateColorResult();
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
    DinoBattlePlayerInfoSignal();
    virtual u32 GetSize() { return 276; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(mUnk10, (u8 *)buf + 8, 108);
        MI_CpuCopy8(mUnk7C, (u8 *)buf + 116, 40);
        MI_CpuCopy8(mUnkA4, (u8 *)buf + 156, 120);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, mUnk10, 108);
        MI_CpuCopy8((const u8 *)buf + 116, mUnk7C, 40);
        MI_CpuCopy8((const u8 *)buf + 156, mUnkA4, 120);
    }

    u32 mUnk0C;
    u8 mUnk10[0x6C];
    u8 mUnk7C[0x28];
    u8 mUnkA4[0x78];
};

class DinoBattlePlayerLevelSignal : public MMessage {
public:
    DinoBattlePlayerLevelSignal();
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
    DinoBattleReadyToBeginBattleSignal();
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
    DinoBattleReadyToContinueBattleSignal();
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
    DinoBattleModelLiveEventSignal();
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
    DinoBattleViewCreatureViewInfoMoveRequest();
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
    DinoBattleViewCreatureViewInfoMoveResult();
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
    DinoBattleView3DGameSetMotionCancelRequest();
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
    DinoBattleView3DGameSetMotionCancelResult();
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
    DinoWirelessCannonKasekiRockSignal();
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
    DinoWirelessCannonPingSignal();
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
    DinoWirelessCannonPongSignal();
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
    DinoExchangeKasekiRockSignal();
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
    DinoExchangeDoneSignal();
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
    DinoExchangeSenryuSignal();
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

#endif
