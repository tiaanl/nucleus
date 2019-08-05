
#ifndef NUCLEUS_TEXT_STRING_VIEW_H_
#define NUCLEUS_TEXT_STRING_VIEW_H_

#include "nucleus/Streams/OutputStream.h"
#include "nucleus/Types.h"

#include <algorithm>
#include <cstring>
#include <ostream>

namespace nu {

class StringView {
public:
  static StringLength kInvalidPosition;

  // The default constructor is used when you want to return a piece of text, but it is not
  // available.
  StringView() : m_text{nullptr}, m_length{0} {}

  StringView(const char* text) : StringView{text, std::strlen(text)} {}

  StringView(const char* text, StringLength length)
    : m_text{static_cast<Char*>(const_cast<Char*>(text))}, m_length{length} {}

  StringView(const StringView& other, StringLength length)
    : m_text{other.m_text}, m_length{length} {}

  Char operator[](StringLength index) const {
    return m_text[index];
  }

  Char& operator[](StringLength index) {
    return m_text[index];
  }

  bool operator==(const StringView& other) const {
    return compare(other) == 0;
  }

  bool operator!=(const StringView& other) const {
    return compare(other) != 0;
  }

  bool operator<(const StringView& other) const {
    return compare(other) < 0;
  }

  Char* getData() const {
    return m_text;
  }

  StringLength getLength() const {
    return m_length;
  }

  bool isEmpty() const {
    return m_length == 0;
  }

  I32 compare(const StringView& other) const {
    if (m_length != other.m_length) {
      return static_cast<I32>(m_length - other.m_length);
    }

    return ::strncmp(m_text, other.m_text, std::min(m_length, other.m_length));
  }

  // Return a new StringView, starting from the startIndex and ending where this StringView ended.
  StringView subString(StringLength startIndex) const {
    return StringView{m_text + startIndex, m_length - startIndex};
  }

  // Get a new StringView for a part of string inside this one.
  StringView subString(StringLength startIndex, StringLength length) const {
    return StringView{m_text + startIndex,
                      (startIndex + length > m_length) ? m_length - startIndex : length};
  }

  // Return the position of the first character that matches the given character.
  StringLength findFirstOf(Char ch) const {
    for (StringLength i = 0; i < m_length; ++i) {
      if (ch == m_text[i]) {
        return i;
      }
    }

    return kInvalidPosition;
  }

  // Return the position of the first character that matches any of the predicate characters.
  StringLength findFirstOfAny(const StringView& characters) const {
    for (StringLength i = 0; i < m_length; ++i) {
      auto pos = characters.findFirstOf(m_text[i]);
      if (pos != kInvalidPosition) {
        return i;
      }
    }

    return kInvalidPosition;
  }

  // Return the position of the last character that matches any of the predicate characters.
  StringLength findLastOfAny(const StringView& characters) {
    for (StringLength i = 0; i <= m_length; ++i) {
      auto currentPos = m_length - 1 - i;
      auto pos = characters.findFirstOf(m_text[currentPos]);
      if (pos != kInvalidPosition) {
        return currentPos;
      }
    }

    return kInvalidPosition;
  }

protected:
  Char* m_text;
  StringLength m_length;
};

inline OutputStream& operator<<(OutputStream& stream, const nu::StringView& value) {
  stream.write(value.getData(), value.getLength());
  return stream;
}

inline std::ostream& operator<<(std::ostream& os, const StringView& value) {
  for (StringLength i = 0; i < value.getLength(); ++i) {
    os << static_cast<I8>(value[i]);
  }
  return os;
}

}  // namespace nu

#endif  // NUCLEUS_TEXT_STRING_VIEW_H_
