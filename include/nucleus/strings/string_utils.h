
#ifndef NUCLEUS_STRINGS_STRING_UTILS_H_
#define NUCLEUS_STRINGS_STRING_UTILS_H_

#include <string>

namespace nu {

bool StartsWithASCII(const std::string& str, const std::string& search, bool caseSensitive);

bool StartsWith(const std::u16string& str, const std::u16string& search, bool caseSensitive);

}  // namespace nu

#endif  // NUCLEUS_STRINGS_STRING_UTILS_H_
