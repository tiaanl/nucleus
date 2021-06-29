#pragma once

#include <cstring>

#include "nucleus/text/string_view.h"
#include "nucleus/config.h"
#include "nucleus/logging.h"
#include "nucleus/types.h"

namespace nu {

// TODO: Maybe return the amount of bytes used by stored strings.

// Stores strings in a pool of fixed sized chunks.
// - Size specified the size of each chunk in a block.
// - Count is the number of chunks we store in a single block.
// If a string is longer than one chunk, we use additional contiguous chunks to fit it in.  When we
// need more space, more blocks will be allocated.
template <MemSize Size = 64, MemSize Count = 64>
class StringPool {
public:
  StringPool() = default;
  StringPool(const StringPool&) = delete;
  StringPool(StringPool&&) = delete;
  ~StringPool() {}

  StringPool& operator=(const StringPool&) = delete;
  StringPool& operator=(StringPool&&) = delete;

  MemSize getCapacity() const {
    return m_blockCount * Size * Count;
  }

  // Store a source string inside the pool and return the location where it is stored.
  StringView store(const StringView& source) {
    // If the string we're trying to store is for some reason bigger than a single block, we report
    // an error.
    if (source.length() > Size * Count) {
      LOG(Error) << "Tried to store a string in a string pool that will not fit.";
      return StringView{};
    }

    // Make sure there is a first block and hence also a last block.
    if (!m_first) {
      m_first = allocateBlock();
      m_last = m_first;
    }

    DCHECK(m_last != nullptr);

    // The amount of strings the source will take up.  Because we return a StringView that already
    // has a length, we don't have to add space for a null terminator.
    MemSize stringsRequired = source.length() / Size + 1;

    // If the last block doesn't have space, then we append a new block.
    if (Count - m_last->used < stringsRequired) {
      m_last->next = allocateBlock();
      m_last = m_last->next;
    }

    DCHECK(Count - m_last->used >= stringsRequired);

    // Store the source into the block and increase the used count.
    Char* destination = m_last->chunks[m_last->used];
#if COMPILER(MSVC)
    strncpy_s(destination, stringsRequired * Size, source.data(), source.length());
#else
    std::strncpy(destination, source.data(), source.length());
#endif
    m_last->used += stringsRequired;

    return StringView(destination, source.length());
  }

private:
  struct Block {
    Char chunks[Size][Count];
    Block* next = nullptr;
    MemSize used = 0;
  };

  Block* allocateBlock() {
    ++m_blockCount;
    return new Block{};
  }

  MemSize m_blockCount = 0;
  Block* m_first = nullptr;
  Block* m_last = nullptr;
};

}  // namespace nu
