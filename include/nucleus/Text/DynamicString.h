
#ifndef NUCLEUS_TEXT_DYNAMIC_STRING_H_
#define NUCLEUS_TEXT_DYNAMIC_STRING_H_

#include "nucleus/Logging.h"
#include "nucleus/Text/CharTraits.h"
#include "nucleus/Text/StringView.h"

namespace nu {

class DynamicString {
public:
  constexpr DynamicString() : m_data{nullptr}, m_length{0}, m_capacity{0} {}

  DynamicString(const char* text)
    : DynamicString{text, CharTraits<Char>::calculateZeroTerminatedLength(text)} {}

  DynamicString(const char* text, std::size_t length)
    : m_data{nullptr}, m_length{length}, m_capacity{0} {
    ensureAllocated(length, false);
    std::memcpy(m_data, text, length);
  }

  explicit DynamicString(const StringView& text) : DynamicString{text.data(), text.length()} {}

  DynamicString(const DynamicString& other) : DynamicString{other.m_data, other.m_length} {}

  DynamicString(DynamicString&& other)
    : m_data{other.m_data}, m_length{other.m_length}, m_capacity{other.m_capacity} {
    other.m_data = nullptr;
    other.m_length = 0;
    other.m_capacity = 0;
  }

  ~DynamicString() {
    delete[] m_data;
  }

  DynamicString& operator=(const DynamicString& other) {
    ensureAllocated(other.m_length + 1, false);
    std::memcpy(m_data, other.m_data, other.m_length);
    m_data[other.m_length] = '\0';
    m_length = other.m_length;

    return *this;
  }

  DynamicString& operator=(DynamicString&& other) {
    m_data = other.m_data;
    m_length = other.m_length;
    m_capacity = other.m_capacity;
    other.m_data = nullptr;
    other.m_length = 0;
    other.m_capacity = 0;

    return *this;
  }

  DynamicString& operator=(const StringView& other) {
    auto otherLength = other.length();
    ensureAllocated(otherLength, false);
    std::memcpy(m_data, other.data(), otherLength);
    m_length = otherLength;

    return *this;
  }

  Char& operator[](StringLength index) {
    DCHECK(index < m_length) << "Index out of range.";
    return m_data[index];
  }

  const Char& operator[](StringLength index) const {
    DCHECK(index < m_length) << "Index out of range.";
    return m_data[index];
  }

  const Char* data() const {
    return m_data;
  }

  Char* data() {
    return m_data;
  }

  StringLength length() const {
    return m_length;
  }

  bool empty() const {
    return m_length == 0;
  }

  MemSize capacity() const {
    return m_capacity;
  }

  StringView view() const {
    return {m_data, m_length};
  }

  void append(Char ch) {
    ensureAllocated(m_length + 1, true);
    m_data[m_length++] = ch;
  }

  void append(const char* text) {
    append(text, std::strlen(text));
  }

  void append(const char* text, StringLength length) {
    ensureAllocated(m_length + length + 1, true);
    std::memcpy(m_data + m_length, text, length);
    m_length += length;
    m_data[m_length] = '\0';
  }

  void append(StringView text) {
    append(text.data(), text.length());
  }

  void resize(StringLength length) {
    ensureAllocated(length, true);
    m_length = length;
  }

  void erase(StringLength position, StringLength count) {
    if (position >= m_length) {
      return;
    }

    count = std::min(count, m_length - position);

    for (auto i = position; i < m_length - count; ++i) {
      m_data[i] = m_data[i + count];
    }
    m_length -= count;
    m_data[m_length] = '\0';
  }

private:
  void ensureAllocated(MemSize sizeRequired, bool keepOld);

  Char* m_data;
  StringLength m_length;
  MemSize m_capacity;
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_DYNAMIC_STRING_H_
