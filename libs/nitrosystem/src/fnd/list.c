// NitroSystem foundation: intrusive doubly linked lists.

#include "nnsys/fnd/list.h"

#define LINK(list, object) ((NNSFndLink *) ((u32) (object) + (list)->offset))

void NNS_FndInitList(NNSFndList *list, u16 offset) {
    list->headObject = NULL;
    list->tailObject = NULL;
    list->numObjects = 0;
    list->offset = offset;
}

static void SetFirstObject(NNSFndList *list, void *object) {
    NNSFndLink *link = LINK(list, object);
    link->nextObject = NULL;
    link->prevObject = NULL;
    list->headObject = object;
    list->tailObject = object;
    list->numObjects++;
}

void NNS_FndAppendListObject(NNSFndList *list, void *object) {
    if (list->headObject == NULL) {
        SetFirstObject(list, object);
    } else {
        NNSFndLink *link = LINK(list, object);
        link->prevObject = list->tailObject;
        link->nextObject = NULL;
        LINK(list, list->tailObject)->nextObject = object;
        list->tailObject = object;
        list->numObjects++;
    }
}

void NNS_FndPrependListObject(NNSFndList *list, void *object) {
    if (list->headObject == NULL) {
        SetFirstObject(list, object);
    } else {
        NNSFndLink *link = LINK(list, object);
        link->prevObject = NULL;
        link->nextObject = list->headObject;
        LINK(list, list->headObject)->prevObject = object;
        list->headObject = object;
        list->numObjects++;
    }
}

void NNS_FndInsertListObject(NNSFndList *list, void *target, void *object) {
    if (target == NULL) {
        NNS_FndAppendListObject(list, object);
    } else if (target == list->headObject) {
        NNS_FndPrependListObject(list, object);
    } else {
        NNSFndLink *link = LINK(list, object);
        u32 offset = list->offset;
        void *prev = ((NNSFndLink *) ((u32) target + offset))->prevObject;
        link->prevObject = prev;
        link->nextObject = target;
        ((NNSFndLink *) ((u32) prev + offset))->nextObject = object;
        LINK(list, target)->prevObject = object;
        list->numObjects++;
    }
}

void NNS_FndRemoveListObject(NNSFndList *list, void *object) {
    NNSFndLink *link = LINK(list, object);
    if (link->prevObject == NULL) {
        list->headObject = link->nextObject;
    } else {
        LINK(list, link->prevObject)->nextObject = link->nextObject;
    }
    if (link->nextObject == NULL) {
        list->tailObject = link->prevObject;
    } else {
        LINK(list, link->nextObject)->prevObject = link->prevObject;
    }
    link->prevObject = NULL;
    link->nextObject = NULL;
    list->numObjects--;
}

void *NNS_FndGetNextListObject(NNSFndList *list, void *object) {
    if (object == NULL) {
        return list->headObject;
    }
    return LINK(list, object)->nextObject;
}

void *NNS_FndGetPrevListObject(NNSFndList *list, void *object) {
    if (object == NULL) {
        return list->tailObject;
    }
    return LINK(list, object)->prevObject;
}
