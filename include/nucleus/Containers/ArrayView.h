#pragma once

#include "nucleus/Logging.h"
#include "nucleus/Types.h"

namespace nu {

template <typename T>
class ArrayView {
public:
  constexpr ArrayView() : m_data{nullptr}, m_size{0} {}

  constexpr ArrayView(const T* data, MemSize size) : m_data{data}, m_size{size} {}

  template <typename ContainerType>
  constexpr ArrayView(const ContainerType& container)
    : m_data(container.data()), m_size(container.size()) {}

  constexpr const T* data() const {
    return m_data;
  }

  constexpr MemSize size() const {
    return m_size;
  }

  constexpr bool empty() const {
    return m_size == 0;
  }

  const T& operator[](MemSize index) const {
    DCHECK(index < m_size);
    return m_data[index];
  };

  T& operator[](MemSize index) {
    DCHECK(index < m_size);
    return m_data[index];
  };

  T* begin() const {
    return m_data;
  }

  T* begin() {
    return m_data;
  }

  T* end() {
    return m_data + m_size;
  }

  T* end() const {
    return m_data + m_size;
  }

private:
  const T* m_data;
  MemSize m_size;
};

}  // namespace nu
