
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
    std::memcpy(m_text, text.getData(), bytesToCopy);
    m_length = bytesToCopy;
    m_text[m_length] = 0;
  }

  StaticString(const StaticString& other) : StaticString{} {
    std::memcpy(m_storage, other.m_storage, Size);
    m_length = other.m_length;
  }

  Char& operator[](StringLength index) {
    return m_text[index];
  }

  StaticString& operator=(const StaticString& other) {
    m_text = m_storage;

    std::memcpy(m_storage, other.m_storage, Size);
    m_length = other.m_length;

    return *this;
  }

  MemSize getStorageSize() {
    return Size;
  }

  void resize(StringLength length, Char fill = '\0') {
    length = std::min(length, Size);

    if (length > m_length) {
      for (auto i = m_length; i < length; ++i) {
        m_storage[i] = fill;
      }
    }

    m_length = length;
  }

  void append(const nu::StringView& text) {
    append(text.getData(), text.getLength());
  }

  void append(const char* text) {
    append(text, std::strlen(text));
  }

  void append(const char* text, StringLength length) {
    StringLength bytesToCopy = std::min(Size - m_length - 1, length);
    std::memcpy(m_storage + m_length, text, bytesToCopy);
    m_length += bytesToCopy;
    m_text[m_length] = '\0';
  }

private:
  Char m_storage[Size];
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_STATIC_STRING_H_
