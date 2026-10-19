// The string half of MSL's scanf. Not decompiled yet: func_0207f1bc and the formatter itself (__sformatter).

#include <stdarg.h>
#include <stdio.h>
#include <stdio_api.h>

#define EOF -1

int __sformatter(int (*ReadProc)(void *, int, int), void *ReadProcArg, const char *format, va_list arg, int isWide);

int __StringRead(void *isc, int ch, int Action) {
    char RetVal;
    __InStrCtrl *Iscp = (__InStrCtrl *) isc;
    switch (Action) {
    case __GetAChar:
        RetVal = *(Iscp->NextChar);
        if (RetVal == '\0') {
            Iscp->NullCharDetected = 1;
            return EOF;
        } else {
            Iscp->NextChar++;
            return (unsigned char) RetVal;
        }
    case __UngetAChar:
        if (Iscp->NullCharDetected == 0) {
            Iscp->NextChar--;
        } else {
            Iscp->NullCharDetected = 0;
        }
        return ch;
    case __TestForError:
        return Iscp->NullCharDetected;
    }
    return 0;
}

int vsscanf(const char *s, const char *format, va_list arg) {
    __InStrCtrl isc;
    isc.NextChar = (char *) s;
    if ((s == NULL) || (*isc.NextChar == '\0')) {
        return EOF;
    }
    isc.NullCharDetected = 0;
    return __sformatter(&__StringRead, (void *) &isc, format, arg, 0);
}

int sscanf(const char *s, const char *pformat, ...) {
    va_list args;
    va_start(args, pformat);
    return vsscanf(s, pformat, args);
}
