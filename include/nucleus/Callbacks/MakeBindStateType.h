
#ifndef NUCLEUS_CALLBACKS_MAKE_BIND_STATE_TYPE_H_
#define NUCLEUS_CALLBACKS_MAKE_BIND_STATE_TYPE_H_

#include "nucleus/Callbacks/BindState.h"

namespace nu {

namespace detail {

template <bool IsMethod, typename Functor, typename... BoundArgs>
struct MakeBindStateTypeImpl;

template <typename Functor, typename... BoundArgs>
struct MakeBindStateTypeImpl<false, Functor, BoundArgs...> {
#if 0
  static_assert(!HasRefCountedTypeAsRawPtr<std::decay_t<BoundArgs>...>::value,
                "A parameter is a refcounted type and needs Ref.");
#endif  // 0
  using Type = BindState<std::decay_t<Functor>, std::decay_t<BoundArgs>...>;
};

template <typename Functor>
struct MakeBindStateTypeImpl<true, Functor> {
  using Type = BindState<std::decay_t<Functor>>;
};

template <typename Functor, typename Receiver, typename... BoundArgs>
struct MakeBindStateTypeImpl<true, Functor, Receiver, BoundArgs...> {
private:
  using DecayedReceiver = std::decay_t<Receiver>;

  static_assert(!std::is_array<std::remove_reference_t<Receiver>>::value,
                "First bound argument to a method cannot be an array.");
#if 0
  static_assert(!std::is_pointer<DecayedReceiver>::value ||
                    IsRefCountedType<std::remove_pointer_t<DecayedReceiver>>::value,
                "Receivers may not be raw pointers. If using a raw pointer here is safe and has no lifetime concerns, "
                "use base::Unretained() and document why it's safe.");
#endif  // 0
#if 0
  static_assert(!HasRefCountedTypeAsRawPtr<std::decay_t<BoundArgs>...>::value,
                "A parameter is a refcounted type and needs scoped_refptr.");
#endif  // 0

public:
  using Type = BindState<
      std::decay_t<Functor>,
      std::conditional_t<std::is_pointer<DecayedReceiver>::value,
                         ScopedRefPtr<std::remove_pointer_t<DecayedReceiver>>, DecayedReceiver>,
      std::decay_t<BoundArgs>...>;
};

template <typename Functor, typename... BoundArgs>
using MakeBindStateType = typename MakeBindStateTypeImpl<MakeFunctorTraits<Functor>::isMethod,
                                                         Functor, BoundArgs...>::Type;

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_MAKE_BIND_STATE_TYPE_H_
