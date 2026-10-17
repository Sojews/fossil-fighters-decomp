// NitroSDK FS: archives. Each archive has a name of up to three letters, tables of its files and directories,
// and a queue of the commands its files have sent, which it runs one at a time.

#include "fs_internal.h"

static FSArchive *arc_list;
FSDirPos current_dir_pos;

static u32 FSi_GetPackedName(const char *name, int name_len) {
    u32 ret = 0;
    if (name_len <= FS_ARCHIVE_NAME_LEN_MAX) {
        int i = 0;
        for (; i < name_len; ++i) {
            u32 c = *(const u8 *) (name + i);
            if (!c) {
                break;
            }
            c = (u32) (c - 'A');
            if (c <= (u32) ('Z' - 'A')) {
                c = (u32) (c + 'a');
            } else {
                c = (u32) (c + 'A');
            }
            ret |= (u32) (c << (i * 8));
        }
    }
    return ret;
}

static FSResult FSi_ReadMemCallback(FSArchive *p_arc, void *dst, u32 pos, u32 size) {
    MI_CpuCopy8((const void *) (p_arc->base + pos), dst, size);
    return FS_RESULT_SUCCESS;
}

static FSResult FSi_WriteMemCallback(FSArchive *p_arc, const void *src, u32 pos, u32 size) {
    MI_CpuCopy8(src, (void *) (p_arc->base + pos), size);
    return FS_RESULT_SUCCESS;
}

static FSResult FSi_ReadMemoryCore(FSArchive *p_arc, void *dst, u32 pos, u32 size) {
#pragma unused(p_arc)
    MI_CpuCopy8((const void *) pos, dst, size);
    return FS_RESULT_SUCCESS;
}

static FSFile *FSi_NextCommand(FSArchive *p_arc) {
    OSIntrMode bak_psr = OS_DisableInterrupts();

    if (FSi_IsArchiveCanceling(p_arc)) {
        FSFile *p, *q;
        p_arc->flag &= ~FS_ARCHIVE_FLAG_CANCELING;
        for (p = p_arc->list.next; p; p = q) {
            q = p->link.next;
            if (FS_IsCanceling(p)) {
                if (p_arc->list.next == p) {
                    p_arc->list.next = q;
                }
                FSi_ReleaseCommand(p, FS_RESULT_CANCELED);
                if (!q) {
                    q = p_arc->list.next;
                }
            }
        }
    }

    if (!FS_IsArchiveSuspending(p_arc) && !FS_IsArchiveSuspended(p_arc) && p_arc->list.next) {
        FSFile *p_file = p_arc->list.next;
        const BOOL is_start = !FSi_IsArchiveRunning(p_arc);
        if (is_start) {
            p_arc->flag |= FS_ARCHIVE_FLAG_RUNNING;
        }
        (void) OS_RestoreInterrupts(bak_psr);
        if (is_start) {
            if ((p_arc->proc_flag & FS_ARCHIVE_PROC_ACTIVATE) != 0) {
                (void) (*p_arc->proc)(p_file, FS_COMMAND_ACTIVATE);
            }
        }
        bak_psr = OS_DisableInterrupts();
        p_file->stat |= FS_FILE_STATUS_OPERATING;
        if (FS_IsFileSyncMode(p_file)) {
            OS_WakeupThread(p_file->queue);
            (void) OS_RestoreInterrupts(bak_psr);
            return NULL;
        }
        (void) OS_RestoreInterrupts(bak_psr);
        return p_file;
    }

    if (FSi_IsArchiveRunning(p_arc)) {
        p_arc->flag &= ~FS_ARCHIVE_FLAG_RUNNING;
        if (p_arc->proc_flag & FS_ARCHIVE_PROC_IDLE) {
            FSFile tmp;
            FS_InitFile(&tmp);
            tmp.arc = p_arc;
            (void) (*p_arc->proc)(&tmp, FS_COMMAND_IDLE);
        }
    }
    if (FS_IsArchiveSuspending(p_arc)) {
        p_arc->flag &= ~FS_ARCHIVE_FLAG_SUSPENDING;
        p_arc->flag |= FS_ARCHIVE_FLAG_SUSPEND;
        OS_WakeupThread(&p_arc->stat_q);
    }
    (void) OS_RestoreInterrupts(bak_psr);
    return NULL;
}

