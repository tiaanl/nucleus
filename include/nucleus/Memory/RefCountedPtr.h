
#ifndef NUCLEUS_REF_COUNTED_PTR_H_
#define NUCLEUS_REF_COUNTED_PTR_H_

#include "nucleus/RefCounted.h"

namespace nu {

namespace detail {

template <typename T>
struct DeletingRefCountedTraits {
  static void destruct(const T* ptr) {
      delete ptr;
  }
};

}  // namespace detail

template <typename T>
using RefCountedPtr = RefCounted<T, detail::DeletingRefCountedTraits<T>>;

}  // namespace nu

#endif  // NUCLEUS_REF_COUNTED_PTR_H_
