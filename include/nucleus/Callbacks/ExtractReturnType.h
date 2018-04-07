
#ifndef NUCLEUS_CALLBACKS_EXTRACT_RETURN_TYPE_H_
#define NUCLEUS_CALLBACKS_EXTRACT_RETURN_TYPE_H_

#include "nucleus/Callbacks/TypeList.h"

namespace nu {

namespace detail {

template <typename Signature>
struct ExtractArgsImpl;

template <typename R, typename... Args>
struct ExtractArgsImpl<R(Args...)> {
  using ReturnType = R;
  using ArgsList = TypeList<Args...>;
};

// Extracts function arguments into a TypeList.
// e.g. ExtractArgs<R(A, B, C)> is evaluated to TypeList<A, B, C>.
template <typename Signature>
using ExtractArgs = typename ExtractArgsImpl<Signature>::ArgsList;

// Extracts the return type of a function.
// e.g. ExtractReturnType<R(A, B, C)> is evaluated to R.
template <typename Signature>
using ExtractReturnType = typename ExtractArgsImpl<Signature>::ReturnType;

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_EXTRACT_RETURN_TYPE_H_
