// Message types for wireless play and download play, and the factory that creates them from received data.

#include "m/multi_message.h"

// Creates a message from its serialized form.
MMessage *CreateMultiMessage(const void *data) {
    MMessage *msg;

    switch (*(const u32 *)data) {
    default:
        return NULL;
    case 0x1A:
        msg = new MMultiPlayAnotherGameParentFoundMessage();
        break;
    case 0x0:
        msg = new MMultiPlayParentFoundMessage();
        break;
    case 0x1:
        msg = new MMultiPlayParentLostMessage();
        break;
    case 0x2:
        msg = new MMultiPlayParentUpdatedMessage();
        break;
    case 0x3:
        msg = new MMultiPlayParentConnectedMessage();
        break;
    case 0x4:
        msg = new MMultiPlayParentDisconnectedMessage();
        break;
    case 0x5:
        msg = new MMultiPlayChildConnectedMessage();
        break;
    case 0x6:
        msg = new MMultiPlayChildDisconnectedMessage();
        break;
    case 0x7:
        msg = new MMultiPlayFatalErrorMessage();
        break;
    case 0x8:
        msg = new MMultiPlayResetMessage();
        break;
    case 0x9:
        msg = new MMultiPlayInitializedMessage();
        break;
    case 0xA:
        msg = new MMultiPlayFinishedMessage();
        break;
    case 0xB:
        msg = new MMultiPlayMpStartedMessage();
        break;
    case 0xC:
        msg = new MMultiPlaySynchronizedMessage();
        break;
    case 0xD:
        msg = new MMultiBootInitializeMessage();
        break;
    case 0xE:
        msg = new MMultiBootFinishedMessage();
        break;
    case 0xF:
        msg = new MMultiBootChildConnectedMessage();
        break;
    case 0x10:
        msg = new MMultiBootChildDisconnectedMessage();
        break;
    case 0x11:
        msg = new MMultiBootChildRequestedMessage();
        break;
    case 0x12:
        msg = new MMultiBootChildRequestRejectedMessage();
        break;
    case 0x13:
        msg = new MMultiBootChildRequestAcceptedMessage();
        break;
    case 0x14:
        msg = new MMultiBootChildWaitToSendMessage();
        break;
    case 0x15:
        msg = new MMultiBootChildSendingMessage();
        break;
    case 0x16:
        msg = new MMultiBootChildSendedMessage();
        break;
    case 0x17:
        msg = new MMultiBootChildBootingMessage();
        break;
    case 0x18:
        msg = new MMultiBootChildBootedMessage();
        break;
    case 0x19:
        msg = new MMultiBootFatalErrorMessage();
        break;
    case 0x1B:
        msg = new MMultiBootFakeChildInitializeMessage();
        break;
    case 0x1C:
        msg = new MMultiBootFakeChildFinishedMessage();
        break;
    case 0x1D:
        msg = new MMultiBootFakeChildFatalErrorMessage();
        break;
    case 0x1E:
        msg = new MMultiBootFakeChildParentFoundMessage();
        break;
    case 0x1F:
        msg = new MMultiBootFakeChildParentLostMessage();
        break;
    case 0x20:
        msg = new MMultiBootFakeChildParentUpdatedMessage();
        break;
    }
    msg->Read(data);
    return msg;
}
