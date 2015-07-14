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

#ifndef NUCLEUS_STRINGS_CHAR_PTR_TRAITS_W_H_
#define NUCLEUS_STRINGS_CHAR_PTR_TRAITS_W_H_

#include <cstdint>

namespace nu {

struct CharPtrTraitsW {
  using CharType = wchar_t;
  using SizeType = usize;

  // Return the unicode character that the pointer is pointing to.
  static char32_t getUnicodeChar(CharType* ptr) {
    return static_cast<char32_t>(*ptr);
  }

  // Return the number of bytes required to represent the given character in
  // this encoding.
  static uint8_t bytesRequiredFor(char32_t ch) {
    return 1;
  }

  // Given a pointer to an array of characters, advance the pointer to the
  // beginning of the next character.
  static CharType* nextCharacter(CharType* ptr) {
    ++ptr;
    return ptr;
  }
};

}  // namespace nu

#endif  // NUCLEUS_STRINGS_CHAR_PTR_TRAITS_W_H_
