#ifndef NUCLEUS_CONTAINERS_GROWING_ARRAY_H_
#define NUCLEUS_CONTAINERS_GROWING_ARRAY_H_

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"

namespace nu {

template <typename T, MemSize BlockSize = 64>
class GrowingArray {
public:
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

  const T* operator[](MemSize index) const {
    return operator[](index);
  }

  void append(const T& item) {
    ensureSpace();
    m_currentBlock->items[m_currentBlock->size] = item;
    ++m_size;
  }

  void append(T&& item) {
    ensureSpace();
    m_currentBlock->items[m_currentBlock->size] = item;
    ++m_size;
  }

private:
  DELETE_COPY_AND_MOVE(GrowingArray);

  struct Block {
    T items[BlockSize];
    MemSize size = 0;
    Block* next = nullptr;
  };

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
