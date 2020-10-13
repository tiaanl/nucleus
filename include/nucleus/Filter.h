#ifndef NUCLEUS_FILTER_H_
#define NUCLEUS_FILTER_H_

#include "nucleus/Containers/DynamicArray.h"

namespace nu {

template <typename T, typename F>
auto filter(const DynamicArray<T>& container, F predicate) -> DynamicArray<T> {
  DynamicArray<T> result;

  for (auto& e : container) {
    if (predicate(e)) {
      result.pushBack(e);
    }
  }

  return result;
}

}  // namespace nu

#endif  // NUCLEUS_FILTER_H_
