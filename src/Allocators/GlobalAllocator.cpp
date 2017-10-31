
#include "nucleus/Allocators/GlobalAllocator.h"

#include <malloc.h>

namespace nu {

void* GlobalAllocator::doAllocate(USize bytes, USize alignment) {
  return _aligned_malloc(bytes, alignment);
}

void GlobalAllocator::doFree(void* p, USize bytes, USize alignment) {
  _aligned_free(p);
}

bool GlobalAllocator::doIsEqual(const Allocator& other) const noexcept {
  return this == &other;
}

GlobalAllocator* globalAllocatorSingleton() {
  static GlobalAllocator singleton;
  return &singleton;
}

}  // namespace nu
