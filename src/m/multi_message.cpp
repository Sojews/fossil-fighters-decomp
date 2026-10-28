// Message types for wireless play and download play, and the factory that creates them from received data.

#pragma ipa file

#include <vector>
#include "m/multi_message.h"
#include <algorithm>
#include "m/observer.h"

MMessage *CreateMultiMessage(const void *data);

// (Pushed through a reference, which gives the game's order for push_back's byte temps under -ipa file.)
template <class T> static inline void MPush(std::vector<T> &v, const T &x) {
    v.push_back(x);
}

std::vector<MMessageFactory> gMessageFactories;

void RegisterMultiMessages() {
    AddMessageFactory(CreateMultiMessage);
}

void AddMessageFactory(MMessageFactory factory) {
    MPush(gMessageFactories, factory);
}

// Creates a message from its serialized form, trying the most recently added factories first.
MMessage *CreateMessage(const void *data) {
    for (std::vector<MMessageFactory>::reverse_iterator it = gMessageFactories.rbegin();
         it != gMessageFactories.rend(); ++it) {
        MMessage *msg = (*it)(data);
        if (msg != NULL) {
            return msg;
        }
    }
    return NULL;
}

void MMessage::AddRef() {
    MIrqLock lock;
    mRefCount++;
}

void MMessage::Release() {
    int count;

    {
        MIrqLock lock;
        count = --mRefCount;
    }
    if (count <= 0) {
        delete this;
    }
}

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

MMessageObserver::~MMessageObserver() {
    while (mSubjects.size() != 0) {
        mSubjects[0]->RemoveObserver(this);
    }
    ClearQueue();
}

void MMessageObserver::Post(MMessage *msg) {
    MScopedLock lock(&mMutex, TRUE);
    msg->AddRef();
    MPush(mQueue, msg);
}

void MMessageObserver::ClearQueue() {
    MScopedLock lock(&mMutex, TRUE);
    for (std::vector<MMessage *>::iterator it = mQueue.begin(); it != mQueue.end(); it++) {
        (*it)->Release();
    }
    mQueue.clear();
}

void MMessageObserver::Dispatch() {
    MScopedLock lock(&mMutex, TRUE);
    for (std::vector<MMessage *>::iterator it = mQueue.begin(); it != mQueue.end(); it++) {
        OnMessage(*it);
    }
    ClearQueue();
}

MTask *MMessageObserver::CreatePostTask(MMessage *msg) {
    return new MMessageObserverSendMessageTask(this, msg);
}

MMessageSubject::~MMessageSubject() {
    while (mObservers.size() != 0) {
        RemoveObserver(mObservers[0]);
    }
}

void MMessageSubject::AddObserver(MMessageObserver *observer) {
    MScopedLock lock(&mMutex, TRUE);
    MPush(mObservers, observer);
    MPush(observer->mSubjects, (MMessageSubject *)this);
}

// NONMATCHING: the game's stack frame is 8 bytes bigger (unused space, as if for a lock object kept in a
// register).
void MMessageSubject::RemoveObserver(MMessageObserver *observer) {
    OSMutex *mutex = &mMutex;
    OS_LockMutex(mutex);
    std::vector<MMessageObserver *>::iterator it = std::find(mObservers.begin(), mObservers.end(), observer);
    mObservers.erase(it);
    std::vector<MMessageSubject *>::iterator it2 =
        std::find(observer->mSubjects.begin(), observer->mSubjects.end(), this);
    observer->mSubjects.erase(it2);
    OS_UnlockMutex(mutex);
}

void MMessageSubject::Notify(MMessage *msg) {
    MScopedLock lock(&mMutex, TRUE);
    msg->AddRef();
    for (std::vector<MMessageObserver *>::iterator it = mObservers.begin(); it != mObservers.end(); it++) {
        (*it)->Post(msg);
    }
    msg->Release();
}

void MMessageObserverSendMessageTask::Transfer() {
    mObserver->Post(mMsg);
}
