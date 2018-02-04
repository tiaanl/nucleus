
#include "nucleus/Allocators/GlobalAllocator.h"

#include <cstdlib>

#include "nucleus/Config.h"
#include "nucleus/Logging.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

#if COMPILER(MINGW)
void* GlobalAllocator::doAllocate(USize bytes, USize) {
  return ::malloc(bytes);
}
#elif COMPILER(GCC)
void* GlobalAllocator::doAllocate(USize bytes, USize alignment) {
  // LOG(Info) << "allocating: bytes=" << bytes << ", alignment=" << alignment;

  alignment = sizeof(void*);
  void* result;
  int error = ::posix_memalign(&result, alignment, bytes);
  if (error != 0) {
    switch (error) {
      case EINVAL:
        LOG(Fatal) << "Could not allocate memory (invalid alignment)";
        break;

      case ENOMEM:
        LOG(Fatal) << "Could not allocate memory (insufficient memory available with the requested alignment)";
        break;

      default:
        LOG(Fatal) << "Could not allocate memory";
        break;
    }
    return nullptr;
  }

  // LOG(Info) << "allocated: " << result;

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
  // LOG(Info) << "deallocating: " << p;
  ::free(p);
}
#endif

bool GlobalAllocator::doIsEqual(const Allocator& other) const noexcept {
  return this == &other;
}

}  // namespace nu