static void FSi_ExecuteAsyncCommand(FSFile *p_file) {
    FSArchive *const p_arc = p_file->arc;
    while (p_file) {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        p_file->stat |= FS_FILE_STATUS_OPERATING;
        if (FS_IsFileSyncMode(p_file)) {
            OS_WakeupThread(p_file->queue);
            (void) OS_RestoreInterrupts(bak_psr);
            break;
        }
        p_file->stat |= FS_FILE_STATUS_ASYNC;
        (void) OS_RestoreInterrupts(bak_psr);
        if (FSi_TranslateCommand(p_file, p_file->command) == FS_RESULT_PROC_ASYNC) {
            break;
        }
        p_file = FSi_NextCommand(p_arc);
    }
}

BOOL FSi_ExecuteSyncCommand(FSFile *p_com) {
    FSFile *p_target;
    FSi_ReleaseCommand(p_com, FSi_TranslateCommand(p_com, p_com->command));
    p_target = FSi_NextCommand(p_com->arc);
    if (p_target) {
        FSi_ExecuteAsyncCommand(p_target);
    }
    return FS_IsSucceeded(p_com);
}

static inline void FSi_CutFromList(FSFile *p_file) {
    FSFile *const prev = p_file->link.prev;
    FSFile *const next = p_file->link.next;
    if (prev) {
        prev->link.next = next;
    }
    if (next) {
        next->link.prev = prev;
    }
}

static inline void FSi_AppendToList(FSFile *p_file, FSFile *p_list) {
    FSi_CutFromList(p_file);
    while (p_list->link.next) {
        p_list = p_list->link.next;
    }
    p_list->link.next = p_file;
    p_file->link.prev = p_list;
    p_file->link.next = NULL;
}

BOOL FSi_SendCommand(FSFile *p_file, FSCommandType command) {
    FSArchive *const p_arc = p_file->arc;
    const int bit = 1 << command;

    p_file->command = command;
    p_file->error = FS_RESULT_BUSY;
    p_file->stat |= FS_FILE_STATUS_BUSY;
    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        if ((p_arc->flag & FS_ARCHIVE_FLAG_UNLOADING) != 0) {
            FSi_ReleaseCommand(p_file, FS_RESULT_CANCELED);
            (void) OS_RestoreInterrupts(bak_psr);
            return FALSE;
        }
        if ((bit & FS_ARCHIVE_PROC_SYNC) != 0) {
            p_file->stat |= FS_FILE_STATUS_SYNC;
        }
        FSi_AppendToList(p_file, (FSFile *) &p_arc->list);
        if (!FS_IsArchiveSuspended(p_arc) && !FSi_IsArchiveRunning(p_arc)) {
            p_arc->flag |= FS_ARCHIVE_FLAG_RUNNING;
            (void) OS_RestoreInterrupts(bak_psr);
            if (p_arc->proc_flag & FS_ARCHIVE_PROC_ACTIVATE) {
                (void) (*p_arc->proc)(p_file, FS_COMMAND_ACTIVATE);
            }
            bak_psr = OS_DisableInterrupts();
            p_file->stat |= FS_FILE_STATUS_OPERATING;
            if (!FS_IsFileSyncMode(p_file)) {
                (void) OS_RestoreInterrupts(bak_psr);
                FSi_ExecuteAsyncCommand(p_file);
                return TRUE;
            }
            (void) OS_RestoreInterrupts(bak_psr);
        } else if (!FS_IsFileSyncMode(p_file)) {
            (void) OS_RestoreInterrupts(bak_psr);
            return TRUE;
        } else {
            do {
                OS_SleepThread(p_file->queue);
            } while ((p_file->stat & FS_FILE_STATUS_OPERATING) == 0);
            (void) OS_RestoreInterrupts(bak_psr);
        }
    }
    return FSi_ExecuteSyncCommand(p_file);
}

void FS_InitArchive(FSArchive *p_arc) {
    MI_CpuFill8(p_arc, 0, sizeof(FSArchive));
    OS_InitThreadQueue(&p_arc->sync_q);
    OS_InitThreadQueue(&p_arc->stat_q);
}

