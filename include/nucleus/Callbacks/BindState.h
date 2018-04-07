
#ifndef NUCLEUS_CALLBACKS_BIND_STATE_H_
#define NUCLEUS_CALLBACKS_BIND_STATE_H_

#include <tuple>

#include "nucleus/Callbacks/CallbackDetail.h"

namespace nu {

namespace detail {

// This stores all the state passed into `bind()`.
template <typename Functor, typename... BoundArgs>
struct BindState : BindStateBase {
  template <typename ForwardFunctor, typename... ForwardBoundArgs>
  explicit BindState(BindStateBase::InvokeFuncStorage invokeFunc, ForwardFunctor&& functor,
                     ForwardBoundArgs&&... boundArgs)
    : BindStateBase(invokeFunc), m_functor(std::forward<ForwardFunctor>(functor)),
      m_boundArgs(std::forward<ForwardBoundArgs>(bound_args)...) {
    DCHECK(!isNull(m_functor));
  }

  Functor m_functor;
  std::tuple<BoundArgs...> m_boundArgs;

private:
  ~BindState() = default;

#if 0
  static void destroy(const BindStateBase* self) {
    delete static_cast<const BindState*>(self);
  }
#endif  // 0
};

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_BIND_STATE_H_
