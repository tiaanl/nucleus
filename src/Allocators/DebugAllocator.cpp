
#include "nucleus/Allocators/DebugAllocator.h"

#include <algorithm>

#include "nucleus/Logging.h"
#include "nucleus/Win/WindowsMixin.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

USize DebugAllocator::s_leakedBytes = 0;
USize DebugAllocator::s_leakedBlocks = 0;

DebugAllocator::DebugAllocator(Allocator* parent) : m_parent(parent) {}

DebugAllocator::~DebugAllocator() {
  // If any blocks have not been released, report them as leaked.
  s_leakedBlocks += getBlocksOutstanding();

  // Reclaim blocks that would have been leaked.
  for (auto& record : m_blocks) {
    s_leakedBytes += record.bytes;
    m_parent->free(record.ptr, record.bytes, record.alignment);
  }
}

void DebugAllocator::clearLeaked() {
  s_leakedBytes = 0;
  s_leakedBlocks = 0;
}

void* DebugAllocator::doAllocate(USize bytes, USize alignment) {
  void* ret = m_parent->allocate(bytes, alignment);

  m_blocks.emplaceBack(ret, bytes, alignment);
  m_bytesAllocated += bytes;
  m_bytesOutstanding += bytes;
  if (m_bytesOutstanding > m_maxAllocated) {
    m_maxAllocated = m_bytesOutstanding;
  }

  return ret;
}

void DebugAllocator::doFree(void* p, USize bytes, USize alignment) {
  // Check that deallocation arguments match some allocation arguments.
  auto it = std::find_if(std::begin(m_blocks), std::end(m_blocks), [p](Record& r) { return r.ptr = p; });

#if 0
  if (it == std::end(m_blocks)) {
    LOG(Error) << "Invalid pointer passed to free().";
  } else if (it->bytes != bytes) {
    LOG(Error) << "Block size mismatch on free().";
  } else if (it->alignment != alignment) {
    LOG(Error) << "Alignment mismatch on free().";
  }
#endif  // 0

  m_parent->free(p, bytes, alignment);
  m_blocks.remove(it);
  m_bytesOutstanding -= bytes;
}

}  // namespace nu
