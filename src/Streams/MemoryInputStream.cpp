
#include "nucleus/Streams/MemoryInputStream.h"

#include "nucleus/Logging.h"

#include <algorithm>

#include "nucleus/MemoryDebug.h"

namespace nu {

MemoryInputStream::MemoryInputStream(void* sourceData, MemSize sourceDataSize)
  : m_currentPosition(0), m_buffer{static_cast<U8*>(sourceData), sourceDataSize} {
}

MemoryInputStream::MemoryInputStream(const nu::DynamicArray<U8>& data)
  : m_currentPosition(0), m_buffer{data} {}

MemoryInputStream::~MemoryInputStream() = default;

MemoryInputStream::SizeType MemoryInputStream::getLength() {
  return m_buffer.getSize();
}

MemoryInputStream::SizeType MemoryInputStream::read(void* buffer, SizeType bytesToRead) {
  SizeType num = std::min(bytesToRead, m_buffer.getSize() - m_currentPosition);
  if (num <= 0)
    return 0;

  memcpy(buffer, static_cast<const U8*>(m_buffer.getData()) + m_currentPosition,
         static_cast<MemSize>(num));
  m_currentPosition += static_cast<MemSize>(num);

  return num;
}

bool MemoryInputStream::isExhausted() {
  return m_currentPosition >= m_buffer.getSize();
}

bool MemoryInputStream::setPosition(SizeType newPosition) {
  m_currentPosition = std::max(newPosition, static_cast<SizeType>(0));
  m_currentPosition = std::min(newPosition, m_buffer.getSize());
  return true;
}

MemoryInputStream::SizeType MemoryInputStream::getPosition() {
  return m_currentPosition;
}

}  // namespace nu
