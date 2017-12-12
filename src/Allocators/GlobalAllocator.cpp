
#include "nucleus/Allocators/GlobalAllocator.h"

#include <cstdlib>

#include "nucleus/MemoryDebug.h"

namespace nu {

void* GlobalAllocator::doAllocate(USize bytes, USize alignment) {
#if COMPILER(GCC)
  return ::malloc(bytes);
#else   // COMPILER(GCC)
  return ::_aligned_malloc(bytes, alignment);
#endif  // COMPILER(GCC)
}

void GlobalAllocator::doFree(void* p, USize bytes, USize alignment) {
#if COMPILER(MSVC)
  ::_aligned_free(p);
#else
  ::free(p);
#endif
}

bool GlobalAllocator::doIsEqual(const Allocator& other) const noexcept {
  return this == &other;
}

}  // namespace nu
