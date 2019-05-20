
#ifndef NUCLEUS_TEXT_STRING_VIEW_H_
#define NUCLEUS_TEXT_STRING_VIEW_H_

#include "nucleus/Types.h"

#include <algorithm>
#include <cstring>

namespace nu {

using StringLength = MemSize;
using Char = I8;

class StringView {
public:
  // The default constructor is used when you want to return a piece of text, but it is not
  // available.
  StringView() : m_text(0), m_length(0) {}

  StringView(const char* text)
    : m_text{zeroTerminatedToCharPointer(text)},
      m_length{calculateLength(zeroTerminatedToCharPointer(text))} {}

  StringView(const char* text, MemSize length)
    : m_text{zeroTerminatedToCharPointer(text)}, m_length{length} {}

  StringView(const StringView& other, StringLength length)
    : m_text{other.m_text}, m_length{length} {}

  Char operator[](StringLength index) const {
    return m_text[index];
  }

  Char operator[](StringLength index) {
    return m_text[index];
  }

  Char* getData() const {
    return m_text;
  }

  MemSize getLength() const {
    return m_length;
  }

  I32 compare(StringView other) {
    return std::strncmp(m_text, other.m_text, std::min(m_length, other.m_length));
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

private:
  static Char* zeroTerminatedToCharPointer(const char* zeroTerminated) {
    return static_cast<Char*>(const_cast<char*>(zeroTerminated));
  }

  static StringLength calculateLength(Char* text) {
    StringLength length = 0;
    for (auto p = text; *p; ++p, ++length) {
    }
    return length;
  }

  Char* m_text;
  StringLength m_length;
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_STRING_VIEW_H_
