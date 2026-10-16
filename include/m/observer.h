// Subjects and observers: a subject sends messages to the observers attached to it, which queue them until
// they're dispatched.
#ifndef M_OBSERVER_H
#define M_OBSERVER_H

#include <vector>
#include "m/message.h"
#include "m/task.h"

class MMessageSubject;

class MMessageObserver {
public:
    virtual void OnMessage(MMessage *msg) = 0;
    ~MMessageObserver();

    // Queues a message.
    void Post(MMessage *msg);
    void ClearQueue();
    // Passes the queued messages to OnMessage.
    void Dispatch();
    // A task that posts a message to this observer when it runs.
    MTask *CreatePostTask(MMessage *msg);

    std::vector<MMessage *> mQueue;             // 0x04
    std::vector<MMessageSubject *> mSubjects;   // 0x10
    OSMutex mMutex;                             // 0x1C
};

class MMessageSubject {
public:
    virtual ~MMessageSubject();

    void AddObserver(MMessageObserver *observer);
    void RemoveObserver(MMessageObserver *observer);
    // Posts a message to every observer.
    void Notify(MMessage *msg);

    std::vector<MMessageObserver *> mObservers; // 0x04
    OSMutex mMutex;                             // 0x10
};

class MMessageObserverSendMessageTask : public MTransfer {
public:
    MMessageObserverSendMessageTask(MMessageObserver *observer, MMessage *msg) {
        mObserver = observer;
        mMsg = msg;
        mMsg->AddRef();
    }
    virtual ~MMessageObserverSendMessageTask() { mMsg->Release(); }
    virtual void Transfer();

    MMessageObserver *mObserver;    // 0x10
    MMessage *mMsg;                 // 0x14
};

#endif
