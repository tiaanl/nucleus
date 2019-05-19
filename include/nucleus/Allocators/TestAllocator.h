
#ifndef NUCLEUS_ALLOCATORS_TEST_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_TEST_ALLOCATOR_H_

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/Allocators/DefaultAllocator.h"

namespace nu {

class TestAllocator : public Allocator {
public:
  explicit TestAllocator(Allocator* parentAllocator = getDefaultAllocator())
    : m_parentAllocator(parentAllocator), m_allocations(0), m_frees(0) {}

  ~TestAllocator() override = default;

  I32 getAllocations() const {
    return m_allocations;
  }

  I32 getFrees() const {
    return m_frees;
  }

protected:
  void* doAllocate(MemSize bytes, MemSize alignment) override {
    ++m_allocations;
    return m_parentAllocator->allocate(bytes, alignment);
  }

  void doFree(void* p, MemSize bytes, MemSize alignment) override {
    ++m_frees;
    m_parentAllocator->free(p, bytes, alignment);
  }

  bool doIsEqual(const Allocator& other) const override {
    return this == &other;
  }

private:
  Allocator* m_parentAllocator;

  I32 m_allocations;
  I32 m_frees;
};

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_TEST_ALLOCATOR_H_
