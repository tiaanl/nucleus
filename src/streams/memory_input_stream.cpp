
#include "nucleus/streams/memory_input_stream.h"

#include <algorithm>

#include "nucleus/logging.h"

namespace nu {

MemoryInputStream::MemoryInputStream(void* sourceData, MemSize sourceDataSize)
  : m_buffer{static_cast<U8*>(sourceData), sourceDataSize}, m_currentPosition{0} {}

MemoryInputStream::MemoryInputStream(nu::DynamicArray<U8> data)
  : m_buffer{data}, m_currentPosition{0} {}

MemoryInputStream::~MemoryInputStream() = default;

MemoryInputStream::SizeType MemoryInputStream::getSize() {
  return m_buffer.size();
}

MemoryInputStream::SizeType MemoryInputStream::read(void* buffer, SizeType bytesToRead) {
  SizeType num = std::min(bytesToRead, m_buffer.size() - m_currentPosition);
  if (num <= 0) {
    return 0;
  }

  memcpy(buffer, static_cast<const U8*>(m_buffer.data()) + m_currentPosition,
         static_cast<MemSize>(num));
  m_currentPosition += static_cast<MemSize>(num);

  return num;
}

bool MemoryInputStream::isExhausted() {
  return m_currentPosition >= m_buffer.size();
}

bool MemoryInputStream::setPosition(SizeType newPosition) {
  m_currentPosition = std::max(newPosition, static_cast<SizeType>(0));
  m_currentPosition = std::min(newPosition, m_buffer.size());
  return true;
}

MemoryInputStream::SizeType MemoryInputStream::getPosition() {
  return m_currentPosition;
}

}  // namespace nu
