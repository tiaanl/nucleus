
#include "nucleus/Allocators/DefaultAllocator.h"

namespace nu {

static std::atomic<Allocator*> gs_defaultAllocator;

Allocator* getDefaultAllocator() {
  Allocator* ret = gs_defaultAllocator.load();

  if (ret == nullptr) {
    ret = globalAllocatorSingleton();
  }

  return ret;
}

Allocator* setDefaultAllocator(Allocator* allocator) {
  if (allocator == nullptr) {
    allocator = globalAllocatorSingleton();
  }

  // TODO: Use atomic swap to set the allocator.
  Allocator* prev = getDefaultAllocator();
  gs_defaultAllocator.store(allocator);

  return prev;
}

}  // namespace nu
