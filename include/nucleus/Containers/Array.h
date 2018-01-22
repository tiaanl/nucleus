
#ifndef NUCLEUS_CONTAINERS_ARRAY_H_
#define NUCLEUS_CONTAINERS_ARRAY_H_

#include "nucleus/Types.h"

namespace nu {

template <typename T, USize Size>
class Array {
public:
  using SizeType = USize;

  Array() {}

  const T& operator[](SizeType index) const {
    return m_data[index];
  }

  T& operator[](SizeType index) {
    return m_data[index];
  }

  SizeType getSize() const {
    return Size;
  }

  const T* getData() const {
    return m_data;
  }

  T* getData() {
    return m_data;
  }

private:
  T m_data[Size];
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_ARRAY_H_
