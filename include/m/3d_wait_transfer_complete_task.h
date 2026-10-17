#ifndef M_3D_WAIT_TRANSFER_COMPLETE_TASK_H
#define M_3D_WAIT_TRANSFER_COMPLETE_TASK_H

#include "m/3d.h"
#include "m/task.h"

class M3dWaitTransferCompleteTask : public MTask {
public:
    virtual ~M3dWaitTransferCompleteTask();
    virtual void Execute();

    M3d *mM3d;  // 0x10
};

#endif
