
#ifndef NUCLEUS_CALLBACKS_INVOKER_H_
#define NUCLEUS_CALLBACKS_INVOKER_H_

#include "nucleus/Callbacks/FunctorTraits.h"
#include "nucleus/Callbacks/PassingTraits.h"

namespace nu {

namespace detail {

// InvokeHelper<>

template <typename R>
struct InvokeHelper {
  template <typename Functor, typename... RunArgs>
  static inline R makeItSo(Functor&& functor, RunArgs&&... args) {
    using Traits = MakeFunctorTraits<Functor>;
    return Traits::invoke(std::forward<Functor>(functor), std::forward<RunArgs>(args)...);
  }
};

// unwrap()

template <typename>
struct BindUnwrapTraits {
  template <typename T>
  static T&& unwrap(T&& o) {
    return std::forward<T>(o);
  }
};

#if 0
template <typename T>
struct BindUnwrapTraits<internal::UnretainedWrapper<T>> {
  static T* Unwrap(const internal::UnretainedWrapper<T>& o) { return o.get(); }
};

template <typename T>
struct BindUnwrapTraits<internal::ConstRefWrapper<T>> {
  static const T& Unwrap(const internal::ConstRefWrapper<T>& o) {
    return o.get();
  }
};

template <typename T>
struct BindUnwrapTraits<internal::RetainedRefWrapper<T>> {
  static T* Unwrap(const internal::RetainedRefWrapper<T>& o) { return o.get(); }
};

template <typename T>
struct BindUnwrapTraits<internal::OwnedWrapper<T>> {
  static T* Unwrap(const internal::OwnedWrapper<T>& o) { return o.get(); }
};

template <typename T>
struct BindUnwrapTraits<internal::PassedWrapper<T>> {
  static T Unwrap(const internal::PassedWrapper<T>& o) { return o.Take(); }
};
#endif  // 0

template <typename T>
using Unwrapper = BindUnwrapTraits<std::decay_t<T>>;

template <typename T>
decltype(auto) unwrap(T&& o) {
  return Unwrapper<T>::unwrap(std::forward<T>(o));
}

// Invoker<>

template <typename StorageType, typename UnboundRunType>
struct Invoker;

template <typename StorageType, typename R, typename... UnboundArgs>
struct Invoker<StorageType, R(UnboundArgs...)> {
  static R run(BindStateBase* base, PassingTraitsType<UnboundArgs>... unboundArgs) {
    const StorageType* storage = static_cast<StorageType*>(base);
    static constexpr size_t numBoundArgs = std::tuple_size<decltype(storage->m_boundArgs)>::value;
    return runImpl(storage->m_functor, storage->m_boundArgs,
                   std::make_index_sequence<numBoundArgs>(),
                   std::forward<UnboundArgs>(unboundArgs)...);
  }

private:
  template <typename Functor, typename BoundArgsTuple, size_t... Indices>
  static inline R runImpl(Functor&& functor, BoundArgsTuple&& bound,
                          std::index_sequence<Indices...>, UnboundArgs&&... unboundArgs) {
    static constexpr bool isMethod = MakeFunctorTraits<Functor>::isMethod;

    return InvokeHelper<R>::makeItSo(
        std::forward<Functor>(functor),
        unwrap(std::get<Indices>(std::forward<BoundArgsTuple>(bound)))...,
        std::forward<UnboundArgs>(unboundArgs)...);
  }
};

}  //  namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_INVOKER_H_
