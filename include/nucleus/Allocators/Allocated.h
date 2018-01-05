
#ifndef NUCLEUS_MEMORY_ALLOCATED_H_
#define NUCLEUS_MEMORY_ALLOCATED_H_

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/Macros.h"
#include "nucleus/Types.h"

#undef free

namespace nu {

template <typename T>
class Allocated {
public:
  using ElementType = T;

  COPY_DELETE(Allocated);

  // Construct an empty `Allocated`, but with a valid `Allocator`.  Use `construct` to create a new object into this
  // `Allocated`.
  explicit Allocated(Allocator* allocator) : m_allocator(allocator), m_ptr(nullptr) {}

  // Move another `Allocated`'s internals into this one.  Leave the other other one's allocator in tact, because an
  // `Allocated` object is invalid without one.
  Allocated(Allocated&& other) : m_allocator(other.m_allocator), m_ptr(other.m_ptr) {
    other.m_ptr = nullptr;
  }

  // Destruct the `Allocated` freeing any memory we might have allocated.
  ~Allocated() {
    free();
  }

  // Move another `Allocated`'s internals into this one.  Leave the other other one's allocator in tact, because an
  // `Allocated` object is invalid without one.
  Allocated& operator=(Allocated&& other) {
    m_allocator = other.m_allocator;
    m_ptr = other.m_ptr;

    other.m_ptr = nullptr;

    return *this;
  }

  bool isEmpty() const {
    return !m_ptr;
  }

  // Allocate a new object of type `ElementType` and store it in this `Allocated`.
  template <typename... Args>
  void allocate(Args&&... args) {
    void* data = m_allocator->allocate(sizeof(T), alignof(T));
    m_ptr = new (data) T(std::forward<Args>(args)...);
  }

  // Free the memory we might have allocated.
  void free() {
    if (m_ptr) {
      m_allocator->free(m_ptr, sizeof(T), alignof(T));
    }
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
  template <typename U, typename... Args>
  friend Allocated<U> allocate(Allocator* allocator, Args... args);

  Allocated(Allocator* allocator, T* ptr) : m_allocator(allocator), m_ptr(ptr) {}

  Allocator* m_allocator;
  ElementType* m_ptr;
};

template <typename T, typename... Args>
inline Allocated<T> allocate(Allocator* allocator, Args... args) {
  Allocated<T> obj{allocator};
  obj.allocate(std::forward<Args>(args)...);
  return obj;
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_ALLOCATED_H_
