
#ifndef NUCLEUS_CALLBACKS_PASSING_TRAITS_H_
#define NUCLEUS_CALLBACKS_PASSING_TRAITS_H_

#include <type_traits>

namespace nu {

namespace detail {

template <typename T, bool IsScalar = std::is_scalar<T>::value>
struct PassingTraits;

template <typename T>
struct PassingTraits<T, false> {
  using Type = T&&;
};

template <typename T>
struct PassingTraits<T, true> {
  using Type = T;
};

template <typename T>
using PassingTraitsType = typename PassingTraits<T>::Type;

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_PASSING_TRAITS_H_
