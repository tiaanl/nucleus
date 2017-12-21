
#ifndef NUCLEUS_MEMORY_ALLOCATED_H_
#define NUCLEUS_MEMORY_ALLOCATED_H_

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/Types.h"
#include "nucleus/Utils/Move.h"

#undef free

namespace nu {

template <typename T>
class Allocated {
public:
  using ElementType = T;

  explicit Allocated(Allocator* allocator) : m_allocator(allocator), m_ptr(nullptr), m_size(0), m_alignment(0) {}

  Allocated(const Allocated& other) = delete;

  Allocated(Allocated&& other)
    : m_allocator(other.m_allocator), m_ptr(other.m_ptr), m_size(other.m_size), m_alignment(other.m_alignment) {
    other.m_ptr = nullptr;
    other.m_size = 0;
    other.m_alignment = 0;
  }

  ~Allocated() {
    if (m_ptr && m_size) {
      m_allocator->free(m_ptr, m_size, m_alignment);
    }
  }

  Allocated& operator=(const Allocated& other) = delete;

  Allocated& operator=(Allocated&& other) {
    m_allocator = other.m_allocator;
    m_ptr = other.m_ptr;
    m_size = other.m_size;
    m_alignment = other.m_alignment;

    other.m_ptr = nullptr;
    other.m_size = 0;
    other.m_alignment = 0;

    return *this;
  }

  ElementType* operator->() const {
    return m_ptr;
  }

  ElementType& operator*() const {
    return *m_ptr;
  }

  ElementType* get() const {
    return m_ptr;
  }

  bool operator!() const {
    return !m_ptr;
  }

private:
  template <typename S, typename... Args>
  friend Allocated<S> allocate(Allocator* allocator, Args... args);

  Allocated(Allocator* allocator, T* ptr, USize size, USize alignment)
    : m_allocator(allocator), m_ptr(ptr), m_size(size), m_alignment(alignment) {}

  Allocator* m_allocator;
  ElementType* m_ptr;
  USize m_size;
  USize m_alignment;
};

template <typename T, typename... Args>
inline Allocated<T> allocate(Allocator* allocator, Args... args) {
  USize sizeToAllocate = sizeof(T);
  USize alignment = alignof(T);

  void* data = allocator->allocate(sizeToAllocate, alignment);

  // Construct the new object onto the allocated data and return the the Allocated wrapper.
  return Allocated<T>{allocator, new (data) T(forward<Args>(args)...), sizeToAllocate, alignment};
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_ALLOCATED_H_
