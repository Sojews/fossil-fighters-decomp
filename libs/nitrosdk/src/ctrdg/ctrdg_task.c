// NitroSDK CTRDG: a thread that runs one task at a time (backup memory access) and calls back when it's done.

#include "ctrdg_internal.h"

static CTRDGiTaskWork *ctrdgi_task_work = NULL;
static CTRDGTaskInfo ctrdgi_task_list;
static void CTRDGi_TaskThread(void *arg);
u64 ctrdg_task_stack[CTRDG_TASK_STACK_SIZE / sizeof(u64)];

void CTRDGi_InitTaskThread(void *p_work) {
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    if (!ctrdgi_task_work) {
        CTRDGiTaskWork *const p = (CTRDGiTaskWork *) p_work;
        ctrdgi_task_work = p;
        CTRDGi_InitTaskInfo(&p->end_task);
        CTRDGi_InitTaskInfo(&ctrdgi_task_list);
        p->list = NULL;
        OS_CreateThread(p->th, CTRDGi_TaskThread, p, ctrdg_task_stack + CTRDG_TASK_STACK_SIZE / sizeof(u64),
                        CTRDG_TASK_STACK_SIZE, CTRDG_TASK_PRIORITY_DEFAULT);
        OS_WakeupThreadDirect(p->th);
    }
    OS_RestoreInterrupts(bak_cpsr);
}

void CTRDGi_InitTaskInfo(CTRDGTaskInfo *pt) {
    MI_CpuFill8(pt, 0, sizeof(*pt));
}

static void CTRDGi_TaskThread(void *arg) {
    CTRDGiTaskWork *const p = (CTRDGiTaskWork *) arg;

    for (;;) {
        CTRDGTaskInfo trg;
        MI_CpuFill8(&trg, 0, sizeof(CTRDGTaskInfo));
        {
            OSIntrMode bak_cpsr = OS_DisableInterrupts();
            while (!p->list) {
                OS_SleepThread(NULL);
            }
            trg = *p->list;
            OS_RestoreInterrupts(bak_cpsr);
        }
        if (trg.task) {
            trg.result = (u32) (*trg.task)(&trg);
        }
        {
            OSIntrMode bak_cpsr = OS_DisableInterrupts();
            CTRDG_TASK_FUNC callback = trg.callback;
            ctrdgi_task_list.busy = FALSE;
            if (callback) {
                (*callback)(&trg);
            }
            if (ctrdgi_task_work == NULL) {
                break;
            }
            p->list = NULL;
            OS_RestoreInterrupts(bak_cpsr);
        }
    }
    OS_ExitThread();
}
