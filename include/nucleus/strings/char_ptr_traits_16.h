
#ifndef NUCLEUS_STRINGS_CHAR_PTR_TRAITS_16_H_
#define NUCLEUS_STRINGS_CHAR_PTR_TRAITS_16_H_

#include <cstdint>

namespace nu {

struct CharPtrTraits16 {
  using CharType = char16_t;
  using SizeType = size_t;

  // Return the unicode character that the pointer is pointing to.
  static char32_t getUnicodeChar(CharType* ptr) { return static_cast<char32_t>(*ptr); }

  // Return the number of bytes required to represent the given character in
  // this encoding.
  static uint8_t bytesRequiredFor(char32_t ch) { return 1; }

  // Given a pointer to an array of characters, advance the pointer to the
  // beginning of the next character.
  static CharType* nextCharacter(CharType* ptr) {
    ++ptr;
    return ptr;
  }
};

}  // namespace nu

#endif  // NUCLEUS_STRINGS_CHAR_PTR_TRAITS_16_H_
