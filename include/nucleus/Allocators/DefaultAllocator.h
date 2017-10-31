
#ifndef NUCLEUS_ALLOCATORS_DEFAULT_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_DEFAULT_ALLOCATOR_H_

#include "nucleus/Allocators/GlobalAllocator.h"

namespace nu {

Allocator* setDefaultAllocator(Allocator* allocator);
Allocator* getDefaultAllocator();

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_DEFAULT_ALLOCATOR_H_
