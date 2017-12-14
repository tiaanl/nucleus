
#include "nucleus/Config.h"

#if OS(WIN)
#include <cstdlib>

#include "nucleus/Allocators/DebugAllocator.h"
#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/Config.h"
#include "nucleus/win/WindowsMixin.h"

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

#endif  // OS(WIN)
