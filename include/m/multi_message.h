// Messages the wireless play and download play libraries send to the game.
#ifndef M_MULTI_MESSAGE_H
#define M_MULTI_MESSAGE_H

#include "m/message.h"

class MMultiPlayAnotherGameParentFoundMessage : public MMessage {
public:
    MMultiPlayAnotherGameParentFoundMessage() : MMessage(0x1A) {}
    virtual u32 GetSize() { return 128; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(mUnk10, (u8 *)buf + 8, 112);
        MI_CpuCopy8(mUnk80, (u8 *)buf + 120, 6);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, mUnk10, 112);
        MI_CpuCopy8((const u8 *)buf + 120, mUnk80, 6);
    }

    u32 mUnk0C;
    u8 mUnk10[0x70];
    u8 mUnk80[0x6];
    u8 mPad86[0x2];
};

class MMultiPlayParentFoundMessage : public MMessage {
public:
    MMultiPlayParentFoundMessage() : MMessage(0x0) {}
    virtual u32 GetSize() { return 128; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(mUnk10, (u8 *)buf + 8, 112);
        MI_CpuCopy8(mUnk80, (u8 *)buf + 120, 6);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, mUnk10, 112);
        MI_CpuCopy8((const u8 *)buf + 120, mUnk80, 6);
    }

    u32 mUnk0C;
    u8 mUnk10[0x70];
    u8 mUnk80[0x6];
    u8 mPad86[0x2];
};

class MMultiPlayParentLostMessage : public MMessage {
public:
    MMultiPlayParentLostMessage() : MMessage(0x1) {}
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

class MMultiPlayParentUpdatedMessage : public MMessage {
public:
    MMultiPlayParentUpdatedMessage() : MMessage(0x2) {}
    virtual u32 GetSize() { return 120; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(mUnk10, (u8 *)buf + 8, 112);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, mUnk10, 112);
    }

    u32 mUnk0C;
    u8 mUnk10[0x70];
};

class MMultiPlayParentConnectedMessage : public MMessage {
public:
    MMultiPlayParentConnectedMessage() : MMessage(0x3) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiPlayParentDisconnectedMessage : public MMessage {
public:
    MMultiPlayParentDisconnectedMessage() : MMessage(0x4) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiPlayChildConnectedMessage : public MMessage {
public:
    MMultiPlayChildConnectedMessage() : MMessage(0x5) {}
    virtual u32 GetSize() { return 36; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
        MI_CpuCopy8(mUnk0E, (u8 *)buf + 6, 24);
        MI_CpuCopy8(mUnk26, (u8 *)buf + 30, 6);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
        MI_CpuCopy8((const u8 *)buf + 6, mUnk0E, 24);
        MI_CpuCopy8((const u8 *)buf + 30, mUnk26, 6);
    }

    u16 mUnk0C;
    u8 mUnk0E[0x18];
    u8 mUnk26[0x6];
};

class MMultiPlayChildDisconnectedMessage : public MMessage {
public:
    MMultiPlayChildDisconnectedMessage() : MMessage(0x6) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
    }

    u16 mUnk0C;
    u8 mPad0E[0x2];
};

class MMultiPlayFatalErrorMessage : public MMessage {
public:
    MMultiPlayFatalErrorMessage() : MMessage(0x7) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiPlayResetMessage : public MMessage {
public:
    MMultiPlayResetMessage() : MMessage(0x8) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiPlayInitializedMessage : public MMessage {
public:
    MMultiPlayInitializedMessage() : MMessage(0x9) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiPlayFinishedMessage : public MMessage {
public:
    MMultiPlayFinishedMessage() : MMessage(0xA) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiPlayMpStartedMessage : public MMessage {
public:
    MMultiPlayMpStartedMessage() : MMessage(0xB) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiPlaySynchronizedMessage : public MMessage {
public:
    MMultiPlaySynchronizedMessage() : MMessage(0xC) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiBootInitializeMessage : public MMessage {
public:
    MMultiBootInitializeMessage() : MMessage(0xD) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiBootFinishedMessage : public MMessage {
public:
    MMultiBootFinishedMessage() : MMessage(0xE) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiBootChildConnectedMessage : public MMessage {
public:
    MMultiBootChildConnectedMessage() : MMessage(0xF) {}
    virtual u32 GetSize() { return 32; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
        MI_CpuCopy8(mUnk0E, (u8 *)buf + 6, 24);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
        MI_CpuCopy8((const u8 *)buf + 6, mUnk0E, 24);
    }

    u16 mUnk0C;
    u8 mUnk0E[0x18];
    u8 mPad26[0x2];
};

class MMultiBootChildDisconnectedMessage : public MMessage {
public:
    MMultiBootChildDisconnectedMessage() : MMessage(0x10) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
    }

    u16 mUnk0C;
    u8 mPad0E[0x2];
};

class MMultiBootChildRequestedMessage : public MMessage {
public:
    MMultiBootChildRequestedMessage() : MMessage(0x11) {}
    virtual u32 GetSize() { return 28; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
        MI_CpuCopy8(mUnk0E, (u8 *)buf + 6, 22);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
        MI_CpuCopy8((const u8 *)buf + 6, mUnk0E, 22);
    }

    u16 mUnk0C;
    u8 mUnk0E[0x16];
};

class MMultiBootChildRequestRejectedMessage : public MMessage {
public:
    MMultiBootChildRequestRejectedMessage() : MMessage(0x12) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
    }

    u16 mUnk0C;
    u8 mPad0E[0x2];
};

class MMultiBootChildRequestAcceptedMessage : public MMessage {
public:
    MMultiBootChildRequestAcceptedMessage() : MMessage(0x13) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
    }

    u16 mUnk0C;
    u8 mPad0E[0x2];
};

class MMultiBootChildWaitToSendMessage : public MMessage {
public:
    MMultiBootChildWaitToSendMessage() : MMessage(0x14) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
    }

    u16 mUnk0C;
    u8 mPad0E[0x2];
};

class MMultiBootChildSendingMessage : public MMessage {
public:
    MMultiBootChildSendingMessage() : MMessage(0x15) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
    }

    u16 mUnk0C;
    u8 mPad0E[0x2];
};

class MMultiBootChildSendedMessage : public MMessage {
public:
    MMultiBootChildSendedMessage() : MMessage(0x16) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
    }

    u16 mUnk0C;
    u8 mPad0E[0x2];
};

class MMultiBootChildBootingMessage : public MMessage {
public:
    MMultiBootChildBootingMessage() : MMessage(0x17) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
    }

    u16 mUnk0C;
    u8 mPad0E[0x2];
};

class MMultiBootChildBootedMessage : public MMessage {
public:
    MMultiBootChildBootedMessage() : MMessage(0x18) {}
    virtual u32 GetSize() { return 8; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 2);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 2);
    }

