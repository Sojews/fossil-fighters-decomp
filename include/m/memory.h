// The heap (dlmalloc in the arena) behind operator new and delete.
#ifndef M_MEMORY_H
#define M_MEMORY_H

#include <stddef.h>
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void MemInit(void *lo, void *hi);
void *MemAlloc(size_t size);
void MemFree(void *p);

#ifdef __cplusplus
}
#endif

#endif
