#ifndef NUCLEUS_TEXT_TEXT_H_
#define NUCLEUS_TEXT_TEXT_H_

#include "nucleus/Types.h"

namespace nu {

template <typename CharType>
struct CharTraits;

template <>
struct CharTraits<Char> {
  // Calculate the length of a zero terminated string.
  static constexpr auto calculateZeroTerminatedLength(const char* text) noexcept -> StringLength {
    StringLength result = 0;
    for (; *text; ++text, ++result) {
    }
    return result;
  }
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_TEXT_H_
