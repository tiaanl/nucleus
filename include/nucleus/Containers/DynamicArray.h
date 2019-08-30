
#ifndef NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_
#define NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_

#include "nucleus/Logging.h"
#include "nucleus/Types.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <new>
#include <type_traits>
#include <utility>

namespace nu {

template <typename T>
class DynamicArray {
public:
  using ElementType = T;
  using SizeType = MemSize;
  using Iterator = ElementType*;
  using ConstIterator = const ElementType*;

  // Construct/destruct

  DynamicArray() = default;

  DynamicArray(ElementType* data, SizeType dataSize) : m_size(dataSize) {
    ensureAllocated(m_size, DiscardOldData);

    // Do not do a memcpy here, because we need to call the copy constructor of the elements being
    // copied.
    std::copy(m_data, m_data + dataSize, data);

    // for (SizeType i = 0; i < dataSize; ++i) {
    //   m_data[i] = data[i];
    // }
  }

  DynamicArray(const DynamicArray& other) : m_size(other.m_size) {
    ensureAllocated(m_size, DiscardOldData);
    ::memcpy(m_data, other.m_data, m_size * sizeof(ElementType));
  }

  DynamicArray(DynamicArray&& other) noexcept
    : m_data(other.m_data), m_size(other.m_size), m_allocated(other.m_allocated) {
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_allocated = 0;
  }

  explicit DynamicArray(SizeType initialCapacity) {
    ensureAllocated(initialCapacity, DiscardOldData);
  }

  ~DynamicArray() {
    free();
  }

  // Operators

  DynamicArray& operator=(const DynamicArray& other) {
    m_data = nullptr;
    m_size = other.m_size;
    m_allocated = 0;

    ensureAllocated(m_size, DiscardOldData);
    ::memcpy(m_data, other.m_data, m_size * sizeof(ElementType));

    return *this;
  }

  DynamicArray& operator=(DynamicArray&& other) noexcept {
    m_data = other.m_data;
    m_size = other.m_size;
    m_allocated = other.m_allocated;

    other.m_data = nullptr;
    other.m_size = 0;
    other.m_allocated = 0;

    return *this;
  }

  // State

  SizeType getSize() const {
    return m_size;
  }

  SizeType getAllocated() const {
    return m_allocated;
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

  const ElementType& operator[](SizeType index) const {
    return m_data[index];
  }

  ElementType& operator[](SizeType index) {
    return m_data[index];
  }

  const ElementType& last() const {
    return m_data[m_size - 1];
  }

  ElementType& last() {
    return m_data[m_size - 1];
  }

  // Push

  class PushBackResult {
  public:
    PushBackResult(ElementType* element, SizeType index) : m_element{element}, m_index{index} {}

    ElementType& element() {
      return *m_element;
    }

    SizeType index() {
      return m_index;
    }

  private:
    ElementType* m_element;
    SizeType m_index;
  };

  PushBackResult pushBack(const ElementType& element) {
    ensureAllocated(m_size + 1, KeepOldData);

    SizeType index = m_size++;
    ElementType* storage = &m_data[index];

    // Invoke the copy constructor on the ElementType to copy the element into place.
    new (storage) ElementType(element);

    return {storage, index};
  }

  PushBackResult pushBack(ElementType&& element) {
    ensureAllocated(m_size + 1, KeepOldData);

    SizeType index = m_size++;
    ElementType* storage = &m_data[index];

    // Invoke the move constructor on ElementType to move the element into place.
    new (storage) ElementType(std::forward<ElementType>(element));

    return {storage, index};
  }

#if 0
  template <typename Func>
  PushBackResult constructBack(Func func) {
    ensureAllocated(m_size + 1, KeepOldData);

    SizeType index = m_size++;
    ElementType* storage = &m_data[index];

    func(storage);

    return {storage, index};
  }
#endif  // 0

  // Push back a range of elements.
  void pushBack(Iterator begin, Iterator end) {
    ensureAllocated(m_size + end - begin, KeepOldData);

    m_size += end - begin;

    for (Iterator i = m_data; begin != end;) {
      new (i++) ElementType(*begin++);
    }
  }

  template <typename... Args>
  PushBackResult emplaceBack(Args&&... args) {
    ensureAllocated(m_size + 1, KeepOldData);

    SizeType index = m_size++;
    ElementType* storage = &m_data[index];

    new (storage) ElementType{std::forward<Args>(args)...};

    return {storage, index};
  }

  // Modify

  void remove(Iterator pos) {
    // Destroy the element.
    pos->~ElementType();

    // If we didn't remove the last item, then move all the items one to the left.
    if (pos + 1 < m_data + m_size) {
      for (auto i = pos; i != m_data + (m_size - 1); ++i) {
        *i = std::move(*(i + 1));
      }
    }

    // We have 1 item less now.
    --m_size;
  }

  void remove(Iterator begin, Iterator end) {
    // Destruct all the elements we are about to remove from the array.
    for (Iterator e = begin; e != end; ++e) {
      e->~ElementType();
    }

    const auto numberOfElementsToRemove = end - begin;
    if (end < m_data + m_size) {
      for (auto i = begin; i != m_data + (m_size - numberOfElementsToRemove); ++i) {
        *i = std::move(*(i + numberOfElementsToRemove));
      }
    }

    m_size -= numberOfElementsToRemove;
  }

  void reserve(SizeType size) {
    if (size <= m_allocated) {
      return;
    }

    ensureAllocated(size, KeepOldData);
  }

  void resize(SizeType newSize) {
    ensureAllocated(newSize, KeepOldData);
    m_size = newSize;
  }

  void resize(SizeType newSize, const ElementType& fillValue) {
    ensureAllocated(newSize, KeepOldData);

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

protected:
  enum KeepOld { KeepOldData = true, DiscardOldData = false };

  // Ensure that we can accommodate `elementsRequired` elements.
  void ensureAllocated(SizeType elementsRequired, KeepOld keepOld) {
    if ((elementsRequired * sizeof(ElementType)) > m_allocated) {
      // We take the maximum number between 16 or twice our current size.
      MemSize bytesToAllocate = (elementsRequired * sizeof(ElementType)) << 1;
      resizeData(std::max<SizeType>(bytesToAllocate, sizeof(ElementType) << 4), keepOld);
    }
  }

private:
  void resizeData(MemSize bytesRequired, KeepOld keepOld) {
    DCHECK(m_allocated <= bytesRequired)
        << "The container already has enough space to fit the new elements.";

    ElementType* newData = static_cast<ElementType*>(std::malloc(bytesRequired));

    // If we have old data already in the buffer, then we have to destroy it.  If `keepOld` is set,
    // then we copy the old data to the new data first.
    if (m_data) {
      if (keepOld == KeepOldData) {
        std::move(m_data, m_data + m_size, newData);
      }

      std::free(m_data);
    }

    m_allocated = bytesRequired;
    m_data = newData;
  }

  void free() {
    if (m_data) {
      // Destruct all the objects we contain.
      for (ElementType* el = m_data; el != m_data + m_size; el++) {
        el->~ElementType();
      }

      std::free(m_data);

      m_data = nullptr;
      m_size = 0;
      m_allocated = 0;
    }
  }

  // Pointer to the data inside this container.
  ElementType* m_data = nullptr;

  // Total amount of elements currently in the container.
  SizeType m_size = 0;

  // Total amount of bytes allocated for this container.
  MemSize m_allocated = 0;
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_
