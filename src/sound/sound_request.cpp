// Queue of delayed sound requests, and the global functions the sound API uses to reach it.
// Built as ARM code.

#pragma thumb off

#include "sound.h"

SoundRequestQueue::SoundRequestQueue() {
    Init();
}

SoundRequestQueue::~SoundRequestQueue() {
}

// Node 0 heads the circular free list (2..255), node 1 the circular pending list (empty).
void SoundRequestQueue::Init() {
    int i;

    nodes[SOUND_REQUEST_FREE_HEAD].prev = SOUND_REQUEST_COUNT - 1;
    nodes[SOUND_REQUEST_FREE_HEAD].next = 2;
    nodes[SOUND_REQUEST_LIST_HEAD].prev = SOUND_REQUEST_LIST_HEAD;
    nodes[SOUND_REQUEST_LIST_HEAD].next = SOUND_REQUEST_LIST_HEAD;
    for (i = 2; i < SOUND_REQUEST_COUNT; i++) {
        nodes[i].prev = i - 1;
        nodes[i].next = i + 1;
    }
    nodes[2].prev = SOUND_REQUEST_FREE_HEAD;
    nodes[SOUND_REQUEST_COUNT - 1].next = SOUND_REQUEST_FREE_HEAD;
    cursor = 0;
}

void SoundRequestQueue::InsertAfter(int node, int at) {
    nodes[nodes[at].next].prev = node;
    nodes[node].next = nodes[at].next;
    nodes[node].prev = at;
    nodes[at].next = node;
}

void SoundRequestQueue::Unlink(int node) {
    nodes[nodes[node].next].prev = nodes[node].prev;
    nodes[nodes[node].prev].next = nodes[node].next;
}

int SoundRequestQueue::PeekFree() {
    int node = nodes[SOUND_REQUEST_FREE_HEAD].next;

    if (node == SOUND_REQUEST_FREE_HEAD) {
        node = -1;
    }
    return node;
}

void SoundRequestQueue::Push(int type, int delay, int arg0, int arg1, int arg2, int arg3) {
    int node = PeekFree();
    SoundRequest *req = &nodes[node];

    req->type = type;
    req->args[0] = arg0;
    req->args[1] = arg1;
    req->args[2] = arg2;
    req->args[3] = arg3;
    req->delay = delay;
    Unlink(node);
    InsertAfter(node, nodes[SOUND_REQUEST_LIST_HEAD].prev);
}

// Moves the node back to the free list, stepping the cursor past it first.
void SoundRequestQueue::Remove(int node) {
    Next();
    Unlink(node);
    InsertAfter(node, nodes[SOUND_REQUEST_FREE_HEAD].prev);
}

void SoundRequestQueue::Begin() {
    cursor = nodes[SOUND_REQUEST_LIST_HEAD].next;
}

void SoundRequestQueue::Tick() {
    if (nodes[cursor].delay == 0) {
        return;
    }
    nodes[cursor].delay--;
}

int SoundRequestQueue::GetDelay() {
    return nodes[cursor].delay;
}

BOOL SoundRequestQueue::AtEnd() {
    return cursor == SOUND_REQUEST_LIST_HEAD;
}

int SoundRequestQueue::GetCursor() {
    return cursor;
}

SoundRequest *SoundRequestQueue::GetCurrent() {
    return &nodes[cursor];
}

void SoundRequestQueue::Next() {
    cursor = nodes[cursor].next;
}

static SoundRequestQueue gSoundRequests;

void PushSoundRequest(int type, int delay, int arg0, int arg1, int arg2, int arg3) {
    gSoundRequests.Push(type, delay, arg0, arg1, arg2, arg3);
}

void RemoveSoundRequest(int node) {
    gSoundRequests.Remove(node);
}

void BeginSoundRequests() {
    gSoundRequests.Begin();
}

void TickSoundRequest() {
    gSoundRequests.Tick();
}

int GetSoundRequestDelay() {
    return gSoundRequests.GetDelay();
}

BOOL AtSoundRequestsEnd() {
    return gSoundRequests.AtEnd();
}

int GetSoundRequestCursor() {
    return gSoundRequests.GetCursor();
}

SoundRequest *GetCurrentSoundRequest() {
    return gSoundRequests.GetCurrent();
}

void NextSoundRequest() {
    gSoundRequests.Next();
}
