// Metrowerks C++ runtime: terminate/unexpected, global object registration, type matching and frame unwinding.

#include "types.h"
#include <exception>
#include <abort_exit_arm_eabi.h>
#include <MWException.h>
#include "exception_internal.h"

extern "C" ExceptionTableIndex __exception_table_start__[];   // defined by the linker script
extern "C" ExceptionTableIndex __exception_table_end__[];
extern "C" void func_02087a64(ThrowContext *context);

static void dthandler(void) {
    abort();
}

static void duhandler(void) {
    std::terminate();
}

static struct {
    std::terminate_handler terminate;
    std::unexpected_handler unexpected;
} handlers = { dthandler, duhandler };

void std::terminate(void) {
    handlers.terminate();
}

void std::unexpected(void) {
    handlers.unexpected();
}

extern "C" {

void *__register_global_object(void *object, void *destructor, DestructorChain *link) {
    link->next = __global_destructor_chain;
    link->dtor = (DestructorFunc)destructor;
    link->object = object;
    __global_destructor_chain = link;
    return object;
}

// Compares Metrowerks type strings: "!name!offset!" lists for classes, mangled names otherwise.
char __throw_catch_compare(const u8 *throw_type, const u8 *catch_type, s32 *offset_result) {
    const u8 *t;
    const u8 *c;

    *offset_result = 0;
    if ((c = catch_type) == NULL) {
        return 1;
    }
    t = throw_type;
    if (*c == 'P') {
        c++;
        if (*c == 'V') {
            c++;
        }
        if (*c == 'K') {
            c++;
        }
        if (*c == 'v') {
            if (*throw_type == 'P' || *throw_type == '*') {
                return 1;
            }
        }
        c = catch_type;
    }
    switch (*throw_type) {
    case '!':
    case '*':
        if (*t++ != *c++) {
            return 0;
        }
        for (;;) {
            if (*t == *c++) {
                if (*t++ == '!') {
                    s32 offset = 0;
                    while (*t != '!') {
                        offset = offset * 10 + *t++ - '0';
                    }
                    *offset_result = offset;
                    return 1;
                }
            } else {
                while (*t++ != '!') {
                }
                while (*t++ != '!') {
                }
                if (*t == 0) {
                    return 0;
                }
                c = catch_type + 1;
            }
        }
    }
    while ((*t == 'P' || *t == 'R') && *t == *c) {
        c++;
        t++;
        if (*c == 'K') {
            if (*t == 'K') {
                t++;
            }
            c++;
        }
        if (*t == 'K') {
            return 0;
        }
        if (*c == 'V') {
            if (*t == 'V') {
                t++;
            }
            c++;
        }
        if (*t == 'V') {
            return 0;
        }
    }
    for (; *t == *c; t++, c++) {
        if (*t == 0) {
            return 1;
        }
    }
    return 0;
}

// Restores the registers the current frame saved and returns its return address.
char *func_020881bc(ThrowContext *context, ExceptionInfo *info) {
    u32 frame_size = context->regs.frame_size;
    u32 *p = (u32 *)(context->frame + frame_size - (context->regs.unk6A ? 0x10 : 0));
    int i;

    for (i = 15; i >= 0; i--) {
        if (context->regs.saved_mask & (1 << i)) {
            context->regs.r[i] = *--p;
        }
    }
    context->unk14 = (u32)(context->frame + frame_size);
    return (char *)context->regs.r[14];
}

// Reads the unwind information of the frame the exception record describes.
void func_02088204(ThrowContext *context, ExceptionInfo *info) {
    char *p = info->exception_record;
    u8 flags = p[0];
    u32 has_fp = flags & 0x40;
    u32 thumb;

    context->regs.unk6B = has_fp ? 1 : 0;
    context->regs.unk6A = (flags & 0x20) ? 1 : 0;
    thumb = flags & 0x80;
    context->regs.unk6C = thumb ? 1 : 0;
    context->regs.saved_mask = (u8)p[1] << 4;
    context->regs.saved_mask |= 1 << 14;
    p = func_02086dcc(p + 2, &context->regs.frame_size);
    if (has_fp) {
        func_02086dcc(p, &context->regs.unk64);
    }
    if (has_fp) {
        if (thumb) {
            context->frame = (char *)context->regs.r[7];
        } else {
            context->frame = (char *)context->regs.r[11];
        }
    } else {
        context->frame = (char *)context->unk14;
    }
}

int __FindExceptionTable(ExceptionInfo *info, char *return_address) {
    info->table_start = __exception_table_start__;
    info->table_end = __exception_table_end__;
    return 1;
}

// Skips the unwind information at the start of an exception record.
char *func_020882a8(char *record) {
    u32 value;
    u8 flags = record[0];

    record = func_02086dcc(record + 2, &value);
    if (flags & 0x40) {
        record = func_02086dcc(record, &value);
    }
    return record;
}

// Loads the saved registers of the catching frame and jumps to the handler.
asm void func_020882c8(ThrowContext *context, ExceptionInfo *info, char *address) {
    ldr r4, [r0, #0x2c]
    ldr r5, [r0, #0x30]
    ldr r6, [r0, #0x34]
    ldr r7, [r0, #0x38]
    ldr r3, [r0, #0x3c]
    mov r8, r3
    ldr r3, [r0, #0x40]
    mov r9, r3
    ldr r3, [r0, #0x44]
    mov r10, r3
    ldr r3, [r0, #0x48]
    mov r11, r3
    ldr r3, [r0, #0x5c]
    mov sp, r3
    ldr r3, [r0, #0x64]
    neg r3, r3
    add sp, r3
    mov pc, r2
}

// Builds a ThrowContext on the stack from the thrower's registers and handles the exception.
asm void __throw(char *throwtype, void *location, void *dtor) {
    mov r12, sp
    mov r3, #0x70
    neg r3, r3
    add sp, r3
    str r4, [sp, #0x2c]
    str r5, [sp, #0x30]
    str r6, [sp, #0x34]
    str r7, [sp, #0x38]
    mov r3, r8
    str r3, [sp, #0x3c]
    mov r3, r9
    str r3, [sp, #0x40]
    mov r3, r10
    str r3, [sp, #0x44]
    mov r3, r11
    str r3, [sp, #0x48]
    mov r3, r12
    str r3, [sp, #0x14]
    str r3, [sp, #0x5c]
    mov r3, lr
    str r3, [sp, #0x10]
    str r0, [sp]
    str r1, [sp, #4]
    str r2, [sp, #8]
    mov r0, sp
    bl func_02087a64
}

asm void __rethrow(void) {
    mov r0, sp
    mov r1, #0x70
    neg r1, r1
    add sp, r1
    str r4, [sp, #0x2c]
    str r5, [sp, #0x30]
    str r6, [sp, #0x34]
    str r7, [sp, #0x38]
    mov r1, r8
    str r1, [sp, #0x3c]
    mov r1, r9
    str r1, [sp, #0x40]
    mov r1, r10
    str r1, [sp, #0x44]
    mov r1, r11
    str r1, [sp, #0x48]
    str r0, [sp, #0x14]
    str r0, [sp, #0x5c]
    mov r0, lr
    str r0, [sp, #0x10]
    mov r0, #0
    str r0, [sp]
    str r0, [sp, #4]
    str r0, [sp, #8]
    mov r0, sp
    bl func_02087a64
}

}
