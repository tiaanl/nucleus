
#ifndef NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_
#define NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_

#include <cstring>  // memcpy
#include <new>      // new
#include <utility>  // std::forward

#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/algorithms/Utility.h"

namespace nu {

template <typename T>
class DynamicArray {
public:
  using ElementType = T;
  using SizeType = USize;
  using Iterator = ElementType*;
  using ConstIterator = const ElementType*;

  // Construct/desctruct

  DynamicArray(Allocator* allocator = getDefaultAllocator())
    : m_allocator(allocator), m_data(nullptr), m_size(0), m_allocated(0) {}

  DynamicArray(const DynamicArray& other)
    : m_allocator(other.m_allocator), m_data(nullptr), m_size(other.m_size), m_allocated(0) {
    ensureAllocated(m_size);
    ::memcpy(m_data, other.m_data, m_size * sizeof(ElementType));
  }

  DynamicArray(DynamicArray&& other)
    : m_allocator(other.m_allocator), m_data(other.m_data), m_size(other.m_size), m_allocated(other.m_allocated) {
    other.m_allocator = nullptr;
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_allocated = 0;
  }

  ~DynamicArray() {
    free();
  }

  // Operators

  DynamicArray& operator=(const DynamicArray& other) {
    m_allocator = other.m_allocator;
    m_data = nullptr;
    m_size = other.m_size;
    m_allocated = 0;

    ensureAllocated(m_size);
    ::memcpy(m_data, other.m_data, m_size * sizeof(ElementType));

    return *this;
  }

  DynamicArray& operator=(DynamicArray&& other) {
    m_allocator = other.m_allocator;
    m_data = other.m_data;
    m_size = other.m_size;
    m_allocated = other.m_allocated;

    other.m_allocator = nullptr;
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_allocated = 0;

    return *this;
  }

  // State

  SizeType getSize() const {
    return m_size;
  }

  // Get

  const ElementType& get(SizeType index) const {
    return m_data[index];
  }

  ElementType& get(SizeType index) {
    return m_data[index];
  }

  // Push

  void pushBack(const ElementType& element) {
    ensureAllocated(m_size + 1);

    m_data[m_size++] = element;
  }

  template <typename... Args>
  void emplaceBack(Args&&... args) {
    ensureAllocated(m_size + 1);

    new (&m_data[m_size++]) ElementType(std::forward<Args>(args)...);
  }

  // Remove

  void remove(Iterator pos) {
    ::memcpy(pos, pos + 1, (m_data + (m_size - 1) - pos) * sizeof(ElementType));
    --m_size;
  }

  void remove(Iterator begin, Iterator end) {
    if (end < m_data + m_size) {
      ::memcpy(begin, end, (end - begin) * sizeof(ElementType));
    }
    m_size -= end - begin;
  }

  // Iterators

  Iterator begin() {
    return m_data;
  }

  Iterator end() {
    return m_data + m_size;
  }

  ConstIterator begin() const {
    return m_data;
  }

  ConstIterator end() const {
    return m_data + m_size;
  }

private:
  void ensureAllocated(SizeType newSize) {
    if (newSize > m_allocated) {
      allocateData(nu::min<SizeType>(newSize << 1, 1 << 4), true);
    }
  }

  void allocateData(SizeType size, bool keepOld) {
    const USize newSizeInBytes = size * sizeof(ElementType);
    const USize oldSizeInBytes = m_size * sizeof(ElementType);

    ElementType* newData = static_cast<ElementType*>(m_allocator->allocate(size * sizeof(ElementType)));

    if (m_data) {
      if (keepOld) {
        ::memcpy(newData, m_data, oldSizeInBytes);
      }

      m_allocator->free(m_data, oldSizeInBytes);
    }

    m_allocated = size;
    m_data = newData;
  }

  void free() {
    if (!m_data) {
      return;
    }

    // Destruct all the objects we contain.
    for (ElementType* el = m_data; el != m_data + m_size; el++) {
      el->~ElementType();
    }

    m_allocator->free(m_data, m_allocated * sizeof(ElementType));
  }

  Allocator* m_allocator;
  ElementType* m_data;
  SizeType m_size;
  SizeType m_allocated;
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_
