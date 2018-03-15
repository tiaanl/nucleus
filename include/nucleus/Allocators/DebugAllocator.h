
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

  USize getBytesAllocated() const {
    return m_bytesAllocated;
  }

  USize getBytesFreed() const {
    return m_bytesFreed;
  }

  USize getBytesOutstanding() const {
    return m_bytesAllocated - m_bytesFreed;
  }

protected:
  void* doAllocate(USize bytes, USize alignment) override;
  void doFree(void* p, USize bytes, USize alignment) override;
  bool doIsEqual(const Allocator& other) const noexcept override {
    return this == &other;
  }

private:
  Allocator* m_parent;

  USize m_bytesAllocated = 0;
  USize m_bytesFreed = 0;
};

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_DEBUG_ALLOCATOR_H_
