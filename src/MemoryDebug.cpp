
#include <cstdlib>

#include "nucleus/Allocators/DebugAllocator.h"
#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/config.h"
#include "nucleus/win/windows_mixin.h"

#include "nucleus/MemoryDebug.h"

#undef new

#if 0
void* operator new(std::size_t size) {
  ::OutputDebugStringA("new\n");

  nu::Allocator* allocator = nu::getDefaultAllocator();

  return allocator->allocate(size);
}

void operator delete(void* ptr) noexcept {
  ::OutputDebugStringA("delete\n");

  nu::Allocator* allocator = nu::getDefaultAllocator();

  allocator->free(ptr, 0);
}
#endif  // 0
