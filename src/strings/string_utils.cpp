
#include "nucleus/strings/string_utils.h"

#include "nucleus/config.h"

#if COMPILER(MSVC)
#include <cctype>
#endif

#include <cstring>

namespace nu {

namespace {

template <typename CharType>
struct CaseInsensitiveCompare {
  bool operator()(CharType left, CharType right) const { return std::tolower(left) == std::tolower(right); }
};

}  // namespace

bool StartsWithASCII(const std::string& str, const std::string& search, bool caseSensitive) {
  if (caseSensitive) {
    return str.compare(0, search.length(), search) == 0;
  } else {
#if COMPILER(MSVC)
    return _strnicmp(str.c_str(), search.c_str(), search.length()) == 0;
#elif COMPILER(GCC)
    return strncmp(str.c_str(), search.c_str(), search.length()) == 0;
#endif
  }
  return false;
}

template <typename StrType>
bool StartsWithT(const StrType& str, const StrType& search, bool caseSensitive) {
  if (caseSensitive) {
    return str.compare(0, search.length(), search) == 0;
  } else {
    if (search.size() > str.size())
      return false;
    return std::equal(std::begin(search), std::end(search), std::begin(str),
                      CaseInsensitiveCompare<typename StrType::value_type>());
  }
}

bool StartsWith(const std::u16string& str, const std::u16string& search, bool caseSensitive) {
  return StartsWithT(str, search, caseSensitive);
}

}  // namespace nu
