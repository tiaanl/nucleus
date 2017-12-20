
#ifndef NUCLEUS_ALLOCATORS_LINEAR_ALLOCATOR_H
#define NUCLEUS_ALLOCATORS_LINEAR_ALLOCATOR_H

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/Types.h"

namespace nu {

class LinearAllocator : public Allocator {
public:
  LinearAllocator(void* data, USize size);
  ~LinearAllocator() override;

private:
  void* doAllocate(USize bytes, USize alignment) override;
  void doFree(void* p, USize bytes, USize alignment) override;
  bool doIsEqual(const Allocator& other) const noexcept override;

  void* m_data;
  USize m_size;
  USize m_offset;
};

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_LINEAR_ALLOCATOR_H
