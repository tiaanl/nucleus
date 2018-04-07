
#ifndef NUCLEUS_CALLBACKS_CALLBACK_H_
#define NUCLEUS_CALLBACKS_CALLBACK_H_

#include "nucleus/Callbacks/CallbackDetail.h"
#include "nucleus/Callbacks/CallbackForward.h"

namespace nu {

template <typename R, typename... Args>
class Callback<R(Args...)> : public detail::CallbackBase {
public:
  using RunType = R(Args...);
  using PolymorphicInvoke = R (*)(detail::BindStateBase*, Args&&...);

  constexpr Callback() = default;

  explicit Callback(detail::BindStateBase* bindState) : detail::CallbackBase(bindState) {}

  bool equals(const Callback& other) const {
    return equalsBase(other);
  }

  R run(Args... args) const {
    PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(getPolymorphicInvoke());
    return f(m_bindState.get(), std::forward<Args>(args)...);
  }
};

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_CALLBACK_H_
