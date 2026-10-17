// Metrowerks C++ runtime: exception table lookup and unwinding.

#include "types.h"
#include <exception>

extern "C" {

typedef struct ExceptionTableIndex {
    u32 function;       // start address
    u32 size_flags;     // size, bit 0 set when the exception record is stored inline
    u32 record;         // exception record (or its first word, when inline)
} ExceptionTableIndex;

typedef struct ExceptionInfo {
    char *function;
    char *exception_record;
    u8 *action_pointer;
    ExceptionTableIndex *table_start;
    ExceptionTableIndex *table_end;
    u32 unk14;
} ExceptionInfo;

typedef struct ThrowRegisters {
    u32 r[21];
} ThrowRegisters;

// State of a throw: what is being thrown and the registers of the frame being unwound.
typedef struct ThrowContext {
    char *throw_type;           // mangled type name of the thrown object
    void *location;             // the thrown object
    void (*dtor)(void *, s16);
    char *catch_info;
    char *return_address;
    u32 unk14;
    char *frame;
    ThrowRegisters regs;
} ThrowContext;

// Kept in the catching frame while a handler runs.
typedef struct CatchInfo {
    void *location;
    char *throw_type;
    void (*dtor)(void *, s16);
    void *sublocation;
    s32 pointer_copy;
    u8 *stack_top;
} CatchInfo;

typedef struct ActionIterator {
    ExceptionInfo info;
    ThrowContext context;
} ActionIterator;

typedef struct ExceptionSpecification {
    u32 count;
    u32 unk4;
    u32 unk8;
    u8 *types;          // unaligned type_info pointers
} ExceptionSpecification;

extern int __throw_catch_compare(char *throw_type, char *catch_type, void **adjust);
extern void func_0208714c(ThrowContext *context, ExceptionInfo *info, u8 *target);
extern void func_020882c8(ThrowContext *context, ExceptionInfo *info, char *address);
extern int __FindExceptionTable(ExceptionInfo *info, char *return_address);
extern char *func_020881bc(ThrowContext *context, ActionIterator *iter);
extern void func_02088204(ThrowContext *context, ActionIterator *iter);
extern char *func_020882a8(char *record);

// Variable-length numbers: the low bits of the first byte say how many bytes follow.
char *func_02086d6c(char *p, s32 *value) {
    s32 b0 = p[0];
    u8 b1;
    u8 b2;

    if (!(b0 & 1)) {
        *value = b0 >> 1;
        return p + 1;
    }
    b1 = p[1];
    if (!(b0 & 2)) {
        *value = ((b0 >> 2) << 8) | b1;
        return p + 2;
    }
    b2 = p[2];
    if (!(b0 & 4)) {
        *value = ((b0 >> 3) << 16) | (b1 << 8) | b2;
        return p + 3;
    }
    *value = ((b0 >> 3) << 24) | (b1 << 16) | (b2 << 8) | (u8)p[3];
    return p + 4;
}

char *func_02086dcc(char *p, u32 *value) {
    u32 b0 = (u8)p[0];
    u8 b1;
    u8 b2;

    if (!(b0 & 1)) {
        *value = b0 >> 1;
        return p + 1;
    }
    b1 = p[1];
    if (!(b0 & 2)) {
        *value = ((b0 >> 2) << 8) | b1;
        return p + 2;
    }
    b2 = p[2];
    if (!(b0 & 4)) {
        *value = ((b0 >> 3) << 16) | (b1 << 8) | b2;
        return p + 3;
    }
    *value = ((b0 >> 3) << 24) | (b1 << 16) | (b2 << 8) | (u8)p[3];
    return p + 4;
}

ExceptionTableIndex *func_02086e28(ExceptionTableIndex *table, int count, u32 address) {
    int lo = 0;
    int hi = count - 1;

    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        ExceptionTableIndex *p = &table[mid];
        u32 start = p->function;

        if (address < start) {
            hi = mid - 1;
        } else if (address <= start + (p->size_flags & ~1)) {
            return p;
        } else {
            lo = mid + 1;
        }
    }
    return NULL;
}

