#pragma once

#include "nucleus/logging.h"
#include "nucleus/types.h"

namespace nu {

template <typename T>
class ArrayView {
public:
  constexpr ArrayView() : data_{nullptr}, size_{0} {}

  constexpr ArrayView(const T* data, MemSize size) : data_{data}, size_{size} {}

  template <typename ContainerType>
  constexpr ArrayView(const ContainerType& container)
    : data_(container.data()), size_(container.size()) {}

  constexpr const T* data() const {
    return data_;
  }

  constexpr MemSize size() const {
    return size_;
  }

  constexpr bool empty() const {
    return size_ == 0;
  }

  const T& operator[](MemSize index) const {
    DCHECK(index < size_);
    return data_[index];
  };

  T& operator[](MemSize index) {
    DCHECK(index < size_);
    return data_[index];
  };

  T* begin() const {
    return data_;
  }

  T* begin() {
    return data_;
  }

  T* end() {
    return data_ + size_;
  }

  T* end() const {
    return data_ + size_;
  }

private:
  const T* data_;
  MemSize size_;
};

}  // namespace nu
