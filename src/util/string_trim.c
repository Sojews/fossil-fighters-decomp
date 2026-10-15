// String helpers used by text/script parsing in ARM9 main.

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
