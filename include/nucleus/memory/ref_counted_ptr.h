#pragma once

#include "nucleus/ref_counted.h"

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
