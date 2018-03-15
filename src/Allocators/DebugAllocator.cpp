
#include "nucleus/Allocators/DebugAllocator.h"

#include <algorithm>

#include "nucleus/Logging.h"
#include "nucleus/Win/WindowsMixin.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

DebugAllocator::DebugAllocator(Allocator* parent) : m_parent(parent) {}

DebugAllocator::~DebugAllocator() = default;

void* DebugAllocator::doAllocate(USize bytes, USize alignment) {
  void* ret = m_parent->allocate(bytes, alignment);

  m_bytesAllocated += bytes;

  return ret;
}

void DebugAllocator::doFree(void* p, USize bytes, USize alignment) {
  m_parent->free(p, bytes, alignment);

  m_bytesFreed += bytes;
}

}  // namespace nu
