#if !defined(NUCLEUS_CALLBACKS_DETAIL_FUNCTOR_TRAITS_H_)
#define NUCLEUS_CALLBACKS_DETAIL_FUNCTOR_TRAITS_H_

#include "nucleus/Callbacks/detail/TypeHelpers.h"

namespace nu::detail {

template <typename Functor, typename SFINAE = void>
struct FunctorTraits;

template <typename Functor, typename SFINAE>
struct FunctorTraits;

template <typename Functor>
struct FunctorTraits<Functor, std::enable_if_t<std::is_empty<Functor>::value>> {
  using RunType = ExtractCallableRunType<Functor>;
  static constexpr bool IsMethod = false;
  static constexpr bool IsNullable = false;

  template <typename RunFunctor, typename... RunArguments>
  static ExtractReturnType<RunType> invoke(RunFunctor&& functor, RunArguments... arguments) {
    return std::forward<RunFunctor>(functor)(std::forward<RunArguments>(arguments)...);
  }
};

// For functions.

template <typename ReturnType, typename... Arguments>
struct FunctorTraits<ReturnType (*)(Arguments...)> {
  using RunType = ReturnType(Arguments...);
  static constexpr bool IsMethod = false;
  static constexpr bool IsNullable = true;

  template <typename RunFunctor, typename... RunArguments>
  static ReturnType invoke(RunFunctor&& functor, RunArguments&&... arguments) {
    return std::forward<RunFunctor>(functor)(std::forward<RunArguments>(arguments)...);
  }
};

#if 0

#endif  // 0

template <typename Functor>
using MakeFunctorTraits = FunctorTraits<std::decay_t<Functor>>;

}  // namespace nu::detail

#endif  // NUCLEUS_CALLBACKS_DETAIL_FUNCTOR_TRAITS_H_
