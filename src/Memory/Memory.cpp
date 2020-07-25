#include "nucleus/Memory/Memory.h"

#include <cstdlib>

#include "nucleus/Macros.h"

namespace nu {

auto allocate(MemSize size, const SourceLocation& NU_UNUSED(here)) -> void* {
  return std::malloc(size);
}

auto deallocate(void* ptr, const SourceLocation& NU_UNUSED(here)) -> void {
  std::free(ptr);
}

}  // namespace nu
