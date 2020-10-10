
#include "nucleus/Streams/WrappedMemoryInputStream.h"

#include <algorithm>

#include "nucleus/Logging.h"

namespace nu {

WrappedMemoryInputStream::WrappedMemoryInputStream(const void* data, MemSize size)
  : m_data(data), m_size(size) {}

WrappedMemoryInputStream::WrappedMemoryInputStream(const nu::DynamicArray<I8>& data)
  : m_data(data.data()), m_size(data.size()) {}

WrappedMemoryInputStream::SizeType WrappedMemoryInputStream::getSize() {
  return m_size;
}

WrappedMemoryInputStream::SizeType WrappedMemoryInputStream::read(void* buffer,
                                                                  SizeType bytesToRead) {
  SizeType num = std::min(bytesToRead, getBytesRemaining());

  std::memcpy(buffer, static_cast<const U8*>(m_data) + m_currentPosition,
              static_cast<MemSize>(num));
  m_currentPosition += num;

  return num;
}

bool WrappedMemoryInputStream::isExhausted() {
  return m_currentPosition >= m_size;
}

bool WrappedMemoryInputStream::setPosition(SizeType newPosition) {
  m_currentPosition = std::max(newPosition, static_cast<SizeType>(0));
  m_currentPosition = std::min(newPosition, m_size);
  return true;
}

WrappedMemoryInputStream::SizeType WrappedMemoryInputStream::getPosition() {
  return m_currentPosition;
}

}  // namespace nu
