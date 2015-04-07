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

#ifndef BASE_STRINGS_CHAR_PTR_H_
#define BASE_STRINGS_CHAR_PTR_H_

#include "base/strings/char_ptr_traits_a.h"
#include "base/strings/char_ptr_traits_8.h"
#include "base/strings/char_ptr_traits_16.h"
#include "base/strings/char_ptr_traits_32.h"

namespace base {

template <typename Traits>
class CharPtrBase {
public:
  using CharType = Traits::CharType;

  CharPtr() {}

  CharPtr(const CharType* ptr) : m_ptr(ptr) {}

  // Return the unicode character that we are currently pointing to.
  char32_t getUnicodeChar() const { return Traits::getUnicodeChar(m_ptr); }

  // Return true if we are pointing to a null terminator.
  bool isNull() const { return getUnicodeChar() == 0; }

  // Return the number of bytes required to represent the given character in
  // this encoding.
  static uint8_t bytesRequiredFor(CharType ch) {
    return Traits::bytesRequiredFor(ch);
  }

private:
  CharType* m_ptr{nullptr};
};

using CharPtrA = CharPtrBase<CharPtrTraitsA>;
using CharPtr8 = CharPtrBase<CharPtrTraits8>;
using CharPtr16 = CharPtrBase<CharPtrTraits16>;
using CharPtr32 = CharPtrBase<CharPtrTraits32>;

}  // namespace base

#endif  // BASE_STRINGS_CHAR_PTR_H_
