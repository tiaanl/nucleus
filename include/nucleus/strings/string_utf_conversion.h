
#ifndef NUCLEUS_STRINGS_STRING_UTF_CONVERSION_H_
#define NUCLEUS_STRINGS_STRING_UTF_CONVERSION_H_

#include <string>

namespace nu {

// This converts an ASCII string, typically a hardcoded constant, to a UTF16
// string.
std::u16string ASCIIToUTF16(const std::string& ascii);

// Converts to 7-bit ASCII by truncating. The result must be known to be ASCII
// beforehand.
std::string UTF16ToASCII(const std::u16string& utf16);

}  // namespace nu

#endif  // NUCLEUS_STRINGS_STRING_UTF_CONVERSION_H_
