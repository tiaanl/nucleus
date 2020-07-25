#ifndef NUCLEUS_MEMORY_MEMORY_H_
#define NUCLEUS_MEMORY_MEMORY_H_

#include "nucleus/SourceLocation.h"
#include "nucleus/Types.h"

namespace nu {

auto allocate(MemSize size, const SourceLocation& here) -> void*;
auto deallocate(void* ptr, const SourceLocation& here) -> void;

}  // namespace nu

#endif  // NUCLEUS_MEMORY_MEMORY_H_
