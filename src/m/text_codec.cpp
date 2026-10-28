// Trimming, splitting and joining strings, and converting text between the game's encodings (Shift-JIS and
// UTF-8) and wide characters.

#pragma ipa file

#define MSL_STRING_DEFINITIONS
#define MSL_STRING_PLUS_DEFINITIONS
#include <set>
#include "m/text_codec.h"
#include "nitro/os.h"

// (Callees instantiated ahead of their callers, so that none is followed by its callee in our object.)
template std::string::size_type std::string::find_first_not_of(const char *s, std::string::size_type pos,
                                                               std::string::size_type n) const;
template std::string::size_type std::string::find_last_not_of(const char *s, std::string::size_type pos,
                                                              std::string::size_type n) const;
template void std::__red_black_tree<wchar_t, wchar_t, std::__identity<wchar_t>, std::less<wchar_t>,
                                    std::allocator<wchar_t> >::destroy(std::__red_black_tree_common<true>::node_base *);

// How many bytes follow a UTF-8 sequence's first byte, by its top four bits.
static const int sUtf8TrailBytes[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2};

// The default encoding (MTEXT_SJIS), in the TU's .data pool.
extern "C" struct {
    u32 unk00[2];
    int mDefaultEncoding;   // 0x08
} data_020c09c4;
#define sDefaultEncoding (data_020c09c4.mDefaultEncoding)
static std::set<wchar_t> sUnkTextChars;
static std::set<wchar_t> sUnkChars;
static OSOwnerInfo *sOwnerInfo;

// Removes the spaces at each end.
static std::string MTrimString(std::string text) {
    if (text.empty()) {
        return text;
    }
    u32 first = text.find_first_not_of(" ");
    u32 last = text.find_last_not_of(" ");
    if (first == std::string::npos) {
        return "";
    }
    if (first > last) {
        return "";
    }
    return text.substr(first, last - first + 1);
}

std::vector<std::string> MSplitString(std::string text, char separator) {
    std::vector<std::string> parts;
    text = MTrimString(text);
    if (text.empty()) {
        return parts;
    }
    for (u32 pos = text.find(separator); pos != std::string::npos; pos = text.find(separator)) {
        parts.push_back(text.substr(0, pos));
        text = MTrimString(text.substr(pos + 1, text.size() - pos));
    }
    if (!text.empty()) {
        parts.push_back(text);
    }
    return parts;
}

std::string MJoinStrings(const std::vector<std::string> &parts, char separator) {
    std::string text;
    for (std::vector<std::string>::const_iterator it = parts.begin(); it != parts.end(); it++) {
        text += *it;
        if (it != parts.end() - 1) {
            text.push_back(separator);
        }
    }
    return text;
}

void MSetDefaultEncoding(int encoding) {
    sDefaultEncoding = encoding;
}

std::wstring MDecodeText(std::string text, int encoding) {
    if (encoding == MTEXT_DEFAULT) {
        encoding = sDefaultEncoding;
    }
    switch (encoding) {
    default:
        return std::wstring();
    case MTEXT_SJIS:
        return MDecodeSjis(text);
    case MTEXT_UTF8:
        return MDecodeUtf8(text);
    }
}

std::string MEncodeText(std::wstring text, int encoding) {
    if (encoding == MTEXT_DEFAULT) {
        encoding = sDefaultEncoding;
    }
    switch (encoding) {
    default:
        return std::string();
    case MTEXT_SJIS:
        return MEncodeSjis(text);
    case MTEXT_UTF8:
        return MEncodeUtf8(text);
    }
}

std::wstring MDecodeSjis(std::string text) {
    std::wstring out;
    const u8 *p = (const u8 *) text.c_str();
    for (u8 c = *p; c != 0; c = *p) {
        if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xEF)) {
            out.push_back((c << 8) | p[1]);
            p += 2;
        } else {
            out.push_back(*p++);
        }
    }
    return out;
}

std::string MEncodeSjis(std::wstring text) {
    std::string out;
    for (const wchar_t *p = text.begin(); p != text.end(); p++) {
        wchar_t c = *p;
        if (c & 0xFF00) {
            out.push_back(c >> 8);
        }
        if ((u8) c != 0) {
            out.push_back(c);
        }
    }
    return out;
}

static int MGetUtf8TrailBytes(u8 c);
static wchar_t MDecodeUtf8Char(u32 c);
static u32 MEncodeUtf8Char(wchar_t c);

std::wstring MDecodeUtf8(std::string text) {
    std::wstring out;
    const u8 *p = (const u8 *) text.c_str();
    for (u32 c = *p; c != 0; c = *p) {
        int n = MGetUtf8TrailBytes(c);
        for (int i = 1; i <= n; i++) {
            c = (c << 8) | p[i];
        }
        p += n + 1;
        out.push_back(MDecodeUtf8Char(c));
    }
    return out;
}

std::string MEncodeUtf8(std::wstring text) {
    std::string out;
    for (const wchar_t *p = text.begin(); p != text.end(); p++) {
        u32 c = MEncodeUtf8Char(*p);
        if (c & 0xFF0000) {
            out.push_back(c >> 16);
        }
        if (c & 0xFF00) {
            out.push_back(c >> 8);
        }
        if ((u8) c != 0) {
            out.push_back(c);
        }
    }
    return out;
}

static void MSetUnkTextChars(std::wstring text);

void MSetUnkText(std::string text) {
    MSetUnkTextChars(MDecodeText(text, MTEXT_DEFAULT));
}

static void MSetUnkTextChars(std::wstring text) {
    sUnkTextChars.clear();
    const wchar_t *e = text.end().p_;
    sUnkTextChars.insert(text.begin(), std::wstring::const_iterator(e));
}

BOOL MIsUnkTextChar(wchar_t c) {
    if (sUnkTextChars.count(c)) {
        return TRUE;
    }
    return FALSE;
}

BOOL MIsUnkChar(wchar_t c) {
    if (sUnkChars.count(c)) {
        return TRUE;
    }
    return FALSE;
}

static int MGetUtf8TrailBytes(u8 c) {
    return sUtf8TrailBytes[c >> 4];
}

static u32 MEncodeUtf8Char(wchar_t c) {
    if (c <= 0x7F) {
        return c;
    }
    if (c <= 0x7FF) {
        return 0xC080 | ((c & 0x3F) | ((c & 0x7C0) << 2));
    }
    return 0xE08080 | (((c & 0xF000) << 4) | ((c & 0x3F) | ((c & 0xFC0) << 2)));
}

static wchar_t MDecodeUtf8Char(u32 c) {
    if (c <= 0x7F) {
        return c;
    }
    if (c <= 0x7FFF) {
        return (c & 0x3F) | ((c & 0x1F00) >> 2);
    }
    return (c & 0x3F) | ((c & 0x3F00) >> 2) | ((c & 0xF0000) >> 4);
}

static OSOwnerInfo *MGetOwnerInfo() {
    if (sOwnerInfo == NULL) {
        sOwnerInfo = new OSOwnerInfo;
        OS_GetOwnerInfo(sOwnerInfo);
    }
    return sOwnerInfo;
}

static std::wstring MGetOwnerName() {
    OSOwnerInfo *info = MGetOwnerInfo();
    return std::wstring((const wchar_t *) info->nickName, info->nickNameLength);
}

std::string MGetOwnerNameUtf8() {
    return MEncodeUtf8(MGetOwnerName());
}

// (Used by MMakeRomPath; the game keeps the instance here.)
template std::string std::operator+(const std::string &lhs, const std::string &rhs);
