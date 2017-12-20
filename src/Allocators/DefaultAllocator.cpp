
#include "nucleus/Allocators/DefaultAllocator.h"

#include <atomic>

#include "nucleus/Allocators/DebugAllocator.h"
#include "nucleus/Allocators/GlobalAllocator.h"
#include "nucleus/Config.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

static std::atomic<Allocator*> gs_defaultAllocator;

Allocator* getDefaultAllocator() {
  Allocator* ret = gs_defaultAllocator.load();

  static nu::GlobalAllocator globalAllocator;

  if (ret == nullptr) {
#if BUILD(DEBUG)
    static nu::DebugAllocator debugAllocator{&globalAllocator};
    ret = &debugAllocator;
#else
    ret = &globalAllocator;
#endif

    gs_defaultAllocator.store(ret);
  }

  return ret;
}

Allocator* setDefaultAllocator(Allocator* allocator) {
  // TODO: Use atomic swap to set the allocator.
  Allocator* prev = getDefaultAllocator();
  gs_defaultAllocator.store(allocator);

  return prev;
}

}  // namespace nu
