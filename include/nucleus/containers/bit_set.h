#pragma once

#include "nucleus/config.h"
#include "nucleus/logging.h"
#include "nucleus/macros.h"
#include "nucleus/types.h"

namespace nu {

template <MemSize BitCount>
class BitSet {
public:
  static_assert(BitCount >= 8, "BitCount should be at least the size of a byte.");

  using IndexType = MemSize;

  BitSet() = default;

  constexpr MemSize bit_count() const {
    return BitCount;
  }

  void set(IndexType index, bool value = true) {
    DCHECK(index < BitCount);

    auto i = index / bits_per_storage();
    auto new_value = static_cast<StorageType>(1) << index;
    if (value) {
      value_[i] |= new_value;
    } else {
      value_[i] &= ~new_value;
    }
  }

  bool test(IndexType index) const {
    DCHECK(index < BitCount);

    auto i = index / bits_per_storage();
    auto test_value = static_cast<StorageType>(1) << index;
    return (value_[i] & test_value) != 0;
  }

  // Set all bits to 0.
  constexpr void reset() {
    value_ = {};
  }

  // Operators

  bool operator==(const BitSet& right) const {
    return std::memcmp(&value_, sizeof(value_), &right.value_) == 0;
  }

  bool operator!=(const BitSet& right) const {
    return !operator==(right);
  }

  BitSet& operator&=(const BitSet& right) {
    for (IndexType i = 0; i < storage_array_size(); ++i) {
      value_[i] &= right.value_[i];
    }

    return *this;
  }

  BitSet& operator|=(const BitSet& right) {
    for (IndexType i = 0; i < storage_array_size(); ++i) {
      value_[i] |= right.value_[i];
    }

    return *this;
  }

  BitSet& operator^=(const BitSet& right) {
    for (IndexType i = 0; i < storage_array_size(); ++i) {
      value_[i] ^= right.value_[i];
    }

    return *this;
  }

private:
  using StorageType = MemSize;

  constexpr static MemSize bits_per_storage() {
    return sizeof(StorageType) * 8;
  }

  constexpr static MemSize storage_array_size() {
    return (BitCount - 1) / bits_per_storage() + 1;
  }

  StorageType value_[storage_array_size()] = {};
};

template <MemSize BitCount>
inline BitSet<BitCount> operator&(const BitSet<BitCount>& left, const BitSet<BitCount>& right) {
  BitSet<BitCount> result = left;
  return (result &= right);
}

template <MemSize BitCount>
inline BitSet<BitCount> operator|(const BitSet<BitCount>& left, const BitSet<BitCount>& right) {
  BitSet<BitCount> result = left;
  return (result |= right);
}

template <MemSize BitCount>
inline BitSet<BitCount> operator^(const BitSet<BitCount>& left, const BitSet<BitCount>& right) {
  BitSet<BitCount> result = left;
  return (result ^= right);
}

}  // namespace nu
