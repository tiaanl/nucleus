
#ifndef NUCLEUS_ALLOCATORS_LINEAR_ALLOCATOR_H
#define NUCLEUS_ALLOCATORS_LINEAR_ALLOCATOR_H

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/Types.h"

namespace nu {

class LinearAllocator : public Allocator {
public:
  LinearAllocator(void* data, MemSize size);
  ~LinearAllocator() override;

private:
  void* doAllocate(MemSize bytes, MemSize alignment) override;
  void doFree(void* p, MemSize bytes, MemSize alignment) override;
  bool doIsEqual(const Allocator& other) const noexcept override;

  void* m_data;
  MemSize m_size;
  MemSize m_offset;
};

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_LINEAR_ALLOCATOR_H
