
#ifndef NUCLEUS_MEMORY_ALLOCATED_H_
#define NUCLEUS_MEMORY_ALLOCATED_H_

#include "nucleus/Types.h"

#undef free

namespace nu {

class Allocator;

template <typename T>
class Allocated {
public:
  using ElementType = T;

  explicit Allocated(Allocator* allocator) : m_allocator(allocator), m_ptr(nullptr), m_size(0), m_alignment(0) {}

  Allocated(Allocator* allocator, ElementType* ptr, USize size, USize alignment)
    : m_allocator(allocator), m_ptr(ptr), m_size(size), m_alignment(alignment) {}

  ~Allocated() {
    if (m_ptr && m_size) {
      m_allocator->free(m_ptr, m_size, m_alignment);
    }
  }

  ElementType* get() const {
    return m_ptr;
  }

  bool operator!() const {
    return !m_ptr;
  }

private:
  Allocator* m_allocator;
  ElementType* m_ptr;
  USize m_size;
  USize m_alignment;
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_ALLOCATED_H_