void func_02086e60(char *return_address, ExceptionInfo *info) {
    ExceptionTableIndex *p;
    char *e;
    u32 code_offset, code_size, action_offset;
    u32 ret_offset;
    u32 offset;

    info->exception_record = NULL;
    info->action_pointer = NULL;
    if (__FindExceptionTable(info, return_address) == 0) {
        return;
    }
    p = func_02086e28(info->table_start, info->table_end - info->table_start, (u32)return_address);
    if (p == NULL) {
        return;
    }
    info->exception_record = (p->size_flags & 1) ? (char *)&p->record : (char *)p->record;
    info->function = (char *)p->function;
    ret_offset = (u32)return_address - p->function;
    e = func_020882a8(info->exception_record);
    offset = 0;
    for (;;) {
        e = func_02086dcc(e, &code_offset);
        if (code_offset == 0) {
            return;
        }
        e = func_02086dcc(e, &code_size);
        e = func_02086dcc(e, &action_offset);
        offset += code_offset;
        if (ret_offset < offset) {
            return;
        }
        offset += code_size;
        if (ret_offset <= offset) {
            info->action_pointer = (u8 *)info->exception_record + action_offset;
            return;
        }
    }
}

u8 func_02086eec(ExceptionInfo *info) {
    return info->action_pointer ? *info->action_pointer & 0x1F : 0;
}

