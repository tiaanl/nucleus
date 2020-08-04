#if !defined(NUCLEUS_CALLBACKS_CALLBACK_H_)
#define NUCLEUS_CALLBACKS_CALLBACK_H_

#include "nucleus/Logging.h"

namespace nu {

class Callback {
public:
  using FunctorType = void(*)();

  explicit Callback(FunctorType&& functor) : m_functor{functor} {}

  template <typename... Args>
  auto invoke(Args&&... args) -> void {
    LOG(Info) << "Invoke";
  }

private:
  FunctorType m_functor;
};

template <typename Functor>
auto bind(Functor&& functor) -> Callback {
  return Callback{functor};
}

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_CALLBACK_H_
