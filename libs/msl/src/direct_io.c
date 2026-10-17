// MSL: fwrite.

#include "types.h"
#include <buffer_io.h>
#include <file_struc.h>
#include <mem.h>
#include <string.h>
#include <wchar_io.h>

size_t fwrite(const void *buffer, size_t size, size_t count, FILE *file) {
    unsigned char *write_ptr = (unsigned char *)buffer;
    size_t num_bytes;
    size_t bytes_to_go;
    size_t bytes_written;
    unsigned char *save_buffer;
    int always_buffer;
    unsigned char *newline;

    if (fwide(file, 0) == 0) {
        fwide(file, -1);
    }
    bytes_to_go = size * count;
    if (bytes_to_go == 0 || file->state.error || file->mode.file_kind == __closed_file) {
        return 0;
    }

    always_buffer = 1;
    if (file->mode.binary_io && file->mode.buffer_mode != _IOFBF && file->mode.buffer_mode != _IOLBF) {
        always_buffer = 0;
    }
    if (file->state.io_state == __neutral && (file->mode.io_mode & __write)) {
        file->state.io_state = __writing;
        __prep_buffer(file);
    }
    if (file->state.io_state != __writing) {
        set_error(file);
        return 0;
    }

    bytes_written = 0;
    if (bytes_to_go && (file->buffer_ptr != file->buffer || always_buffer)) {
        file->buffer_len = file->buffer_size - (file->buffer_ptr - file->buffer);
        do {
            newline = NULL;
            num_bytes = file->buffer_len;
            if (num_bytes > bytes_to_go) {
                num_bytes = bytes_to_go;
            }
            if (file->mode.buffer_mode == _IOLBF && num_bytes) {
                if ((newline = (unsigned char *)__memrchr(write_ptr, '\n', num_bytes)) != NULL) {
                    num_bytes = newline + 1 - write_ptr;
                }
            }
            if (num_bytes) {
                memcpy(file->buffer_ptr, write_ptr, num_bytes);
                write_ptr += num_bytes;
                bytes_to_go -= num_bytes;
                file->buffer_ptr += num_bytes;
                file->buffer_len -= num_bytes;
            }
            if ((!file->buffer_len || newline || file->mode.buffer_mode == _IONBF) && __flush_buffer(file, NULL)) {
                set_error(file);
                bytes_to_go = 0;
                break;
            }
            bytes_written += num_bytes;
        } while (bytes_to_go && always_buffer);
    }

    if (bytes_to_go && !always_buffer) {
        size_t save_size = file->buffer_size;

        save_buffer = file->buffer;

        file->buffer = write_ptr;
        file->buffer_size = bytes_to_go;
        file->buffer_ptr = write_ptr + bytes_to_go;
        if (__flush_buffer(file, &num_bytes)) {
            set_error(file);
        } else {
            bytes_written += num_bytes;
        }
        file->buffer = save_buffer;
        file->buffer_size = save_size;
        __prep_buffer(file);
        file->buffer_len = 0;
    }
    if (file->mode.buffer_mode != _IOFBF) {
        file->buffer_len = 0;
    }
    return bytes_written / size;
}
