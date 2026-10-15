// Helpers for parsing the game's text data files: lines, comma-separated fields and numbers.

#include <string.h>
#include "types.h"

// sFracTable[n][d] is the fx32 value of digit d at decimal place n+1, rounded down.
static const fx32 sFracTable[8][10] = {
    {0, 409, 819, 1228, 1638, 2048, 2457, 2867, 3276, 3686},
    {0, 40, 81, 122, 163, 204, 245, 286, 327, 368},
    {0, 4, 8, 12, 16, 20, 24, 28, 32, 36},
    {0, 0, 0, 1, 1, 2, 2, 2, 3, 3},
    {0},
    {0},
    {0},
    {0},
};

static char sLineBuf[256];

// Advances *p past any spaces and tabs.
void SkipWhitespace(char **p) {
    while ((*p)[0] == ' ' || (*p)[0] == '\t') {
        (*p)++;
    }
}

// Copies src to dst with leading and trailing spaces/tabs removed.
void TrimCopy(const char *src, char *dst) {
    while ((*src == ' ' || *src == '\t') && *src != '\0') {
        src++;
    }
    if (*src == '\0') {
        *dst = '\0';
        return;
    }
    while (*src != '\0') {
        *dst++ = *src++;
    }
    *dst = '\0';
    dst--;
    while (*dst == ' ' || *dst == '\t') {
        *dst-- = '\0';
    }
}

// Trims str in place and removes one pair of surrounding double quotes.
void StripQuotes(char *str) {
    char buf[256];

    TrimCopy(str, buf);
    if (strlen(buf) >= 2 && buf[0] == '"' && buf[strlen(buf) - 1] == '"') {
        buf[strlen(buf) - 1] = '\0';
        strcpy(str, buf + 1);
        TrimCopy(str, buf);
    }
    strcpy(str, buf);
}

// Reads one line (up to 256 chars) from *p, skipping the CR/LF after it. Returns NULL at the end of the text.
char *ReadLine(char **p) {
    int i = 0;

    if ((*p)[0] == '\0') {
        return NULL;
    }
    while ((*p)[0] != '\r' && (*p)[0] != '\n' && (*p)[0] != '\0') {
        sLineBuf[i++] = *(*p)++;
        if (i == 256) {
            break;
        }
    }
    sLineBuf[i] = '\0';
    if ((*p)[0] == '\r') {
        (*p)++;
        if ((*p)[0] == '\n') {
            (*p)++;
        }
    }
    if ((*p)[0] == '\n') {
        (*p)++;
        if ((*p)[0] == '\r') {
            (*p)++;
        }
    }
    return sLineBuf;
}

// Parses a decimal number like "-12.5" as fx32. Returns 0x7FFFFFFF if there are no digits.
fx32 ParseFx32(char **p) {
    fx32 value;
    int place;
    BOOL found;
    BOOL negative;

    SkipWhitespace(p);
    value = 0;
    place = 0;
    found = FALSE;
    negative = FALSE;
    if ((*p)[0] == '-') {
        negative = TRUE;
        (*p)++;
    }
    while ((*p)[0] >= '0' && (*p)[0] <= '9') {
        value *= 10;
        value += ((*p)[0] - '0') << FX32_SHIFT;
        (*p)++;
        found = TRUE;
    }
    if ((*p)[0] == '.') {
        (*p)++;
        while ((*p)[0] >= '0' && (*p)[0] <= '9') {
            value += sFracTable[place++][(*p)[0] - '0'];
            (*p)++;
            found = TRUE;
        }
    }
    if (negative) {
        value *= -1;
    }
    if (!found) {
        value = 0x7FFFFFFF;
    }
    return value;
}

// Parses a decimal integer. Returns 0x7FFFFFFF if there are no digits.
s32 ParseInt(char **p) {
    s32 value;
    BOOL negative;
    BOOL found;
    char c;

    SkipWhitespace(p);
    value = 0;
    negative = FALSE;
    found = FALSE;
    if ((*p)[0] == '-') {
        negative = TRUE;
        (*p)++;
    }
    while ((c = (*p)[0]) >= '0' && c <= '9') {
        value *= 10;
        value += c - '0';
        (*p)++;
        found = TRUE;
    }
    if (negative) {
        value *= -1;
    }
    if (!found) {
        value = 0x7FFFFFFF;
    }
    return value;
}

// Copies the field up to the next ',' (or the end) into out, then trims and unquotes it.
void ReadField(char **p, char *out) {
    char *start = out;

    while ((*p)[0] != ',' && (*p)[0] != '\0') {
        *out++ = *(*p)++;
    }
    *out = '\0';
    StripQuotes(start);
}

// Skips a ',' at *p. Returns whether there was one.
BOOL SkipComma(char **p) {
    if ((*p)[0] == ',') {
        (*p)++;
        return TRUE;
    }
    return FALSE;
}
