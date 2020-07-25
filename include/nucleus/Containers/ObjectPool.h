#ifndef NUCLEUS_CONTAINERS_OBJECT_POOL_H_
#define NUCLEUS_CONTAINERS_OBJECT_POOL_H_

#include <algorithm>

#include "nucleus/Logging.h"
#include "nucleus/Types.h"

namespace nu {

template <typename T>
class ObjectPool {
public:
  using ElementType = T;
  using SizeType = MemSize;

  ObjectPool() = default;

  ~ObjectPool() {
    releaseAll(true);
  }

  auto size() const -> SizeType {
    return m_size;
  }

  auto capacity() const -> SizeType {
    return m_blockCount * bitsPerBitSetType;
  }

  auto isEmpty() const -> bool {
    return m_size == 0;
  }

  template <typename... Args>
  auto acquire(Args&&... args) -> ElementType* {
    ElementType* element = acquireAvailableElement();

    new (element) ElementType(std::forward<Args>(args)...);

    ++m_size;

    return element;
  }

  auto release(ElementType* element) -> void {
    Block* bl = m_firstBlock;
    for (; bl; bl = bl->next) {
      for (BitSetType index = 0; index < bitsPerBitSetType; ++index) {
        ElementType* el = &bl->elements[index];
        if (el == element) {
          bl->bitSet &= ~(static_cast<BitSetType>(1) << static_cast<BitSetType>(index));
          (el)->~ElementType();
          return;
        }
      }
    }
  }

  auto releaseAll() -> void {
    releaseAll(false);
  }

private:
  using BitSetType = U32;

  static constexpr U32 bitsPerBitSetType = sizeof(BitSetType) * 8;

  struct Block {
    ElementType elements[bitsPerBitSetType];
    BitSetType bitSet;
    Block* next;
  };

  auto acquireAvailableElementInBlock(Block* block) -> ElementType* {
    DCHECK(block->bitSet != std::numeric_limits<BitSetType>::max());

    for (auto i = static_cast<BitSetType>(0); i < bitsPerBitSetType; ++i) {
      auto bit = static_cast<BitSetType>(1) << static_cast<BitSetType>(i);
      if (!(block->bitSet & bit)) {
        block->bitSet |= bit;
        return &block->elements[i];
      }
    }

    NOTREACHED();
    return nullptr;
  }

  auto acquireAvailableElement() -> ElementType* {
    // Find the first bit set that is not full.
    Block* block = m_firstBlock;
    for (; block; block = block->next) {
      if (block->bitSet != std::numeric_limits<BitSetType>::max()) {
        break;
      }
    }

    // If we could not find an available space, then we have to allocate a new block.
    if (!block) {
      Block* newBlock = static_cast<Block*>(std::malloc(sizeof(Block)));
      newBlock->bitSet = 0;

      newBlock->next = m_firstBlock;
      m_firstBlock = newBlock;

      block = m_firstBlock;
    }

    DCHECK(block);

    return acquireAvailableElementInBlock(block);
  }

  void releaseAll(bool mustFree = false) const {
    for (Block* current = m_firstBlock; current;) {
      Block* next = current->next;

      for (BitSetType index = 0; index < bitsPerBitSetType; ++index) {
        auto isSet =
            (current->bitSet & static_cast<BitSetType>(1) << static_cast<BitSetType>(index));
        if (isSet) {
          current->elements[index].~ElementType();
        }
      }

      current->bitSet = 0;

      if (mustFree) {
        std::free(current);
      }

      current = next;
    }
  }

  SizeType m_size = 0;
  SizeType m_blockCount = 0;
  Block* m_firstBlock = nullptr;
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_OBJECT_POOL_H_
