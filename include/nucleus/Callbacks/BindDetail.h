
#ifndef NUCLEUS_CALLBACKS_BIND_DETAIL_H_
#define NUCLEUS_CALLBACKS_BIND_DETAIL_H_

#include <type_traits>
#include <utility>

#include "nucleus/Callbacks/CallbackForward.h"
#include "nucleus/Callbacks/ExtractReturnType.h"
#include "nucleus/Callbacks/FunctorTraits.h"
#include "nucleus/Callbacks/MakeBindStateType.h"
#include "nucleus/Callbacks/MakeFunctionType.h"
#include "nucleus/Callbacks/TypeList.h"

namespace nu {

namespace detail {

// BindTypeHelper<>

// Extracts necessary type info from `Functor` and `BoundArgs`.  Used to implement `MakeUnboundRunType` and `bind`.
template <typename Functor, typename... BoundArgs>
struct BindTypeHelper {
  static constexpr size_t numBound = sizeof...(BoundArgs);
  using FunctorTraits = MakeFunctorTraits<Functor>;

  // Example:
  //   When `Functor` is `double (Foo::*)(int, const std::string&)`, and `BoundArgs` is a template pack of
  //   `Foo*` and `int16_t`:
  //    - RunType is `double(Foo*, int, const std::string&)`
  //    - ReturnType is `double`
  //    - RunParamsList is `TypeList<Foo*, int, const std::string&>`
  //    - BoundParamsList is `TypeList<Foo*, int>`
  //    - UnboundParamsList is `TypeList<const std::string&>`
  //    - BoundArgsList is `TypeList<Foo*, int16_t>`
  //    - UnboundRunType is `double(const std::string&)`

  using RunType = typename FunctorTraits::RunType;
  using ReturnType = ExtractReturnType<RunType>;

  using RunParamsList = ExtractArgs<RunType>;
  using BoundParamsList = TakeTypeListItem<numBound, RunParamsList>;
  using UnboundParamsList = DropTypeListItem<numBound, RunParamsList>;

  using BoundArgsList = TypeList<BoundArgs...>;

  using UnboundRunType = MakeFunctionType<ReturnType, UnboundParamsList>;
};

}  // namespace detail

// MakeUnboundRunType<>

// Returns a `RunType` of bound functor.
// e.g. `MakeUnboundRunType<R(A, B, C)>, A, B>` is evaluated to `R(C)`.
template <typename Functor, typename... BoundArgs>
using MakeUnboundRunType = typename detail::BindTypeHelper<Functor, BoundArgs...>::UnboundRunType;

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_BIND_DETAIL_H_
