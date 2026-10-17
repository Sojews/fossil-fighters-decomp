// NitroSDK FS: the default implementation of each file command, in terms of an archive's tables and its read
// function. An archive's own procedure may take a command over or leave it to these.

#include "fs_internal.h"

static FSResult FSi_ReadFileCommand(FSFile *p_file);
static FSResult FSi_WriteFileCommand(FSFile *p_file);
static FSResult FSi_SeekDirCommand(FSFile *p_file);
static FSResult FSi_ReadDirCommand(FSFile *p_file);
static FSResult FSi_FindPathCommand(FSFile *p_file);
static FSResult FSi_GetPathCommand(FSFile *p_file);
static FSResult FSi_OpenFileFastCommand(FSFile *p_file);
static FSResult FSi_OpenFileDirectCommand(FSFile *p_file);
static FSResult FSi_CloseFileCommand(FSFile *p_file);

typedef FSResult (*FSiCommandFunc)(FSFile *p_file);

static const FSiCommandFunc fsi_default_command[FS_COMMAND_MAX] = {
    FSi_ReadFileCommand,
    FSi_WriteFileCommand,
    FSi_SeekDirCommand,
    FSi_ReadDirCommand,
    FSi_FindPathCommand,
    FSi_GetPathCommand,
    FSi_OpenFileFastCommand,
    FSi_OpenFileDirectCommand,
    FSi_CloseFileCommand,
};

void FSi_ReleaseCommand(FSFile *p_file, FSResult ret) {
    OSIntrMode bak_psr = OS_DisableInterrupts();
    {
        FSFile *const prev = p_file->link.prev;
        FSFile *const next = p_file->link.next;
        if (prev) {
            prev->link.next = next;
        }
        if (next) {
            next->link.prev = prev;
        }
        p_file->link.prev = NULL;
        p_file->link.next = NULL;
    }
    p_file->stat &= ~(FS_FILE_STATUS_CANCEL | FS_FILE_STATUS_BUSY | FS_FILE_STATUS_SYNC | FS_FILE_STATUS_ASYNC |
                      FS_FILE_STATUS_OPERATING);
    p_file->error = ret;
    OS_WakeupThread(p_file->queue);
    (void) OS_RestoreInterrupts(bak_psr);
}

FSResult FSi_TranslateCommand(FSFile *p_file, FSCommandType command) {
    FSResult ret;
    FSArchive *const p_arc = p_file->arc;
    const int bit = 1 << command;

    if (FS_IsFileSyncMode(p_file)) {
        p_arc->flag |= FS_ARCHIVE_FLAG_IS_SYNC;
    } else {
        p_arc->flag |= FS_ARCHIVE_FLAG_IS_ASYNC;
    }
    if ((p_arc->proc_flag & bit) != 0) {
        ret = (*p_arc->proc)(p_file, command);
        switch (ret) {
        case FS_RESULT_SUCCESS:
        case FS_RESULT_FAILURE:
        case FS_RESULT_UNSUPPORTED:
            p_file->error = ret;
            break;
        case FS_RESULT_PROC_UNKNOWN:
            ret = FS_RESULT_PROC_DEFAULT;
            p_arc->proc_flag &= ~bit;
            break;
        }
    } else {
        ret = FS_RESULT_PROC_DEFAULT;
    }
    if (ret == FS_RESULT_PROC_DEFAULT) {
        ret = (*fsi_default_command[command])(p_file);
    }
    if (ret == FS_RESULT_PROC_ASYNC) {
        if (FS_IsFileSyncMode(p_file)) {
            OSIntrMode bak_psr = OS_DisableInterrupts();
            if (FSi_IsArchiveSync(p_arc)) {
                do {
                    OS_SleepThread(&p_arc->sync_q);
                } while (FSi_IsArchiveSync(p_arc));
            }
            ret = p_file->error;
            (void) OS_RestoreInterrupts(bak_psr);
        }
    } else if (!FS_IsFileSyncMode(p_file)) {
        p_arc->flag &= ~FS_ARCHIVE_FLAG_IS_ASYNC;
        FSi_ReleaseCommand(p_file, ret);
    } else {
        p_arc->flag &= ~FS_ARCHIVE_FLAG_IS_SYNC;
        p_file->error = ret;
    }
    return ret;
}

int FSi_StrNICmp(const char *str1, const char *str2, u32 len) {
    u32 i;
    for (i = 0; i < len; ++i) {
        u32 c = (u32) (*(const u8 *) (str1 + i) - 'A');
        u32 d = (u32) (*(const u8 *) (str2 + i) - 'A');
        if (c <= (u32) ('Z' - 'A')) {
            c += 'a' - 'A';
        }
        if (d <= (u32) ('Z' - 'A')) {
            d += 'a' - 'A';
        }
        if (c != d) {
            return (int) (c - d);
        }
    }
    return 0;
}

