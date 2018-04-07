
#ifndef NUCLEUS_CALLBACKS_INVOKER_H_
#define NUCLEUS_CALLBACKS_INVOKER_H_

#include "nucleus/Callbacks/FunctorTraits.h"
#include "nucleus/Callbacks/PassingTraits.h"

namespace nu {

namespace detail {

template <typename StorageType, typename UnboundRunType>
struct Invoker;

template <typename StorageType, typename R, typename... UnboundArgs>
struct Invoker<StorageType, R(UnboundArgs...)> {
  static R run(BindStateBase* base, PassingTraitsType<UnboundArgs>... unboundArgs) {
    const StorageType* storage = static_cast<StorageType*>(base);
    static constexpr size_t numBoundArgs = std::tuple_size<decltype(storage->m_boundArgs)>::value;
    return runImpl(storage->m_functor, storage->m_boundArgs, std::make_index_sequence<numBoundArgs>(),
                   std::forward<UnboundArgs>(unboundArgs)...);
  }

private:
  template <typename Functor, typename BoundArgsTuple, size_t... Indices>
  static inline R runImpl(Functor&& functor, BoundArgsTuple&& bound, std::index_sequence<Indices...>,
                          UnboundArgs&&... unboundArgs) {
    static constexpr bool isMethod = MakeFunctorTraits<Functor>::isMethod;

    using DecayedArgsTuple = std::decay_t<BoundArgsTuple>;
    static constexpr bool isWeakCall = IsWeakMethod<isMethod, std::tuple_element_t<Indices, DecayedArgsTuple>...>();

    return InvokeHelper<isWeakCall, R>::makeItSo(std::forward<Functor>(functor),
                                                 unwrap(std::get<Indices>(std::forward<BoundArgsTuple>(bound)))...,
                                                 std::forward<UnboundArgs>(unboundArgs)...);
  }
};

}  //  namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_INVOKER_H_