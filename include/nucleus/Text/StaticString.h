
#ifndef NUCLEUS_TEXT_STATIC_STRING_H_
#define NUCLEUS_TEXT_STATIC_STRING_H_

#include "nucleus/Text/StringView.h"

namespace nu {

template <MemSize Size = 64>
class StaticString {
public:
  StaticString() : m_data{0}, m_length{0} {}

  StaticString(const StringView& source) {
    MemSize bytesToCopy = std::min(Size - 1, source.getLength());
    std::memcpy(m_data, source.getData(), bytesToCopy);
    m_data[bytesToCopy] = 0;
    m_length = bytesToCopy;
  }

  const Char* getData() const {
    return m_data;
  }

  StringLength getLength() const {
    return m_length;
  }

private:
  Char m_data[Size];
  StringLength m_length;
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_STATIC_STRING_H_
