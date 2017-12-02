
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
    ensureAllocated(m_size, false);
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

    ensureAllocated(m_size, false);
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

  bool isEmpty() const {
    return m_size == 0;
  }

  // Get

  ElementType* getData() {
    return m_data;
  }

  const ElementType* getData() const {
    return m_data;
  }

  const ElementType& get(SizeType index) const {
    return m_data[index];
  }

  ElementType& get(SizeType index) {
    return m_data[index];
  }

  const ElementType& last() const {
    return m_data[m_size - 1];
  }

  ElementType& last() {
    return m_data[m_size - 1];
  }

  // Push

  void pushBack(const ElementType& element) {
    ensureAllocated(m_size + 1, true);

    m_data[m_size++] = element;
  }

  template <typename... Args>
  void emplaceBack(Args&&... args) {
    ensureAllocated(m_size + 1, true);

    new (&m_data[m_size++]) ElementType{std::forward<Args>(args)...};
  }

  // Modify

  void remove(Iterator pos) {
    // Destroy the object.
    pos->~ElementType();
    ::memcpy(pos, pos + 1, (m_data + (m_size - 1) - pos) * sizeof(ElementType));
    --m_size;
  }

  void remove(Iterator begin, Iterator end) {
    for (Iterator e = begin; e != end; ++e) {
      e->~ElementType();
    }

    if (end < m_data + m_size) {
      ::memcpy(begin, end, (end - begin) * sizeof(ElementType));
    }

    m_size -= end - begin;
  }

  void reserve(SizeType size) {
    if (size <= m_allocated) {
      return;
    }

    ensureAllocated(size, true);
  }

  void resize(SizeType newSize) {
    ensureAllocated(newSize, true);
    m_size = newSize;
  }

  void resize(SizeType newSize, const ElementType& fillValue) {
    ensureAllocated(newSize, true);

    for (SizeType i = m_size; i < newSize; ++i) {
      m_data[i] = fillValue;
    }

    m_size = newSize;
  }

  void clear() {
    free();
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
  void ensureAllocated(SizeType newSize, bool keepOld) {
    if (newSize > m_allocated) {
      allocateData(nu::max<SizeType>(newSize << 1, 1 << 4), true);
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
    if (m_data) {
      // Destruct all the objects we contain.
      for (ElementType* el = m_data; el != m_data + m_size; el++) {
        el->~ElementType();
      }

      m_allocator->free(m_data, m_allocated * sizeof(ElementType));
    }

    m_data = nullptr;
    m_size = 0;
    m_allocated = 0;
  }

  Allocator* m_allocator;
  ElementType* m_data;
  SizeType m_size;
  SizeType m_allocated;
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_
