
#ifndef NUCLEUS_TEXT_STRING_VIEW_H_
#define NUCLEUS_TEXT_STRING_VIEW_H_

#include <algorithm>
#include <cstring>
#include <ostream>
#include <utility>

#include "nucleus/Streams/OutputStream.h"
#include "nucleus/Text/CharTraits.h"
#include "nucleus/Types.h"

namespace nu {

class StringView {
public:
  static constexpr StringLength npos = -1;

  // Construct

  constexpr StringView() noexcept : m_text{nullptr}, m_length{0} {}

  constexpr StringView(const StringView&) noexcept = default;

  // Construct a StringView from a zero-terminated c-style string.  The length of text is
  // calculated.
  constexpr StringView(const char* text) noexcept
    : StringView{text, CharTraits<Char>::calculateZeroTerminatedLength(text)} {}

  // Construct a StringView from a zero-terminated c-style string and a length.
  constexpr StringView(const char* text, std::size_t length) noexcept
    : m_text{static_cast<Char*>(const_cast<Char*>(text))}, m_length{length} {}

  // Element Access

  constexpr auto operator[](StringLength index) const noexcept -> Char {
    return m_text[index];
  }

  // Capacity

  constexpr auto data() const noexcept -> Char* {
    return m_text;
  }

  constexpr auto length() const noexcept -> StringLength {
    return m_length;
  }

  constexpr auto empty() const noexcept -> bool {
    return m_length == 0;
  }

  // Operations

  bool operator==(const StringView& other) const {
    return compare(other) == 0;
  }

  bool operator!=(const StringView& other) const {
    return compare(other) != 0;
  }

  bool operator<(const StringView& other) const {
    return compare(other) < 0;
  }

  I32 compare(const StringView& other) const {
    if (m_length != other.m_length) {
      return static_cast<I32>(m_length - other.m_length);
    }

    return ::strncmp(m_text, other.m_text, std::min(m_length, other.m_length));
  }

  // Returns the substring [position, position + length).  If the position or length is outside of
  // the string, then we return what we can.
  constexpr auto subString(StringLength position, StringLength length = npos) const noexcept
      -> StringView {
    if (position >= m_length) {
      return {};
    }
    return {m_text + position, std::min(length, m_length - position)};
  }

  // Return the position of the first character that matches the given character.
  StringLength findFirstOf(Char ch) const {
    for (StringLength i = 0; i < m_length; ++i) {
      if (ch == m_text[i]) {
        return i;
      }
    }

    return npos;
  }

  // Return the position of the first character that matches any of the predicate characters.
  StringLength findFirstOfAny(const StringView& characters) const {
    for (StringLength i = 0; i < m_length; ++i) {
      auto pos = characters.findFirstOf(m_text[i]);
      if (pos != npos) {
        return i;
      }
    }

    return npos;
  }

  // Return the position of the last character that matches any of the predicate characters.
  StringLength findLastOfAny(const StringView& characters) {
    for (StringLength i = 0; i < m_length; ++i) {
      auto currentPos = m_length - 1 - i;
      auto pos = characters.findFirstOf(m_text[currentPos]);
      if (pos != npos) {
        return currentPos;
      }
    }

    return npos;
  }

  bool contains(StringView needle) {
    for (StringLength i = 0; i < m_length - needle.length() + 1; ++i) {
      if (std::strncmp(m_text + i, needle.m_text, needle.m_length) == 0) {
        return true;
      }
    }

    return false;
  }

protected:
  template <MemSize Size>
  friend class StaticString;

  Char* m_text;
  StringLength m_length;
};

inline OutputStream& operator<<(OutputStream& stream, const nu::StringView& value) {
  stream.write(value.data(), value.length());
  return stream;
}

inline std::ostream& operator<<(std::ostream& os, const StringView& value) {
  for (StringLength i = 0; i < value.length(); ++i) {
    os.rdbuf()->sputc(value[i]);
  }
  return os;
}

}  // namespace nu

template <>
struct std::hash<nu::StringView> {
  std::size_t operator()(const nu::StringView& value) const {
    std::size_t hash = 7;
    for (StringLength i = 0; i < value.length(); ++i) {
      hash = hash * 31 + value[i];
    }
    return hash;
  }
};

#endif  // NUCLEUS_TEXT_STRING_VIEW_H_
