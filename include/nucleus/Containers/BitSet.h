
#ifndef NUCLEUS_CONTAINERS_BIT_SET_H_
#define NUCLEUS_CONTAINERS_BIT_SET_H_

#include "nucleus/Config.h"
#include "nucleus/Types.h"

namespace nu {

template <PtrDiff BitCount>
class BitSet {
public:
  using WordType = MemSize;
  using IndexType = PtrDiff;

  BitSet() {
    reset();
  }

  constexpr PtrDiff getBitCount() const {
    return BitCount;
  }

  WordType getWord(IndexType pos) const {
    return m_data[pos];
  }

  void set(IndexType index, bool value = true) {
    if (value) {
      m_data[index / kBitsPerWord] |= static_cast<MemSize>(1) << (index % kBitsPerWord);
    } else {
      m_data[index / kBitsPerWord] &= ~(static_cast<MemSize>(1) << (index % kBitsPerWord));
    }
  }

  constexpr bool test(IndexType index) const {
    return (m_data[index / kBitsPerWord] & (static_cast<MemSize>(1) << index % kBitsPerWord)) != 0;
  }

  // Set all bits to 0.
  void reset() {
    for (IndexType i = kWords; 0 <= i; --i) {
      m_data[i] = static_cast<MemSize>(0);
    }
  }

  // Operators

  bool operator==(const BitSet& right) const {
    for (IndexType i = kWords; 0 <= i; --i) {
      if (m_data[i] != right.getWord(i)) {
        return false;
      }
    }

    return true;
  }

  bool operator!=(const BitSet& right) const {
    return !(*this == right);
  }

  BitSet& operator&=(const BitSet& right) {
    for (IndexType i = kWords; 0 <= i; --i) {
      m_data[i] &= right.getWord(i);
    }

    return *this;
  }

  BitSet& operator|=(const BitSet& right) {
    for (IndexType i = kWords; 0 <= i; --i) {
      m_data[i] |= right.getWord(i);
    }

    return *this;
  }

  BitSet& operator^=(const BitSet& right) {
    for (IndexType i = kWords; 0 <= i; --i) {
      m_data[i] ^= right.getWord(i);
    }

    return *this;
  }

private:
  enum : IndexType {
    kBitsPerWord = static_cast<IndexType>(8 * sizeof(IndexType)),
    kWords = static_cast<IndexType>(BitCount == 0 ? 0 : (BitCount - 1) / kBitsPerWord)
  };

  WordType m_data[kWords + 1];
};

template <PtrDiff BitCount>
inline BitSet<BitCount> operator&(const BitSet<BitCount>& left, const BitSet<BitCount>& right) {
  BitSet<BitCount> result = left;
  return (result &= right);
}

template <PtrDiff BitCount>
inline BitSet<BitCount> operator|(const BitSet<BitCount>& left, const BitSet<BitCount>& right) {
  BitSet<BitCount> result = left;
  return (result |= right);
}

template <PtrDiff BitCount>
inline BitSet<BitCount> operator^(const BitSet<BitCount>& left, const BitSet<BitCount>& right) {
  BitSet<BitCount> result = left;
  return (result ^= right);
}

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_BIT_SET_H_