// Steps to the next action of the current frame, unwinding to the caller's frame when the frame has none left.
// Returns the action type, or 0xFF at the end.
// NONMATCHING: the game keeps the action pointer in r0 and the type byte in r2 (ours: r1 and r0); the record
// fields are separate scalars because mwcc sorts local structs over 8 bytes to the top of the frame.
u8 func_02086f04(ActionIterator *iter) {
    u8 *ap;
    u8 action;
    u8 type;
    s32 r2_a;
    s32 r3_b;
    s32 r3_a;
    s32 r4_a;
    u32 r5_c;
    u32 r5_b;
    s32 r5_a;
    s32 r6_b;
    s32 r6_a;
    s32 r7_b;
    s32 r7_a;
    s32 r8_c;
    s32 r8_b;
    s32 r8_a;
    u32 r9_d;
    u32 r9_c;
    s32 r9_b;
    s32 r9_a;
    s32 r10_a;
    s32 r11_b;
    s32 r11_a;
    s32 r12_b;
    u32 r12_a;
    s32 r13_a;
    s32 r15_c;
    u32 r15_b;
    u32 r15_count;
    volatile u32 r16_d;
    s32 r16_c;
    s32 r16_b;
    s32 r16_a;
    s32 r17_d;
    s32 r17_b;
    s32 r17_c;
    s32 r17_a;
    u32 r18_c;
    s32 r18_b;
    s32 r18_a;
    s32 r19_a;
    s32 offset;

    for (;;) {
        ap = iter->info.action_pointer;
        if (ap != NULL && !((type = *ap) & 0x80)) {
            break;
        }
        func_02086e60(func_020881bc(&iter->context, iter), &iter->info);
        if (iter->info.exception_record == NULL) {
            return 0xFF;
        }
        func_02088204(&iter->context, iter);
        if (iter->info.action_pointer != NULL) {
            goto skip_branches;
        }
    }
    switch (type & 0x1F) {
        case 2: {
            iter->info.action_pointer = (u8 *)func_02086d6c((char *)ap + 1, &r2_a) + 4;
            break;
        }
        case 3: {
            iter->info.action_pointer = (u8 *)func_02086d6c(func_02086d6c((char *)ap + 1, &r3_a), &r3_b) + 4;
            break;
        }
        case 4: {
            iter->info.action_pointer = (u8 *)func_02086d6c((char *)ap + 1, &r4_a) + 4;
            break;
        }
        case 5: {
            iter->info.action_pointer = (u8 *)func_02086dcc(func_02086dcc(func_02086d6c((char *)ap + 1, &r5_a), &r5_b), &r5_c) + 4;
            break;
        }
        case 6: {
            iter->info.action_pointer = (u8 *)func_02086d6c(func_02086d6c((char *)ap + 1, &r6_a), &r6_b) + 4;
            break;
        }
        case 7: {
            iter->info.action_pointer = (u8 *)func_02086d6c(func_02086d6c((char *)ap + 1, &r7_a), &r7_b) + 4;
            break;
        }
        case 8: {
            iter->info.action_pointer = (u8 *)func_02086d6c(func_02086d6c(func_02086d6c((char *)ap + 1, &r8_a), &r8_b), &r8_c) + 4;
            break;
        }
        case 9: {
            iter->info.action_pointer = (u8 *)func_02086dcc(func_02086dcc(func_02086d6c(func_02086d6c((char *)ap + 1, &r9_a), &r9_b), &r9_c), &r9_d) + 4;
            break;
        }
        case 10: {
            iter->info.action_pointer = (u8 *)func_02086d6c((char *)ap + 1, &r10_a) + 4;
            break;
        }
        case 11: {
            iter->info.action_pointer = (u8 *)func_02086d6c(func_02086d6c((char *)ap + 1, &r11_a), &r11_b) + 4;
            break;
        }
        case 12: {
            iter->info.action_pointer = (u8 *)func_02086d6c(func_02086dcc((char *)ap + 5, &r12_a), &r12_b);
            break;
        }
        case 13: {
            iter->info.action_pointer = (u8 *)func_02086d6c((char *)ap + 1, &r13_a);
            break;
        }
        case 15: {
            iter->info.action_pointer = (u8 *)func_02086d6c(func_02086dcc(func_02086dcc((char *)ap + 1, &r15_count), &r15_b), &r15_c) + r15_count * 4;
            break;
        }
        case 16: {
            ap = (u8 *)func_02086d6c((char *)ap + 1, &r16_a);
            ap = (u8 *)func_02086d6c((char *)ap, &r16_b);
            r16_d = ap[0] | (ap[1] << 8) | (ap[2] << 16) | (ap[3] << 24);
            ap = (u8 *)func_02086d6c((char *)ap + 4, &r16_c);
            iter->info.action_pointer = ap + 4;
            break;
        }
        case 17: {
            iter->info.action_pointer = (u8 *)func_02086d6c(func_02086d6c(func_02086d6c(func_02086d6c((char *)ap + 1, &r17_a), &r17_b) + 1, &r17_c), &r17_d) + 4;
            break;
        }
        case 18: {
            iter->info.action_pointer = (u8 *)func_02086dcc(func_02086d6c(func_02086d6c((char *)ap + 1, &r18_a) + 1, &r18_b), &r18_c) + 4;
            break;
        }
        case 19: {
            iter->info.action_pointer = (u8 *)func_02086d6c((char *)ap + 1, &r19_a);
            break;
        }
        default:
            return 0xFF;
    }

skip_branches:
    while ((action = *iter->info.action_pointer & 0x1F) == 1) {
        func_02086d6c((char *)iter->info.action_pointer + 1, &offset);
        iter->info.action_pointer += offset;
    }
    return action;
}

// Walks the actions from the throw point to the catch block that takes the exception.
char *func_0208775c(ThrowContext *context, ExceptionInfo *info) {
    ActionIterator iter;
    s32 offset;
    u8 action;
    char *p;

    iter.info = *info;
    iter.context = *context;
    action = func_02086eec(&iter.info);
    for (;;) {
        switch (action) {
        default:
            return NULL;
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 15:
        case 16:
        case 17:
        case 18:
            action = func_02086f04(&iter);
            break;
        case 13:
            func_02086d6c((char *)iter.info.action_pointer + 1, &offset);
            p = iter.context.frame + offset;
            context->throw_type = *(char **)(p + 4);
            context->location = *(void **)p;
            context->dtor = NULL;
            context->catch_info = p;
            return p;
        }
    }
}

// Whether the thrown type is one of the types an exception specification allows.
int func_02087818(char *throw_type, ExceptionSpecification *spec) {
    u8 *p;
    u32 i;

    for (i = 0, p = spec->types; i < spec->count; i++, p += 4) {
        struct {
            void *adjust;
            char *type;
        } c;
        c.type = (char *)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
        if (__throw_catch_compare(throw_type, c.type, &c.adjust)) {
            return 1;
        }
    }
    return 0;
}

