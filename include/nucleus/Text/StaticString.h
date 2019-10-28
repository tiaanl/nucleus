
#ifndef NUCLEUS_TEXT_STATIC_STRING_H_
#define NUCLEUS_TEXT_STATIC_STRING_H_

#include "nucleus/Macros.h"
#include "nucleus/Text/StringView.h"

namespace nu {

template <MemSize Size = 64>
class StaticString : public StringView {
public:
  StaticString() : StringView{m_storage, 0} {}

  StaticString(const StaticString& other) : StaticString{} {
    auto bytesToCopy = std::min(Size, other.getLength());
    std::memcpy(m_text, other.getData(), bytesToCopy);
    m_length = bytesToCopy;
  }

  StaticString(StaticString&&) = delete;

  StaticString(const StringView& other) : StaticString{} {
    auto bytesToCopy = std::min(Size, other.getLength());
    std::memcpy(m_text, other.getData(), bytesToCopy);
    m_length = bytesToCopy;
  }

  Char& operator[](StringLength index) {
    return m_text[index];
  }

  StaticString& operator=(const StaticString& other) {
    std::memcpy(m_text, other.m_text, Size);
    m_length = other.m_length;

    return *this;
  }

  StaticString& operator=(StaticString&&) = delete;

  StaticString& operator=(const StringView& other) {
    std::memcpy(m_text, other.m_text, Size);
    m_length = other.m_length;

    return *this;
  }

  constexpr MemSize getStorageSize() {
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
  }

private:
  Char m_storage[Size];
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_STATIC_STRING_H_
