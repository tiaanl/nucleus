
#ifndef NUCLEUS_CALLBACKS_MAKE_FUNCTION_TYPE_H_
#define NUCLEUS_CALLBACKS_MAKE_FUNCTION_TYPE_H_

#include "nucleus/Callbacks/TypeList.h"

namespace nu {

namespace detail {

template <typename R, typename ArgList>
struct MakeFunctionTypeImpl;

template <typename R, typename... Args>
struct MakeFunctionTypeImpl<R, TypeList<Args...>> {
  typedef R Type(Args...);
};

// A type-level function that constructs a function type that has `R` as its return type and has `TypeList`s items as its arguments.
template <typename R, typename ArgList>
using MakeFunctionType = typename MakeFunctionTypeImpl<R, ArgList>::Type;

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_MAKE_FUNCTION_TYPE_H_