
#include "nucleus/Streams/WrappedMemoryInputStream.h"

#include "nucleus/Logging.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

WrappedMemoryInputStream::WrappedMemoryInputStream(const void* data, USize size) : m_data(data), m_size(size) {}

WrappedMemoryInputStream::WrappedMemoryInputStream(const nu::DynamicArray<I8>& data)
  : m_data(data.getData()), m_size(data.getSize()) {}

WrappedMemoryInputStream::SizeType WrappedMemoryInputStream::getLength() {
  return m_size;
}

WrappedMemoryInputStream::SizeType WrappedMemoryInputStream::read(void* buffer, SizeType bytesToRead) {
  DCHECK(bytesToRead >= 0);

  SizeType num = std::min(bytesToRead, getBytesRemaining());

  std::memcpy(buffer, static_cast<const U8*>(m_data) + m_currentPosition, static_cast<USize>(num));
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
