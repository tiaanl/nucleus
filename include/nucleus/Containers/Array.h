
#ifndef NUCLEUS_CONTAINERS_ARRAY_H_
#define NUCLEUS_CONTAINERS_ARRAY_H_

#include "nucleus/Types.h"

namespace nu {

template <typename T, USize Size>
class Array {
public:
  Array() {}

private:
  T m_data[Size];
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_ARRAY_H_
