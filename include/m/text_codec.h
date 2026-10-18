// Converting the game's 8-bit text to wide characters and back, and splitting and joining strings.
#ifndef M_TEXT_CODEC_H
#define M_TEXT_CODEC_H

#include <string>
#include <vector>
#include "types.h"

enum {
    MTEXT_DEFAULT = 0,  // the default encoding (set by MSetDefaultEncoding)
    MTEXT_SJIS = 1,
    MTEXT_UTF8 = 2,
};

// Splits text at each separator, trimming the spaces around each part.
std::vector<std::string> MSplitString(std::string text, char separator);
std::string MJoinStrings(const std::vector<std::string> &parts, char separator);

void MSetDefaultEncoding(int encoding);
std::wstring MDecodeText(std::string text, int encoding);
std::string MEncodeText(std::wstring text, int encoding);
std::wstring MDecodeSjis(std::string text);
std::string MEncodeSjis(std::wstring text);
std::wstring MDecodeUtf8(std::string text);
std::string MEncodeUtf8(std::wstring text);

// Sets the characters of a text (in the default encoding) that MIsUnkTextChar looks for.
void MSetUnkText(std::string text);
BOOL MIsUnkTextChar(wchar_t c);
// (Looks in a second set, which nothing fills.)
BOOL MIsUnkChar(wchar_t c);

// The console owner's nickname.
std::string MGetOwnerNameUtf8();

#endif
