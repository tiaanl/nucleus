#ifndef NUCLEUS_TESTING_DYNAMIC_ARRAY_TESTING_H_
#define NUCLEUS_TESTING_DYNAMIC_ARRAY_TESTING_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Testing.h"

namespace nu {

template <typename T, typename Compare>
struct Assertions<DynamicArray<T>, Compare> : BaseAssertions<DynamicArray<T>, Compare> {
  using SizeType = typename DynamicArray<T>::SizeType;

  explicit Assertions(const DynamicArray<T>& value) : BaseAssertions{value} {}

  void hasSizeOf(typename DynamicArray<T>::SizeType size) {
    Compare::equal(value.size(), size);
  }

  void isEmpty() {
    Compare::equal(value.isEmpty(), true);
  }

  Assertions<T, Compare> element(SizeType index) {
    return Assertions<T, Compare>(value[index]);
  }
};

}  // namespace nu

#endif  // NUCLEUS_TESTING_DYNAMIC_ARRAY_TESTING_H_
