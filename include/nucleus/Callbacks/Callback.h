#if !defined(NUCLEUS_CALLBACKS_CALLBACK_H_)
#define NUCLEUS_CALLBACKS_CALLBACK_H_

#include <tuple>
#include <type_traits>

#include "nucleus/Callbacks/detail/BoundState.h"
#include "nucleus/Callbacks/detail/FunctorTraits.h"
#include "nucleus/Callbacks/detail/Invoker.h"
#include "nucleus/Logging.h"
#include "nucleus/Memory/ScopedRefPtr.h"

namespace nu {

namespace detail {

// MakeFunctionType

template <typename ReturnType, typename ArgumentsTypeList>
struct MakeFunctionTypeImpl;

template <typename ReturnType, typename... Arguments>
struct MakeFunctionTypeImpl<ReturnType, TypeList<Arguments...>> {
  using Type = ReturnType(Arguments...);
};

template <typename ReturnType, typename ArgumentsTypeList>
using MakeFunctionType = typename MakeFunctionTypeImpl<ReturnType, ArgumentsTypeList>::Type;

// MakeBoundStateType

template <bool IsMethod, typename Functor, typename... BoundArguments>
struct MakeBoundStateTypeImpl;

template <typename Functor, typename... BoundArguments>
struct MakeBoundStateTypeImpl<false, Functor, BoundArguments...> {
  // using Type = BoundState<std::decay_t<Functor>, std::decay_t<BoundArguments>...>;
  using Type = BoundState<Functor>;
};

template <typename Functor>
struct MakeBoundStateTypeImpl<true, Functor> {
  using Type = BoundState<std::decay_t<Functor>>;
};

template <typename Functor, typename ObjectType, typename... BoundArguments>
struct MakeBoundStateTypeImpl<true, Functor, ObjectType, BoundArguments...> {
  using Type =
      BoundState<std::decay_t<Functor>, std::decay_t<ObjectType>, std::decay_t<BoundArguments>...>;
};

template <typename Functor, typename... BoundArguments>
using MakeBoundStateType = typename MakeBoundStateTypeImpl<MakeFunctorTraits<Functor>::IsMethod,
                                                           Functor, BoundArguments...>::Type;

template <typename Functor, typename... BoundArguments>
struct BindHelper {
  static constexpr size_t NumBound = sizeof...(BoundArguments);
  using FunctorTraits = MakeFunctorTraits<Functor>;

  // Functor:               double (Foo::*)(I32, const DynamicString&)
  // BoundArguments:        Foo*, I16
  //
  // RunType:               double(Foo*, I32, const DynamicString&)
  // ReturnType:            double
  // RunParametersList:     TypeList<Foo*, I32, const DynamicString&>
  // BoundParametersList:   TypeList<Foo*, I32>
  // UnboundParametersList: TypeList<const DynamicString&>
  // BoundArgumentsList:    TypeList<Foo*, I16>
  // UnboundRunType:        double(const DynamicString&)

  using RunType = typename FunctorTraits::RunType;
  using ReturnType = ExtractReturnType<RunType>;

  using RunParametersList = ExtractArguments<RunType>;
  using BoundParametersList = TakeTypeListItem<NumBound, RunParametersList>;
  using UnboundParametersList = DropTypeListItem<NumBound, RunParametersList>;

  using BoundArgumentsList = TypeList<BoundArguments...>;

  using UnboundRunType = MakeFunctionType<ReturnType, UnboundParametersList>;
};

template <typename Functor, typename... BoundArguments>
using MakeUnboundRunType = typename BindHelper<Functor, BoundArguments...>::UnboundRunType;

class CallbackBase {
public:
  auto null() const -> bool {
    return !m_boundState;
  }

  explicit operator bool() const {
    return !null();
  }

  auto reset() -> void {
    // TODO(tiaanl): Implement
  }

protected:
  using InvokeFunctionStorage = BoundStateBase::InvokeFunctionStorage;

  explicit CallbackBase(BoundStateBase* boundState) : m_boundState{boundState} {}

  InvokeFunctionStorage invokeFunction() const {
    return m_boundState->m_invokeFunction;
  }

  ScopedRefPtr<BoundStateBase> m_boundState;
};

}  // namespace detail

template <typename Signature>
class Callback;

template <typename ReturnType, typename... Arguments>
class Callback<ReturnType(Arguments...)> : public detail::CallbackBase {
public:
  using RunType = ReturnType(Arguments...);
  using InvokeFunctionType = ReturnType (*)(detail::BoundStateBase*,
                                            detail::PassingType<Arguments>...);

  constexpr Callback() = default;
  Callback(std::nullptr_t) = delete;

  explicit Callback(detail::BoundStateBase* boundState) : detail::CallbackBase{boundState} {}

  ReturnType run(Arguments... arguments) const {
    auto f = reinterpret_cast<InvokeFunctionType>(this->invokeFunction());
    return f(m_boundState.get(), std::forward<Arguments>(arguments)...);
  }
};

template <typename Functor, typename... Arguments>
inline auto bind(Functor&& functor, Arguments&&... arguments)
    -> Callback<detail::MakeUnboundRunType<Functor, Arguments...>> {
  using BoundState = detail::MakeBoundStateType<Functor, Arguments...>;

  using UnboundRunType = detail::MakeUnboundRunType<Functor, Arguments...>;
  using Invoker = detail::Invoker<BoundState, UnboundRunType>;

  using CallbackType = Callback<UnboundRunType>;

  using InvokeFunctionType = typename CallbackType::InvokeFunctionType;
  InvokeFunctionType invokeFunction = &Invoker::run;

  using InvokeFunctionStorage = detail::BoundStateBase::InvokeFunctionStorage;
  return CallbackType{BoundState::create(reinterpret_cast<InvokeFunctionStorage>(invokeFunction),
                                         std::forward<Functor>(functor),
                                         std::forward<Arguments>(arguments)...)};
}

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_CALLBACK_H_
