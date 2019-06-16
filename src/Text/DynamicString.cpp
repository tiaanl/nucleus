
#include "nucleus/Text/DynamicString.h"

namespace nu {

void DynamicString::ensureAllocated(MemSize sizeRequired, bool keepOld) {
  if (sizeRequired <= m_allocated) {
    return;
  }

  // Minimum of 16 bytes.
  MemSize bytesToAllocate = std::max<MemSize>(m_allocated, 16);
  while (bytesToAllocate < sizeRequired) {
    bytesToAllocate *= 2;
  }

  DCHECK(bytesToAllocate != 0);
  DCHECK((bytesToAllocate & (bytesToAllocate - 1)) == 0) << "We only work with power of 2 numbers.";

  Char* newText = new Char[bytesToAllocate];

  if (m_text) {
    if (keepOld) {
      std::memcpy(newText, m_text, m_length + 1);
    }

    delete[] m_text;
  }

  m_text = newText;
  m_allocated = bytesToAllocate;
}

}  // namespace nu
