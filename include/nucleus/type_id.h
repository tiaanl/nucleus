#pragma once

#include <nucleus/types.h>

namespace nu {

using TypeId = MemSize;

namespace detail {

template <typename T>
struct TypeIdStorage {
  static constexpr TypeId get() {
    static constexpr char id = 0;
    return reinterpret_cast<TypeId>(&id);
  }
};

}  // namespace detail

// Return a process wide unique identifier for the specified type.
template <typename T>
NU_NEVER_INLINE TypeId type_id_for() {
  return detail::TypeIdStorage<T>::get();
}

}  // namespace nu