static FSResult FSi_ReadTable(FSiSyncReadParam *p, void *dst, u32 len) {
    FSResult ret;
    FSArchive *const p_arc = p->arc;

    p_arc->flag |= FS_ARCHIVE_FLAG_IS_SYNC;
    switch (ret = (*p_arc->table_func)(p_arc, dst, p->pos, len)) {
    case FS_RESULT_SUCCESS:
    case FS_RESULT_FAILURE:
        p_arc->flag &= ~FS_ARCHIVE_FLAG_IS_SYNC;
        break;
    case FS_RESULT_PROC_ASYNC: {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        if (FSi_IsArchiveSync(p_arc)) {
            do {
                OS_SleepThread(&p_arc->sync_q);
            } while (FSi_IsArchiveSync(p_arc));
        }
        (void) OS_RestoreInterrupts(bak_psr);
        ret = p_arc->list.next->error;
        break;
    }
    }
    p->pos += len;
    return ret;
}

static FSResult FSi_SeekDirDirect(FSFile *p_file, u32 id) {
    p_file->stat |= FS_FILE_STATUS_SYNC;
    p_file->arg.seekdir.pos.arc = p_file->arc;
    p_file->arg.seekdir.pos.pos = 0;
    p_file->arg.seekdir.pos.index = 0;
    p_file->arg.seekdir.pos.own_id = (u16) id;
    return FSi_TranslateCommand(p_file, FS_COMMAND_SEEKDIR);
}

static FSResult FSi_ReadFileCommand(FSFile *p_file) {
    FSArchive *const p_arc = p_file->arc;
    const u32 pos = p_file->prop.file.pos;
    const u32 len = p_file->arg.readfile.len;
    void *const dst = p_file->arg.readfile.dst;
    p_file->prop.file.pos += len;
    return (*p_arc->read_func)(p_arc, dst, pos, len);
}

static FSResult FSi_WriteFileCommand(FSFile *p_file) {
    FSArchive *const p_arc = p_file->arc;
    const u32 pos = p_file->prop.file.pos;
    const u32 len = p_file->arg.writefile.len;
    const void *const src = p_file->arg.writefile.src;
    p_file->prop.file.pos += len;
    return (*p_arc->write_func)(p_arc, src, pos, len);
}

static FSResult FSi_SeekDirCommand(FSFile *p_file) {
    FSResult ret;
    FSArchive *const p_arc = p_file->arc;
    const FSDirPos *const arg = &p_file->arg.seekdir.pos;
    FSArchiveFNT fnt;
    FSiSyncReadParam param;

    param.arc = p_arc;
    param.pos = p_arc->fnt + arg->own_id * sizeof(fnt);
    ret = FSi_ReadTable(&param, &fnt, sizeof(fnt));
    if (ret == FS_RESULT_SUCCESS) {
        p_file->prop.dir.pos = *arg;
        if (arg->index == 0 && arg->pos == 0) {
            p_file->prop.dir.pos.index = fnt.index;
            p_file->prop.dir.pos.pos = p_arc->fnt + fnt.start;
        }
        p_file->prop.dir.parent = (u32) (fnt.parent & 0xfff);
    }
    return ret;
}

static FSResult FSi_ReadDirCommand(FSFile *p_file) {
    FSDirEntry *p_entry = p_file->arg.readdir.p_entry;
    FSResult ret;
    FSiSyncReadParam param;
    u8 len;

    param.arc = p_file->arc;
    param.pos = p_file->prop.dir.pos.pos;
    ret = FSi_ReadTable(&param, &len, sizeof(len));
    if (ret == FS_RESULT_SUCCESS) {
        p_entry->name_len = (u32) (len & 0x7f);
        p_entry->is_directory = (u32) ((len >> 7) & 1);
        if (p_entry->name_len == 0) {
            return FS_RESULT_FAILURE;
        }
        if (!p_file->arg.readdir.skip_string) {
            ret = FSi_ReadTable(&param, p_entry->name, p_entry->name_len);
            if (ret != FS_RESULT_SUCCESS) {
                return ret;
            }
            *(u8 *) (p_entry->name + p_entry->name_len) = '\0';
        } else {
            param.pos += p_entry->name_len;
        }
        if (p_entry->is_directory) {
            u16 id;
            ret = FSi_ReadTable(&param, &id, sizeof(id));
            if (ret != FS_RESULT_SUCCESS) {
                return ret;
            }
            p_entry->id.dir_id.arc = p_file->arc;
            p_entry->id.dir_id.own_id = (u16) (id & 0xfff);
            p_entry->id.dir_id.index = 0;
            p_entry->id.dir_id.pos = 0;
        } else {
            p_entry->id.file_id.arc = p_file->arc;
            p_entry->id.file_id.file_id = p_file->prop.dir.pos.index;
            ++p_file->prop.dir.pos.index;
        }
        p_file->prop.dir.pos.pos = param.pos;
    }
    return ret;
}

