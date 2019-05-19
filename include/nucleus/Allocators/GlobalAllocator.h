
#ifndef NUCLEUS_ALLOCATORS_GLOBAL_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_GLOBAL_ALLOCATOR_H_

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/Types.h"

namespace nu {

class GlobalAllocator : public Allocator {
protected:
  void* doAllocate(MemSize bytes, MemSize alignment) override;
  void doFree(void* p, MemSize bytes, MemSize alignment) override;
  bool doIsEqual(const Allocator& other) const noexcept override;
};

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_GLOBAL_ALLOCATOR_H_
