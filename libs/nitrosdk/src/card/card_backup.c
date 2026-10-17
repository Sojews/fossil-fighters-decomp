// NitroSDK CARD: the backup chip (EEPROM, flash or FRAM). What the chip can do follows from its type, which the
// game declares; reads and writes go to the ARM7 a page at a time through a buffer in the shared state.

#include "card_common.h"

#define CARD_BACKUP_TYPE_DEVICE_SHIFT 0
#define CARD_BACKUP_TYPE_DEVICE_MASK 0xff
#define CARD_BACKUP_TYPE_SIZEBIT_SHIFT 8
#define CARD_BACKUP_TYPE_SIZEBIT_MASK 0xff
#define CARD_BACKUP_TYPE_DEVICE_EEPROM 1
#define CARD_BACKUP_TYPE_DEVICE_FLASH 2
#define CARD_BACKUP_TYPE_DEVICE_FRAM 3

typedef enum {
    CARD_BACKUP_TYPE_NOT_USE = 0
} CARDBackupType;

#define CARD_BACKUP_CAPS_AVAILABLE ((1 << CARD_REQ_READ_BACKUP) - 1)
#define CARD_BACKUP_CAPS_READ (1 << CARD_REQ_READ_BACKUP)
#define CARD_BACKUP_CAPS_WRITE (1 << CARD_REQ_WRITE_BACKUP)
#define CARD_BACKUP_CAPS_PROGRAM (1 << CARD_REQ_PROGRAM_BACKUP)
#define CARD_BACKUP_CAPS_VERIFY (1 << CARD_REQ_VERIFY_BACKUP)
#define CARD_BACKUP_CAPS_ERASE_PAGE (1 << CARD_REQ_ERASE_PAGE_BACKUP)
#define CARD_BACKUP_CAPS_ERASE_SECTOR (1 << CARD_REQ_ERASE_SECTOR_BACKUP)
#define CARD_BACKUP_CAPS_ERASE_CHIP (1 << CARD_REQ_ERASE_CHIP_BACKUP)

// The library's "[SDK+NINTENDO:BACKUP]" tag. It sits in the version section, which the linker puts after the
// crt0's build information; referring to it keeps it in the link.
extern char cardi_backup_id_string[];

static void CARDi_IdentifyBackupCore(CARDBackupType type) {
    CARDiCommandArg *const cmd = cardi_common.cmd;

    MI_CpuFill8(&cmd->spec, 0, sizeof(cmd->spec));
    cmd->type = type;
    cmd->spec.caps = CARD_BACKUP_CAPS_AVAILABLE;
    if (type == CARD_BACKUP_TYPE_NOT_USE) {
        return;
    }
    {
        const u32 size = (u32) (1 << ((type >> CARD_BACKUP_TYPE_SIZEBIT_SHIFT) & CARD_BACKUP_TYPE_SIZEBIT_MASK));
        const int device = (int) ((type >> CARD_BACKUP_TYPE_DEVICE_SHIFT) & CARD_BACKUP_TYPE_DEVICE_MASK);

        cmd->spec.total_size = size;
        cmd->spec.initial_status = 0xff;
        if (device == CARD_BACKUP_TYPE_DEVICE_EEPROM) {
            switch (size) {
            default:
                goto invalid_type;
            case 0x000200:
                cmd->spec.page_size = 0x10;
                cmd->spec.addr_width = 1;
                cmd->spec.program_page = 5;
                cmd->spec.initial_status = 0xf0;
                break;
            case 0x002000:
                cmd->spec.page_size = 0x20;
                cmd->spec.addr_width = 2;
                cmd->spec.program_page = 5;
                cmd->spec.initial_status = 0x00;
                break;
            case 0x010000:
                cmd->spec.page_size = 0x80;
                cmd->spec.addr_width = 2;
                cmd->spec.program_page = 10;
                cmd->spec.initial_status = 0x00;
                break;
            }
            cmd->spec.sect_size = cmd->spec.page_size;
            cmd->spec.caps |= CARD_BACKUP_CAPS_READ;
            cmd->spec.caps |= CARD_BACKUP_CAPS_PROGRAM;
            cmd->spec.caps |= CARD_BACKUP_CAPS_VERIFY;
            return;
        } else if (device == CARD_BACKUP_TYPE_DEVICE_FLASH) {
            switch (size) {
            default:
                goto invalid_type;
            case 0x040000:
            case 0x080000:
            case 0x100000:
                cmd->spec.write_page = 25;
                cmd->spec.write_page_total = 300;
                cmd->spec.erase_page = 300;
                cmd->spec.erase_sector = 5000;
                cmd->spec.caps |= CARD_BACKUP_CAPS_WRITE;
                cmd->spec.caps |= CARD_BACKUP_CAPS_ERASE_PAGE;
                break;
            case 0x200000:
                cmd->spec.write_page = 23;
                cmd->spec.write_page_total = 300;
                cmd->spec.erase_sector = 500;
                cmd->spec.erase_sector_total = 5000;
                cmd->spec.erase_chip = 10000;
                cmd->spec.erase_chip_total = 60000;
                cmd->spec.initial_status = 0x00;
                cmd->spec.caps |= CARD_BACKUP_CAPS_WRITE;
                cmd->spec.caps |= CARD_BACKUP_CAPS_ERASE_PAGE;
                cmd->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                break;
            case 0x800000:
                cmd->spec.erase_sector = 1000;
                cmd->spec.erase_sector_total = 3000;
                cmd->spec.erase_chip = 68000;
                cmd->spec.erase_chip_total = 160000;
                cmd->spec.initial_status = 0x00;
                cmd->spec.caps |= CARD_BACKUP_CAPS_ERASE_CHIP;
                break;
            }
            cmd->spec.sect_size = 0x10000;
            cmd->spec.page_size = 0x100;
            cmd->spec.addr_width = 3;
            cmd->spec.program_page = 5;
            cmd->spec.caps |= CARD_BACKUP_CAPS_READ;
            cmd->spec.caps |= CARD_BACKUP_CAPS_PROGRAM;
            cmd->spec.caps |= CARD_BACKUP_CAPS_VERIFY;
            cmd->spec.caps |= CARD_BACKUP_CAPS_ERASE_SECTOR;
            return;
        } else if (device == CARD_BACKUP_TYPE_DEVICE_FRAM) {
            switch (size) {
            default:
                goto invalid_type;
            case 0x002000:
            case 0x008000:
                break;
            }
            cmd->spec.page_size = size;
            cmd->spec.sect_size = size;
            cmd->spec.addr_width = 2;
            cmd->spec.initial_status = 0x00;
            cmd->spec.caps |= CARD_BACKUP_CAPS_READ;
            cmd->spec.caps |= CARD_BACKUP_CAPS_PROGRAM;
            cmd->spec.caps |= CARD_BACKUP_CAPS_VERIFY;
            return;
        } else {
        invalid_type:
            cmd->type = CARD_BACKUP_TYPE_NOT_USE;
            cmd->spec.total_size = 0;
            cardi_common.cmd->result = CARD_RESULT_UNSUPPORTED;
        }
    }
}

