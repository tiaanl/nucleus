
#ifndef NUCLEUS_TEXT_DYNAMIC_STRING_H_
#define NUCLEUS_TEXT_DYNAMIC_STRING_H_

#include "nucleus/Logging.h"
#include "nucleus/Text/CharTraits.h"
#include "nucleus/Text/StringView.h"
#include "nucleus/hash.h"

#undef min
#undef max
#undef free

namespace nu {

class DynamicString {
public:
  constexpr DynamicString() = default;

  DynamicString(StringView text) {
    ensureAllocated(text.length(), false);
    std::memcpy(m_data, text.data(), text.length());
    m_length = text.length();
  }

  DynamicString(const DynamicString& other) {
    ensureAllocated(other.length(), false);
    std::memcpy(m_data, other.data(), other.length());
    m_length = other.length();
  }

  DynamicString(DynamicString&& other)
    : m_data{other.m_data}, m_length{other.m_length}, m_capacity{other.m_capacity} {
    other.m_data = nullptr;
    other.m_length = 0;
    other.m_capacity = 0;
  }

  ~DynamicString() {
    free();
  }

  friend bool operator==(const DynamicString& left, const DynamicString& right) {
    return left.view() == right.view();
  }

  friend bool operator!=(const DynamicString& left, const DynamicString& right) {
    return left.view() != right.view();
  }

  DynamicString& operator=(const DynamicString& other) {
    ensureAllocated(other.m_length, false);
    std::memcpy(m_data, other.m_data, other.m_length);
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

  DynamicString& operator=(StringView text) {
    ensureAllocated(text.length(), false);
    std::memcpy(m_data, text.data(), text.length());
    m_length = text.length();

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
  auto ensureAllocated(MemSize sizeRequired, bool keepOld) -> void;
  auto free() -> void;

  Char* m_data = nullptr;
  StringLength m_length = 0;
  MemSize m_capacity = 0;
};

inline std::ostream& operator<<(std::ostream& os, const DynamicString& value) {
  os.rdbuf()->sputn(value.data(), value.length());
  return os;
}

template <>
struct Hash<DynamicString> {
  static HashedValue hashed(const DynamicString& value) {
    return Hash<decltype(value.view())>::hashed(value.view());
  }
};

}  // namespace nu

namespace std {

template <>
struct hash<nu::DynamicString> {
  std::size_t operator()(const nu::DynamicString& s) const noexcept {
    return nu::Hash<nu::DynamicString>::hashed(s);
  }
};

}  // namespace std

#endif  // NUCLEUS_TEXT_DYNAMIC_STRING_H_
