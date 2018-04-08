
#ifndef NUCLEUS_CALLBACKS_BIND_H_
#define NUCLEUS_CALLBACKS_BIND_H_

#include "nucleus/Callbacks/BindDetail.h"
#include "nucleus/Callbacks/Invoker.h"

namespace nu {

template <typename Functor, typename... Args>
inline Callback<MakeUnboundRunType<Functor, Args...>> bind(Functor&& functor, Args&&... args) {
  using BindState = detail::MakeBindStateType<Functor, Args...>;
  using UnboundRunType = MakeUnboundRunType<Functor, Args...>;
  using Invoker = detail::Invoker<BindState, UnboundRunType>;
  using CallbackType = Callback<UnboundRunType>;

  // Store the invoke func into PolymorphicInvoke before casting it to InvokeFuncStorage, so that we
  // can ensure its type matches to PolymorphicInvoke, to which CallbackType will cast back.
  using PolymorphicInvoke = typename CallbackType::PolymorphicInvoke;
  // TODO(tiaan): There shouldn't be a reinterpret_cast here
  PolymorphicInvoke invokeFunc = reinterpret_cast<PolymorphicInvoke>(&Invoker::run);

  using InvokeFuncStorage = detail::BindStateBase::InvokeFuncStorage;
  return CallbackType(new BindState(reinterpret_cast<InvokeFuncStorage>(invokeFunc),
                                    std::forward<Functor>(functor), std::forward<Args>(args)...));
}

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_BIND_H_
