#ifndef NUCLEUS_CONTAINERS_GROWING_ARRAY_H_
#define NUCLEUS_CONTAINERS_GROWING_ARRAY_H_

#include <functional>

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"

namespace nu {

template <typename T, MemSize BlockSize = 64>
class GrowingArray {
public:
  NU_DELETE_COPY_AND_MOVE(GrowingArray);

  struct Block {
    T items[BlockSize];
    MemSize size = 0;
    Block* next = nullptr;
  };

  class Iterator {
  public:
    Iterator& operator++() {
      next();
      return *this;
    }

    bool operator==(const Iterator& other) const {
      if (m_block == nullptr && other.m_block == nullptr) {
        return true;
      }

      return m_block == other.m_block && m_index == other.m_index;
    }

    bool operator!=(const Iterator& other) const {
      return !operator==(other);
    }

    T& get() const {
      return m_block->items[m_index];
    }

    T& operator*() {
      return m_block->items[m_index];
    }

    T* operator->() const {
      return &m_block->items[m_index];
    }

  private:
    friend class GrowingArray;

    explicit Iterator(Block* firstBlock = nullptr) : m_block{firstBlock}, m_index{0} {}

    void next() {
      if (!m_block) {
        return;
      }

      ++m_index;

      if (m_index < m_block->size) {
        return;
      }

      m_block = m_block->next;
      m_index = 0;
    }

    GrowingArray::Block* m_block;
    MemSize m_index;
  };

  GrowingArray() = default;

  ~GrowingArray() {
    while (m_firstBlock) {
      Block* next = m_firstBlock->next;
      delete m_firstBlock;
      m_firstBlock = next;
    }
  }

  MemSize size() const {
    return m_size;
  }

  bool empty() const {
    return m_firstBlock == nullptr;
  }

  T* operator[](MemSize index) {
    MemSize blockIndex = index;
    Block* block = m_firstBlock;
    while (blockIndex >= BlockSize) {
      index -= BlockSize;
      block = block->next;
    }
    if (!block) {
      LOG(Warning) << "Index out of bounds. (" << index << ")";
      return nullptr;
    }
    return &block->items[blockIndex];
  }

  Iterator begin() {
    return Iterator{m_firstBlock};
  }

  Iterator end() {
    return Iterator{};
  }

  const T* operator[](MemSize index) const {
    return operator[](index);
  }

  T* append(const T& item) {
    ensureSpace();
    T* storage = &m_currentBlock->items[m_currentBlock->size];

    // Invoke the copy constructor on the element.
    new (storage) T(item);

    ++m_currentBlock->size;
    ++m_size;

    return storage;
  }

  T* emplace(T&& item) {
    ensureSpace();
    T* storage = &m_currentBlock->items[m_currentBlock->size];

    // Invoke the move constructor of the item if it is available.
    new (storage) T(std::forward<T>(item));

    ++m_currentBlock->size;
    ++m_size;

    return storage;
  }

private:
  // Make sure there is enough space to append one more item.
  void ensureSpace() {
    if (!m_firstBlock) {
      m_firstBlock = new Block;
      m_currentBlock = m_firstBlock;
    }

    if (m_currentBlock->size == BlockSize) {
      m_currentBlock->next = new Block;
      m_currentBlock = m_currentBlock->next;
    }

    DCHECK(m_currentBlock != nullptr);
    DCHECK(m_currentBlock->size <= BlockSize);
  }

  MemSize m_size = 0;
  Block* m_firstBlock = nullptr;
  Block* m_currentBlock = nullptr;
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_GROWING_ARRAY_H_
