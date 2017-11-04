
#ifndef NUCLEUS_CONTAINERS_VECTOR_H_
#define NUCLEUS_CONTAINERS_VECTOR_H_

#include <new>

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/types.h"

namespace nu {

template <typename T>
class Vector {
public:
  Vector(Allocator* allocator = getDefaultAllocator())
    : m_allocator(allocator), m_data(nullptr), m_allocated(0), m_size(0) {}

  Vector(const Vector& other, Allocator* allocator = getDefaultAllocator())
    : m_size(other.m_size), m_allocator(other.m_allocator) {
    ensureAllocated(other.m_allocated, false);
    std::memcpy(m_data, other.m_data, other.m_allocated);
  }

  Vector(Vector&& other)
    : m_allocator(other.m_allocator), m_data(other.m_data), m_allocated(other.m_allocated), m_size(other.m_size) {
    other.m_allocator = nullptr;
    other.m_data = nullptr;
    other.m_allocated = 0;
    other.m_size = 0;
  }

  Vector(Vector&& other, Allocator* allocator) : Vector(allocator) {
    if (allocator == m_allocator) {
      operator=(std::move(other));
    } else {
      operator=(other);
    }
  }

  ~Vector() {
    if (m_data) {
      m_allocator->free(m_data, m_allocated);
    }
  }

  USize getSize() const { return m_size; }

  void pushBack(const T& item) {
    ensureAllocated((m_size + 1) * sizeof(T), true);

    auto pp = m_data[m_size];

    ::new (reinterpret_cast<void*>(&m_data[m_size])) T(item);

    ++m_size;
  }

private:
  void ensureAllocated(USize bytes, bool keepOld) {
    // Allocate a new buffer.
    T* newData = static_cast<T*>(m_allocator->allocate(bytes));

    // If we should keep the old data, then copy it over.
    if (m_data) {
      if (keepOld) {
        std::memcpy(newData, m_data, std::min(bytes, m_allocated));
      }

      // Free the old block.
      m_allocator->free(m_data, m_allocated);
    }

    // Update the internal state.
    m_data = newData;
    m_allocated = bytes;
  }

  Allocator* m_allocator;
  T* m_data;
  USize m_allocated;
  USize m_size;
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_VECTOR_H_
