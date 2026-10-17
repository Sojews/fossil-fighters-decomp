#ifndef RUNTIME_EXCEPTION_INTERNAL_H
#define RUNTIME_EXCEPTION_INTERNAL_H

// Metrowerks C++ runtime: exception handling structures shared by its files.

#include "types.h"
#include <MWException.h>

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
    u32 r[16];
    u32 sp;
    u32 frame_size;
    u32 unk64;
    u16 saved_mask;     // registers the frame saved
    u8 unk6A;
    u8 unk6B;
    u8 unk6C;
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

extern "C" {

char *func_02086d6c(char *p, s32 *value);
char *func_02086dcc(char *p, u32 *value);
void func_0208714c(ThrowContext *context, ExceptionInfo *info, u8 *target);
char *func_020881bc(ThrowContext *context, ExceptionInfo *info);
void func_02088204(ThrowContext *context, ExceptionInfo *info);
int __FindExceptionTable(ExceptionInfo *info, char *return_address);
char *func_020882a8(char *record);
void func_020882c8(ThrowContext *context, ExceptionInfo *info, char *address);

}

#endif