FSArchive *FS_FindArchive(const char *name, int name_len) {
    const u32 pack = FSi_GetPackedName(name, name_len);
    OSIntrMode bak_psr = OS_DisableInterrupts();
    FSArchive *p_arc = arc_list;
    while (p_arc && p_arc->name.pack != pack) {
        p_arc = p_arc->next;
    }
    (void) OS_RestoreInterrupts(bak_psr);
    return p_arc;
}

BOOL FS_RegisterArchiveName(FSArchive *p_arc, const char *name, u32 name_len) {
    BOOL ret = FALSE;
    OSIntrMode bak_intr = OS_DisableInterrupts();
    if (!FS_FindArchive(name, (s32) name_len)) {
        FSArchive *p_tail = arc_list;
        if (!p_tail) {
            arc_list = p_arc;
            current_dir_pos.arc = p_arc;
            current_dir_pos.pos = 0;
            current_dir_pos.index = 0;
            current_dir_pos.own_id = 0;
        } else {
            while (p_tail->next) {
                p_tail = p_tail->next;
            }
            p_tail->next = p_arc;
            p_arc->prev = p_tail;
        }
        p_arc->name.pack = FSi_GetPackedName(name, (s32) name_len);
        p_arc->flag |= FS_ARCHIVE_FLAG_REGISTER;
        ret = TRUE;
    }
    (void) OS_RestoreInterrupts(bak_intr);
    return ret;
}

void FS_ReleaseArchiveName(FSArchive *p_arc) {
    if (p_arc->name.pack) {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        if (p_arc->next) {
            p_arc->next->prev = p_arc->prev;
        }
        if (p_arc->prev) {
            p_arc->prev->next = p_arc->next;
        }
        p_arc->name.pack = 0;
        p_arc->next = p_arc->prev = NULL;
        p_arc->flag &= ~FS_ARCHIVE_FLAG_REGISTER;
        if (current_dir_pos.arc == p_arc) {
            current_dir_pos.arc = arc_list;
            current_dir_pos.pos = 0;
            current_dir_pos.index = 0;
            current_dir_pos.own_id = 0;
        }
        (void) OS_RestoreInterrupts(bak_psr);
    }
}

BOOL FS_LoadArchive(FSArchive *p_arc, u32 base, u32 fat, u32 fat_size, u32 fnt, u32 fnt_size,
                    FS_ARCHIVE_READ_FUNC read_func, FS_ARCHIVE_WRITE_FUNC write_func) {
    p_arc->base = base;
    p_arc->fat_size = fat_size;
    p_arc->fat = p_arc->fat_bak = fat;
    p_arc->fnt_size = fnt_size;
    p_arc->fnt = p_arc->fnt_bak = fnt;
    p_arc->read_func = read_func ? read_func : FSi_ReadMemCallback;
    p_arc->write_func = write_func ? write_func : FSi_WriteMemCallback;
    p_arc->table_func = p_arc->read_func;
    p_arc->load_mem = NULL;
    p_arc->flag |= FS_ARCHIVE_FLAG_LOADED;
    return TRUE;
}

BOOL FS_UnloadArchive(FSArchive *p_arc) {
    OSIntrMode bak_psr = OS_DisableInterrupts();
    if (FS_IsArchiveLoaded(p_arc)) {
        if (FS_IsArchiveTableLoaded(p_arc)) {
            // (A warning in debug builds: the tables should be unloaded first.)
        }
        {
            BOOL bak_state = FS_SuspendArchive(p_arc);
            p_arc->flag |= FS_ARCHIVE_FLAG_UNLOADING;
            {
                FSFile *p, *q;
                for (p = p_arc->list.next; p; p = q) {
                    q = p->link.next;
                    FSi_ReleaseCommand(p, FS_RESULT_CANCELED);
                }
                p_arc->list.next = NULL;
            }
            if (bak_state) {
                FS_ResumeArchive(p_arc);
            }
        }
        p_arc->base = 0;
        p_arc->fat = 0;
        p_arc->fat_size = 0;
        p_arc->fnt = 0;
        p_arc->fnt_size = 0;
        p_arc->fat_bak = p_arc->fnt_bak = 0;
        p_arc->flag &= ~(FS_ARCHIVE_FLAG_CANCELING | FS_ARCHIVE_FLAG_LOADED | FS_ARCHIVE_FLAG_UNLOADING);
    }
    (void) OS_RestoreInterrupts(bak_psr);
    return TRUE;
}

