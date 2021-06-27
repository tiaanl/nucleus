#pragma once

#include <algorithm>

#include "nucleus/logging.h"
#include "nucleus/types.h"

namespace nu {

template <typename T, MemSize Size>
struct StaticArray {
  using ElementType = T;
  using SizeType = MemSize;

  using Iterator = T*;
  using ConstIterator = const T*;

  ElementType elements[Size];

  auto fill(const ElementType& value) -> void {
    std::fill_n(elements, Size, value);
  }

  auto swap(StaticArray& other) {
    std::swap_ranges(elements, elements + Size, other.elements);
  }

  constexpr auto begin() -> Iterator {
    return elements;
  }

  constexpr auto begin() const -> ConstIterator {
    return elements;
  }

  constexpr auto end() -> Iterator {
    return elements + Size;
  }

  constexpr auto end() const -> ConstIterator {
    return elements + Size;
  }

  constexpr auto size() const -> SizeType {
    return Size;
  }

  constexpr auto capacity() const -> SizeType {
    return Size;
  }

  constexpr auto empty() const -> bool {
    return false;
  }

  constexpr auto at(SizeType index) -> ElementType& {
    DCHECK(index < Size) << "Index out of bounds";

    return elements[index];
  }

  constexpr auto at(SizeType index) const -> const ElementType& {
    DCHECK(index < Size) << "Index out of bounds";

    return elements[index];
  }

  constexpr auto operator[](SizeType index) -> ElementType& {
    return at(index);
  }

  constexpr auto operator[](SizeType index) const -> const ElementType& {
    return at(index);
  }

  constexpr auto data() -> ElementType* {
    return elements;
  }

  constexpr auto data() const -> const ElementType* {
    return elements;
  }
};

template <typename T, MemSize Size>
auto operator==(const StaticArray<T, Size>& left, const StaticArray<T, Size>& right) -> bool {
  return std::equal(left.begin(), left.end(), right.begin());
}

template <typename T, MemSize Size>
auto operator!=(const StaticArray<T, Size>& left, const StaticArray<T, Size>& right) -> bool {
  return !(left == right);
}

template <typename T, MemSize Size>
auto operator<(const StaticArray<T, Size>& left, const StaticArray<T, Size>& right) -> bool {
  return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
}

template <typename T, MemSize Size>
auto operator>(const StaticArray<T, Size>& left, const StaticArray<T, Size>& right) -> bool {
  return right < left;
}

template <typename T, MemSize Size>
auto operator<=(const StaticArray<T, Size>& left, const StaticArray<T, Size>& right) -> bool {
  return !(right < left);
}

template <typename T, MemSize Size>
auto operator>=(const StaticArray<T, Size>& left, const StaticArray<T, Size>& right) -> bool {
  return !(left < right);
}

}  // namespace nu
