
#include "nucleus/Text/DynamicString.h"

namespace nu {

void DynamicString::ensureAllocated(MemSize sizeRequired, bool keepOld) {
  if (sizeRequired <= m_capacity) {
    return;
  }

  // Minimum of 16 bytes.
  MemSize bytesToAllocate = std::max<MemSize>(m_capacity, 16);
  while (bytesToAllocate < sizeRequired) {
    bytesToAllocate *= 2;
  }

  DCHECK(bytesToAllocate != 0);
  DCHECK((bytesToAllocate & (bytesToAllocate - 1)) == 0) << "We only work with power of 2 numbers.";

  Char* newText = new Char[bytesToAllocate];

  if (m_data) {
    if (keepOld) {
      std::memcpy(newText, m_data, m_length);
    }

    delete[] m_data;
  }

  m_data = newText;
  m_capacity = bytesToAllocate;
}

}  // namespace nu
