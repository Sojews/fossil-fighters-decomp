#ifndef _C_WCTYPE_H
#define _C_WCTYPE_H

#include <ctype.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wide characters below 128 use the narrow table.
static inline int iswdigit(unsigned int c) {
    return ((c < 0) || (c >= __msl_cmap_size)) ? 0 : (int) (_MSL_CMAP_ACCESS[c] & __msl_digit);
}

static inline int iswupper(unsigned int c) {
    return ((c < 0) || (c >= __msl_cmap_size)) ? 0 : (int) (_MSL_CMAP_ACCESS[c] & __msl_upper);
}

#ifdef __cplusplus
}
#endif

#endif
