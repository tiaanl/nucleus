// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

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
  bool operator()(CharType left, CharType right) const {
    return std::tolower(left) == std::tolower(right);
  }
};

}  // namespace

bool StartsWithASCII(const std::string& str, const std::string& search,
                     bool caseSensitive) {
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
bool StartsWithT(const StrType& str, const StrType& search,
                 bool caseSensitive) {
  if (caseSensitive) {
    return str.compare(0, search.length(), search) == 0;
  } else {
    if (search.size() > str.size())
      return false;
    return std::equal(std::begin(search), std::end(search), std::begin(str),
                      CaseInsensitiveCompare<typename StrType::value_type>());
  }
}

bool StartsWith(const std::u16string& str, const std::u16string& search,
                bool caseSensitive) {
  return StartsWithT(str, search, caseSensitive);
}

}  // namespace nu
