
#include "nucleus/Allocators/LinearAllocator.h"

namespace nu {

LinearAllocator::LinearAllocator(void* data, USize size) : m_data(data), m_size(size), m_offset(0) {}

LinearAllocator::~LinearAllocator() {}

void* LinearAllocator::doAllocate(USize bytes, USize) {
  USize padding = 0;
  // USize paddedAddress = 0;

  const USize currentAddress = (USize)m_data + m_offset;

  // if (alignment != 0 && m_offset % alignment != 0) {
  //   padding = calculatePadding(currentAddress, alignment);
  // }

  if (m_offset + padding + bytes > m_size) {
    return nullptr;
  }

  m_offset += padding;
  const USize nextAddress = currentAddress + padding;
  m_offset += bytes;

  // m_used = m_offset;
  // m_peak = max(m_peak, m_used);

  return (void*)nextAddress;
}

void LinearAllocator::doFree(void*, USize, USize) {
  // This is a noop.  When the allocator is destroyed, all the parent memory is destroyed as well.
}

bool LinearAllocator::doIsEqual(const Allocator& other) const noexcept {
  return this == &other;
}

}  // namespace nu