
#ifndef NUCLEUS_TEXT_STRING_VIEW_H_
#define NUCLEUS_TEXT_STRING_VIEW_H_

#include "nucleus/Types.h"

namespace nu {

using StringLength = MemSize;
using Char = I8;

class StringView {
public:
  StringView(const char* text)
    : m_text{zeroTerminatedToCharPointer(text)},
      m_length{calculateLength(zeroTerminatedToCharPointer(text))} {}

  StringView(const char* text, MemSize length)
    : m_text{zeroTerminatedToCharPointer(text)}, m_length{length} {}

  Char* getData() const {
    return m_text;
  }

  MemSize getLength() const {
    return m_length;
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
