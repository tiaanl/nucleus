
#ifndef NUCLEUS_ALLOCATORS_DEBUG_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_DEBUG_ALLOCATOR_H_

#include "nucleus/Allocators/Allocator.h"

#include "nucleus/Containers/DynamicArray.h"

namespace nu {

class DebugAllocator : public Allocator {
public:
  explicit DebugAllocator(Allocator* parent);
  ~DebugAllocator() override;

  Allocator* getParent() const {
    return m_parent;
  }

  MemSize getBytesAllocated() const {
    return m_bytesAllocated;
  }

  MemSize getBytesFreed() const {
    return m_bytesFreed;
  }

  MemSize getBytesOutstanding() const {
    return m_bytesAllocated - m_bytesFreed;
  }

protected:
  void* doAllocate(MemSize bytes, MemSize alignment) override;
  void doFree(void* p, MemSize bytes, MemSize alignment) override;
  bool doIsEqual(const Allocator& other) const noexcept override {
    return this == &other;
  }

private:
  Allocator* m_parent;

  MemSize m_bytesAllocated = 0;
  MemSize m_bytesFreed = 0;
};

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_DEBUG_ALLOCATOR_H_
