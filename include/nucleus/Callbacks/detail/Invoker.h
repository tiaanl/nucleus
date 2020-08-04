#if !defined(NUCLEUS_CALLBACKS_DETAIL_INVOKER_H_)
#define NUCLEUS_CALLBACKS_DETAIL_INVOKER_H_

#include "nucleus/Callbacks/detail/TypeHelpers.h"

namespace nu::detail {

template <typename StorageType, typename UnboundRunType>
struct Invoker;

template <typename BoundState, typename ReturnType, typename... UnboundArguments>
struct Invoker<BoundState, ReturnType(UnboundArguments...)> {
  static ReturnType run(BoundStateBase* boundStateBase,
                        PassingType<UnboundArguments>... unboundArguments) {
    const BoundState* boundState = static_cast<BoundState*>(boundStateBase);
    static constexpr size_t boundArgumentsCount =
        std::tuple_size<decltype(boundState->m_boundArguments)>::value;
    return runImpl(boundState->m_functor, boundState->m_boundArguments,
                   std::make_index_sequence<boundArgumentsCount>(),
                   std::forward<UnboundArguments>(unboundArguments)...);
  }

private:
  template <typename Functor, typename BoundArgumentsTuple, size_t... Indices>
  static inline ReturnType runImpl(Functor&& functor, BoundArgumentsTuple&& boundArguments,
                                   std::index_sequence<Indices...>,
                                   UnboundArguments&&... unboundArguments) {
    static constexpr bool isMethod = MakeFunctorTraits<Functor>::IsMethod;

    using FunctorTraits = MakeFunctorTraits<Functor>;
    return FunctorTraits::invoke(
        std::forward<Functor>(functor),
        unwrap(std::get<Indices>(std::forward<BoundArgumentsTuple>(boundArguments)))...,
        std::forward<UnboundArguments>(unboundArguments)...);
  }
};

}  // namespace nu::detail

#endif  // NUCLEUS_CALLBACKS_DETAIL_INVOKER_H_