static inline BOOL FSi_IsNameChar(u32 c) {
    BOOL ret = FALSE;
    if (c != '\0' && c != '/' && c != '\\') {
        ret = TRUE;
    }
    return ret;
}

static FSResult FSi_FindPathCommand(FSFile *p_dir) {
    const char *path = p_dir->arg.findpath.path;
    const BOOL is_dir = p_dir->arg.findpath.find_directory;
    FSDirEntry entry;

    (void) FSi_TranslateCommand(p_dir, FS_COMMAND_SEEKDIR);
    for (; *(const u8 *) path; path += (*(const u8 *) path ? 1 : 0)) {
        int name_len = 0;
        u32 is_directory;
        while (FSi_IsNameChar(is_directory = *(const u8 *) (path + name_len))) {
            ++name_len;
        }
        if (is_directory || is_dir) {
            is_directory = 1;
        }
        if (name_len == 0) {
            return FS_RESULT_FAILURE;
        }
        if (*(const u8 *) path == '.') {
            if (name_len == 1) {
                path += 1;
                continue;
            }
            if ((name_len == 2) & (*(const u8 *) (path + 1) == '.')) {
                if (p_dir->prop.dir.pos.own_id != 0) {
                    FSi_SeekDirDirect(p_dir, p_dir->prop.dir.parent);
                }
                path += 2;
                continue;
            }
        }
        if (name_len > FS_ENTRY_LONGNAME_MAX - 1) {
            return FS_RESULT_FAILURE;
        }
        p_dir->arg.readdir.p_entry = &entry;
        p_dir->arg.readdir.skip_string = FALSE;
        for (;;) {
            if (FSi_TranslateCommand(p_dir, FS_COMMAND_READDIR) != FS_RESULT_SUCCESS) {
                return FS_RESULT_FAILURE;
            }
            if (is_directory != entry.is_directory || name_len != entry.name_len ||
                FSi_StrNICmp(path, entry.name, (u32) name_len)) {
                continue;
            }
            if (is_directory) {
                path += name_len;
                p_dir->arg.seekdir.pos = entry.id.dir_id;
                (void) FSi_TranslateCommand(p_dir, FS_COMMAND_SEEKDIR);
                break;
            } else if (is_dir) {
                return FS_RESULT_FAILURE;
            } else {
                *p_dir->arg.findpath.result.file = entry.id.file_id;
                return FS_RESULT_SUCCESS;
            }
        }
    }
    if (!is_dir) {
        return FS_RESULT_FAILURE;
    }
    *p_dir->arg.findpath.result.dir = p_dir->prop.dir.pos;
    return FS_RESULT_SUCCESS;
}

