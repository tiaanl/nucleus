
#include "nucleus/Allocators/GlobalAllocator.h"

#include <cstdlib>

#include "nucleus/Config.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

#if COMPILER(MINGW)
void* GlobalAllocator::doAllocate(USize bytes, USize) {
  return ::malloc(bytes);
}
#elif COMPILER(GCC)
void* GlobalAllocator::doAllocate(USize bytes, USize alignment) {
  void* result;
  ::posix_memalign(&result, alignment, bytes);
  return result;
}
#else   // COMPILER(GCC)
void* GlobalAllocator::doAllocate(USize bytes, USize alignment) {
  return ::_aligned_malloc(bytes, alignment);
}
#endif  // COMPILER(GCC)

#if COMPILER(MSVC)
void GlobalAllocator::doFree(void* p, USize, USize) {
  ::_aligned_free(p);
}
#else
void GlobalAllocator::doFree(void* p, USize, USize) {
  ::free(p);
}
#endif

bool GlobalAllocator::doIsEqual(const Allocator& other) const noexcept {
  return this == &other;
}

}  // namespace nu
