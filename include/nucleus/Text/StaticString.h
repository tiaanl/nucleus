
#ifndef NUCLEUS_TEXT_STATIC_STRING_H_
#define NUCLEUS_TEXT_STATIC_STRING_H_

#include "nucleus/Text/StringView.h"

namespace nu {

template <MemSize Size = 64>
class StaticString : public StringView {
public:
  StaticString() : StringView{m_storage, 0} {}

  StaticString(const StringView& text) : StaticString{} {
    MemSize bytesToCopy = std::min(Size - 1, text.getLength());
    std::strncpy(m_text, text.getData(), bytesToCopy);
    m_length = bytesToCopy;
    m_text[m_length] = 0;
  }

  MemSize getStorageSize() {
    return Size;
  }

  void append(const char* text) {
    append(text, std::strlen(text));
  }

  void append(const char* text, StringLength length) {
    StringLength bytesToCopy = std::min(Size - m_length - 1, length);
    std::strncat(m_storage, text, bytesToCopy);
    m_length += bytesToCopy;
    m_text[m_length] = '\0';
  }

private:
  Char m_storage[Size];
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_STATIC_STRING_H_
