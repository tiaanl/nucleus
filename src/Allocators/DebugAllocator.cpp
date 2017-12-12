
#include "nucleus/Allocators/DebugAllocator.h"

#include <algorithm>

#include "nucleus/logging.h"
#include "nucleus/win/windows_mixin.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

USize DebugAllocator::s_leakedBytes = 0;
USize DebugAllocator::s_leakedBlocks = 0;

DebugAllocator::DebugAllocator(Allocator* parent)
  : m_parent(parent), m_bytesAllocated(0), m_bytesOutstanding(0), m_maxAllocated(0), m_blocks(parent) {}

DebugAllocator::~DebugAllocator() {
  // If any blocks have not been released, report them as leaked.
  s_leakedBlocks += getBlocksOutstanding();

  // Reclaim blocks that would have been leaked.
  for (auto& record : m_blocks) {
    s_leakedBytes += record.bytes;
    // m_parent->free(record.ptr, record.bytes, record.alignment);
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

  if (it == std::end(m_blocks)) {
    // LOG(Fatal) << "Invalid pointer passed to free().";
#if OS(WIN)
    ::OutputDebugStringA("Invalid pointer passed to free().\n");
#endif  // OS(WIN)
  }     /*else if (it->bytes != bytes) {
        // LOG(Fatal) << "Block size mismatch on free().";
        ::OutputDebugStringA("Block size mismatch on free().\n");
      }*/
  else if (it->alignment != alignment) {
    // LOG(Fatal) << "Alignment mismatch on free().";
#if OS(WIN)
    ::OutputDebugStringA("Alignment mismatch on free().\n");
#endif  // OS(WIN)
  }

  m_parent->free(p, bytes, alignment);
  m_blocks.remove(it);
  m_bytesOutstanding -= bytes;
}

}  // namespace nu
