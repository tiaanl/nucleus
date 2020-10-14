#ifndef NUCLEUS_CONTAINERS_ARRAY_VIEW_H_
#define NUCLEUS_CONTAINERS_ARRAY_VIEW_H_

#include "nucleus/Types.h"

namespace nu {

template <typename T>
class ArrayView {
public:
  constexpr ArrayView() : m_data{nullptr}, m_size{0} {}

  constexpr ArrayView(T* data, MemSize size) : m_data{data}, m_size{size} {}

  template <typename ContainerType>
  constexpr ArrayView(const ContainerType& container) : m_data(container.data()), m_size(container.size()) {}

  constexpr const T* data() const {
    return m_data;
  }

  constexpr MemSize size() const {
    return m_size;
  }

private:
  const T* m_data;
  MemSize m_size;
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_ARRAY_VIEW_H_