    u16 mUnk0C;
    u8 mPad0E[0x2];
};

class MMultiBootFatalErrorMessage : public MMessage {
public:
    MMultiBootFatalErrorMessage() : MMessage(0x19) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiBootFakeChildInitializeMessage : public MMessage {
public:
    MMultiBootFakeChildInitializeMessage() : MMessage(0x1B) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiBootFakeChildFinishedMessage : public MMessage {
public:
    MMultiBootFakeChildFinishedMessage() : MMessage(0x1C) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiBootFakeChildFatalErrorMessage : public MMessage {
public:
    MMultiBootFakeChildFatalErrorMessage() : MMessage(0x1D) {}
    virtual u32 GetSize() { return 4; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
    }

};

class MMultiBootFakeChildParentFoundMessage : public MMessage {
public:
    MMultiBootFakeChildParentFoundMessage() : MMessage(0x1E) {}
    virtual u32 GetSize() { return 1232; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(mUnk10, (u8 *)buf + 8, 1216);
        MI_CpuCopy8(mUnk4D0, (u8 *)buf + 1224, 6);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, mUnk10, 1216);
        MI_CpuCopy8((const u8 *)buf + 1224, mUnk4D0, 6);
    }

    u32 mUnk0C;
    u8 mUnk10[0x4C0];
    u8 mUnk4D0[0x6];
    u8 mPad4D6[0x2];
};

class MMultiBootFakeChildParentLostMessage : public MMessage {
public:
    MMultiBootFakeChildParentLostMessage() : MMessage(0x1F) {}
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

class MMultiBootFakeChildParentUpdatedMessage : public MMessage {
public:
    MMultiBootFakeChildParentUpdatedMessage() : MMessage(0x20) {}
    virtual u32 GetSize() { return 1224; }
    virtual void Write(void *buf) {
        MI_CpuCopy8(&mId, buf, 4);
        MI_CpuCopy8(&mUnk0C, (u8 *)buf + 4, 4);
        MI_CpuCopy8(mUnk10, (u8 *)buf + 8, 1216);
    }
    virtual void Read(const void *buf) {
        MI_CpuCopy8(buf, &mId, 4);
        MI_CpuCopy8((const u8 *)buf + 4, &mUnk0C, 4);
        MI_CpuCopy8((const u8 *)buf + 8, mUnk10, 1216);
    }

    u32 mUnk0C;
    u8 mUnk10[0x4C0];
};

#endif