u32 FS_LoadArchiveTables(FSArchive *p_arc, void *p_mem, u32 max_size) {
    u32 total_size = ((u32) (p_arc->fat_size + p_arc->fnt_size + 32) + 31) & ~31;
    if (total_size <= max_size) {
        u8 *p_cache = (u8 *) (((u32) p_mem + 31) & ~31);
        FSFile tmp;
        FS_InitFile(&tmp);
        if (FS_OpenFileDirect(&tmp, p_arc, p_arc->fat, p_arc->fat + p_arc->fat_size, (u32) ~0)) {
            if (FS_ReadFile(&tmp, p_cache, (s32) p_arc->fat_size) < 0) {
                MI_CpuFill8(p_cache, 0x00, p_arc->fat_size);
            }
            (void) FS_CloseFile(&tmp);
        }
        p_arc->fat = (u32) p_cache;
        p_cache += p_arc->fat_size;
        if (FS_OpenFileDirect(&tmp, p_arc, p_arc->fnt, p_arc->fnt + p_arc->fnt_size, (u32) ~0)) {
            if (FS_ReadFile(&tmp, p_cache, (s32) p_arc->fnt_size) < 0) {
                MI_CpuFill8(p_cache, 0x00, p_arc->fnt_size);
            }
            (void) FS_CloseFile(&tmp);
        }
        p_arc->fnt = (u32) p_cache;
        p_arc->load_mem = p_mem;
        p_arc->table_func = FSi_ReadMemoryCore;
        p_arc->flag |= FS_ARCHIVE_FLAG_TABLE_LOAD;
    }
    return total_size;
}

BOOL FS_SuspendArchive(FSArchive *p_arc) {
    BOOL ret;
    OSIntrMode bak_psr = OS_DisableInterrupts();
    ret = !FS_IsArchiveSuspended(p_arc);
    if (ret) {
        if (FSi_IsArchiveRunning(p_arc)) {
            p_arc->flag |= FS_ARCHIVE_FLAG_SUSPENDING;
            do {
                OS_SleepThread(&p_arc->stat_q);
            } while (FS_IsArchiveSuspending(p_arc));
        } else {
            p_arc->flag |= FS_ARCHIVE_FLAG_SUSPEND;
        }
    }
    (void) OS_RestoreInterrupts(bak_psr);
    return ret;
}

BOOL FS_ResumeArchive(FSArchive *p_arc) {
    BOOL ret;
    FSFile *p_target = NULL;
    OSIntrMode bak_irq = OS_DisableInterrupts();
    ret = !FS_IsArchiveSuspended(p_arc);
    if (!ret) {
        p_arc->flag &= ~FS_ARCHIVE_FLAG_SUSPEND;
        p_target = FSi_NextCommand(p_arc);
    }
    (void) OS_RestoreInterrupts(bak_irq);
    if (p_target) {
        FSi_ExecuteAsyncCommand(p_target);
    }
    return ret;
}

void FS_SetArchiveProc(FSArchive *p_arc, FS_ARCHIVE_PROC_FUNC proc, u32 flags) {
    if (!flags) {
        proc = NULL;
    } else if (!proc) {
        flags = 0;
    }
    p_arc->proc = proc;
    p_arc->proc_flag = flags;
}

void FS_NotifyArchiveAsyncEnd(FSArchive *p_arc, FSResult ret) {
    if (FSi_IsArchiveAsync(p_arc)) {
        FSFile *p_file = p_arc->list.next;
        p_arc->flag &= ~FS_ARCHIVE_FLAG_IS_ASYNC;
        FSi_ReleaseCommand(p_file, ret);
        p_file = FSi_NextCommand(p_arc);
        if (p_file) {
            FSi_ExecuteAsyncCommand(p_file);
        }
    } else {
        FSFile *p_file = p_arc->list.next;
        OSIntrMode bak_psr = OS_DisableInterrupts();
        p_file->error = ret;
        p_arc->flag &= ~FS_ARCHIVE_FLAG_IS_SYNC;
        OS_WakeupThread(&p_arc->sync_q);
        (void) OS_RestoreInterrupts(bak_psr);
    }
}
