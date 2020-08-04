#if !defined(NUCLEUS_CALLBACKS_DETAIL_EXTRACT_TYPES_H_)
#define NUCLEUS_CALLBACKS_DETAIL_EXTRACT_TYPES_H_

#include "nucleus/Callbacks/detail/TypeList.h"

namespace nu::detail {

// ExtractArguments & ExtractReturnType

template <typename Signature>
struct ExtractArgumentsImpl;

template <typename InReturnType, typename... InArguments>
struct ExtractArgumentsImpl<InReturnType(InArguments...)> {
  using ReturnType = InReturnType;
  using ArgumentList = TypeList<InArguments...>;
};

template <typename Signature>
using ExtractArguments = typename ExtractArgumentsImpl<Signature>::ArgumentList;

template <typename Signature>
using ExtractReturnType = typename ExtractArgumentsImpl<Signature>::ReturnType;

// ExtractCallableRunType

template <typename Callable, typename Signature = decltype(&Callable::operator())>
struct ExtractCallableRunTypeImpl;

template <typename Callable, typename ReturnType, typename... Arguments>
struct ExtractCallableRunTypeImpl<Callable, ReturnType (Callable::*)(Arguments...)> {
  using Type = ReturnType(Arguments...);
};

template <typename Callable, typename ReturnType, typename... Arguments>
struct ExtractCallableRunTypeImpl<Callable, ReturnType (Callable::*)(Arguments...) const> {
  using Type = ReturnType(Arguments...);
};

template <typename Callable>
using ExtractCallableRunType = typename ExtractCallableRunTypeImpl<Callable>::Type;

// PassingType

template <typename T>
using PassingType = std::conditional_t<std::is_scalar<T>::value, T, T&&>;

}  // namespace nu::detail

#endif  // NUCLEUS_CALLBACKS_DETAIL_EXTRACT_TYPES_H_
