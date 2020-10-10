
#include "nucleus/Streams/StringOutputStream.h"

namespace nu {

StringOutputStream::SizeType StringOutputStream::write(const void *buffer, SizeType size) {
  m_text.append(reinterpret_cast<const char*>(buffer), size);
  return size;
}

}  // namespace nu
