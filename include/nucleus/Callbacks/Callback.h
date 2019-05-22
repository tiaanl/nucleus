
#ifndef NUCLEUS_CALLBACKS_CALLBACK_H_
#define NUCLEUS_CALLBACKS_CALLBACK_H_

#include "nucleus/Callbacks/CallbackDetail.h"
#include "nucleus/Callbacks/CallbackForward.h"
#include "nucleus/Callbacks/PassingTraits.h"

namespace nu {

template <typename Ret, typename... Args>
class Callback<Ret(Args...)> : public detail::CallbackBase {
public:
  using RunType = Ret(Args...);
  using PolymorphicInvoke = Ret (*)(detail::BindStateBase*, detail::PassingTraitsType<Args>...);

  constexpr Callback() = default;

  explicit Callback(detail::BindStateBase* bindState) : detail::CallbackBase(bindState) {}

  bool equals(const Callback& other) const {
    return equalsBase(other);
  }

  Ret run(Args... args) const {
    PolymorphicInvoke invoker = reinterpret_cast<PolymorphicInvoke>(getPolymorphicInvoke());
    return invoker(m_bindState.get(), std::forward<Args>(args)...);
  }
};

using Closure = Callback<void()>;

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_CALLBACK_H_
