// NitroSDK OS: the exception handler. A debugger's handler takes precedence; otherwise the registers at the
// exception are saved, a user handler may look at them, and the system stops.

#include "nitro/os.h"

// The exception vector the BIOS jumps through, and its copy in the data TCM's system area.
#define HW_EXCP_VECTOR_MAIN 0x027ffd9c
#define HW_EXCP_VECTOR_BUF ((u32) SDK_AUTOLOAD_DTCM_START + 0x3fdc)
#define HW_ITCM_END 0x02000000
#define HW_PSR_SYS_MODE 0x9f

typedef void (*OSExceptionHandler)(u32 context, void *arg);

// The registers at the exception.
typedef struct {
    OSContext context;      // 0x00
    u32 cp15;               // 0x64
    u32 spsr;               // 0x68
    u32 exinfo;             // 0x6c
    u32 debug[4];           // 0x70
} OSiExContext;

static OSiExContext OSi_ExContext;
static OSExceptionHandler OSi_UserExceptionHandler;
static void *OSi_UserExceptionHandlerArg;
static void *OSi_DebuggerHandler;

void OS_EnableProtectionUnit(void);
void OS_DisableProtectionUnit(void);
static void OSi_ExceptionHandler(void);
static void OSi_GetAndDisplayContext(void);
static void OSi_SetExContext(void);
static void OSi_DisplayExContext(void);

void OS_InitException(void) {
    if (0x2600000 <= *(u32 *) HW_EXCP_VECTOR_MAIN && *(u32 *) HW_EXCP_VECTOR_MAIN < 0x2800000) {
        OSi_DebuggerHandler = *(void **) HW_EXCP_VECTOR_MAIN;
    } else {
        OSi_DebuggerHandler = NULL;
    }
    if (!OSi_DebuggerHandler) {
        *(u32 *) HW_EXCP_VECTOR_MAIN = (u32) OSi_ExceptionHandler;
        *(u32 *) HW_EXCP_VECTOR_BUF = (u32) OSi_ExceptionHandler;
    }
    OSi_UserExceptionHandler = NULL;
}

#pragma thumb off
static asm void OSi_ExceptionHandler(void) {
    ldr r12, =OSi_DebuggerHandler
    ldr r12, [r12]
    cmp r12, #0
    movne lr, pc
    bxne r12

    // Save registers below the ITCM end and switch to that stack.
    ldr r12, =HW_ITCM_END
    stmdb r12!, {r0-r3, sp, lr}
    and r0, sp, #1
    mov sp, r12

    mrs r1, cpsr
    and r1, r1, #0x1f
    teq r1, #0x17
    bne @undef
    bl OSi_GetAndDisplayContext
    b @stop
@undef:
    teq r1, #0x1b
    bne @stop
    bl OSi_GetAndDisplayContext
@stop:
    ldr r12, =OSi_DebuggerHandler
    ldr r12, [r12]
    cmp r12, #0
@halt:
    beq @halt
@spin:
    nop
    b @spin

    ldmia sp!, {r0-r3, r12, lr}
    mov sp, r12
    bx lr
}

static asm void OSi_GetAndDisplayContext(void) {
    stmfd sp!, {r0, lr}
    bl OSi_SetExContext
    bl OSi_DisplayExContext
    ldmfd sp!, {r0, lr}
    bx lr
}

static asm void OSi_SetExContext(void) {
    ldr r1, =OSi_ExContext
    mrs r2, cpsr
    str r2, [r1, #0x74]
    str r0, [r1, #0x6c]
    ldr r0, [r12, #0]
    str r0, [r1, #4]
    ldr r0, [r12, #4]
    str r0, [r1, #8]
    ldr r0, [r12, #8]
    str r0, [r1, #12]
    ldr r0, [r12, #12]
    str r0, [r1, #16]
    ldr r2, [r12, #16]
    bic r2, r2, #1
    add r0, r1, #20
    stmia r0, {r4-r11}
    str r12, [r1, #0x70]
    ldr r0, [r2, #0]
    str r0, [r1, #0x64]
    ldr r3, [r2, #4]
    str r3, [r1, #0]
    ldr r0, [r2, #8]
    str r0, [r1, #0x34]
    ldr r0, [r2, #12]
    str r0, [r1, #0x40]
    mrs r0, cpsr
    orr r3, r3, #0x80
    bic r3, r3, #0x20
    msr cpsr_fsxc, r3
    str sp, [r1, #0x38]
    str lr, [r1, #0x3c]
    mrs r2, spsr
    str r2, [r1, #0x7c]
    msr cpsr_fsxc, r0
    bx lr
}

static void OSi_DisplayExContext(void) {
    if (OSi_UserExceptionHandler) {
        // Into system mode, keeping the stack.
        asm {
            mov r0, sp
            ldr r1, =HW_PSR_SYS_MODE
            msr cpsr_fsxc, r1
            mov sp, r0
        }
        OS_EnableProtectionUnit();
        OSi_UserExceptionHandler((u32) &OSi_ExContext, OSi_UserExceptionHandlerArg);
        OS_DisableProtectionUnit();
    }
}
#pragma thumb reset
