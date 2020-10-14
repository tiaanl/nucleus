#include "nucleus/Streams/DynamicBufferOutputStream.h"

namespace nu {

DynamicBufferOutputStream::DynamicBufferOutputStream() : OutputStream{OutputStreamMode::Binary} {}

DynamicBufferOutputStream::SizeType DynamicBufferOutputStream::write(const void* buffer,
                                                                     SizeType size) {
  // Expand the buffer if there is not enough space already.
  if (m_currentPosition + size > m_sink.size()) {
    m_sink.resize(m_currentPosition + size);
  }

  // Write the source into the buffer.
  const U8* src = static_cast<const U8*>(buffer);
  std::memcpy(m_sink.data() + m_currentPosition, src, size);

  // Advance the current position.
  m_currentPosition += size;

  return size;
}

}  // namespace nu
