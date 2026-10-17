// Metrowerks C++ runtime: running static initializers and destroying global objects.

#include "types.h"
#include <MWException.h>

typedef void (*StaticInitializer)(void);

extern StaticInitializer ARM9_CTOR_START[];   // defined by the linker script

void __call_static_initializers(void) {
    StaticInitializer *p;

    for (p = ARM9_CTOR_START; p != NULL && *p != NULL; p++) {
        (*p)();
    }
}

void __destroy_global_chain(void) {
    DestructorChain *gdc;

    while ((gdc = __global_destructor_chain) != NULL) {
        __global_destructor_chain = gdc->next;
        DTORCALL_COMPLETE(gdc->dtor, gdc->object);
    }
}
