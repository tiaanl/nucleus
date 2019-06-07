
#include "nucleus/Streams/StringOutputStream.h"

namespace nu {

StringOutputStream::SizeType StringOutputStream::write(void* buffer, SizeType size) {
  m_text.append(static_cast<Char*>(buffer), size);
  return size;
}

}  // namespace nu
