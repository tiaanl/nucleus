
#include "nucleus/Streams/ArrayInputStream.h"

#include <cstring>

namespace nu {

ArrayInputStream::ArrayInputStream(ArrayView<U8> buffer) : m_buffer{buffer} {}

ArrayInputStream::SizeType ArrayInputStream::getSize() {
  return m_buffer.size();
}

ArrayInputStream::SizeType ArrayInputStream::read(void* destination,
                                                                  SizeType bytesToRead) {
  SizeType num = std::min(bytesToRead, getBytesRemaining());

  std::memcpy(destination, m_buffer.data() + m_position, static_cast<MemSize>(num));

  m_position += num;

  return num;
}

bool ArrayInputStream::isExhausted() {
  return m_position >= m_buffer.size();
}

bool ArrayInputStream::setPosition(SizeType newPosition) {
  m_position = std::min(std::max(newPosition, static_cast<SizeType>(0)), m_buffer.size());
  return true;
}

ArrayInputStream::SizeType ArrayInputStream::getPosition() {
  return m_position;
}

}  // namespace nu
