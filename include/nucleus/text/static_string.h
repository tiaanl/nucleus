#pragma once

#include "nucleus/text/string_view.h"
#include "nucleus/logging.h"
#include "nucleus/macros.h"

namespace nu {

template <MemSize Size = 64>
class StaticString {
public:
  StaticString() : m_data{}, m_length{0} {}

  StaticString(StringView text) {
    initFrom(text.data(), text.length());
  }

  StaticString(const StaticString& other) {
    initFrom(other.m_data, other.m_length);
  }

  StaticString(StaticString&&) = delete;

  StaticString& operator=(StringView text) {
    initFrom(text.data(), text.length());

    return *this;
  }

  StaticString& operator=(const StaticString& other) {
    initFrom(other.m_data, other.m_length);

    return *this;
  }

  const Char& operator[](StringLength index) const {
    DCHECK(index < m_length) << "Index out of range.";
    return m_data[index];
  }

  Char& operator[](StringLength index) {
    DCHECK(index < m_length) << "Index out of range.";
    return m_data[index];
  }

  StaticString& operator=(StaticString&&) = delete;

  const Char* data() const {
    return m_data;
  }

  Char* data() {
    return m_data;
  }

  StringLength length() const {
    return m_length;
  }

  constexpr MemSize capacity() const {
    return Size;
  }

  constexpr bool empty() const {
    return m_length == 0;
  }

  StringView view() const {
    return {m_data, m_length};
  }

  void append(StringView text) {
    append(text.data(), text.length());
  }

  void append(const char* text) {
    append(StringView{text});
  }

  void append(const char* text, StringLength length) {
    if (m_length == Size) {
      return;
    }

    StringLength lengthToAppend = std::min(Size - m_length - 1, length);
    std::memcpy(m_data + m_length, text, lengthToAppend);
    m_length += lengthToAppend;
  }

private:
  auto initFrom(const Char* text, StringLength length) -> void {
    m_length = std::min(Size, std::min<StringLength>(length, std::strlen(text)));
    std::memcpy(m_data, text, m_length);
  }

  Char m_data[Size];
  StringLength m_length;
};

}  // namespace nu

template <MemSize Size>
std::ostream& operator<<(std::ostream& os, const nu::StaticString<Size>& value) {
  os << value.view();
  return os;
}
