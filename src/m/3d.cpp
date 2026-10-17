// M3d (the 3D engine's layer), M3DVInt, M3dWaitTransferCompleteTask.
// Only the destructors are decompiled so far.

#include "m/3d.h"
#include "m/3d_wait_transfer_complete_task.h"

M3DVInt::~M3DVInt() {
    MScopedLock lock(&gM3dMutex, TRUE);
    Disable();
}

M3dWaitTransferCompleteTask::~M3dWaitTransferCompleteTask() {
}
