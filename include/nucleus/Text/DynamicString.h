
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
    std::strncpy(m_text, text, length);
    m_text[length] = '\0';
    m_length = length;
  }

  explicit DynamicString(const StringView& text)
    : DynamicString{text.getData(), text.getLength()} {}

  DynamicString(const DynamicString& other) : DynamicString{other.m_text, other.m_length} {}

  ~DynamicString() {
    delete[] m_text;
  }

  DynamicString& operator=(const DynamicString& other) {
    ensureAllocated(other.m_length, false);
    std::strncpy(m_text, other.m_text, other.m_length);
    m_text[other.m_length] = '\0';
    m_length = other.m_length;

    return *this;
  }

  // Return the amount of bytes allocated to store text data.
  MemSize getAllocated() const {
    return m_allocated;
  }

  void append(const char* text) {
    append(text, std::strlen(text));
  }

  void append(const char* text, StringLength length) {
    ensureAllocated(m_length + length + 1, true);
    std::strncat(m_text, text, length);
    m_length += length;
    m_text[m_length] = '\0';
  }

  void append(const StringView& text) {
    append(text.getData(), text.getLength());
  }

private:
  void ensureAllocated(MemSize sizeRequired, bool keepOld);

  MemSize m_allocated;
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_DYNAMIC_STRING_H_
