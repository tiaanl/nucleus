
#ifndef NUCLEUS_TEXT_DYNAMIC_STRING_H_
#define NUCLEUS_TEXT_DYNAMIC_STRING_H_

#include "nucleus/Logging.h"
#include "nucleus/Text/StringView.h"

namespace nu {

class DynamicString : public StringView {
public:
  DynamicString() : StringView(), m_allocated{0} {}

  DynamicString(const char* text) : DynamicString{text, std::strlen(text)} {}

  DynamicString(const char* text, StringLength length) : StringView{}, m_allocated{0} {
    ensureAllocated(length + 1, false);
    std::memcpy(m_text, text, length);
    m_text[length] = '\0';
    m_length = length;
  }

  explicit DynamicString(const StringView& text) : DynamicString{text.data(), text.length()} {}

  DynamicString(const DynamicString& other) : DynamicString{other.m_text, other.m_length} {}

  ~DynamicString() {
    delete[] m_text;
  }

  Char& operator[](StringLength index) {
    return m_text[index];
  }

  DynamicString& operator=(const DynamicString& other) {
    ensureAllocated(other.m_length + 1, false);
    std::memcpy(m_text, other.m_text, other.m_length);
    m_text[other.m_length] = '\0';
    m_length = other.m_length;

    return *this;
  }

  DynamicString& operator=(const StringView& other) {
    auto otherLength = other.length();

    ensureAllocated(otherLength + 1, false);
    std::memcpy(m_text, other.data(), otherLength);
    m_length = otherLength;
    m_text[m_length] = '\0';

    return *this;
  }

  // Return the amount of bytes allocated to store text data.
  MemSize getAllocated() const {
    return m_allocated;
  }

  // Append a single character.
  void append(Char ch) {
    ensureAllocated(m_length + 2, true);
    m_text[m_length++] = ch;
    m_text[m_length] = '\0';
  }

  void append(const char* text) {
    append(text, std::strlen(text));
  }

  void append(const char* text, StringLength length) {
    ensureAllocated(m_length + length + 1, true);
    std::memcpy(m_text + m_length, text, length);
    m_length += length;
    m_text[m_length] = '\0';
  }

  void append(const StringView& text) {
    append(text.data(), text.length());
  }

  void resize(StringLength length) {
    if (m_length == length) {
      return;
    }

    if (length > m_length) {
      ensureAllocated(length + 1, true);
    }

    m_length = length;
    m_text[m_length] = '\0';
  }

  void erase(StringLength position, StringLength count) {
    if (position >= m_length) {
      return;
    }

    count = std::min(count, m_length - position);

    for (auto i = position; i < m_length - count; ++i) {
      m_text[i] = m_text[i + count];
    }
    m_length -= count;
    m_text[m_length] = '\0';
  }

private:
  void ensureAllocated(MemSize sizeRequired, bool keepOld);

  MemSize m_allocated;
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_DYNAMIC_STRING_H_
