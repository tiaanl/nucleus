
#ifndef NUCLEUS_STRINGS_CHAR_PTR_H_
#define NUCLEUS_STRINGS_CHAR_PTR_H_

#include "nucleus/strings/char_ptr_traits_16.h"
#include "nucleus/strings/char_ptr_traits_32.h"
#include "nucleus/strings/char_ptr_traits_8.h"
#include "nucleus/strings/char_ptr_traits_a.h"
#include "nucleus/strings/char_ptr_traits_w.h"

namespace nu {

template <typename Traits>
class CharPtrBase {
public:
  using CharType = typename Traits::CharType;

  // Return the number of bytes required to represent the given character in
  // this encoding.
  static uint8_t bytesRequiredFor(CharType ch) { return Traits::bytesRequiredFor(ch); }

  // Construct a CharPtrBase which is wrapping a null pointer.
  CharPtrBase() {}

  // Construct a CharPtrBase which points to the given pointer.
  CharPtrBase(const CharType* ptr) : m_ptr(const_cast<CharType*>(ptr)) {}

  // Comparison operators.

  bool operator==(const CharPtrBase& other) const { return m_ptr == other.m_ptr; }

  bool operator!=(const CharPtrBase& other) const { return m_ptr != other.m_ptr; }

  bool operator<(const CharPtrBase& other) const { return m_ptr < other.m_ptr; }

  bool operator<=(const CharPtrBase& other) const { return m_ptr <= other.m_ptr; }

  bool operator>(const CharPtrBase& other) const { return m_ptr > other.m_ptr; }

  bool operator>=(const CharPtrBase& other) const { return m_ptr >= other.m_ptr; }

  // Return the unicode character that we are currently pointing to.
  char32_t getUnicodeChar() const { return Traits::getUnicodeChar(m_ptr); }

  // Return true if we are pointing to a null terminator.
  bool isNull() const { return getUnicodeChar() == 0; }

private:
  CharType* m_ptr{nullptr};
};

using CharPtrA = CharPtrBase<CharPtrTraitsA>;
using CharPtrW = CharPtrBase<CharPtrTraitsW>;

using CharPtr8 = CharPtrBase<CharPtrTraits8>;
using CharPtr16 = CharPtrBase<CharPtrTraits16>;
using CharPtr32 = CharPtrBase<CharPtrTraits32>;

}  // namespace nu

#endif  // NUCLEUS_STRINGS_CHAR_PTR_H_
