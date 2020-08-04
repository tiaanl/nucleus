#if !defined(NUCLEUS_CALLBACKS_DETAIL_BOUND_STATE_H_)
#define NUCLEUS_CALLBACKS_DETAIL_BOUND_STATE_H_

#include <tuple>

#include "nucleus/RefCounted.h"

namespace nu {

namespace detail {

class BoundStateBase : public RefCounted<BoundStateBase> {
public:
  using InvokeFunctionStorage = void (*)();

protected:
  friend struct DefaultRefCountedTraits<BoundStateBase>;
  friend class CallbackBase;

  explicit BoundStateBase(InvokeFunctionStorage invokeFunction)
    : m_invokeFunction{invokeFunction} {}

  ~BoundStateBase() = default;

  InvokeFunctionStorage m_invokeFunction;
};

}  // namespace detail

template <typename Functor, typename... BoundArguments>
class BoundState : public detail::BoundStateBase {
public:
  template <typename ForwardFunctor, typename... ForwardBoundArguments>
  static BoundState* create(BoundStateBase::InvokeFunctionStorage invokeFunction,
                            ForwardFunctor&& functor, ForwardBoundArguments&&... boundArguments) {
    return new BoundState{invokeFunction, std::forward<ForwardFunctor>(functor),
                          std::forward<ForwardBoundArguments>(boundArguments)...};
  }

private:
  template <typename ForwardFunctor, typename... ForwardBoundArguments>
  explicit BoundState(BoundStateBase::InvokeFunctionStorage invokeFunction,
                      ForwardFunctor&& functor, ForwardBoundArguments&&... boundArguments)
    : BoundStateBase{invokeFunction},
      m_functor{std::forward<ForwardFunctor>(functor)},
      m_boundArguments{std::forward<ForwardBoundArguments>(boundArguments)...} {}

  ~BoundState() = default;

public:
  Functor m_functor;
  std::tuple<BoundArguments...> m_boundArguments;
};

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_DETAIL_BOUND_STATE_H_
