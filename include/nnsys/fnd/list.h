// NitroSystem foundation: intrusive doubly linked lists. Each object holds an NNSFndLink at the list's offset.
#ifndef NNSYS_FND_LIST_H
#define NNSYS_FND_LIST_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *prevObject;
    void *nextObject;
} NNSFndLink;

typedef struct {
    void *headObject;
    void *tailObject;
    u16 numObjects;
    u16 offset;
} NNSFndList;

void NNS_FndInitList(NNSFndList *list, u16 offset);
void NNS_FndAppendListObject(NNSFndList *list, void *object);
void NNS_FndPrependListObject(NNSFndList *list, void *object);
void NNS_FndInsertListObject(NNSFndList *list, void *target, void *object);
void NNS_FndRemoveListObject(NNSFndList *list, void *object);
void *NNS_FndGetNextListObject(NNSFndList *list, void *object);
void *NNS_FndGetPrevListObject(NNSFndList *list, void *object);

#ifdef __cplusplus
}
#endif

#endif
