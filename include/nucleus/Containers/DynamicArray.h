#ifndef NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_
#define NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_

#include <algorithm>
#include <cstring>
#include <functional>
#include <new>
#include <type_traits>
#include <utility>

#include "nucleus/Containers/ArrayView.h"
#include "nucleus/Logging.h"
#include "nucleus/Types.h"

#undef free

namespace nu {

template <typename T>
class DynamicArray {
public:
  using ElementType = T;
  using SizeType = MemSize;
  using Iterator = ElementType*;
  using ConstIterator = const ElementType*;

  // Factory Methods

  static auto withInitialCapacity(SizeType initialCapacity) -> DynamicArray {
    DynamicArray result;

    result.ensureAllocated(initialCapacity, DiscardOldData);

    return result;
  }

  static auto withInitialSize(SizeType initialSize, const ElementType& value = ElementType{})
      -> DynamicArray {
    DynamicArray result;

    result.ensureAllocated(initialSize, DiscardOldData);
    result.m_size = initialSize;
    std::fill(result.m_data, result.m_data + result.m_size, value);

    return result;
  }

  // Construct/destruct

  DynamicArray() = default;

  DynamicArray(ElementType* data, SizeType size) : m_size{size} {
    ensureAllocated(size, DiscardOldData);
    std::copy(data, data + size, m_data);
  }

  DynamicArray(const DynamicArray& other) : m_size{other.m_size} {
    ensureAllocated(m_size, DiscardOldData);
    std::copy(other.m_data, other.m_data + other.m_size, m_data);
  }

  DynamicArray(DynamicArray&& other) noexcept
    : m_data{other.m_data}, m_size{other.m_size}, m_capacity{other.m_capacity} {
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
  }

  DynamicArray(std::initializer_list<ElementType> list) : m_size{list.size()} {
    ensureAllocated(list.size(), DiscardOldData);
    std::copy(list.begin(), list.end(), m_data);
  }

  ~DynamicArray() {
    DynamicArray::free();
  }

  // Operators

  DynamicArray& operator=(const DynamicArray& other) {
    ensureAllocated(other.m_size, DiscardOldData);

    std::copy(other.m_data, other.m_data + other.m_size, m_data);
    m_size = other.m_size;

    return *this;
  }

  DynamicArray& operator=(DynamicArray&& other) noexcept {
    m_data = other.m_data;
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;

    return *this;
  }

  // State

  SizeType size() const {
    return m_size;
  }

  SizeType capacity() const {
    return m_capacity;
  }

  bool isEmpty() const {
    return m_size == 0;
  }

  // Get

  ArrayView<ElementType> view() const {
    return ArrayView<T>{m_data, m_size};
  }

  ElementType* data() {
    return m_data;
  }

  const ElementType* data() const {
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

  // Push back a range of elements.
  void pushBack(ConstIterator begin, ConstIterator end) {
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

  template <typename Func>
  PushBackResult constructBack(Func init) {
    ensureAllocated(m_size + 1, KeepOldData);

    SizeType index = m_size++;
    ElementType* storage = &m_data[index];

    init(storage);

    return {storage, index};
  }

  // Modify

  void remove(Iterator pos) {
    DCHECK(pos >= m_data && pos < m_data + m_size) << "Iterator out of bounds.";

    // Destroy the element.
    pos->~ElementType();

    // If we didn't remove the last item, then move all the items one to the left.
    if (pos + 1 != m_data + m_size) {
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

  // Remove all the elements from the array, but keep the current capacity.
  auto removeAll() -> void {
    for (Iterator it = m_data; it != m_data + m_size; ++it) {
      it->~ElementType();
    }

    m_size = 0;
  }

  void reserve(SizeType size) {
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
    if (elementsRequired > m_capacity) {
      // We take the maximum number between 16 or twice our current capacity.
      SizeType requiredCapacity = std::max<SizeType>(m_capacity, 1 << 4);
      while (requiredCapacity < elementsRequired) {
        requiredCapacity <<= 1;
      }

      resizeData(requiredCapacity, keepOld);
    }
  }

private:
  void resizeData(MemSize elementsRequired, KeepOld keepOld) {
    DCHECK(m_capacity <= elementsRequired)
        << "The container already has enough space to fit the new elements.";

    MemSize bytesRequired = elementsRequired * sizeof(ElementType);

    ElementType* newData = static_cast<ElementType*>(std::malloc(bytesRequired));

    // If we have old data already in the buffer, then we have to destroy it.  If `keepOld` is set,
    // then we copy the old data to the new data first.
    if (m_data) {
      if (keepOld == KeepOldData) {
        std::move(m_data, m_data + m_capacity, newData);
      }

      // Free the old data.
      std::free(m_data);
    }

    m_capacity = elementsRequired;
    m_data = newData;
  }

  void free() {
    if (m_data) {
      // Destruct all the objects we contain.
      for (ElementType* el = m_data; el != m_data + m_size; ++el) {
        el->~ElementType();
      }

      std::free(m_data);
      m_data = nullptr;
    }

    m_size = 0;
    m_capacity = 0;
  }

  // Pointer to the data inside this container.
  ElementType* m_data = nullptr;

  // Total amount of elements currently in the container.
  SizeType m_size = 0;

  // Total amount of elements currently allocated.
  MemSize m_capacity = 0;
};

}  // namespace nu

template <typename T>
std::ostream& operator<<(std::ostream& os, const nu::DynamicArray<T>& value) {
  os << '[';
  auto size = value.size();
  for (auto it = value.begin(), eit = value.end(); it != eit; ++it) {
    os << *it;
    if (size-- != 1) {
      os << ", ";
    }
  }
  os << ']';

  return os;
}

#endif  // NUCLEUS_CONTAINERS_DYNAMIC_ARRAY_H_