// The thrown type broke an exception specification: unwind to it and run its unexpected handler.
// NONMATCHING: the game copies the action argument into r2 before saving the context pointer (two moves swapped).
void func_020878e8(ThrowContext *context, ExceptionInfo *info, ExceptionSpecification *spec, u8 *action) {
    CatchInfo *ci;

    func_0208714c(context, info, action);
    ci = (CatchInfo *)(context->frame + spec->unk8);
    ci->location = context->location;
    ci->throw_type = context->throw_type;
    ci->dtor = context->dtor;
    ci->stack_top = action;
    func_020882c8(context, info, info->function + spec->unk4);
}

// Finds the action that handles the throw: a catch block taking the type or a broken specification.
u8 *func_0208791c(ThrowContext *context, ExceptionInfo *info, s32 *adjust) {
    ActionIterator iter;
    struct {
        char *type;
        u32 offset;
        s32 catch_offset;
    } cb;
    ExceptionSpecification spec;
    u8 action;

    iter.info = *info;
    iter.context = *context;
    action = func_02086eec(&iter.info);
    for (;;) {
        switch (action) {
        case 12: {
            u8 *p = iter.info.action_pointer;
            cb.type = (char *)(p[1] | (p[2] << 8) | (p[3] << 16) | (p[4] << 24));
            func_02086d6c(func_02086dcc((char *)p + 5, &cb.offset), &cb.catch_offset);
            if (__throw_catch_compare(context->throw_type, cb.type, (void **)adjust)) {
                goto found;
            }
            break;
        }
        case 15:
            spec.types = (u8 *)func_02086d6c(func_02086dcc(func_02086dcc((char *)iter.info.action_pointer + 1, &spec.count), &spec.unk4), (s32 *)&spec.unk8);
            if (!func_02087818(context->throw_type, &spec)) {
                func_020878e8(context, info, &spec, iter.info.action_pointer);
            }
            break;
        default:
        case 1:
        case 14:
            std::terminate();
            goto found;
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 13:
        case 16:
        case 17:
        case 18:
        case 19:
            break;
        }
        action = func_02086f04(&iter);
    }
found:
    return iter.info.action_pointer;
}

void func_02087a2c(ThrowContext *context, s32 catch_offset, s32 adjust) {
    CatchInfo *ci = (CatchInfo *)(context->frame + catch_offset);

    ci->location = context->location;
    ci->throw_type = context->throw_type;
    ci->dtor = context->dtor;
    if (*(u8 *)context->throw_type == '*') {
        ci->sublocation = &ci->pointer_copy;
        ci->pointer_copy = *(s32 *)context->location + adjust;
    } else {
        ci->sublocation = (char *)context->location + adjust;
    }
}

void func_02087a64(ThrowContext *context) {
    ExceptionInfo info;
    struct {
        char *type;
        u32 offset;
        s32 catch_offset;
    } cb;
    s32 adjust;
    u8 *action;

    func_02086e60(context->return_address, &info);
    if (info.exception_record == NULL) {
        std::terminate();
    }
    func_02088204(context, (ActionIterator *)&info);
    if (context->throw_type != NULL) {
        context->catch_info = NULL;
    } else {
        context->catch_info = func_0208775c(context, &info);
        if (context->catch_info == NULL) {
            std::terminate();
        }
    }
    action = func_0208791c(context, &info, &adjust);
    cb.type = (char *)(action[1] | (action[2] << 8) | (action[3] << 16) | (action[4] << 24));
    func_02086d6c(func_02086dcc((char *)action + 5, &cb.offset), &cb.catch_offset);
    func_0208714c(context, &info, action);
    func_02087a2c(context, cb.catch_offset, adjust);
    func_020882c8(context, &info, info.function + cb.offset);
}

// Destroys a caught exception object.
void __end__catch(CatchInfo *ci) {
    if (ci->location && ci->dtor) {
        ci->dtor(ci->location, -1);
    }
}

}
