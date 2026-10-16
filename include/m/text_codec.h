// Converting the game's 8-bit text to wide characters in a font's encoding.
#ifndef M_TEXT_CODEC_H
#define M_TEXT_CODEC_H

#include <string>
#include <vector>

// Encoding 0 means the default one.
std::wstring MDecodeText(std::string text, int encoding);
// Splits text at each separator.
std::vector<std::string> MSplitString(std::string text, char separator);

#endif
