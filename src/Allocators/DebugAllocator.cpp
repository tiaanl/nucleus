
#include "nucleus/Allocators/DebugAllocator.h"

#include <algorithm>

#include "nucleus/Logging.h"
#include "nucleus/Win/WindowsMixin.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

DebugAllocator::DebugAllocator(Allocator* parent) : m_parent(parent) {}

DebugAllocator::~DebugAllocator() = default;

void* DebugAllocator::doAllocate(MemSize bytes, MemSize alignment) {
  void* ret = m_parent->allocate(bytes, alignment);

  m_bytesAllocated += bytes;

  return ret;
}

void DebugAllocator::doFree(void* p, MemSize bytes, MemSize alignment) {
  m_parent->free(p, bytes, alignment);

  m_bytesFreed += bytes;
}

}  // namespace nu