void CARDi_RequestStreamCommandCore(CARDiCommon *p) {
    const int req_type = p->req_type;
    const int req_mode = p->req_mode;
    const int retry_count = p->req_retry;
    u32 size = sizeof(p->backup_cache_page_buf);

    OSi_ReferSymbol(cardi_backup_id_string);
    if (req_type == CARD_REQ_ERASE_SECTOR_BACKUP) {
        size = CARD_GetBackupSectorSize();
    }
    do {
        const u32 len = (size < p->len) ? size : p->len;
        p->cmd->len = len;
        if ((p->flag & CARD_STAT_CANCEL) != 0) {
            p->flag &= ~CARD_STAT_CANCEL;
            p->cmd->result = CARD_RESULT_CANCELED;
            break;
        }
        switch (req_mode) {
        case CARD_REQUEST_MODE_RECV:
            DC_InvalidateRange(p->backup_cache_page_buf, len);
            p->cmd->src = (u32) p->src;
            p->cmd->dst = (u32) p->backup_cache_page_buf;
            break;
        case CARD_REQUEST_MODE_SEND:
        case CARD_REQUEST_MODE_SEND_VERIFY:
            MI_CpuCopy8((const void *) p->src, p->backup_cache_page_buf, len);
            DC_FlushRange(p->backup_cache_page_buf, len);
            DC_WaitWriteBufferEmpty();
            p->cmd->src = (u32) p->backup_cache_page_buf;
            p->cmd->dst = (u32) p->dst;
            break;
        case CARD_REQUEST_MODE_SPECIAL:
            p->cmd->src = (u32) p->src;
            p->cmd->dst = (u32) p->dst;
            break;
        }
        if (!CARDi_Request(p, req_type, retry_count)) {
            break;
        }
        if (req_mode == CARD_REQUEST_MODE_SEND_VERIFY) {
            if (!CARDi_Request(p, CARD_REQ_VERIFY_BACKUP, 1)) {
                break;
            }
        } else if (req_mode == CARD_REQUEST_MODE_RECV) {
            MI_CpuCopy8(p->backup_cache_page_buf, (void *) p->dst, len);
        }
        p->src += len;
        p->dst += len;
        p->len -= len;
    } while (p->len > 0);
    CARDi_EndTask(p, TRUE);
}

BOOL CARDi_RequestStreamCommand(u32 src, u32 dst, u32 len, MIDmaCallback callback, void *arg, BOOL is_async,
                                CARDRequest req_type, int req_retry, CARDRequestMode req_mode) {
    CARDiCommon *const p = &cardi_common;

    OSi_ReferSymbol(cardi_backup_id_string);
    CARDi_WaitTask(p, callback, arg);
    p->src = src;
    p->dst = dst;
    p->len = len;
    p->req_type = req_type;
    p->req_retry = req_retry;
    p->req_mode = req_mode;
    if (is_async) {
        CARDi_SetTask(CARDi_RequestStreamCommandCore);
        return TRUE;
    }
    cardi_common.cur_th = OS_GetCurrentThread();
    CARDi_RequestStreamCommandCore(p);
    return p->cmd->result == CARD_RESULT_SUCCESS;
}

u32 CARD_GetBackupTotalSize(void) {
    return cardi_common.cmd->spec.total_size;
}

u32 CARD_GetBackupSectorSize(void) {
    return cardi_common.cmd->spec.sect_size;
}

u32 CARD_GetBackupPageSize(void) {
    return cardi_common.cmd->spec.page_size;
}

BOOL CARD_IdentifyBackup(CARDBackupType type) {
    CARDiCommon *const p = &cardi_common;

    OSi_ReferSymbol(cardi_backup_id_string);
    if (type == CARD_BACKUP_TYPE_NOT_USE) {
        OS_Terminate();
    }
    CARD_CheckEnabled();
    CARDi_WaitTask(p, NULL, NULL);
    CARDi_IdentifyBackupCore(type);
    cardi_common.cur_th = OS_GetCurrentThread();
    (void) CARDi_Request(p, CARD_REQ_IDENTIFY, 1);
    p->cmd->src = 0;
    p->cmd->dst = (u32) p->backup_cache_page_buf;
    p->cmd->len = 1;
    (void) CARDi_Request(p, CARD_REQ_READ_BACKUP, 1);
    CARDi_EndTask(p, TRUE);
    return p->cmd->result == CARD_RESULT_SUCCESS;
}
