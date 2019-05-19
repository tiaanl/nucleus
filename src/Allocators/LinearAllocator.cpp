
#include "nucleus/Allocators/LinearAllocator.h"

namespace nu {

LinearAllocator::LinearAllocator(void* data, MemSize size) : m_data(data), m_size(size), m_offset(0) {}

LinearAllocator::~LinearAllocator() {}

void* LinearAllocator::doAllocate(MemSize bytes, MemSize) {
  MemSize padding = 0;
  // MemSize paddedAddress = 0;

  const MemSize currentAddress = (MemSize)m_data + m_offset;

  // if (alignment != 0 && m_offset % alignment != 0) {
  //   padding = calculatePadding(currentAddress, alignment);
  // }

  if (m_offset + padding + bytes > m_size) {
    return nullptr;
  }

  m_offset += padding;
  const MemSize nextAddress = currentAddress + padding;
  m_offset += bytes;

  // m_used = m_offset;
  // m_peak = max(m_peak, m_used);

  return (void*)nextAddress;
}

void LinearAllocator::doFree(void*, MemSize, MemSize) {
  // This is a noop.  When the allocator is destroyed, all the parent memory is destroyed as well.
}

bool LinearAllocator::doIsEqual(const Allocator& other) const noexcept {
  return this == &other;
}

}  // namespace nu