static FSResult FSi_GetPathCommand(FSFile *p_file) {
    FSArchive *const p_arc = p_file->arc;
    FSDirEntry entry;
    FSFile tmp;
    u32 file_id;
    u32 dir_id;
    struct {
        u8 *buf;
        u32 buf_len;
        u16 total_len;
        u16 dir_id;
    } *p_info = (void *) &p_file->arg.getpath;
    u32 len;

    FS_InitFile(&tmp);
    tmp.arc = p_file->arc;
    if (FS_IsDir(p_file)) {
        dir_id = p_file->prop.dir.pos.own_id;
        file_id = FS_INVALID_FILE_ID;
    } else {
        file_id = p_file->prop.file.own_id;
        if (p_info->total_len != 0) {
            dir_id = p_info->dir_id;
        } else {
            u32 pos = 0;
            u32 num_dir = 0;
            dir_id = FS_INVALID_FILE_ID;
            do {
                FSi_SeekDirDirect(&tmp, pos);
                if (!pos) {
                    num_dir = tmp.prop.dir.parent;
                }
                tmp.arg.readdir.p_entry = &entry;
                tmp.arg.readdir.skip_string = TRUE;
                while (FSi_TranslateCommand(&tmp, FS_COMMAND_READDIR) == FS_RESULT_SUCCESS) {
                    if (!entry.is_directory && entry.id.file_id.file_id == file_id) {
                        dir_id = tmp.prop.dir.pos.own_id;
                        break;
                    }
                }
            } while (dir_id == FS_INVALID_FILE_ID && ++pos < num_dir);
        }
    }
    if (dir_id == FS_INVALID_FILE_ID) {
        p_info->total_len = 0;
        return FS_RESULT_FAILURE;
    }

    if (p_info->total_len == 0) {
        len = 0;
        if (p_arc->name.pack <= 0x000000ff) {
            len += 1;
        } else if (p_arc->name.pack <= 0x0000ff00) {
            len += 2;
        } else {
            len += 3;
        }
        len += 2;
        if (file_id != FS_INVALID_FILE_ID) {
            len += entry.name_len;
        }
        {
            u32 id = dir_id;
            if (dir_id != 0) {
                FSi_SeekDirDirect(&tmp, dir_id);
                do {
                    FSi_SeekDirDirect(&tmp, tmp.prop.dir.parent);
                    tmp.arg.readdir.p_entry = &entry;
                    tmp.arg.readdir.skip_string = TRUE;
                    while (FSi_TranslateCommand(&tmp, FS_COMMAND_READDIR) == FS_RESULT_SUCCESS) {
                        if (entry.is_directory && entry.id.dir_id.own_id == id) {
                            len += entry.name_len + 1;
                            break;
                        }
                    }
                    id = tmp.prop.dir.pos.own_id;
                } while (id != 0);
            }
        }
        p_info->total_len = (u16) (len + 1);
        p_info->dir_id = (u16) dir_id;
    }

    if (!p_info->buf) {
        return FS_RESULT_SUCCESS;
    }
    if (p_info->buf_len < p_info->total_len) {
        return FS_RESULT_FAILURE;
    }
    {
        u8 *const dst = p_info->buf;
        u32 total = p_info->total_len;
        u32 pos = 0;
        u32 name_len = FS_GetArchiveNameLength(p_arc);

        MI_CpuCopy8(p_arc->name.ptr, dst + pos, name_len);
        pos += name_len;
        MI_CpuCopy8(":/", dst + pos, 2);
        FSi_SeekDirDirect(&tmp, dir_id);
        if (file_id != FS_INVALID_FILE_ID) {
            tmp.arg.readdir.p_entry = &entry;
            tmp.arg.readdir.skip_string = FALSE;
            while (FSi_TranslateCommand(&tmp, FS_COMMAND_READDIR) == FS_RESULT_SUCCESS) {
                if (!entry.is_directory && entry.id.file_id.file_id == file_id) {
                    break;
                }
            }
            name_len = entry.name_len + 1;
            MI_CpuCopy8(entry.name, dst + total - name_len, name_len);
            total -= name_len;
        } else {
            *(u8 *) (dst + total - 1) = '\0';
            total -= 1;
        }
        if (dir_id != 0) {
            do {
                FSi_SeekDirDirect(&tmp, tmp.prop.dir.parent);
                tmp.arg.readdir.p_entry = &entry;
                tmp.arg.readdir.skip_string = FALSE;
                *(u8 *) (dst + total - 1) = '/';
                total -= 1;
                while (FSi_TranslateCommand(&tmp, FS_COMMAND_READDIR) == FS_RESULT_SUCCESS) {
                    if (entry.is_directory && entry.id.dir_id.own_id == dir_id) {
                        u32 nl = entry.name_len;
                        MI_CpuCopy8(entry.name, dst + total - nl, nl);
                        total -= nl;
                        break;
                    }
                }
                dir_id = tmp.prop.dir.pos.own_id;
            } while (dir_id != 0);
        }
    }
    return FS_RESULT_SUCCESS;
}

static FSResult FSi_OpenFileFastCommand(FSFile *p_file) {
    FSArchive *const p_arc = p_file->arc;
    const u32 index = p_file->arg.openfilefast.id.file_id;
    const u32 pos = index * sizeof(FSArchiveFAT);
    FSResult ret;

    if (pos >= p_arc->fat_size) {
        return FS_RESULT_FAILURE;
    }
    {
        FSArchiveFAT fat;
        FSiSyncReadParam param;
        param.arc = p_arc;
        param.pos = p_arc->fat + pos;
        ret = FSi_ReadTable(&param, &fat, sizeof(fat));
        if (ret == FS_RESULT_SUCCESS) {
            p_file->arg.openfiledirect.top = fat.top;
            p_file->arg.openfiledirect.bottom = fat.bottom;
            p_file->arg.openfiledirect.index = index;
            ret = FSi_TranslateCommand(p_file, FS_COMMAND_OPENFILEDIRECT);
        }
    }
    return ret;
}

static FSResult FSi_OpenFileDirectCommand(FSFile *p_file) {
    p_file->prop.file.top = p_file->arg.openfiledirect.top;
    p_file->prop.file.pos = p_file->arg.openfiledirect.top;
    p_file->prop.file.bottom = p_file->arg.openfiledirect.bottom;
    p_file->prop.file.own_id = p_file->arg.openfiledirect.index;
    return FS_RESULT_SUCCESS;
}

static FSResult FSi_CloseFileCommand(FSFile *p_file) {
#pragma unused(p_file)
    return FS_RESULT_SUCCESS;
}
