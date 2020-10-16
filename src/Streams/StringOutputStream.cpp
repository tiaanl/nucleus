
#include "nucleus/Streams/StringOutputStream.h"

namespace nu {

StringOutputStream::SizeType StringOutputStream::write(const void *buffer, SizeType size) {
  // Expand the buffer if there is not enough space already.
  if (m_position + size > m_data.length()) {
    m_data.resize(m_position + size);
  }

  // Write the source into the buffer.
  const U8* src = static_cast<const U8*>(buffer);
  std::memcpy(m_data.data() + m_position, src, size);

  // Advance the current position.
  m_position += size;

  return size;
}

}  // namespace nu
