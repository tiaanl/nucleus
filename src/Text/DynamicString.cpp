
#include "nucleus/Text/DynamicString.h"

#include "nucleus/Memory/Memory.h"

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

  // Char* newText = new Char[bytesToAllocate];
  Char* newText = static_cast<Char*>(allocate(bytesToAllocate, FROM_HERE));

  if (m_data) {
    if (keepOld) {
      std::memcpy(newText, m_data, m_length);
    }

    // delete[] m_data;
    free();
  }

  m_data = newText;
  m_capacity = bytesToAllocate;
}

auto DynamicString::free() -> void {
  deallocate(m_data, FROM_HERE);
}

}  // namespace nu
