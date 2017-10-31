#ifndef NUCLEUS_ALLOCATORS_GLOBAL_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_GLOBAL_ALLOCATOR_H_

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/types.h"

namespace nu {

class GlobalAllocator : public Allocator {
protected:
  void* doAllocate(USize bytes, USize alignment) override;
  void doFree(void* p, USize bytes, USize alignment) override;
  bool doIsEqual(const Allocator& other) const noexcept override;
};

GlobalAllocator* globalAllocatorSingleton();

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_GLOBAL_ALLOCATOR_H_
