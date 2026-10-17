// Metrowerks runtime: process exit through semihosting.

#include "types.h"

extern void func_02086d14(void);    // SYS_EXIT

// Not marked complete: the game's exception index entry for it has flags 0x100080, ours 0x000080.
void _ExitProcess(void) {
    func_02086d14();
}
