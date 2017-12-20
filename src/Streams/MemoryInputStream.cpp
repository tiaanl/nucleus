
#include "nucleus/Streams/MemoryInputStream.h"

#include "nucleus/Logging.h"
#include "nucleus/Utils/MinMax.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

MemoryInputStream::MemoryInputStream(const void* sourceData, USize sourceDataSize) : m_currentPosition(0) {
  createInternalCopy(static_cast<const I8*>(sourceData), sourceDataSize);
}

MemoryInputStream::MemoryInputStream(const nu::DynamicArray<I8>& data) : m_currentPosition(0) {
  createInternalCopy(data.getData(), data.getSize());
}

MemoryInputStream::~MemoryInputStream() = default;

MemoryInputStream::SizeType MemoryInputStream::getLength() {
  return m_buffer.getSize();
}

MemoryInputStream::SizeType MemoryInputStream::read(void* buffer, SizeType bytesToRead) {
  DCHECK(bytesToRead >= 0);

  SizeType num = min(bytesToRead, m_buffer.getSize() - m_currentPosition);
  if (num <= 0)
    return 0;

  memcpy(buffer, static_cast<const I8*>(m_buffer.getData()) + m_currentPosition, static_cast<USize>(num));
  m_currentPosition += static_cast<USize>(num);

  return num;
}

bool MemoryInputStream::isExhausted() {
  return m_currentPosition >= m_buffer.getSize();
}

bool MemoryInputStream::setPosition(SizeType newPosition) {
  m_currentPosition = max(newPosition, static_cast<SizeType>(0));
  m_currentPosition = min(newPosition, m_buffer.getSize());
  return true;
}

MemoryInputStream::SizeType MemoryInputStream::getPosition() {
  return m_currentPosition;
}

void MemoryInputStream::createInternalCopy(const I8* data, USize dataSize) {
  m_buffer.resize(dataSize);
  memcpy(m_buffer.getData(), data, dataSize);
}

}  // namespace nu
